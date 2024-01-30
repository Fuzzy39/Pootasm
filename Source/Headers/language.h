#ifndef DEFINE_H   /* Include guard */
#define DEFINE_H

struct language;
struct symbol;


typedef struct language language;
typedef struct symbol symbol;



struct symbol
{
    char* name;
    unsigned int value;
    symbol* next;
};

struct language
{
    unsigned int width;
    unsigned int address; 
    symbol* head;
};

/// @brief 
/// @param lang 
/// @param name 
/// @return 
symbol* findSymbol(language* lang, char* name);

int symbolCount(symbol* head);

void printLanguage(language* lang);

void freeLanguage(language* lang);

language* readDefines(char* fileName);



#endif 