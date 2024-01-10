CC=g++
COMMON= -Wall -Wextra -Wfloat-equal -Wvla -pedantic -std=c++20
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LIB= -ldlib

extension : main.cpp Parsing/commandLineArgumentChecking.cpp
	$(CC) main.cpp Parsing/commandLineArgumentChecking.cpp  -o extension $(SANITIZE) $(LIB) 
