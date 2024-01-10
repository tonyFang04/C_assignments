CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

SDLFLAGS = `sdl2-config --cflags`
SDLLIBS = `sdl2-config --libs`

all: parse interp interp_s extend

parse: main.c Realloc/realloc.h Realloc/realloc.c ../.ADTs/General/general.h ../.ADTs/General/general.c readTTL/readTurtleFile.h readTTL/readTurtleFile.c parser/parser.h parser/parser.c
	$(CC) main.c Realloc/realloc.c ../.ADTs/General/general.c readTTL/readTurtleFile.c parser/parser.c -o parse $(SANITIZE) $(LDLIBS) -DPARSER

interp_s: main.c Realloc/realloc.h Realloc/realloc.c ../.ADTs/General/general.h ../.ADTs/General/general.c readTTL/readTurtleFile.h readTTL/readTurtleFile.c parser/parser.h parser/parser.c interpreter/interpreter.h interpreter/interpreter.c ../.ADTs/Stack/stack.h ../.ADTs/Stack/Linked/specific.h ../.ADTs/Stack/Linked/linked.c
	$(CC) main.c Realloc/realloc.c ../.ADTs/General/general.c readTTL/readTurtleFile.c parser/parser.c interpreter/interpreter.c ../.ADTs/Stack/Linked/linked.c -o interp_s $(SANITIZE) $(LDLIBS) -DINTERP

interp: main.c Realloc/realloc.h Realloc/realloc.c ../.ADTs/General/general.h ../.ADTs/General/general.c readTTL/readTurtleFile.h readTTL/readTurtleFile.c parser/parser.h parser/parser.c interpreter/interpreter.h interpreter/interpreter.c ../.ADTs/Stack/stack.h ../.ADTs/Stack/Linked/specific.h ../.ADTs/Stack/Linked/linked.c neillSDL/neillsdl2.h neillSDL/neillsdl2.c
	$(CC) main.c Realloc/realloc.c ../.ADTs/General/general.c readTTL/readTurtleFile.c parser/parser.c interpreter/interpreter.c ../.ADTs/Stack/Linked/linked.c neillSDL/neillsdl2.c -o interp $(PRODUCTION) $(LDLIBS) -DINTERP -DADDSDL $(SDLFLAGS) $(SDLLIBS)

extend:  main.c extension/postfix.h extension/postfix.c  Realloc/realloc.h Realloc/realloc.c ../.ADTs/General/general.h ../.ADTs/General/general.c readTTL/readTurtleFile.h readTTL/readTurtleFile.c parser/parser.h parser/parser.c interpreter/interpreter.h interpreter/interpreter.c ../.ADTs/Stack/stack.h ../.ADTs/Stack/Linked/specific.h ../.ADTs/Stack/Linked/linked.c neillSDL/neillsdl2.h neillSDL/neillsdl2.c
	$(CC) main.c extension/postfix.c Realloc/realloc.c ../.ADTs/General/general.c readTTL/readTurtleFile.c parser/parser.c interpreter/interpreter.c ../.ADTs/Stack/Linked/linked.c neillSDL/neillsdl2.c -o extend $(PRODUCTION) $(LDLIBS) -DINTERP -DADDSDL -DEXTENSION $(SDLFLAGS) $(SDLLIBS)

clean:
	rm -f parse interp_s interp extend
