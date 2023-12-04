#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include "neillsimplescreen.h"
#define MAX_WIDTH 9
#define MAX_HEIGHT 9
#define NO_OF_COLOURS 8
#define STRMAXLEN 200
enum bool {FALSE, TRUE};
typedef enum bool bool;

enum colour {Black,Red,Green,Yellow,Blue,
             Magenta,Cyan,White,Empty};
typedef enum colour colour;

struct Bookcase {
  int height,width;
  colour book[MAX_HEIGHT][MAX_WIDTH];
};
typedef struct Bookcase Bookcase;

struct Node {
  Bookcase * state;
  struct Node * next;
  struct Node * parent;
};
typedef struct Node Node;

/*functions that deals with inputs*/
void processInput(Bookcase * state, const int argc,
                  const char * filename);
colour charToColour(char book);
char colourToChar(const colour colourtype);
void checkFileName(const char * filename);
void readInputFile(Bookcase * currentState,
                   const char * filename);
void withinBound(const char * name, const int variable,
                 const int upperbound);
void validFullStopPlacement(const char * buffer);

/*functions that deals with outputs*/
neillcol colourToNeillCol(const colour colourtype);
void printBookcase(const Bookcase * currentState);
void displayResult(Node * inputBookcase, const char * flag,
                   const int steps);

/*functions that checks whether a bookcase is happy*/
bool canNeverBeHappy(const Bookcase * state);
bool isHappy(const Bookcase * currentState);
bool colourCount(const Bookcase * currentState);
int spaceCount(const Bookcase * currentState);
void testing();

/*functions that execute the brute-force algorithm for
searching over all the moves to make a bookcase happy*/
Node * bruteForceFindChildren(const Bookcase * state);
Node * pathToHappyBookcase(Node * cp, int * steps);
Node * rearrangeBookcase(Node * cp, Node * tail);
int findRightMostbook(const Bookcase * currentState,
                      const int row);
int findLeftMostSpace(const Bookcase * currentState,
                      const int row);
void swap(colour * i, colour * j);

/*functions that build, traverse and delete the linked list
which stores all the moves to make a bookcase happy*/
Node * initNewNode(const Bookcase * currentState);
Node * deleteNode(Node * current_Node);
Node * append(Node * tail, Node * cp);
Node * deleteList(Node * head);
Node * pushFront(Node * head, const Bookcase * currentState);
void printList(Node * head);

int main(int argc, char ** argv) {
  Node * happyBookcase, * inputBookcase;
  Bookcase currentState;
  int steps = 0;
  testing();
  processInput(&currentState,argc,argv[1]);
  happyBookcase = bruteForceFindChildren(&currentState);
  inputBookcase = pathToHappyBookcase(happyBookcase,&steps);
  displayResult(inputBookcase,argv[2],steps);
  neillfgcol(black);
  return 0;
}

void processInput(Bookcase * state, const int argc,
                  const char * filename) {
  if (argc < 2 || argc > 3) {
    fprintf(stderr,"1 or 2 inputs only.\n");
    exit(EXIT_FAILURE);
  }
  checkFileName(filename);
  readInputFile(state,filename);
  if (canNeverBeHappy(state) == TRUE) {
    fprintf(stdout,"No solution?\n");
    exit(EXIT_SUCCESS);
  }
}

Node * bruteForceFindChildren(const Bookcase * state) {
  Node * head, * tail, * cp;
  head = initNewNode(state);
  cp = tail = head;
  while (isHappy(cp->state) == FALSE) {
    tail = rearrangeBookcase(cp,tail);
    cp = cp->next;
  }
  return cp;
}

Node * pathToHappyBookcase(Node * cp, int * steps) {
  Node * output = NULL;
  while (cp->parent != NULL) {
    output = pushFront(output,cp->state);
    cp = cp->parent;
    (*steps)++;
  }
  output = pushFront(output,cp->state);
  (*steps)++;
  deleteList(cp);
  return output;
}

void displayResult(Node * inputBookcase, const char * flag,
                   const int steps) {
  fprintf(stdout,"%d\n\n",steps);
  if (flag != NULL && strcmp(flag,"verbose") == 0) {
    printList(inputBookcase);
  }
  deleteList(inputBookcase);
}

bool canNeverBeHappy(const Bookcase * state) {
  if (spaceCount(state) == 0 ||
      colourCount(state) == FALSE) {
    return TRUE;
  }
  return FALSE;
}

