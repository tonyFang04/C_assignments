CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wvla -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

DISPLAY= neillsimplescreen

all: bookcase bookcase_s bookcase_v

bookcase : $(DISPLAY).c $(DISPLAY).h bookcase.c
	$(CC) bookcase.c $(DISPLAY).c -o bookcase $(PRODUCTION)

bookcase_s : $(DISPLAY).c $(DISPLAY).h bookcase.c
	$(CC) bookcase.c $(DISPLAY).c -o bookcase_s $(SANITIZE)

bookcase_v : $(DISPLAY).c $(DISPLAY).h bookcase.c
	$(CC) bookcase.c $(DISPLAY).c -o bookcase_v $(VALGRIND)

clean :
	rm -f bookcase bookcase_s bookcase_v
