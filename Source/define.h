#ifndef DEFINE_H   /* Include guard */
#define DEFINE_H

struct language;
struct symbol;
struct token;

typedef struct language language;
typedef struct symbol symbol;
typedef struct token token;



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

struct token
{
    char* value;
    token* next;
};


symbol* findSymbol(language* lang, char* name);

language* readDefines(char* fileName);



#endif 