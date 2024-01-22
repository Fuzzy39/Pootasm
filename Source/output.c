#include "assemble.h"
#include "define.h"
#include "parsing.h"
#include <stdio.h>
#include <math.h>




void debugPrintOutput(output* out, char base)
{
    if(out==NULL)
    {
        printf("Output NULL.\n");
    }

    printf( "Assembler Output:\n\n");

    printLanguage(out->lang);

    printf("\n");
    printLabels(out, base);

    // next up, the actual data.
    section* sect = out->head;
    while(sect!=NULL)
    {
        printSection(out->lang, sect, base);
        sect = sect->next;
    }

    // and that's it! nice.
}

static void printLabels(output* out, char base)
{
    if(out->labels == NULL)
    {
        printf("0 Labels.\n");
    }

    printf("%d Labels:\n", symbolCount(out->labels));

    // Now, print the labels.
    symbol* lab = out->labels;
    while (lab!=NULL)
    {
        printf("\t'%s': ",lab->name);
        if(!printAsLiteral(lab->value, out->lang->width*out->lang->address, base, stdout))
        {
            printf("ERROR");
        }
        printf("\n");
        lab = lab->next;
    }
    
}

static void printSection(language* lang, section* sect, char base)
{
    int addr = lang->width*lang->address;

    printf("Section @");
    if(!printAsLiteral(sect->location, addr, base, stdout))
    {
        printf("ERROR");
    }
    printf(":\n");

    // Now just print chunks. it's that easy!
    chunk* chunk = sect->head;
    while(chunk!=NULL)
    {
        printChunk()
    }

}

static void printChunk()


void writeOutput(output* out, int padding, FILE* stream)
{
    printf("writeOutput is not implemented\n");
}