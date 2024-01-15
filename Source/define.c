
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "define.h"
#include "token.h"


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
        if(0==strcmp(name, compareAgainst)) 
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

    char* line = NULL;
    size_t len = 0; // don't care about this.
    if(getline(&line, &len, definesFile) == EOF)
    {
        printf("Line failed to read...(got:'%s')\n", line);
        return NULL;
    }
    printf("got: '%s'\n", line);

    toUppercase(line);

    
    token* head = makeTokens(line);
    printTokens(head);
    // cleanup.

    freeTokens(line, head);

    return NULL;


}


