#pragma once
#include "../../.ADTs/General/general.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#define PROGRAM_TYPE "Realloc"
#define SCALE_FACTOR 2
#define INITIAL_CAPACITY 16
#define MAXWORDSIZE 51


#ifdef ADDSDL
#include "../neillSDL/neillsdl2.h"
#endif
#ifdef INTERP
#include "../../.ADTs/Stack/stack.h"
#include "../../.ADTs/Stack/Linked/specific.h"
#define NO_Of_VAR 26

struct Node {
  bool set;
  double number;
};
typedef struct Node Node;

struct Turtle {
  double x,y,angle;
};
typedef struct Turtle Turtle;
#endif

struct Program {
  unsigned int currentWord,size,capacity;
  void ** wordsArray;

  #ifdef INTERP
  Node varArray[NO_Of_VAR];
  Turtle turtle;
  stack * s;
  #endif
  #ifdef ADDSDL
  SDL_Simplewin sw;
  #endif
};
typedef struct Program Program;

Program * programInit();
void programFree(Program * prog);
unsigned int programSize(const Program * prog);
void programInsert(Program * prog, const char * word);
