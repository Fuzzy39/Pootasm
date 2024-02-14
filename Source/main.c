#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>
#include <string.h>
#include "Headers/pootasm.h"


static char* dec = "Decimal";
static char* bin = "Binary";
static char* hex = "Hexidecimal";
static char* oct = "Octal";

static char* baseName(char base)
{
    switch (base)
    {
    case 'D':
        return dec;
    case 'X':
        return hex;
    case 'B':
        return bin;
    case 'O':
        return oct;
    default:
        return NULL;
    }   
}


static void assemble(char* filename, char* outputfile, bool displayOutput, char base, int padding)
{
    char* defOut = "out.bin";
    if(outputfile == NULL)
    {
        outputfile = defOut;
    }

    base = toupper(base);
    if(radixFromBase(base)==-1)
    {
        printf("Error: invalid base '%c' for output. Expected 'B', 'O', 'D', or 'X'.", base);
        return;
    }
    

    output* out = firstPass(filename);
    out = decodeLabels(out, filename);
    out = finalizeOutput(out, filename);

    if( out!=NULL)
    {
       
        if(displayOutput)
        {
            printf("######## OUTPUT (for debugging purposes) ########\n");
            printf("All numbers in %s unless otherwise noted.\n\n", baseName(base));
            debugPrintOutput(out, base);
        }
        
        writeBinary(out, padding, outputfile);
        printf("Output saved in '%s'.\n", outputfile);
        freeOutput(out);
    }
}


void printHelp()
{
    printf("Pootasm Usage:\npootasm _input_file_ [-hvb:o:p::]\n\n-h: Show this information.\n"
            "-v: Print a detailed description of the program's output, including a list of symbols and labels.\n"
            "-b_base_: Which base to display numbers in, if v is used. Valid arguments are 'B' (binary), 'O' (octal), 'D' (decimal), and 'X' hexidecimal. Default is 'D'.\n"
            "-o _output_file_: The filename that output should be directed to. Default is 'bin.out'.\n"
            "-p[padding]: The amount of bytes each word should be padded to in the output. Valid values are 0-4. 4 is default if used, if not used, 0 is the default value.\n"
            "   0 and values smaller than the word size are treated as the minimum value of applicable padding.\n");
}


int main(int argc, char* argv[])
{
    char* inputName = NULL;
    bool printOutput = false;
    char base = 'D';
    int padding = 0;

    // first, get the input filename.

    if(argc<2)
    {
        printf("Error: missing input file.\n");
        return 1;
    }

    if(strcmp(argv[1], "-h") == 0)
    {
        printHelp();
        return 0;
    }

    inputName = argv[1];

    // set up getopt
    optind = 2;
    int optReturn = 0;
    int outputind = -1;

    while (true)
    {
        optReturn = getopt(argc, argv, "vp::b:oh");
       

        if(optReturn == '?' || optReturn == ':')
        {
            return 1;
        }

        if(optReturn == -1)
        {
            break;
        }
        

        if(optind == outputind)
        {
            printf("Error: -o must be proceeded by a filename.\n");
            return 1;
        }


        // now we know this argument is at least mostly valid.

        if(optReturn == 'v')
        {
            printOutput = true;
            continue;
        }

        if(optReturn == 'h')
        {
            printHelp();
            return 0;
        }

        if(optReturn == 'o')
        {
            outputind = optind++;
            continue;
        }

        if(optReturn == 'p')
        {
            if(optarg==NULL)
            {
                
                padding = 4;
                continue;
                
            }

            if(optarg[1]!='\0')
            {
                printf("Error: invalid value for -p. Allowed values are 0-4.");
                return 1;
            }

            int value = optarg[0]-'0';
            if(value<0 || value>4)
            {
                printf("Error: invalid value for -p. Allowed values are 0-4.");
                return 1;
            }

            padding = value;
            
            continue;
        }

        if(optReturn == 'b')
        {

            if(!printOutput)
            {
                printf("Error: -b can only be used after -v.\n");
                return 1;
            }

            if(optarg[1]!='\0')
            {
                printf("Error: invalid value for -b. Expected 'B', 'O', 'D', or 'X'.");
                return 1;
            }

            base = optarg[0];
            continue;

        }

    } 
   


   
    
    // v - print debug
    // p - padding in output
    // b - base used
    // o - output
    // h - help

    if(outputind >= argc)
    {
        printf("Error: -o must be proceeded by a file name.\n");
        return 1;
    }


    char* outputName = NULL;
    
    if(outputind!=-1)
    {
        outputName = argv[outputind];

    }

    
    if(argc-1>=optind)
    {
        printf("Error: spurrious arguments. You can use 'pootasm -h' for a description of usage.\n");
        return 1;
    }

    printf("padding: %d\n", padding);

   assemble(inputName, outputName, printOutput, base, padding);
}
