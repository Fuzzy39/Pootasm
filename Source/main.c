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
    //define();

    printAsLiteral(17, 5, 'X', stdout); // should produce "11"
    printf("\n");
    printAsLiteral(255, 8, 'X', stdout); // should produce "FF"
    printf("\n");
    printAsLiteral(255, 12, 'X', stdout); // should produce "0FF"
    printf("\n");
    
    printAsLiteral(36, 8, 'D', stdout); // should produce "036"
    printf("\n");

    printAsLiteral(31, 8, 'B', stdout); // should produce "00011111"
    printf("\n");

}