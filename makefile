sourceRoot=$(wildcard Source/*.c)
sourceOthers=$(wildcard Source/*/*.c)
args =  -g -o0 -Wall -o pootasm.exe -std=c99 -lm

make: Source/*/* Source/*
	gcc $(sourceRoot) $(sourceOthers) $(args) -fsanitize=undefined


# by default, at least with mingw, the undefined behavoir santitizer 
# isn't included, so we have a different target or whatever it's called
win:  Source/*/*
	gcc $(sourceRoot) $(sourceOthers) $(args)

# release version?
release: Source/*/*
	gcc $(sourceRoot) $(sourceOthers) -s -o pootasm.exe -std=c99 -lm
