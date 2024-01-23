#include "assemble.h"
#include "define.h"
#include "Parsing/parsing.h"
#include <stdio.h>
#include <math.h>
#include <string.h>



static const char* DefExplain = "Expected Syntax: .DEF <FILENAME>\n";
static const char* OrgExplain = "Expected Syntax: .ORG <ADDRESS>\n";




static language* getLanguage(FILE* file, int* lineNum, char* filename)
{
   
    line* line;

    if(GetTokensFromNextLine( &line, file, *lineNum)==EOF)
    {
        printf("Error in '%s', Line 0: First statement must be .DEF directive.\n%s", filename, DefExplain);
        freeLine(line);
      
        return NULL;
    }

    *lineNum = line->lineNum;

    if(strcmp(line->head->value, ".DEF") != 0)
    {
        printf("Error in '%s', Line %d: First statement must be .DEF directive.\n%s", filename, *lineNum, DefExplain);
        freeLine(line);
      
        return NULL;
    }


    if(line->head->next == NULL)
    {
        printf("Error in '%s', Line %d: .DEF directive requires a file name.\n%s", filename, *lineNum, DefExplain);
        freeLine(line);
        return NULL;
    }


    // next step is to process all tokens into a filename
    // we don't love spaces here.
    token* current = line->head->next;

    // oh wait. we can... we can do a thing.
    // this might be stupid. 
    // let's just mutilate this token datastructure we've made.
    token* last = current;
    while(last->next !=NULL)
    {
        last = last->next;
    }
  
    // because all of the token values are part of the same chunk of allocated memory,
    // we can compare them...
    // they were once a single big string.
    // so we can simply... remove the terminators and get the orignial text back.
    // more or less.
    // this breaks all of the tokens, but that's fine, we didn't want them.
    for(char* ch = current->value; ch<last->value; ch+=sizeof(char))
    {
        if(*ch == '\0')
        {
            *ch=' ';
        }
    }
    // this is cursed.

    // but hey, it'll work!
    language* lang = readDefines(current->value);
    freeLine(line);
    return lang;

}

// makes a chunk at the end of a section.
static chunk* newChunk(section* sect)
{
    chunk* ch = malloc(sizeof(ch));
    if(ch==NULL)
    {
        printf("malloc failed in newChunk");
        return NULL;
    }

    // you know, maybe I could just use calloc?
    ch->data = NULL;
    ch->label = NULL;
    ch ->labelLineNumber = 0;
    ch ->length = 0;
    ch ->next = NULL;

    // attach
    if(sect->head == NULL)
    {
        sect->head = ch;
        return ch;
    }

    chunk* last = sect->head;
    while(last->next!=NULL)
    {
        last = last->next;
    }
    last->next = ch;
    return ch;

}

// creates a new section.
static section* newSection(output* out, line* line, int linenum, char* filename)
{
    // we know for a fact that this line is an org directive.
    if(tokenCount(line)!=2)
    {
         printf("Error in '%s', Line %d: Invalid syntax for .ORG directive.\n%s", filename, linenum, OrgExplain);
         return NULL;
    }

    token* location = getToken(line, 1);

    if(!checkLiteral(&linenum, location, filename))
    {
        return NULL;
    }

    int address = processLiteral(location);

    section* sect = malloc(sizeof(sect));
    if(sect==NULL)
    {
        printf("malloc failed in newSection");
        return NULL;
    }

    sect->head = NULL;
    sect->location = address;
    sect->next = NULL;

    newChunk(sect);

    return sect;
}



// totally a normal amount of parameters.
static int processDirective(output* out, line* line, int linenum, char* filename, section** sect, chunk** chunk)
{
     // we know this is in fact an org directive.
    char* directive = line->head->value;
    if(!strcmp(directive, ".ORG"))
    {
        section* old = *sect;
        *sect = newSection(out, line, linenum, filename);
        if(*sect!=NULL)
        {
            *chunk = (*sect)->head;
            if(old == NULL)
            {
                out->head = *sect;
            }
            else
            {
                old->next = *sect;
            }
        }
        return *sect != NULL;
            
    }

    if(!strcmp(directive, ".DEF"))
    {
        printf("Error in '%s', Line %d: Invalid context for .DEF directive.\n", filename, linenum);
        return 0;
    }
    
    printf("Error in '%s', Line %d: Unknown directive '%s'\n", filename, linenum, directive);
    return 0;
}




static output* decodeSymbols(output* out, FILE* file, int* lineNum, char* filename)
{
    section* currentSect = NULL;
    chunk* currentChunk = NULL;

    line* line = NULL;
    while(GetTokensFromNextLine( &line, file, *lineNum)!=EOF)
    {
       
        if(currentSect == NULL && strcmp(line->head->value, ".ORG")!=0)
        {
            printf("Error in '%s', Line %d: Expected .ORG directive, not '%s'\n%s", filename, *lineNum, line->head->value, OrgExplain);
            freeLine(line);
            freeOutput(out);
            return NULL;
        }

        // directive(s).
        if(*(line->head->value)== '.')
        {
           
            if(processDirective(out, line, *lineNum, filename, &currentSect, &currentChunk))
            {
                continue;
            }

            freeLine(line);
            freeOutput(out);
            return NULL;
        }


        // Normal processing goes here.

        freeLine(line);
    }
  
    return out;

}

output* firstPass(char* filename)
{

    FILE* file = fopen(filename, "r");

    // fopen sets errno, but I'm lazy.
    if(file == NULL)
    {
        printf("There was a problem opening the assembly file '%s'. Does the file exist?\n", filename);
        // really any number of things could've gone wrong, but meh.
        // since when does malloc fail, right?
        return NULL;
    }
    int lineNumber = 0;
    // First thing's first, get the language definition.
    language* lang = getLanguage(file, &lineNumber, filename);
    if(lang == NULL)
    {
        fclose(file);
        return NULL;
    }

    // now that we've got the language, we can allocate the output.
    output* out = malloc(sizeof(output));

    if(out == NULL)
    {
        printf("Malloc failed in decode symbols.\n");
        fclose(file);
        return NULL;
    }

    out->lang = lang;
    out->head = NULL; // we intend to fill this.
    out->labels = NULL; // will remain null for the rest of this function.

    out = decodeSymbols(out, file, &lineNumber, filename);

    // quickly make sure the file wasn't basically empty.
    if(out !=NULL &&out->head == NULL )
    {
        printf("Error in '%s', Line %d: Expected .ORG directive.\n%s", filename, lineNumber, OrgExplain);
        freeOutput(out);
        return NULL;
    }

    return out;
   
}

output* decodeLabels(output* partial, char* filename)
{
    printf("decodelabels not implemented.");
    return NULL;
}

output* finalizeOutput(output* partial, char* filename)
{
    printf("finalizeOutput not implemented.");
    return NULL;
}


