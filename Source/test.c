#include <stdio.h>
#include "define.h"
#include "parsing/parsing.h"

void define()
{
    language* lang = readDefines("test.txt");
    printLanguage(lang);
    freeLanguage(lang);
}


int main(void)
{
    //literal();
    define();
}