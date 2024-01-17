
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "define.h"
#include "Parsing/parsing.h"

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



static language* processHeader(int* lineNum, FILE* file, line** line)
{
   
    const char* WidthExplain = "Expected Syntax: \"WIDTH <bits>\".\n";
    *line = NULL;
    if(GetTokensFromNextLine(line, file, *lineNum)==EOF)
    {
        printf("Line %d: Expected WIDTH declaration. \n%s", *lineNum, WidthExplain);
        return NULL;
    }
    //printLine(*line);
    *lineNum = (*line)->lineNum;

    if(strcmp((*line)->head->value, "WIDTH")!=0)
    {
        printf("Line %d: Expected WIDTH declaration.\n%s", *lineNum, WidthExplain);
        return NULL;
    }

    if(tokenCount(*line) != 2)
    {
        printf("Line %d: Wrong syntax for WIDTH declaration. \n%s", *lineNum, WidthExplain); 
        return NULL;  
    }

    token* widthTok = (*line)->head->next; 
    if(!isLiteral(widthTok))
    {
        printf("Line %d: Expected number literal instead of '%s'.\n%s", *lineNum, widthTok->value, WidthExplain); 
        return NULL;  
    }

    if(!isValidLiteral(widthTok))
    {
        printf("Line %d: Expected number literal instead of '%s'.\n%s", *lineNum, widthTok->value, WidthExplain); 
        return NULL;  
    }

    if(!isLiteralInBounds(widthTok, 32))
    {
        printf("Line %d: WIDTH too large. Supported values are 2-32.\n", *lineNum); 
        return NULL;  
    }

    unsigned int width = processLiteral(widthTok);

    if(width>32 || width<2)
    {
        printf("Line %d: WIDTH of %d is invalid. Supported values are 2-32.\n", *lineNum, width); 
        return NULL;  
    }

    printf("I'm happy with a WIDTH of %d!\n", width);
    printf("Lines after line %d were not processed.\n", *lineNum);

   
    freeLine(*line);
    *line = NULL;
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


