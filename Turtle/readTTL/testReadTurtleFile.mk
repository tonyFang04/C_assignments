CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
UNIT_TESTING = -fprofile-arcs -ftest-coverage -lcheck -lpthread -lrt -lsubunit -lm

testreadttl_s : readTurtleFile.c readTurtleFile.h ../Realloc/realloc.h ../Realloc/realloc.c ../../General/general.h ../../General/general.c
	$(CC) readTurtleFile.c ../Realloc/realloc.c ../../General/general.c -o testreadttl_s -DTEST_READTURTLEFILE $(SANITIZE) $(UNIT_TESTING)

testreport:
	./testreadttl_s
	gcovr -r . --html --html-details -o coverage_report.html

clean:
	rm -f testreadttl_s *.html *.gcno *.gcda 101
