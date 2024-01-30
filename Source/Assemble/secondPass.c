#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "../Headers/pootasm.h"


static int getWord(unsigned int packed, int bits, int index)
{
    unsigned int mask = 1;
    for(int i = 1; i<bits; i++)
    {
        mask = mask << 1;
        mask++;
    }

    // then shift the mask as appropriate
    for(int i = 0; i<index; i++)
    {
        mask = mask << bits;
    }

    unsigned int unpacked = packed & mask;

    // shift the result back.
    for(int i = 0; i<index; i++)
    {
        unpacked= unpacked >> bits;
    }

    return unpacked;

}

static bool delabelSection(output* out, section* sect, char* filename)
{
    chunk* ch = malloc(sizeof(ch));
    if(ch == NULL)
    {
        printf("Malloc failed in delabelSection.");
        return false;
    }


    ch->next = NULL;
    ch->label = NULL;
    ch->labelLineNumber=0;
    ch->length = EndOfSection(out->lang->address, sect)-sect->location;
    ch->bytes = ch->length*sizeof(int);
    ch->data = malloc(ch->bytes);

    if(ch->data == NULL)
    {
        printf("Malloc failed in delabelSection.");
        return false;
    }

    chunk* current = sect->head;
    unsigned int* data = ch->data;
    while(current!=NULL)
    {
        memcpy(data, current->data, current->length);
        data += current->bytes;
        
        // process labels, if possible.
        if(current->label != NULL)
        {
            symbol* label = findSymbol(out->labels, current->label);
            if(label==NULL)
            {
                printf("Error in '%s', Line %d: Unkown symbol '%s'\n", filename, current->labelLineNumber, current->label);

            }
            // label found.
            int packed = label->value;
            for(int i = 0; i<out->lang->address; i++)
            {
                *data= getWord(packed, out->lang->width, i);
                data+=sizeof(int);
            }
        }

        chunk* next = ch->next;
        free(current->data);
        free(current);
        current = next;
    }

    sect->head = ch;
    return true;
}




output* decodeLabels(output* partial, char* filename)
{
    section* sect = partial->head;
    while(sect!=NULL)
    {
        if(!delabelSection(partial, sect, filename))
        {
            return NULL;
        }
        sect = sect->next;
    }

    return partial;
}


