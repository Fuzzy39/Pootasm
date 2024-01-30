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
    printAsLiteral(0x1FF, 9, 'X', stdout);
    printf("\n");

    printAsLiteral(15, 4, 'X', stdout);
    printf("\n");

    printAsLiteral(0x80000000, 32, 'B', stdout);
    printf("\n");

    printAsLiteral(-1, 32, 'B', stdout);
    printf("\n");

    printAsLiteral(0xFFFF, 16, 'D', stdout);
    printf("\n");

    printAsLiteral(300, -1, 'X', stdout);
    printf("\n");
    
}


int main(void)
{

    
    printTest();
    //assembleTest();
}