#ifndef STRUTIL_H   /* Include guard */
#define STRUTIL_H

#include "parsing.h"

/// @brief creates tokens from a line of text.
/// @param line a string terminated by \\n\0.
/// @return returns the head of a linked list of tokens.
token* makeTokens(char* line);

void toUppercase (char* string);

int getline(char** lineptr, size_t *n, FILE* stream);


#endif