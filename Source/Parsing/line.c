#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Headers/pootasm.h"


void freeLine(line* line)
{
    // we don't free any char* here because
    // we're expecting nextline to deal with it.
   
    if(line == NULL)
    {
        return;
    }

    token* head = line->head;
    while(head->next != NULL)
    {
       token* last = head;
       head = head->next; 
  
       free(last);
    }

    free(head);
    
    if(line->linestart!=NULL)
    {
        free(line->linestart);
    }   


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


static int getTokensFromLine(token** head, FILE* stream, char** line, int caseSensitive)
{

    size_t len = 0; // don't care about this.
    if(getline(line, &len, stream) == EOF)
    {
        return EOF;
    }

    if(!caseSensitive)
    {
        toUppercase(*line);
    }

    *head = makeTokens(*line);
    return 1;
}



int GetTokensFromNextLine(line** line, FILE* stream, int lastLineNumber, int caseSensitive)
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

        if((*line)->linestart !=NULL)
        {
            free((*line)->linestart);
            (*line)->linestart = NULL;

        }
        // maybe?
        if(getTokensFromLine(&((*line)->head), stream, &((*line)->linestart), caseSensitive)==EOF)
        {
            // if this method returns EOF then tokens are never initialized.
            free((*line)->linestart);
            free(*line);

            return EOF;
        }
        

    }
    return 1;
    
   
}

