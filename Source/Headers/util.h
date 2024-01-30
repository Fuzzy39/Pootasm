#ifndef STRUTIL_H   /* Include guard */
#define STRUTIL_H


void toUppercase (char* string);

int getline(char** lineptr, size_t *n, FILE* stream);

char* findNextContent(char* current);

int radixFromBase(char base);

char *strdup(const char *s);


/// @brief 
/// @param num Number to print.
/// @param bits The number of bits that this number takes up. This determines how many leading zeroes there are, if any.
/// -1 for 'as many as required' (up to 32, the maximum)
/// @param base the base to print in. options are B, O, D, and X for base 2, 8, 10, and 16.
/// @param stream 
/// @return 
int printNumber(unsigned int num, int bits, char base, FILE* stream);   


#endif