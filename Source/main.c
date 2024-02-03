#include <stdio.h>
#include "Headers/pootasm.h"



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
    out = decodeLabels(out, "test.txt");
    out = finalizeOutput(out, "test.txt");
    debugPrintOutput(out, 'D');
    freeOutput(out);
}

void printTest()
{
    printNumber(0x1FF, 9, 'X', stdout);
    printf("\n");

    printNumber(15, 4, 'X', stdout);
    printf("\n");

    printNumber(0x80000000, 32, 'B', stdout);
    printf("\n");

    printNumber(-1, 32, 'B', stdout);
    printf("\n");

    printNumber(0xFFFF, 16, 'D', stdout);
    printf("\n");

    printNumber(300, -1, 'X', stdout);
    printf("\n");

    printNumber(0, -1, 'X', stdout);
    printf("\n");
    
}




int main(void)
{

    
    //printTest();
    assembleTest();
}