void printBookcase(const Bookcase * currentState) {
  int row,col;
  for (row = 0; row < currentState->height; row++) {
    for (col = 0; col < currentState->width; col++) {
      neillfgcol(colourToNeillCol(currentState->book[row][col]));
      fprintf(stdout,"%c",
              colourToChar(currentState->book[row][col]));
    }
    fprintf(stdout,"\n");
  }
}

void swap(colour * i, colour * j) {
  colour temp;
  temp = *i;
  *i = *j;
  *j = temp;
}

Node * rearrangeBookcase(Node * cp, Node * tail) {
  int row1, row2, col1, col2;
  for (row1 = 0; row1 < cp->state->height; row1++) {
    col1 = findRightMostbook(cp->state,row1);
    if (col1 != -1) {
      for (row2 = 0; row2 < cp->state->height; row2++) {
        col2 = findLeftMostSpace(cp->state,row2);
        if (col2 != -1 && row1 != row2) {
          swap(&(cp->state->book[row1][col1]),
               &(cp->state->book[row2][col2]));
          tail = append(tail,cp);
          swap(&(cp->state->book[row1][col1]),
               &(cp->state->book[row2][col2]));
        }
      }
    }
  }
  return tail;
}

void checkFileName(const char * filename) {
  int i, j;
  i = 0;j = 0;
  while (filename[i] != '\0') {
    i++;
    if (filename[i] == '.') {
      j++;
    }
  }
  if (j == 2) {
    fprintf(stderr,"Wrong type of file. Please type in a .bc file.\n");
    exit(EXIT_FAILURE);
  }
  if (filename[i - 1] != 'c' ||
      filename[i - 2] != 'b' ||
      filename[i - 3] != '.') {
    fprintf(stderr,"Wrong type of file. Please type in a .bc file.\n");
    exit(EXIT_FAILURE);
  }
}

/*If a bookcase is valid, it will be stoRed. Otherwise, the
program terminates with an error message to the terminal*/
void readInputFile(Bookcase * currentState,
                   const char * filename) {
  int row, col;
  FILE * inputfile;
  char buffer[STRMAXLEN];
  if ((inputfile = fopen(filename,"r")) == NULL) {
    fprintf(stderr,"Can't open file.\n");
    exit(EXIT_FAILURE);
  }
  row = 0;
  while (fgets(buffer,STRMAXLEN,inputfile) != NULL) {
    if (row == 0 && buffer[0] > '0' && buffer[0] <= '9') {
      if (sscanf(buffer,"%d %d",&(currentState->height),
                                &(currentState->width)) != 2) {
        fprintf(stderr,"Can't read in the length and width of the bookcase.\n");
        exit(EXIT_FAILURE);
      }
      withinBound("Height",currentState->height,MAX_HEIGHT + 1);
      withinBound("Width",currentState->width,MAX_WIDTH + 1);
    } else {
      withinBound("Row",row - 1,currentState->height);
      col = 0;
      validFullStopPlacement(buffer);
      while (buffer[col] != '\n') {
        withinBound("Colume",col,currentState->width);
        currentState->book[row - 1][col] = charToColour(buffer[col]);
        col++;
      }
      if (col != currentState->width) {
        fprintf(stderr,"Invalid book case. Wrong width.\n");
        exit(EXIT_FAILURE);
      }
    }
    row++;
  }
  if (row - 1 != currentState->height) {
    fprintf(stderr,"Invalid book case. Wrong height.\n");
    exit(EXIT_FAILURE);
  }
  fclose(inputfile);
}

/*returns the number of Empty spaces in a bookcase*/
int spaceCount(const Bookcase * currentState) {
  int row, col;
  int i = 0;
  for (row = 0; row < currentState->height; row++) {
    for (col = 0; col < currentState->width; col++) {
      if (currentState->book[row][col] == Empty) {
        i++;
      }
    }
  }
  return i;
}

