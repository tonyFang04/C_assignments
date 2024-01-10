CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
UNIT_TESTING = -fprofile-arcs -ftest-coverage -lcheck -lpthread -lrt -lsubunit -lm
SDLFLAGS = `sdl2-config --cflags`
SDLLIBS = `sdl2-config --libs`
testparser_s : parser.c parser.h ../Realloc/realloc.h ../Realloc/realloc.c ../../General/general.h ../../General/general.c ../readTTL/readTurtleFile.h ../readTTL/readTurtleFile.c
	$(CC) parser.c  ../readTTL/readTurtleFile.c ../Realloc/realloc.c ../../General/general.c -o testparser_s -DPARSER -DTEST_PARSER $(SANITIZE) $(UNIT_TESTING)

testinterp_s : testinterp.c ../interpreter/interpreter.c ../interpreter/interpreter.h parser.c parser.h ../Realloc/realloc.h ../Realloc/realloc.c ../../General/general.h ../../General/general.c ../readTTL/readTurtleFile.h ../readTTL/readTurtleFile.c ../../Stack/stack.h ../../Stack/Linked/specific.h ../../Stack/Linked/linked.c
	$(CC) testinterp.c ../interpreter/interpreter.c parser.c  ../readTTL/readTurtleFile.c ../Realloc/realloc.c ../../General/general.c ../../Stack/Linked/linked.c -o testinterp_s -DINTERP $(SANITIZE) $(UNIT_TESTING)

testinterp_sdl : testinterp.c ../interpreter/interpreter.c ../interpreter/interpreter.h parser.c parser.h ../Realloc/realloc.h ../Realloc/realloc.c ../../General/general.h ../../General/general.c ../readTTL/readTurtleFile.h ../readTTL/readTurtleFile.c ../../Stack/stack.h ../../Stack/Linked/specific.h ../../Stack/Linked/linked.c ../neillSDL/neillsdl2.c ../neillSDL/neillsdl2.h
	$(CC) testinterp.c ../neillSDL/neillsdl2.c ../interpreter/interpreter.c parser.c  ../readTTL/readTurtleFile.c ../Realloc/realloc.c ../../General/general.c ../../Stack/Linked/linked.c -o testinterp_s -DINTERP -DADDSDL $(DEBUG) $(UNIT_TESTING) $(SDLLIBS) $(SDLFLAGS)


parser_test_report:
	./testparser_s
	gcovr -r . --html --html-details -o coverage_report.html

interp_test_report:
	./testinterp_s
	gcovr -r . --html --html-details -o coverage_report.html

clean:
	rm -f testparser_s testinterp_s *.html *.gcno *.gcda 101
