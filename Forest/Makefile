CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wvla -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

DISPLAY= neillsimplescreen

all: forest forest_s forest_v

forest : $(DISPLAY).c $(DISPLAY).h forest.c
	$(CC) forest.c $(DISPLAY).c -o forest $(PRODUCTION)

forest_s : $(DISPLAY).c $(DISPLAY).h forest.c
	$(CC) forest.c $(DISPLAY).c -o forest_s $(SANITIZE)

forest_v : $(DISPLAY).c $(DISPLAY).h forest.c
	$(CC) forest.c $(DISPLAY).c -o forest_v $(VALGRIND)

clean :
	rm -f forest forest_s forest_v *.txt

run : forest forest_s forest_v
	./forest 2>output.txt
	./forest_s 2>sanitizer_output.txt
	valgrind ./forest_v 2>valgrind_output.txt