bool isHappy(const Bookcase * currentState) {
  int row, col, i;
  int hist[NO_OF_COLOURS];
  for (row = 0; row < currentState->height; row++) {
    col = 0;
    while (col + 1 < currentState->width) {
      if (currentState->book[row][col] !=
          currentState->book[row][col + 1]) {
        if (currentState->book[row][col + 1] != Empty) {
          return FALSE;
        }
      }
      col++;
    }
  }
  for (i = 0; i < NO_OF_COLOURS; i++) {
    hist[i] = 0;
  }
  col = 0;
  for (row = 0; row < currentState->height; row++) {
    for (i = Black; i <= White; i++) {
      if (i == (int)currentState->book[row][col]) {
        hist[i] += 1;
      }
    }
  }
  for (i = 0; i < NO_OF_COLOURS; i++) {
    if (hist[i] > 1) {
      return FALSE;
    }
  }
  return TRUE;
}

/*returns FALSE when the number of books with the same
colour is larger than the size of the bookshelf*/
bool colourCount(const Bookcase * currentState) {
  int row, col, k;
  int hist[NO_OF_COLOURS];
  for (k = 0; k < NO_OF_COLOURS; k++) {
    hist[k] = 0;
  }
  for (row = 0; row < currentState->height; row++) {
    for (col = 0; col < currentState->width; col++) {
      for (k = Black; k <= White; k++) {
        if (k == (int)currentState->book[row][col]) {
          hist[k] += 1;
        }
      }
    }
  }
  for (k = 0; k < NO_OF_COLOURS; k++) {
    if (hist[k] > currentState->width) {
      return FALSE;
    }
  }
  return TRUE;
}

int findRightMostbook(const Bookcase * currentState,
                      const int row) {
  int col;
  for (col = currentState->width - 1; col >= 0; col--) {
    if (currentState->book[row][col] != Empty) {
      return col;
    }
  }
  return -1;
}

int findLeftMostSpace(const Bookcase * currentState,
                      const int row) {
  int col;
  for (col = 0; col < currentState->width; col++) {
    if (currentState->book[row][col] == Empty) {
      return col;
    }
  }
  return -1;
}

/*If the variable goes out of bound, the program
terminates with an error message to the terminal*/
void withinBound(const char * name, const int variable,
                 const int upperbound) {
  if (variable < upperbound && variable >= 0) {
    return;
  } else {
    fprintf(stderr,"%s out of bound\n",name);
    exit(EXIT_FAILURE);
  }
}

/*If in the .bc file, '.' is followed by a colour, the
program terminates with an error message to the terminal*/
void validFullStopPlacement(const char * buffer) {
  int i = 0;
  while (buffer[i + 1] != '\n') {
    if (charToColour(buffer[i]) == Empty &&
        charToColour(buffer[i + 1]) != Empty) {
      fprintf(stderr,"Invalid book case. '.' should be followed by a colour.\n");
      exit(EXIT_FAILURE);
    }
    i++;
  }
}

Node * initNewNode(const Bookcase * currentState) {
  Node * newNode;
  if ((newNode = (Node * )malloc(sizeof(Node))) == NULL) {
    fprintf(stdout,"No solution?\n");
    exit(EXIT_FAILURE);
  }
  newNode->parent = NULL;
  newNode->next = NULL;
  if ((newNode->state = (Bookcase * )malloc(sizeof(Bookcase))) == NULL) {
    fprintf(stdout,"No solution?\n");
    exit(EXIT_FAILURE);
  }
  memcpy(newNode->state,currentState,sizeof(Bookcase));
  return newNode;
}

Node * deleteNode(Node * current_Node) {
  free(current_Node->state);
  free(current_Node);
  return NULL;
}

Node * append(Node * tail, Node * cp) {
  Node * newtail = initNewNode(cp->state);
  newtail->parent = cp;
  tail->next = newtail;
  return newtail;
}

Node * deleteList(Node * head) {
  Node * temp;
  while (head->next != NULL) {
    temp = head;
    head = head->next;
    deleteNode(temp);
  }
  deleteNode(head);
  return NULL;
}

Node * pushFront(Node * head, const Bookcase * currentState) {
  Node * newhead = initNewNode(currentState);
  if (head == NULL) {
    return newhead;
  }
  newhead->next = head;
  return newhead;
}

void printList(Node * head) {
  Node * ptr_i;
  ptr_i = head;
  while (ptr_i->next != NULL) {
    printBookcase(ptr_i->state);
    fprintf(stdout,"\n");
    ptr_i = ptr_i->next;
  }
  printBookcase(ptr_i->state);
  fprintf(stdout,"\n");
}

