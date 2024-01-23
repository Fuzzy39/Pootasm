#include "assemble.h"
#include "define.h"
#include "Parsing/parsing.h"
#include <stdio.h>
#include <math.h>
#include <string.h>



static const char* DefExplain = "Expected Syntax: .DEF <FILENAME>\n";

static void freeSection(section* sect)
{
    // basically we just need to free chunks and then ourself.
    // we expect the caller to keep a hold of the next element.
    chunk* ch = sect->head;
    while(ch != NULL)
    {
        free(ch->data);
        free(ch->label);
        chunk* next = ch->next;
        free(ch);
        ch = next;
    }
    free(sect);
}

void freeOutput(output* output)
{
    if(output==NULL)
    {
        return;
    }

    freeLanguage(output->lang);
    symbol* sym = output->labels;

    while(sym != NULL)
    {
        free(sym->name);
        symbol* next = sym->next;
        free(sym);
        sym = next;
    }

    // free sections
    section* sect = output->head;
    while(sect != NULL)
    {
        section* next = sect->next;
        freeSection(sect);
        sect = next;
    }
}




static language* getLanguage(FILE* file, char* filename)
{
    int lineNumber = 0;

    line* line;

    if(GetTokensFromNextLine( &line, file, lineNumber)==EOF)
    {
        printf("Error in '%s', Line 0: First statement must be .DEF directive.\n%s", filename, DefExplain);
        freeLine(line);
      
        return NULL;
    }

    if(strcmp(line->head->value, ".DEF") != 0)
    {
        printf("Error in '%s', Line %d: First statement must be .DEF directive.\n%s", filename, line->lineNum, DefExplain);
        freeLine(line);
      
        return NULL;
    }


    if(line->head->next == NULL)
    {
        printf("Error in '%s', Line %d: .DEF directive requires a file name.\n%s", filename, line->lineNum, DefExplain);
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

output* decodeSymbols(char* filename)
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

    // First thing's first, get the language definition.
    language* lang = getLanguage(file, filename);
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
    
    // for now.
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


