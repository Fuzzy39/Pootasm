
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "define.h"
#include "Parsing/parsing.h"

symbol* findSymbol(language* lang, char* name)
{
    symbol* sym = lang->symbol;
    if(sym==NULL)
    {
        // if the language has no symbols, abort.
        return NULL;
    }

    while(sym->next != NULL)
    {
        char* compareAgainst = sym->name;
        if(strcmp(name, compareAgainst) == 0) 
        {
            // Now, my understanding is this method will only return 0 if the strings exactly match.
            // if that's not the case then this is obviously wrong.
            return sym;
        }

        sym = sym->next;
    }

    return NULL;
}



language* readDefines(char* fileName)
{
    FILE* definesFile = fopen(fileName, "r");

    // fopen sets errno, but I'm lazy.
    if(definesFile == NULL)
    {
        printf("There was a problem opening the language definition file '%s'. Does the file exist?\n", fileName);
        // really any number of things could've gone wrong, but meh.
        // since when does malloc fail, right?
        return NULL;
    }

    token* head = NULL;
    char* line = NULL;
    while(GetTokensFromNextLine(&head, definesFile, &line)!=EOF)
    {
        printf("Next.\n");
        printTokens(head);
       
        freeTokens(line, head);
        head = NULL;
        line = NULL;
    }

    return NULL;


}


