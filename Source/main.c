#include <stdio.h>
#include "define.h"
#include "assemble.h"
#include "parsing/parsing.h"

void define()
{
    language* lang = readDefines("test.txt");
    printLanguage(lang);
    freeLanguage(lang);
}


int main(void)
{
    
    output* out = decodeSymbols("test.txt");
    debugPrintOutput(out, 'X');
    freeOutput(out);

}