void testing() {
  int row, col;
  Bookcase test1;
  if (MAX_WIDTH > 3 && MAX_HEIGHT > 3) {
    test1.height = test1.width = 3;
    for (row = 0; row < test1.height; row++) {
      for (col = 0; col < test1.width; col++) {
        test1.book[row][col] = Empty;
      }
    }
    assert(findLeftMostSpace(&test1,0) == 0);
    assert(findRightMostbook(&test1,1) == -1);
    assert(isHappy(&test1) == TRUE);
    test1.book[1][0] = Blue;
    assert(findRightMostbook(&test1,1) == 0);
    assert(isHappy(&test1) == TRUE);
    test1.book[1][1] = Blue;
    assert(findRightMostbook(&test1,1) == 1);
    assert(isHappy(&test1) == TRUE);
    test1.book[1][2] = Blue;
    assert(findRightMostbook(&test1,1) == 2);
    assert(findLeftMostSpace(&test1,1) == -1);
    assert(isHappy(&test1) == TRUE);
    for (col = 0; col < test1.width; col++) {
      test1.book[0][col] = Blue;
      test1.book[1][col] = Cyan;
      test1.book[2][col] = Yellow;
    }
    assert(isHappy(&test1) == TRUE);
  }
  /*smallest book case size*/
  test1.height = test1.width = 1;
  test1.book[0][0] = Empty;
  assert(isHappy(&test1) == TRUE);
  test1.book[0][0] = Black;
  assert(isHappy(&test1) == TRUE);
  /*largest book case size*/
  if (MAX_WIDTH == 9 && MAX_HEIGHT == 9) {
    test1.height = MAX_HEIGHT;
    test1.width = MAX_WIDTH;
    for (row = 0; row < MAX_HEIGHT; row++) {
      for (col = 0; col < MAX_WIDTH; col++) {
        test1.book[row][col] = Empty;
      }
    }
    assert(isHappy(&test1) == TRUE);
    test1.book[1][0] = Blue;
    assert(isHappy(&test1) == TRUE);
    test1.book[1][1] = test1.book[1][2] = Blue;
    assert(isHappy(&test1) == TRUE);
    for (row = Black; row < test1.height; row++) {
      for (col = 0; col < test1.width; col++) {
        test1.book[row][col] = row;
      }
    }
    assert(isHappy(&test1) == TRUE);
    col = 8;
    for (row = Black; row < test1.height; row++) {
      test1.book[row][col] = Empty;
    }
    assert(isHappy(&test1) == TRUE);
    for (row = Black; row < test1.height; row++) {
      test1.book[row][col] = row;
    }
    row = 8;
    for (col = 0; col < test1.width; col++) {
      test1.book[row][col] = Blue;
    }
    assert(isHappy(&test1) == FALSE);
    row = 8;
    for (col = 0; col < test1.width; col++) {
      test1.book[row][col] = Empty;
    }
    test1.book[7][8] = Blue;
    assert(isHappy(&test1) == FALSE);
  }
}

colour charToColour(char book) {
  book = toupper(book);
  switch(book) {
    case 'K':
      return Black;
    case 'R':
      return Red;
    case 'G':
      return Green;
    case 'Y':
      return Yellow;
    case 'B':
      return Blue;
    case 'M':
      return Magenta;
    case 'C':
      return Cyan;
    case 'W':
      return White;
    case '.':
      return Empty;
    default:
      fprintf(stderr,"Invalid book case. Colour %c does not exist.\n",book);
      exit(EXIT_FAILURE);
  }
}

char colourToChar(const colour colourtype) {
  switch(colourtype) {
    case Black:
      return 'K';
    case Red:
      return 'R';
    case Green:
      return 'G';
    case Yellow:
      return 'Y';
    case Blue:
      return 'B';
    case Magenta:
      return 'M';
    case Cyan:
      return 'C';
    case White:
      return 'W';
    case Empty:
      return '.';
    default:
      exit(EXIT_FAILURE);
    }
}

neillcol colourToNeillCol(const colour colourtype) {
  switch(colourtype) {
    case Black:
      return black;
    case Red:
      return red;
    case Green:
      return green;
    case Yellow:
      return yellow;
    case Blue:
      return blue;
    case Magenta:
      return magenta;
    case Cyan:
      return cyan;
    case White:
      return white;
    case Empty:
      return black;
    default:
      exit(EXIT_FAILURE);
    }
}
