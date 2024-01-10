CC=gcc-10
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS =


testrealloc : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../General/general.h ../General/general.c
	$(CC) testboolarr.c Realloc/realloc.c ../General/general.c -o testrealloc -I./Realloc $(PRODUCTION) $(LDLIBS)


testrealloc_s : boolarr.h Realloc/specific.h Realloc/realloc.c testboolarr.c ../General/general.h ../General/general.c
	$(CC) testboolarr.c Realloc/realloc.c ../General/general.c -o testrealloc_s -I./Realloc $(SANITIZE) $(LDLIBS)
