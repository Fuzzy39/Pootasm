
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "Headers/pootasm.h"


void toUppercase (char* string)
{
    while(*string != '\0')
    {
        *string = toupper(*string);
        string++;
    }
}

// finds the next non space character in a string.
char* findNextContent(char* current)
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


int getline(char** lineptr, size_t *n, FILE* stream)
{
    *n = 0;
    int chars = 0;
    *lineptr = NULL;

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
           
            if(chars == 0)
            {
                return EOF;
            } 
            
            (*lineptr)[chars] = '\n';
            chars++;
            (*lineptr)[chars] = '\0';
            chars++;
            return chars;
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


int radixFromBase(char base)
{
    
    switch(base)
    {
        case 'B':
            return 2;       
        case 'O':
            return 8;
        case 'D':
            return 10;
            break;
        case 'X':
            return 16;
        default:
            return -1;
    }
}


char *strdup(const char *s) 
{
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}



// y can't be 0 or negative
// y should be small or this overflows
// it's fine, for our case.
static unsigned long long intPow(long long x, int y)
{
    if(!y)
    {
        return 1;
    }

    long long toRet = x;
    y--;
    while(y>0)
    {
        toRet*=x;
        y--;
    }

    return toRet;
}

/// @brief Converts an int to a string.
/// @param num number to convert.
/// @param strsize the size of the string in bytes, including the null character.
/// @param digits the number of nonzero characters expected in the string.
/// @param str a pointer to a pointer of a string.
/// @param radix radix. Expected value is from 2-16.
/// @return 1 on success. 0 on failure.
static int intToString(unsigned int num, size_t strsize, int digits, char** str, int radix)
{
    **str='\0';

    if(digits>=strsize)
    {
        return 0;
    }

    if(radix>16|| radix<2)
    {
        return 0;
    }

    const char* values = "0123456789ABCDEF";
    int zeros = strsize-digits-1;
    int i =0;
    for(; i<zeros; i++)
    {
        (*str)[i]='0';
    }

    digits--;
    while(digits+1)
    {
        
        int pow = intPow(radix,digits);
        (*str)[i]=values[num/pow];
        num-=(num/pow)*pow;
        i++;
        digits--;
       
    }
    (*str)[i]='\0';
    return 1;

}


int printNumber(unsigned int num, int bits, char base, FILE* stream)
{
    int radix = radixFromBase(base);
    if(radix == -1)
    {
        // base was bad.
        return 0;
    }

    if(bits!=-1 &&( bits>32 || bits<2))
    {
        return 0;
    }

    if(num>intPow(2, bits) && bits!=-1)
    {
        return 0; // number too large.
    }

   

    // next, get how many digits the number should be
    int stringLen = ceil(log(intPow(2, bits))/log(radix));
    int digits =  floor(log(num)/log(radix))+1;

    if(num==0)
    {
        digits = 1;
    }

    if(bits == -1)
    {
        stringLen = digits;
    }

    // digits should be right now?
    char* toPrint = malloc((stringLen+1)*sizeof(char));
    if(toPrint == NULL)
    {
        return 0;
    }

    if(!intToString(num, stringLen+1, digits, &toPrint, radix))
    {
        printf("ERROR");
        free(toPrint);
        return 0;
    }

    
    
    fprintf(stream, "%s", toPrint);
    free(toPrint);
    return 1;

    
}

