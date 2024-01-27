#include <stdio.h>
#include "define.h"
#include "assemble.h"
#include "parsing/parsing.h"

// edited in linux

void define()
{
    language* lang = readDefines("test.txt");
    printLanguage(lang);
    freeLanguage(lang);
}


int main(void)
{
    
    output* out = firstPass("test.txt");
    debugPrintOutput(out, 'D');
    freeOutput(out);

}