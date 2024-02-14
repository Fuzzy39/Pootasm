# Pootasm
A very simple generic assembler that can be adapted for different processors, at least theoretically.
Realistically, it is not particularly usable for any real processor. It was created mainly as a project to become more familiar with C.

## Command Line Usage
**pootasm** *input_file* [**\-hvb**:**o**:**p**::]
    + **-v**: Print a detailed description of the program's output, including a list of symbols and labels.
    + **-b***base*: Which base to display numbers in, if v is used. Valid arguments are 'B' (binary), 'O' (octal), 'D' (decimal), and 'X' hexidecimal. Default is 'D'.
    + **-o** *output_file*: The filename that output should be directed to. Default is 'bin.out'.
    + **-p**[*\padding*]: The amount of bytes each word should be padded to in the output. Valid values are 0-4. 4 is default if used, if not used, 0 is the default value. 0 and values smaller than the word size are treated as the minimum value of applicable padding.
