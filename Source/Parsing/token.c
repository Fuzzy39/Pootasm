
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parsing.h"
#include "stringUtil.h"

void freeTokens(char* line, token* head)
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
    free(line);
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


int tokenCount(token* head)
{
    int toReturn = 1;
    while(head->next != NULL)
    {
        head = head->next;
        toReturn++;
    }

    return toReturn;
}

token* getToken(token* head, int index)
{

    while(index>0)
    {
        if(head == NULL)
        {
            return NULL;
        }
        head = head->next;
        index--;
        
    }

    return head;
}

static int getTokensFromLine(token** head, FILE* stream, char** line)
{
    size_t len = 0; // don't care about this.
    if(getline(line, &len, stream) == EOF)
    {
        return EOF;
    }
    toUppercase(*line);

    *head = makeTokens(*line);
    return 1;
}

int GetTokensFromNextLine(token** head, FILE* stream, char** line)
{
    
    while(*head == NULL)
    {
        if(getTokensFromLine(head, stream, line)==EOF)
        {
            return EOF;
        }
    }

    return 1;
    
   
}

