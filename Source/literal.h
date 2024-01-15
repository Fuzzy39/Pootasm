#ifndef LITERAL_H_   /* Include guard */
#define LITERAL_H_


/// @brief whether a given token is a valid literal.
/// @param token a string without any whitespace.
/// @return returns 1 if the token is a valid literal.
int isLiteral(char* token); 

/// @brief whether a valid literal has a value that fits in a certain amount of bits.
/// @param literal a valid literal token.
/// @param bits [2-32] the maximum number of bits the literal value can be stored in.
/// @return returns 1 if the literal fits.
int isLiteralInBounds(char* literal, int bits);

/// @brief returns the value of a valid literal token.
/// @param literal a valid literal token.
/// @return returns the value of the literal.
unsigned int processLiteral(char* literal);

#endif 
