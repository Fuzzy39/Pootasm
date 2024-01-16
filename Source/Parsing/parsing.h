#ifndef PARSING_H   /* Include guard */
#define PARSING_H

#include <stdlib.h>
#include <stdio.h>

struct token;
typedef struct token token;

struct token
{
    char* value;
    token* next;
};



// ################ TOKENS #################

/// @brief Frees the line of text and the tokens created from it.
/// @param head 
void freeTokens(char* line, token* head);

/// @brief Prints out a linked list of tokens.
/// @param head 
void printTokens(token* head);

/// @brief Gets the number of tokens in a linked list.
/// @param head 
int tokenCount(token* head);

/// @brief Get the nth token in a linked list.
/// @param head 
/// @param index 
token* getToken(token* head, int index);

/// @brief fills *head with a linked list of tokens from the next line with content.
/// @param head A probably null pointer. will be filled with tokens on success.
/// @param stream 
/// @return returns 1 on success. returns -1 on eof.
int GetTokensFromNextLine(token** head, FILE* stream, char** line);


// ######################### LITERALS #####################

/// @brief Whether a token may be a literal. It may or may not be valid.
/// @param token A token.
/// @return returns 1 if the token is a literal.
int isLiteral(token* token);

/// @brief whether a given token is a valid literal.
/// @param token a string without any whitespace.
/// @return returns 1 if the token is a valid literal.
int isValidLiteral(token* token); 

/// @brief whether a valid literal has a value that fits in a certain amount of bits.
/// @param literal a valid literal token.
/// @param bits [2-32] the maximum number of bits the literal value can be stored in.
/// @return returns 1 if the literal fits.
int isLiteralInBounds(token* literal, int bits);

/// @brief returns the value of a valid literal token.
/// @param literal a valid literal token.
/// @return returns the value of the literal.
unsigned int processLiteral(token* literal);

#endif