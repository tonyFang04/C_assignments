#include "parser/parser.h"

int main(int argc, char ** argv) {
  Program * p = programInit();

  #ifdef ADDSDL
  Neill_SDL_Init(&(p->sw));
  Neill_SDL_SetDrawColour(&(p->sw),255,255,255);
  #endif

  if (checkTerminalInputNumber(argc) == false) {
    fprintf(stderr,"Two inputs only\n");
    exit(EXIT_FAILURE);
  }

  if (checkFileName(argv[1]) == false) {
    fprintf(stderr,"Wrong file name\n");
    exit(EXIT_FAILURE);
  }
  if (readFile(p,argv[1]) == false) {
    exit(EXIT_FAILURE);
  }

  #ifdef EXTENSION
  while (*((char*)(p->wordsArray[p->currentWord])) != '\0') {
    if (strcmp(p->wordsArray[p->currentWord],":=") == 0) {
      p->currentWord++;
      infixToPostfix(p);
    }
    p->currentWord++;
  }
  p->currentWord = 0;
  #endif

  if (isMain(p) == false) {
    exit(EXIT_FAILURE);
  }

  #ifdef ADDSDL
  do {
    Neill_SDL_UpdateScreen(&(p->sw));
    Neill_SDL_Events(&(p->sw));
  } while (!((p->sw).finished));
  SDL_Quit();
  atexit(SDL_Quit);
  #endif
  programFree(p);
  return 0;
}
