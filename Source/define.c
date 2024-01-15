
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "define.h"


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

static void FreeTokens(token* head)
{
    // we don't free any char* here because
    // we're expecting nextline to deal with it.
    while(head->next != NULL)
    {
       token* last = head;
       head = head->next; 
       free(last);
    }

    free(head);
}

static void toUppercase (char* string)
{
    while(*string != '\0')
    {
        *string = toupper(*string);
        string++;
    }
}

// finds the next non space character in a string.
static char* findNextContent(char* current)
{
    while(*current != '\0')
    {
        if(*current == ';') // comments
        {
            // short circuit.
            *current = '\0';
            return current;
        }

        if(!isspace(*current))
        {
            return current;
        }

        current+= sizeof(char);
    }

    return current;
}


static char* makeToken(char* current, token** tail)
{
    if(*current == '\0')
    {
        return current;
    }


    // make a new thingy and copy stuff over.
    token* new = malloc(sizeof(token));

    if(new == NULL)
    {
        printf("Malloc for token failed. Line being read is dead.\n");
        *current = '\0';
        return current;
    }

    if(*tail!= NULL)
    {
        (*tail)->next = new;
       
    }


    *tail = new;
    (*tail)->value = current; 
    (*tail)->next = NULL;

   
    // now we find the end of the token and plop a \0 character there.
    while(*current != '\0')
    {
        if(*current == ';')
        {
            // short circuit on comments
            *current = '\0';
            return current;
        }

        if(isspace(*current))
        {
            *current = '\0';
            current += sizeof(char);
            return current;
        }

        current+= sizeof(char);
    }

    return current;


}




static token* makeTokens(char* line)
{
    // line is a malloced pointer.
    // the tokens will survive until  the next line, when they should be destroyed.
    token* head = NULL;
    token* tail = head;

    while(*line != '\0')
    {
        line = findNextContent(line);
        line = makeToken(line, &tail);
      
        if(head == NULL)
        {
            head = tail;
        }
    }

    return head;
}

void printTokens(token* head)
{
    if(head == NULL)
    {
        printf("No tokens.\n");
        return;
    }

    while(head-> next != NULL)
    {
        printf("Token:'%s'\n", head->value);
        head = head->next;
    }
    printf("Token:'%s'\n End line.\n", head->value);


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
    free(line);
    FreeTokens(head);

    return NULL;


}


