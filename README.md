# Pootasm
A very simple generic assembler that can be adapted for different processors, at least theoretically.
Realistically, it is not particularly usable for any real processor. It was created mainly as a project to become more familiar with C.

## Building
Pootasm is a console application written in C, and should compile with the provided makefile using gcc. On Windows, make win should be used. By default, the makefile creates a debug build. use make release for a smaller binary on linux and windows.

Pootasm will certainly not function as expected if `sizeof(int)` is not 4.

In general, Pootasm is probably not the best constructed program, but hopefully it should work as expected.

## Command Line Usage
**pootasm** *input_file* \[**\-hvb**:**o**:**p**::]
+ **\-v**: Print a detailed description of the program's output, including a list of symbols and labels.
+ **\-b***base*: Which base to display numbers in, if v is used. Valid arguments are 'B' (binary), 'O' (octal), 'D' (decimal), and 'X' hexidecimal. Default is 'D'.
+ **\-o** *output_file*: The filename that output should be directed to. Default is 'bin.out'.
+ **\-p**[*\padding*]: The amount of bytes each word should be padded to in the output. Valid values are 0-4. 4 is default if used, if not used, 0 is the default value. 0 and values smaller than the word size are treated as the minimum value of applicable padding.

The input_file should be a path to the assembly file of your program.

## Using Pootasm

A valid program for Pootasm consists of two files, the Assembly File, which contains directives, symbols, literals and labels that are translated into machine code, and a language file, which provides symbol definitions and defines the size of labels and words.


### literals
literals are *literal* number values. Numbers can be in binary, octal, decimal or hexidecimal, and may be signed. Unless there are additional restrictions, literals may not be less than `-(2^31)+1` and may not be greater than `(2^32)-1`. Decimal literals are written as normal, e.g. `10`, `-38` or `0`. other bases are prefixed with 0 and a character, with 'b' being binary, 'o' being octal, and 'x' being hexidecimal.


### General Syntax
Pootasm ignores lines consisting entirely of whitespace, as well as any characters after a semicolon ';' in a line.

Pootasm is case insensitive.


### Language Definition Syntax
The language file describes the assembly language that your program will use. The assembly file defines a path to the langauge file.

The first line of the Language file specifies the width, in bits, of a word for the machine. It has a format **WIDTH** *literal*. 

The second line specifies the address size, in words, of a language. This defines how many words a label is converted to. It has a format **ADDRESS** *literal*.

All following lines describe symbols of the language, symbols are words that can be used in the assembly file, which will be converted to the literal associated with them. These lines have the fomrat **SYMBOL** *name* *literal*. names can be any string of non graphical ascii characters that do not start with a digit (0-9) or '+' or '-'.


### Assembly Syntax

The assembly file contains directives and assembly code, which is assembled into a binary format. 

#### Directives

The first line of an assembly file must have the .def directive. The .def directive has the format **.def** *filename*, and specifies a path to the language definition file. the filename is relative to the current working directory Pootasm was called from, and must not have multiple consecutive spaces.

The .org directive marks the beggining of an assembly section, and also defines the address that the assembly section will start from. It's format is **.org** *literal*. The literal is the Address that the section following it contains. each .org section must have a higher address than the previous.

#### Assembly
An Assembly secion can have two different types of lines:

Code, which consists of symbols, label names, and literals. these are converted into machine code in the output.

label definitions, which are of the form *name*: Names, like symbol names, can be any string of non graphical ascii characters that do not start with a digit (0-9) or '+' or '-'.




    

