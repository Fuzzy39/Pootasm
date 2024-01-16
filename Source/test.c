#include <stdio.h>
#include "define.h"
#include "parsing/parsing.h"

// void literal(void)
// {
//     char input[100];
//     printf("This program will calculate numerical literals in decimal '200', binary '0b1101', ocatal '0o7321' and hexadecimal '0xFF36'\n");
//     while(1)
//     {
//         if(!scanf("%100s", &input[0]))
//         {
//             continue;
//         }

//         if(!isLiteral(input))
//         {
//             printf("Invalid literal.\n");
//             continue;
//         }

//         if(!isLiteralInBounds(input, 32))
//         {
//             printf("Value too large.\n");
//             continue;
//         }

//         unsigned int value = processLiteral(input);
     
//         printf("Signed 32-bit Decimal Value: %d\n", (int)value);

//     }
// }

void define()
{
    readDefines("test.txt");
}


int main(void)
{
    //literal();
    define();
}