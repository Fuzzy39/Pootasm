
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "token.h"

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

void toUppercase (char* string)
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




token* makeTokens(char* line)
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


int getline(char** lineptr, size_t *n, FILE* stream)
{
    *n = 0;
    int chars = 0;
   

    while(1)
    {
            
        if(chars>=(*n))
        {
            *n += 100*sizeof(char);
          
            (*lineptr) = realloc(*lineptr, (*n)+2*sizeof(char));
          
            if(*lineptr == NULL)
            {
                printf("Malloc failed in getline.\n");
                return EOF;
            }
        }

        int ch = fgetc(stream);
        if(ch == EOF)
        {
           
            (*lineptr)[chars] = '\n';
            chars++;
            (*lineptr)[chars] = '\0';
            chars++;
            return EOF;
        }


        (*lineptr)[chars] = (char)ch;
        chars++;
        
        if(ch == '\n')
        {
            (*lineptr)[chars] = '\0';
            chars++;
            return chars;
        }
        
    }
    
}
