CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
UNIT_TESTING = -fprofile-arcs -ftest-coverage -lcheck -lpthread -lrt -lsubunit -lm

testrealloc_s : realloc.h realloc.c ../../General/general.h ../../General/general.c
	$(CC) realloc.c ../../General/general.c -o testrealloc_s -DTEST_REALLOC $(SANITIZE) $(UNIT_TESTING)

testreport:
	./testrealloc_s
	gcovr -r . --html --html-details -o coverage_report.html

clean:
	rm -f testrealloc_s testrealloc *.html *.gcno *.gcda 101
