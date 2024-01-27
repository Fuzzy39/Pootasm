make: Source/*/* Source/*
	gcc -g -o0 -Wall Source/*.c Source/Parsing/*.c -o pootasm.exe -std=c99 -Werror -lm -fsanitize=undefined