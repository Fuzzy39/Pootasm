sourceRoot=$(wildcard Source/*.c)
sourceOthers=$(wildcard Source/*/*.c)
args =  -g -o0 -Wall -o pootasm.exe -std=c99 -lm

make: Source/*/* Source/*
	gcc $(sourceRoot) $(sourceOthers) $(args) -fsanitize=undefined

# you will need some sort of shell like bash to run this still...
win:  Source/*/*
	gcc $(sourceRoot) $(sourceOthers) $(args)