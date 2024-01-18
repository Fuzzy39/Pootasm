#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "parsing.h"


static int isValidDigit(char digit, char base)
{
    // this probably ought to be a switch statement, but I'm lazy.   
    if(base == 'B')
    {
        char* matches = "01";
        return strchr(matches, digit) != NULL;
    }

    if(base == 'O')
    {
        char* matches = "01234567";
        return strchr(matches, digit) != NULL;
    }

    if(base == 'D')
    {
        char* matches = "0123456789";
        return strchr(matches, digit) != NULL;
    }

    if(base == 'X')
    {
        char* matches = "0123456789ABCDEF";
        return strchr(matches, digit) != NULL;
    }
    
    fprintf(stderr, "Something went wrong. Base '%c' is invalid. Expected b, o, d, or x.\n", base);
    return 0;
}


// where base is:
// d - decimal
// o - octal
// b - binary
// x - hex
// returns 1 if number is valid.
static int isValidNumber(char* number, char base)
{
 
    while(*number !='\0')
    {
        if(!isValidDigit(*number, base))
        {
            return 0;
        }
        number += sizeof(char);
        
    } 
    return 1;
}


// this will return 1 if the first character of the token is a digit, + or -.
int isLiteral(token* token)
{
    char c = token->value[0];
    // I recognize this part could be written in like 1 line, but I'm not super confident that it would
    // always return 1 or 0.
    if(isdigit(c))
    {
        return 1;
    }

    if(c=='+' || c=='-')
    {
        return 1;
    }

    return 0;


}


// returns 1 if the token is a valid literal.
int isValidLiteral( token* token)
{
    char* val = token->value;
    if(*val == '+' || *val == '-')
    {
        val+=sizeof(char);  
    }

    if(*val == '\0')
    {
        return 0; // we've got an empty string, or just + or -. that's not a number.
    }

    char base = 'D';
    
    // check if token is binary, octal, or hexadecimal.
    if(*val == '0')
    {
        
        val+=sizeof(char);  

        if(*val == '\0')
        {
            return 1;
        }
        
        int isSpecial = *val == 'B' || *val == 'X' || *val == 'O';

        if(!(isdigit(*val) || isSpecial || *val =='\0') )
        {
            return 0; // something like 0q* or something. invalid.
        }

        if(isSpecial)
        {
            base = *val;
        }

        val +=sizeof(char);

        if(isSpecial && *val=='\0')
        {
            return 0; // we've got "0x" or something like that. invalid.
        }        
    }

    // by now we're just down the the numerical portion. if the number was decimal and started with a zero, we've verified the first two characters of it.
    return isValidNumber(val, base);
}


// returns the base character (b, o, d, or x) of a literal token (that has had its sign stripped)
// the token is assumed valid.
static char getBase(char* token)
{
    if(*token != '0')
    {
        return 'D';
    }

    token += sizeof(char);

    if(*token == '\0')
    {
        return 'D';
    }
    
    if(isdigit(*token))
    {
        return 'D';
    }

    return *token;
}

static int charVal(char digit)
{
    const char* values = "0123456789ABCDEF";
    for(int i = 0; i<strlen(values); i++)
    {
        if(digit == values[i])
        {
            return i;
        }
    }

    return -1; // this will make the value very wrong.
    // should never happen, in any case.
}

static int radixFromBase(char base)
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
        default:
            return 16;
        
    }
}




static int isNumberInBounds(char* number, int radix, int bits)
{
   

    unsigned int maxValue = 1 << (bits);
    maxValue--;
    if(bits >= 32)
    {
       maxValue = UINT_MAX;
    }

    unsigned int value = 0;
    unsigned int old = value;
    while(1)
    {
        old = value;
        value +=charVal(*number);
       
        if(old>value || value > maxValue)
        {
            return 0;
        }
        number += sizeof(char);
        if(*number=='\0')
        {
            return 1;
        }

        old = value;
        value *= radix;
        if(old>value || value > maxValue)
        {
            return 0;
        }
    }
    

}


// returns nonzero if the given (valid) literal token fits within bounds bits
// bits must be between 2 and 32 (inclusive).
int isLiteralInBounds(token* token, int bits)
{
    char* literal = token->value;
    // we will discard the sign.
    if(*literal == '-')
    {
        bits--;
        literal++;
    }

    if(*literal == '+')
    {
        literal++;
    }

    char base = getBase(literal);

    if(base != 'D')
    {
        literal += sizeof(char)*2;
    }

    return isNumberInBounds(literal, radixFromBase(base), bits);
    
}


// gets the value of a number string, assuming its value is 
// within bounds.
static unsigned int strValue(char* number, char base)
{
   
    int radix = radixFromBase(base);

    unsigned int toReturn = 0;
    while(1)
    {
        toReturn +=charVal(*number);
        number += sizeof(char);
        if(*number=='\0')
        {
            return toReturn;
        }

        toReturn *= radix;
    }
}

// string is a string without any whitespace characters.
// it is known valid.
// if the value is larger than UINT_MAX, it will return ULONG_MAX.
// otherwise, the value will be within the range of an int.
unsigned int processLiteral(token* literal)
{
    char* token = literal->value;
    // process signs.
    int isNeg = 0;

    if(*token == '-')
    {
        isNeg = 1;
        token += sizeof(char);
    }

    if(*token == '+')
    {
        token += sizeof(char);
    }

    char base = getBase(token);

    if(base != 'D')
    {
        token += sizeof(char)*2;
    }

    // now we have a sting and we know it's base.
    // we just have to parse it.
    // it is valid, but it could be bigger than an int.

    unsigned int toReturn = strValue(token, base); // an int, unless the string value was too large.
    if(!isNeg)
    {
        return toReturn;
    }

    // find the two's compliment
    toReturn = ~toReturn;
    toReturn ++;

    return toReturn;

}


