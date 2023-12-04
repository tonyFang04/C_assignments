#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "neillsimplescreen.h"

#define fire '*'
#define tree '@'
#define empty ' '
#define WIDTH 80
#define HEIGHT 30
#define G 250
#define L 2500
#define generation 1000
#define probTreeGrowth (1.0 / G)
#define probLightning (1.0 / L)

enum bool {false, true};
typedef enum bool bool;

void test();
void setGrid(char oldGrid[][WIDTH], char newGrid[][WIDTH]);
void driver(char oldGrid[][WIDTH], char newGrid[][WIDTH]);
void update(char oldGrid[][WIDTH], char newGrid[][WIDTH]);

char ceaseFire(const char input, const char input2);
char treeBurn(const int i, const int j, char input[][WIDTH],
              const char input2);
char treeGrowth(const char input, const char input2);
char lightning(const char input, const char input2);

double uniform_distribution(const double ini, const double fin);
bool sampling_outcome(const double probability);
bool test_sampling_outcome(const double probability);
bool edgeSafety(const int index1, const int index2);

int main(void) {
	int iterator;
	char oldGrid[HEIGHT][WIDTH], newGrid[HEIGHT][WIDTH];
	srand((long)time(NULL));
	test();

	setGrid(oldGrid,newGrid);

	for (iterator = 0; iterator < generation; iterator++) {
		driver(oldGrid,newGrid);

    update(oldGrid,newGrid);
	}
  neillfgcol(black);
	return 0;
}

void test() {
	assert(test_sampling_outcome(probTreeGrowth)==true);
	assert(test_sampling_outcome(probLightning)==true);
	assert(edgeSafety(0,0)==true);
	assert(edgeSafety(HEIGHT,WIDTH)==false);
	assert(edgeSafety(HEIGHT,0)==false);
	assert(edgeSafety(0,WIDTH)==false);
}

void setGrid(char oldGrid[][WIDTH], char newGrid[][WIDTH]) {
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			oldGrid[i][j] = tree;
			newGrid[i][j] = tree;
		}
	}
	oldGrid[10][33] = fire;
	newGrid[10][33] = fire;
}

void driver(char oldGrid[][WIDTH], char newGrid[][WIDTH]) {
	int i,j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			/*set neighbours on fire*/
			newGrid[i][j] = treeBurn(i,j,oldGrid,newGrid[i][j]);
			newGrid[i][j] = treeGrowth(oldGrid[i][j],newGrid[i][j]);
			newGrid[i][j] = ceaseFire(oldGrid[i][j],newGrid[i][j]);
			newGrid[i][j] = lightning(oldGrid[i][j],newGrid[i][j]);
		}
	}
}

void update(char oldGrid[][WIDTH], char newGrid[][WIDTH]) {
  int i,j;
  neillclrscrn();
  for (i = 0; i < HEIGHT; i++) {
    for (j = 0; j < WIDTH; j++) {
      if (newGrid[i][j] == fire) {
        neillfgcol(red);
        printf("%c",newGrid[i][j]);
      } else if (newGrid[i][j] == tree) {
        neillfgcol(green);
        printf("%c",newGrid[i][j]);
      } else {
        printf("%c",newGrid[i][j]);
      }
      oldGrid[i][j] = newGrid[i][j];
    }
    printf("\n");
  }
  neillbusywait(0.02);
}

char ceaseFire(const char input, const char input2) {
	if (input == fire) {
		return empty;
	} else {
		return input2;
	}
}

char treeBurn(const int i, const int j, char input[][WIDTH],
              const char input2) {
	int k,l,I,J;
	for (k = -1; k <= 1; k++) {
		for (l = -1; l<= 1; l++) {
			I = i + k;
			J = j + l;
			if (input[i][j] == tree && edgeSafety(I,J) == true
                              && (input[I][J] == fire)) {
				return fire;
			}
		}
	}
	return input2;
}


char treeGrowth(const char input, const char input2) {
	if (input == empty && sampling_outcome(probTreeGrowth)== true) {
		return tree;
	} else {
		return input2;
	}
}

char lightning(const char input, const char input2) {
	if (input == tree && sampling_outcome(probLightning)== true) {
		return fire;
	} else {
		return input2;
	}
}

double uniform_distribution(const double ini, const double fin) {
  return (fin - ini) * (rand()/(double)RAND_MAX) + ini;
}

bool sampling_outcome(const double probability) {
	if (uniform_distribution(0.0,1.0) < probability) {
		return true;
	} else {
		return false;
	}
}

bool test_sampling_outcome(const double probability) {
	int i = 0;
	int hit_time = 0;
	int sampleSize = 10000000;
	double approx_probability;
	for (i = 0; i < sampleSize; i++) {
		if(sampling_outcome(probability)==true) {
			hit_time++;
		}
	}
	approx_probability = (double)hit_time / (double)sampleSize;
	if (approx_probability < (probability + 0.05 * probability)
		&& approx_probability > (probability - 0.05 * probability)) {
		return true;
	} else {
		return false;
	}
}

bool edgeSafety(const int index1, const int index2) {
	if (index1 < HEIGHT && index1 >= 0 && index2 < WIDTH && index2 >= 0) {
		return true;
	} else {
		return false;
	}
}
