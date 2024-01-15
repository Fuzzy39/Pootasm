make: Source/define.c Source/literal.c Source/test.c Source/token.c
	gcc -g -Wall Source/define.c Source/literal.c Source/test.c Source/token.c -o pootasm.exe