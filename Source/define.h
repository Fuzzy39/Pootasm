#ifndef DEFINE_H   /* Include guard */
#define DEFINE_H

struct language;
struct symbol;


typedef struct language language;
typedef struct symbol symbol;



struct symbol
{
    char* name;
    int value;
    symbol* next;
};

struct language
{
    int width;
    int address; 
    int ORacrossLines;
    symbol* symbol;
};


symbol* findSymbol(language* lang, char* name);

language* readDefines(char* fileName);



#endif 