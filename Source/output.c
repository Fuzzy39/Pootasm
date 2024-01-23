#include "assemble.h"
#include "define.h"
#include "Parsing/parsing.h"
#include <stdio.h>
#include <math.h>




static void printChunk(language* lang, chunk* ch, char base)
{
    // Okay, it's been a couple days, what was I doing?
    // first print the data

    printf("\t");
    for(int i = 0; i<ch->length; i++)
    {
        int word = ch->data[i];
        if(!printAsLiteral(word, lang->width, base, stdout))
        {
            // error
            printf("?");
        }
        
        // completely arbitrary choice.
        if(i % 16 == 0)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }

    }

    if(ch->label == NULL)
    {
        printf("\n");
        return;
    }

    printf("\n'%s'", ch->label);

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
        printChunk(lang, chunk, base);
        chunk = chunk->next;
    }

}

static void printLabels(output* out, char base)
{
    if(out->labels == NULL)
    {
        printf("0 Labels.\n");
        return;
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


void debugPrintOutput(output* out, char base)
{
    if(out==NULL)
    {
        printf("Output NULL.\n");
        return;
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

    printf("End.\n");

    // and that's it! nice.
}



void writeOutput(output* out, int padding, FILE* stream)
{
    printf("writeOutput is not implemented\n");
}