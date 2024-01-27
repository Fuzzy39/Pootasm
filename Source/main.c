#include <stdio.h>
#include "define.h"
#include "assemble.h"
#include "Parsing/parsing.h"

// edited in linux

void define()
{
    language* lang = readDefines("DEF.TXT");
    printLanguage(lang);
    freeLanguage(lang);
}


int main(void)
{
    //define();
    output* out = firstPass("test.txt");
    debugPrintOutput(out, 'D');
    freeOutput(out);

}