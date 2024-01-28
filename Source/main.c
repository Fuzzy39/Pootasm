#include <stdio.h>
#include "define.h"
#include "assemble.h"
#include "Parsing/parsing.h"


void define()
{
    language* lang = readDefines("DEF.TXT");
    printLanguage(lang);
    freeLanguage(lang);
}

void assembleTest()
{
     
    output* out = firstPass("test.txt");
    debugPrintOutput(out, 'D');
    freeOutput(out);
}

void printTest()
{
    
}


int main(void)
{
   

}