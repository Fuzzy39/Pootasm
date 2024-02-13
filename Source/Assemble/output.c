#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "../Headers/pootasm.h"


static void PadUntilAddress(int* current, int addr, int padding, FILE* stream)
{
    while(*current<addr)
    {
        for(int i = 0; i<padding; i++)
        {
            putc(0, stream);
        }
        (*current)++;
    }
}

static void WriteWord(int index, int pads,  int words, section* sect, int padding, FILE*stream)
{
   

    // do blank pads
    for(int i =0; i<pads; i++)
    {
         putc(0, stream);
    }

    unsigned int num = sect->head->data[index];
    

    // split into bytes
    unsigned char bytes[4];
    bytes[0] = (num>>24)&0xFF;
    bytes[1] = (num>>16)&0xFF;
    bytes[2] = (num>>8)&0xFF;
    bytes[3] = (num)&0xFF;

    for(int i=3; i>=4-words; i--)
    {
        putc(bytes[i], stream);
    }
}


static void WriteSection(int* current, language* lang, section* sect, int padding, FILE* stream)
{
    int addr = EndOfSection(0, sect);
    int i =0;

    int words = (lang->width/8);
    if(lang->width%8!=0)
    {
        words++;
    }
    int pads = padding-words;

    while(*current<addr)
    {
        WriteWord(i, pads, words, sect, padding, stream);
        (*current)++;
        i++;
    }
}


static void writeOutput(output* out, int padding, FILE* stream)
{
    if(padding > 4)
    {
        padding = 4;
    }

    // ensure padding is large enough.
    int widthBits =out->lang->width; 
    if(padding*8< widthBits)
    {
        padding = widthBits/8;
        if(widthBits%8!=0)
        {
            padding++;
        }
    }


    int current = 0;

    section* sect = out->head;

    while(sect!=NULL)
    {
        PadUntilAddress(&current, sect->location, padding, stream);
        WriteSection(&current, out->lang, sect, padding, stream);

        sect = sect->next;
    }



}



void writeBinary(output* out, int padding, char* filename)
{
    if(access(filename, F_OK))
    {
        // file exists
        remove(filename);
    }

    FILE* file = fopen(filename, "w");
    if(file== NULL)
    {
        printf("Something went wrong. Couldn't  make file '%s'\n", filename);
        return;
    }

    writeOutput(out, padding, file);
    fclose(file);

}