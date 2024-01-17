
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "define.h"
#include "Parsing/parsing.h"

static const char* WidthExplain = "Expected Syntax: \"WIDTH <bits>\".\n";
static const char* AddressExplain = "Expected Syntax: \"ADDRESSS <words>\"\n";

symbol* findSymbol(language* lang, char* name)
{
    symbol* sym = lang->symbol;
    if(sym==NULL)
    {
        // if the language has no symbols, abort.
        return NULL;
    }

    while(sym->next != NULL)
    {
        char* compareAgainst = sym->name;
        if(strcmp(name, compareAgainst) == 0) 
        {
            // Now, my understanding is this method will only return 0 if the strings exactly match.
            // if that's not the case then this is obviously wrong.
            return sym;
        }

        sym = sym->next;
    }

    return NULL;
}

static int checkLiteral(int* lineNum, token* token)
{
    if(!isLiteral(token))
    {
        printf("Line %d: Expected number literal instead of '%s'.\n", *lineNum, token->value); 
        return 0;  
    }

    if(!isValidLiteral(token))
    {
        printf("Line %d: Expected number literal instead of '%s'.\n", *lineNum, token->value); 
        return 0;  
    }

    if(!isLiteralInBounds(token, 32))
    {
        printf("Line %d: literal value too large. Pootasm supports literals that are a maximum of 32 bits wide.\n", *lineNum); 
        return 0;  
    }


    return 1;
}



static int getWidth(int lineNum, line* line)
{
    if(strcmp(line->head->value, "WIDTH")!=0)
    {
        printf("Line %d: Expected WIDTH declaration.\n%s", lineNum, WidthExplain);
        return 0;
    }

    if(tokenCount(line) != 2)
    {
        printf("Line %d: Wrong syntax for WIDTH declaration. \n%s", lineNum, WidthExplain); 
        return 0;  
    }

    token* widthTok = (line)->head->next; 
    if(!checkLiteral(&lineNum, widthTok))
    {
        printf("%s", WidthExplain);
        return 0;
    }

    unsigned int width = processLiteral(widthTok);

    if(width>32 || width<2)
    {
        printf("Line %d: WIDTH of %d is invalid. Supported values are 2-32.\n", lineNum, width); 
        return 0;  
    }

    return width;
}


static language* processHeader(int* lineNum, FILE* file, line** line)
{
   
    // ########### WIDTH #############
    *line = NULL;
    if(GetTokensFromNextLine(line, file, *lineNum)==EOF)
    {
        printf("Line %d: Expected WIDTH declaration. \n%s", *lineNum, WidthExplain);
        
        freeLine(*line);
        *line = NULL;
        return NULL;
    }

    *lineNum = (*line)->lineNum;

    int width = getWidth(*lineNum, *line);
    if(width == 0)
    {
        freeLine(*line);
        *line = NULL;
        return NULL;
    }
  
    printf("I'm happy with a WIDTH of %d!\n", width);
   

   
    freeLine(*line);
    *line = NULL;

    if(GetTokensFromNextLine(line, file, *lineNum)==EOF)
    {
        printf("Line %d: Expected ADDRESS declaration. \n%s", *lineNum, WidthExplain);
        
        freeLine(*line);
        *line = NULL;
        return NULL;
    }


    return NULL;

}



language* readDefines(char* fileName)
{
    FILE* definesFile = fopen(fileName, "r");

    // fopen sets errno, but I'm lazy.
    if(definesFile == NULL)
    {
        printf("There was a problem opening the language definition file '%s'. Does the file exist?\n", fileName);
        // really any number of things could've gone wrong, but meh.
        // since when does malloc fail, right?
        return NULL;
    }

    line* line = NULL;
    int lineNum = 0;

    processHeader(&lineNum, definesFile, &line);
    return NULL;


}


