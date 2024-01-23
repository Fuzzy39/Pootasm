make: Source/*/* Source/*
	gcc -g -Wall Source/*.c Source/Parsing/*.c -o pootasm.exe -std=c99