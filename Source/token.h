#ifndef TOKEN_H   /* Include guard */
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>

struct token;
typedef struct token token;

struct token
{
    char* value;
    token* next;
};


/// @brief Frees the line of text and the tokens created from it.
/// @param line 
/// @param head 
void freeTokens(char* line, token* head);

/// @brief creates tokens from a line of text.
/// @param line a string terminated by \\n\0.
/// @return returns the head of a linked list of tokens.
token* makeTokens(char* line);

/// @brief Prints out a linked list of tokens.
/// @param head 
void printTokens(token* head);

void toUppercase (char* string);

int getline(char** lineptr, size_t *n, FILE* stream);

#endif