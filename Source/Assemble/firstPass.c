#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../Headers/pootasm.h"


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
    chunk* ch = malloc(sizeof(chunk));
    if(ch==NULL)
    {
        printf("malloc failed in newChunk");
        return NULL;
    }

    // you know, maybe I could just use calloc?
    ch ->bytes = 0;
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

    section* sect = malloc(sizeof(section));
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


static int isLabel(char* str)
{
    int i = 0;
    while(str[i]!='\0')
    {
        i++;
    }

    i--;
    return str[i]==':'; 
}

static int decodeToken(language* lang, token* token, section* sect, chunk** ch, int* lineNum, char* filename)
{
    // allocate more space in the chunk if needbe.
    chunk* current = *ch;
    if(current->bytes<=current->length*sizeof(int))
    {   
        current->bytes += 10*sizeof(int);
        current->data = realloc(current->data, current->bytes);
        if(current->data == NULL)
        {
            printf("Internal Error while parsing '%s', around Line %d: Couldn't allocate memory.\n", filename, *lineNum);
            return 0;
        }
    }

    int newData;
    // determine the value of the token if possible.
    if(isLiteral(token))
    {
        if(!checkLiteral(lineNum, token, filename))
        {
            return 0;
        }

        newData = processLiteral(token);
    }
    else
    {
        symbol* sym =findSymbol(lang, token->value);
        if(sym == NULL)
        {
            if(*(token->value)=='.')
            {
                 printf("Error in '%s', Line %d: Directive '%s' must be at the start of a line.\n", filename, *lineNum, token->value);
                 return 0;
            }

            // this is a label.
            current->label = malloc(strlen(token->value));
            if(current->label == NULL)
            {
                printf("Internal Error while parsing '%s', around Line %d: Couldn't allocate memory.\n", filename, *lineNum);
                return 0;
            }
            
            memcpy(current->label, token->value, strlen(token->value)+1);

            current->labelLineNumber=*lineNum;

            // we need to create a new chunk.
            *ch = newChunk(sect);
            return (ch != NULL);
         
        }

        newData = sym->value;
        
    }

    // cram the data in.
    current->data[current->length]=newData;
    current->length++;

    return 1;
}


static int decodeLine(language* lang, line* line, section* sect, chunk** ch, int* lineNum, char* filename)
{
    int tokens = tokenCount(line);
    // obviously not the most effecient, but I can, so I will.
    for(int i=0; i<tokens; i++)
    {
        if(!decodeToken(lang, getToken(line, i), sect, ch, lineNum, filename))
        {
            return 0;
        }
    }

    return 1;
}

static int makeLabel(char* name, output* out, section* sect)
{
    // allocate a new space for the thing
    symbol* label = malloc(sizeof(symbol));
    if(label == NULL)
    {
        printf("Malloc failed in makeLabel.\n");
        return 0;
    }


    // attach it to the list.
    symbol* last = out->labels;
    
    if(last !=NULL)
    {
        while(last->next !=NULL)
        {
            last = last->next;
        }

        last->next = label;
    }
    else
    {
        out->labels = label;
    }

    // initialize.
    label->value = EndOfSection(out->lang->address, sect);
    label->next = NULL;
    label->name = malloc(strlen(name)); // no +1 since we're removing the colon.
    if(label->name == NULL)
    {
        printf("Malloc failed in makeLabel.\n");
        return 0;
    }
    memcpy(label->name, name, strlen(name));
    label->name[strlen(name)-1]='\0';

    return 1;



}

static output* decodeSymbols(output* out, FILE* file, int* lineNum, char* filename)
{
    section* currentSect = NULL;
    chunk* currentChunk = NULL;

    line* line = NULL;
    while(GetTokensFromNextLine( &line, file, *lineNum)!=EOF)
    {
       
        *lineNum = (line)->lineNum;
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
                freeLine(line);
               
                continue;
            }
         
            freeLine(line);
            freeOutput(out);
            return NULL;
        }

        
        // The line is not a directive. It could be a label.
        if(tokenCount(line)==1 && isLabel(line->head->value))
        {
            // label processing goes here.
            if(strlen(line->head->value)==1)
            {
                printf("Error in '%s', Line %d: Invalid label '%s'.\n", filename, *lineNum, (line->head->value));
                freeOutput(out);
                freeLine(line);
                return NULL;

            }

            if(!makeLabel(line->head->value, out, currentSect))
            {
                freeOutput(out);
                freeLine(line);
                return NULL;

            }
            freeLine(line);
            continue;
        }

        // standard line processing.
        // how are chunks allocated? realloc, of course!
        if(!decodeLine(out->lang, line, currentSect, &currentChunk, lineNum, filename))
        {
            freeLine(line);
            freeOutput(out);
            return NULL;
        }

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
    fclose(file);
    // quickly make sure the file wasn't basically empty.
    if(out !=NULL &&out->head == NULL )
    {
        printf("Error in '%s', Line %d: Expected .ORG directive.\n%s", filename, lineNumber, OrgExplain);
        freeOutput(out);
        
        return NULL;
    }

    return out;
   
}


