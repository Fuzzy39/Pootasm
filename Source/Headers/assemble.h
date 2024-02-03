#ifndef ASSEMBLE_H   /* Include guard */
#define ASSEMBLE_H

#include "language.h"
#include <stdio.h>

struct output;
struct section;
struct chunk;

typedef struct output output;
typedef struct section section;
typedef struct chunk chunk;


struct output 
{
    // The goal of the first pass is to fill this data structure.
    // the goal of the second pass is to compress every section into a single chunk (by resolving labels)
    language* lang;
    symbol* labels;
    
   
    section* head;
};

struct section
{
    int location;
    section* next;
    chunk* head;
};

/// @brief A chunk of binary data with a symbol on the end.
struct chunk
{
    size_t bytes;
    unsigned int length; // The length, in dwords, of data this chunk contains. 
    unsigned int* data; 
    int labelLineNumber; // the line number the label was on.
    char* label;            // optionally(for the last chunk), an unknown label after the end of the data.
    chunk* next;
};

// NOTE TO SELF:
// Keep in mind that the size of labels is dependent on the ADDRESS of the language. the value of a label can always fit in one int,
// but will actually be in as many ints as ADDRESS is for the language.



int EndOfSection(int addrWidth, section* sect);

output* firstPass(char* filename);

output* decodeLabels(output* partial, char* filename);

output* finalizeOutput(output* partial, char* filename);

void debugPrintOutput(output* out, char base);

/// @brief 
/// @param out 
/// @param padding 
// @param stream 
void writeBinary(output* out, int padding, char* filename);

void freeOutput(output* output);

#endif
