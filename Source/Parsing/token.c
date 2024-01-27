
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parsing.h"
#include "stringUtil.h"

void freeLine(line* line)
{
    // we don't free any char* here because
    // we're expecting nextline to deal with it.
   
    printLine(line);
    if(line == NULL)
    {
        return;
    }
    
    printf("Freeing tokens.\n");
    token* head = line->head;
    while(head->next != NULL)
    {
       token* last = head;
       head = head->next; 
  
       free(last);
    }
    
    if(line->linestart!=NULL)
    {
        printf("Freeing linestart. '%s'\n", line->linestart);
        free(line->linestart);
    }   


    printf("Finishing Freeing.\n");
    free(line);
    printf("Freed line.\n");
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

void printLine(line* line)
{
    if(line == NULL)
    {
        printf("No line. Did something go wrong?\n");
        return;
    }

    printf("Line %d: ", line->lineNum);

    token* tok = line->head;

    while(tok != NULL)
    {
        printf("'%s'", tok->value);
        if(tok->next ==NULL)
        {
            putc(' ', stdout);
        }
        else
        {
            printf(", ");
        }

        tok = tok->next;
    }

    printf("\n");
}


int tokenCount(line* line)
{
    token* head = line->head;
    int toReturn = 1;
    while(head->next != NULL)
    {
        head = head->next;
        toReturn++;
    }

    return toReturn;
}

token* getToken(line* line, int index)
{
    token* head = line->head;
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
    printf("tasting line...\n");
    size_t len = 0; // don't care about this.
    if(getline(line, &len, stream) == EOF)
    {
        return EOF;
    }

    printf("Got line '%s'", *line);

    toUppercase(*line);

    *head = makeTokens(*line);
    printf("possibly empty line good\n");
    return 1;
}

int GetTokensFromNextLine(line** line, FILE* stream, int lastLineNumber)
{
    // start off by initializing the line struct.
   
    *line = malloc(sizeof(struct line));
   
    if(line == NULL)
    {
        printf("malloc failed in GetTokensFromNextLine (token.c)\n");
        return EOF;
    }


    (*line)->lineNum = lastLineNumber;
    (*line)->linestart = NULL;
    (*line)->head = NULL;


    while( (*line)->head == NULL)
    {
        (*line)->lineNum++;
        printf( "Looking at line: %d\n",(*line)->lineNum);

        if((*line)->linestart !=NULL)
        {
            free((*line)->linestart);
            (*line)->linestart = NULL;

        }
        // maybe?
        if(getTokensFromLine(&((*line)->head), stream, &((*line)->linestart))==EOF)
        {
            // if this method returns EOF then tokens are never initialized.
            free((*line)->linestart);
            free(*line);

            return EOF;
        }
        

    }
    printf("All was well with line!\n");
    return 1;
    
   
}

