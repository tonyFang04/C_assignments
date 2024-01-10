CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
UNIT_TESTING = -fprofile-arcs -ftest-coverage -lcheck -lpthread -lrt -lsubunit -lm

testextension_s : postfix.c postfix.h  ../parser/parser.h ../parser/parser.c ../interpreter/interpreter.h ../interpreter/interpreter.c ../Realloc/realloc.c ../Realloc/realloc.h ../../General/general.c ../../General/general.h ../../Stack/stack.h ../../Stack/Linked/specific.h ../../Stack/Linked/linked.c
	$(CC) postfix.c ../parser/parser.c ../interpreter/interpreter.c ../Realloc/realloc.c ../../General/general.c ../../Stack/Linked/linked.c -o testextension_s $(UNIT_TESTING) $(SANITIZE) -DTEST_POSTFIX -DINTERP -DEXTENSION

report:
	./testextension_s
	gcovr -r . --html --html-details -o coverage_report.html

clean:
	rm -f testextension_s *.html *.gcno *.gcda
