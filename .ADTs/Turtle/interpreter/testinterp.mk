CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
UNIT_TESTING = -fprofile-arcs -ftest-coverage -lcheck -lpthread -lrt -lsubunit -lm

testinterp_s : interpreter.c interpreter.h  ../parser/parser.h ../parser/parser.c ../Realloc/realloc.c ../Realloc/realloc.h ../../General/general.c ../../General/general.h ../../Stack/stack.h ../../Stack/Linked/specific.h ../../Stack/Linked/linked.c
	$(CC) ../parser/parser.c interpreter.c ../Realloc/realloc.c ../../General/general.c ../../Stack/Linked/linked.c -o testinterp_s $(UNIT_TESTING) $(SANITIZE) -DTEST_INTERP -DINTERP

testreport:
	./testinterp_s
	gcovr -r . --html --html-details -o coverage_report.html

clean:
	rm -f testinterp_s *.html *.gcno *.gcda
