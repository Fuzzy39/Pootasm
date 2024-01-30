#include <stdio.h>
#include <math.h>
#include "../Headers/pootasm.h"


static void freeSection(section* sect)
{
    // basically we just need to free chunks and then ourself.
    // we expect the caller to keep a hold of the next element.
    chunk* ch = sect->head;
    while(ch != NULL)
    {
        free(ch->data);
        free(ch->label);
        chunk* next = ch->next;
        free(ch);
        ch = next;
    }
    free(sect);
}

void freeOutput(output* output)
{
    if(output==NULL)
    {
        return;
    }

    freeLanguage(output->lang);
    symbol* sym = output->labels;

    while(sym != NULL)
    {
        free(sym->name);
        symbol* next = sym->next;
        free(sym);
        sym = next;
    }

    // free sections
    section* sect = output->head;
    while(sect != NULL)
    {
        section* next = sect->next;
        freeSection(sect);
        sect = next;
    }

    free(output);
}


static void printChunk(language* lang, chunk* ch, char base)
{
    // Okay, it's been a couple days, what was I doing?
    // first print the data

    printf("\t");
    for(int i = 0; i<ch->length; i++)
    {
        int word = ch->data[i];
        if(!printNumber(word, lang->width, base, stdout))
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


    printf("Section @0x%X", sect->location);
    // this is 32 bits since an address can be any size, really.
  
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
        if(!printNumber(lab->value, out->lang->width*out->lang->address, base, stdout))
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


