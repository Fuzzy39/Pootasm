#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../Headers/pootasm.h"


// we just need to check none of the sections are overflowing. should be easy enough. 
output* finalizeOutput(output* partial, char* filename)
{
    section *prev = partial->head;

    // I'm not certain weather this can happen, but may as well check.
    if(prev == NULL)
    {
       return partial;
    }
    
    
    section* next = prev->next;

    while(next != NULL)
    {
        if(EndOfSection(partial->lang->address, prev)>next->location)
        {
            printf("Error in '%s'. Section at 0x", filename);
            printNumber(next->location, 32, 'X', stdout);
            printf(" is overwritten by the previous section.\n");
            freeOutput(partial);
            return NULL;
        }
        prev = next;
        next = next->next;
    }

    return partial;
}
