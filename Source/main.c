#include <stdio.h>
#include "define.h"
#include "assemble.h"
#include "Parsing/parsing.h"


// An edit to show that I configured git correctly
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
    printAsLiteral(12, 16, 'X', stdout);
    printf("\n");
}


int main(void)
{
   //printTest();
    assembleTest();
}