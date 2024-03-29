#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../Headers/pootasm.h"

static const char* WidthExplain = "Expected Syntax: \"WIDTH <bits>\".\n";
static const char* AddressExplain = "Expected Syntax: \"ADDRESSS <words>\"\n";
static const char* SymbolExplain = "Expected Syntax: \"SYMBOL <name> <value>\"\n";

static int getWidth(int lineNum, line* line, char* filename)
{
    if(strcmp(line->head->value, "WIDTH")!=0)
    {
        printf("Error in '%s', Line %d: Expected WIDTH declaration.\n%s", filename, lineNum, WidthExplain);
        return 0;
    }

    if(tokenCount(line) != 2)
    {
        printf("Error in '%s', Line %d: Wrong syntax for WIDTH declaration. \n%s", filename, lineNum, WidthExplain); 
        return 0;  
    }

    token* widthTok = (line)->head->next; 
    if(!checkLiteral(&lineNum, widthTok, filename))
    {
        printf("%s", WidthExplain);
        return 0;
    }

    unsigned int width = processLiteral(widthTok);

    if(width>32 || width<2)
    {
        printf("Error in '%s', Line %d: WIDTH of %d is invalid. Supported values are 2-32.\n", filename, lineNum, width); 
        return 0;  
    }

    return width;
}



static int getAddress(int lineNum, line* line, unsigned int width, char* filename)
{


    if(strcmp(line->head->value, "ADDRESS")!=0)
    {
        printf("Error in '%s', Line %d: Expected ADDRESS declaration.\n%s", filename, lineNum, AddressExplain);
        return 0;
    }

    if(tokenCount(line) != 2)
    {
        printf("Error in '%s', Line %d: Wrong syntax for ADDRESS declaration. \n%s", filename, lineNum, AddressExplain); 
        return 0;  
    }

    token* addrTok = (line)->head->next; 
    if(!checkLiteral(&lineNum, addrTok, filename))
    {
        printf("%s", AddressExplain);
        return 0;
    }

    unsigned int address = processLiteral(addrTok);

    if(address==0 || address*width>32)
    {
        printf("Error in '%s', Line %d: ADDRESS of %d is invalid. The product of WIDTH and ADDRESS must be nonzero and less than 32.\n", filename, lineNum, address); 
        return 0;  
    }

    return address;
}



static language* processHeader(int* lineNum, FILE* file, char* filename)
{
    language* lang = malloc(sizeof(language));

    if(lang == NULL)
    {
        printf("malloc failed in processHeader...\n");
        return NULL;
    }

    lang->head = NULL;
    
    // ########### WIDTH #############
    line* line = NULL;
    if(GetTokensFromNextLine(&line, file, *lineNum, 0)==EOF)
    {
        printf("Error in '%s', Line %d: Expected WIDTH declaration. \n%s", filename, *lineNum+1, WidthExplain);
        
        freeLanguage(lang);
        freeLine(line);
        line = NULL;
        return NULL;
    }

    *lineNum = (line)->lineNum;

    lang->width = getWidth(*lineNum, line, filename);
    if(lang->width == 0)
    {
        freeLanguage(lang);
        freeLine(line);
        line = NULL;
        return NULL;
    }
  
    //printf("WIDTH: %d!\n", lang->width);
   
    //############## ADDRESS ##################
   
    freeLine(line);
    line = NULL;

    if(GetTokensFromNextLine(&line, file, *lineNum, 0)==EOF)
    {
        printf("Error in '%s', Line %d: Expected ADDRESS declaration. \n%s", filename, *lineNum+1, AddressExplain);
        
        freeLanguage(lang);
        freeLine(line);
        line = NULL;
        return NULL;
    }

    *lineNum = (line)->lineNum;


    lang->address = getAddress(*lineNum, line, lang->width, filename);
    if(lang->address == 0)
    {
        freeLanguage(lang);
        freeLine(line);
        line = NULL;
        return NULL;
    }
  
    //printf("ADDRESS: %d!\n", lang->address);
    freeLine(line);
    return lang;

}





static symbol* readSymbol(line* line, char* filename)
{
    if(strcmp(line->head->value, "SYMBOL")!=0)
    {
        printf("Error in '%s', Line %d: Unknown declaration '%s'. Valid declarations are: SYMBOL.\n", filename, line->lineNum, line->head->value);
        return NULL;
    }

    if(tokenCount(line)!=3)
    {
        printf("Error in '%s', Line %d: Wrong syntax for SYMBOL declaration.\n%s", filename, line->lineNum, SymbolExplain);
        return NULL;
    }

    token* name = line->head->next;
    token* value =name->next;

    symbol* sym = malloc(sizeof(symbol));

    if(sym==NULL)
    {
        printf("Malloc failed in readSymbol.\n");
        return NULL;
    }

    sym->next = NULL;

    if(isLiteral(name))
    {
        printf("Error in '%s', Line %d: Wrong syntax for SYMBOL declaration. Expected a name, not '%s'\n%s", filename, line->lineNum, name->value, SymbolExplain);
        free(sym);
        return NULL;
    }

    if(*(name->value) == '.')
    {
        printf("Error in '%s', Line %d: Wrong syntax for SYMBOL declaration. Symbol name '%s' cannot begin with a '.'. \n%s", filename, line->lineNum, name->value, SymbolExplain);
        free(sym);
        return NULL;
    }

    sym->name = strdup(name->value);

    if(sym->name == NULL)
    {
        printf("strdup failed in readSymbol\n");
        free(sym);
        return NULL;
    }

    
    if(!checkLiteral(&(line->lineNum), value, filename))
    {
        printf("%s", SymbolExplain);
        free(sym->name);
        free(sym);
        return NULL;
    }

    sym->value = processLiteral(value);

    return sym;

    
}


static int readSymbols(int* lineNum, FILE* file, language* lang, char* filename)
{
    
   
    line* line = NULL;
    symbol* last = NULL;

    while(GetTokensFromNextLine(&line, file, *lineNum, 0)!=EOF)
    {
       
        
        *lineNum = line->lineNum;

        symbol* sym = readSymbol(line, filename);

        if(sym == NULL)
        {
            // we can assume that the line that failed was freed properly,
            // and lang is in a valid state.
            freeLine(line);
            freeLanguage(lang);
            return 0;
        }

        if(last==NULL)
        {
            lang->head = sym;
        }
        else
        {
            last->next = sym;
        }

        last = sym;

        freeLine(line);
        line = NULL;
    }

    return 1;
   
}


language* readDefines(char* fileName)
{
    FILE* definesFile = fopen(fileName, "r");
    
    // fopen sets errno, but I'm lazy.
    if(definesFile == NULL)
    {
        printf("Language definition '%s' could not be opened. Does the file exist?\n", fileName);
        // really any number of things could've gone wrong, but meh.
        // since when does malloc fail, right?
        return NULL;
    }

    int lineNum = 0;
    language* toReturn = processHeader(&lineNum, definesFile, fileName);
    if(toReturn==NULL)
    {
        fclose(definesFile);
        return NULL;
    }

    if(!readSymbols(&lineNum, definesFile, toReturn, fileName))
    {
        fclose(definesFile);
        return NULL;
    }    
    
    fclose(definesFile);
    return toReturn;


}


