#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Headers/pootasm.h"


symbol* findSymbol(symbol* head, char* name)
{
    symbol* sym = head;
   

    while(sym != NULL)
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

int symbolCount(symbol* head)
{
    int count = 0;
    symbol* sym = head;
    while(sym!=NULL)
    {
        count++;
        sym = sym->next;
    }
    return count;
}


void printLanguage(language* lang)
{
    if(lang == NULL)
    {
        printf("NULL language.\n");
        return;
    }

    printf("Language:\n");
    printf("WIDTH: %d bits.\n", lang->width);
    printf("ADDRESS: %d bits.\n", (lang->width)*(lang->address));

    // print symbols now.
    printf("\n%d symbols:\n", symbolCount(lang->head));
    symbol* sym = lang->head;

    while(sym != NULL)
    {
        printf("\t%s: %d\n", sym->name, sym->value);
        sym = sym->next;
    }
}

void freeLanguage(language* lang)
{
    if(lang==NULL)
    {
        return;
    }
    // first, free symbols.
    symbol* sym = lang->head;

    while(sym != NULL)
    {
        free(sym->name);
        symbol* next = sym->next;
        free(sym);
        sym = next;
    }

    free(lang);
}
