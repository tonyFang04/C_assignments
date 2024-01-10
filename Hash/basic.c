#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
enum bool {false, true};
typedef enum bool bool;
#define SCALE_FACTOR 2
#define INITIAL_CAPACITY 17
#define LOAD_FACTOR 0.65

/*In this project I will explore how an auto adjusting colour
scheme can reduce eye strain. This will be evaluated by measuring
a physical measurement of eyestrain through the rate of blinking.
The hypothesis is that eyestrain can be measured from rate of blinking.  */


struct Node {
  void * key;
  void * data;
};
typedef struct Node Node;

bool isPrime(const int input);
int getNextPrime(const int current, const int scalefactor);
void swapPtrs(void ** a, void ** b);
bool setNode(Node * newPair, void * key, void * data);

int jenkinsHashStrVer(unsigned int capacity, char *str);
int jenkinsHashIntVer(unsigned int capacity, int number);
int sdbmHashStrVer(unsigned int capacity, char *str);
int unnamedHashIntVer(unsigned int capacity, int number);

void testSetNode();
void testHashFunction();
void testSwapPtrs();
void testPrimeNoGeneration();

int keycmp(void * key1, void * key2, unsigned int keysize) {
  assert(key1 != NULL && key2 != NULL);
  if (keysize == 0) {
    return strcmp(key1,key2);
  } else {
    return memcmp(key1,key2,keysize);
  }
}

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

void testKeycmp() {
  int i,j;
  assert(keycmp("manko","manko",0) == strcmp("manko","manko"));
  assert(keycmp("manko","chinko",0) == strcmp("manko","chinko"));
  assert(keycmp("chinko","manko",0) == strcmp("chinko","manko"));
  i = 0; j = 0;
  assert(keycmp(&i,&j,sizeof(int)) == memcmp(&i,&j,sizeof(int)));
  i = 1;
  assert(keycmp(&i,&j,sizeof(int)) == memcmp(&i,&j,sizeof(int)));
  i = 0; j = 1;
  assert(keycmp(&i,&j,sizeof(int)) == memcmp(&i,&j,sizeof(int)));
}

int main(void) {
  assert(SCALE_FACTOR >= 2);
  assert(isPrime(INITIAL_CAPACITY) == true);
  assert(LOAD_FACTOR <= 0.8 && LOAD_FACTOR >= 0.5);
  testPrimeNoGeneration();
  testSwapPtrs();
  testSetNode();
  testHashFunction();
  testKeycmp();
  assert(max(1,0) == 1);
  assert(max(0,2) == 2);
  assert(max(2,2) == 2);
  assert(max(3,2) == 3);
  printf("done\n");
  return 0;
}

/*https://en.wikipedia.org/wiki/Jenkins_hash_function*/
int jenkinsHashStrVer(unsigned int capacity, char *str) {
  unsigned long hash = 0;
  int c;
  assert(capacity > 0 && str != NULL);
  while ((c = (*str++))) {
    hash += c;
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash % capacity;
}

int jenkinsHashIntVer(unsigned int capacity, int number) {
  unsigned long hash = 0;
  assert(capacity > 0);
  hash += number;
  hash += hash << 10;
  hash ^= hash >> 6;
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash % capacity;
}

/*http://www.cse.yorku.ca/~oz/hash.html*/
int sdbmHashStrVer(unsigned int capacity, char *str) {
  unsigned long hash = 0;
  int c;
  assert(capacity > 0 && str != NULL);
  while ((c = (*str++))) {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }
  return hash % capacity;
}

/*https://stackoverflow.com/questions/664014/what-integer-
hash-function-are-good-that-accepts-an-integer-hash-key*/
int unnamedHashIntVer(unsigned int capacity, int number) {
  unsigned long hash = number;
  assert(capacity > 0);
  hash = ((hash >> 16) ^ hash) * 0x119de1f3;
  hash = ((hash >> 16) ^ hash) * 0x119de1f3;
  hash = (hash >> 16) ^ hash;
  return hash % capacity;
}

int getNextPrime(const int current, const int scalefactor) {
  int next = current * scalefactor;
  do {
    next++;
  } while (isPrime(next) == false);
  return next;
}

bool isPrime(const int input) {
  int i;

  if (input <= 1) {
    return false;
  }

  if (input == 2) {
    return true;
  }

  for (i = 2; i < sqrt(input) + 1; i++) {
    if (input % i == 0) {
      return false;
    }
  }
  return true;
}

bool setNode(Node * newPair, void * key, void * data) {
  if (key == NULL || newPair == NULL) {
    return false;
  }
  newPair->key = key;
  newPair->data = data;
  return true;
}

void swapPtrs(void ** a, void ** b) {
  void * temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

void testPrimeNoGeneration() {
  int i, no, scalefactor;
  assert(isPrime(-7) == false);assert(isPrime(-3) == false);
  assert(isPrime(-2) == false);assert(isPrime(-1) == false);
  assert(isPrime(0) == false);assert(isPrime(1) == false);
  assert(isPrime(2) == true);assert(isPrime(3) == true);
  assert(isPrime(4) == false);assert(isPrime(5) == true);
  assert(isPrime(6) == false);assert(isPrime(7) == true);
  assert(isPrime(9) == false);assert(isPrime(15) == false);
  /*17389 is the 2000th prime number*/
  assert(isPrime(17389) == true);
  no = -7;scalefactor = 1;
  for (i = 0; i < 2000; i++) {
    no = getNextPrime(no,scalefactor);
    assert(isPrime(no) == true);
  }
  assert(no == 17389);
  for (i = 2000; i < 10000; i++) {
    no = getNextPrime(no,scalefactor);
    assert(isPrime(no) == true);
  }
  assert(no == 104729);

  no = -7;scalefactor = 2;
  no = getNextPrime(no,scalefactor);assert(no == 2);
  no = getNextPrime(no,scalefactor);assert(no == 5);
  no = getNextPrime(no,scalefactor);assert(no == 11);
  no = getNextPrime(no,scalefactor);assert(no == 23);
  no = getNextPrime(no,scalefactor);assert(no == 47);
  no = getNextPrime(no,scalefactor);assert(no == 97);
  no = getNextPrime(8693,scalefactor);assert(no == 17387);

  no = -7;scalefactor = 3;
  no = getNextPrime(no,scalefactor);assert(no == 2);
  no = getNextPrime(no,scalefactor);assert(no == 7);
  no = getNextPrime(no,scalefactor);assert(no == 23);
  no = getNextPrime(no,scalefactor);assert(no == 71);
  no = getNextPrime(no,scalefactor);assert(no == 223);
  no = getNextPrime(8693,scalefactor);assert(no == 26083);
}

void testSwapPtrs() {
  void * a, * b;
  a = b = NULL;
  swapPtrs(&a,&b);
  assert(a == NULL && b == NULL);
  a = (void *)0x64;b = (void *)0x4a;
  swapPtrs(&a,&b);
  assert(a == (void *)0x4a && b == (void *)0x64);
  a = NULL;
  swapPtrs(&a,&b);
  assert(a == (void *)0x64 && b == NULL);
  swapPtrs(&a,&b);
  assert(a == NULL && b == (void *)0x64);
  b = NULL;
  swapPtrs(&a,&b);
  assert(a == NULL && b == NULL);
}

void testSetNode() {
  Node newPair;
  assert(setNode(&newPair,(void *)0x64,(void *)0x4a) == true);
  assert(newPair.key == (void *)0x64);
  assert(newPair.data == (void *)0x4a);
  assert(setNode(&newPair,(void *)0x64,NULL) == true);
  assert(newPair.key == (void *)0x64);
  assert(newPair.data == NULL);
  assert(setNode(&newPair,NULL,NULL) == false);
  assert(setNode(NULL,(void *)0x64,NULL) == false);
  assert(setNode(NULL,NULL,NULL) == false);
}

void testHashFunction() {
  int i,j,k,hashIndex;
  unsigned int capacity;
  char str[4];
  int hist[100];
  int hist2[100];
  int hist3[100];
  int hist4[100];
  memset(hist,0,sizeof(int) * 100);
  memset(hist2,0,sizeof(int) * 100);
  memset(hist3,0,sizeof(int) * 100);
  memset(hist4,0,sizeof(int) * 100);
  str[3] = '\0';
  capacity = 104729;
  for (i = 0; i < 256; i++) {
    for (j = 0; j < 256; j++) {
      for (k = 0; k < 256; k++) {
        str[0] = i;
        str[1] = j;
        str[2] = k;
        hashIndex = sdbmHashStrVer(capacity,str);
        assert(hashIndex >= 0 && hashIndex < (int)capacity);
        hist[(int)((double)hashIndex/1047.29)] += 1;
        hashIndex = jenkinsHashStrVer(capacity,str);
        assert(hashIndex >= 0 && hashIndex < (int)capacity);
        hist2[(int)((double)hashIndex/1047.29)] += 1;
      }
    }
  }

  for (i = 0; i < 1000000; i++) {
    hashIndex = unnamedHashIntVer(capacity,i);
    assert(hashIndex >= 0 && hashIndex < (int)capacity);
    hist3[(int)((double)hashIndex/1047.29)] += 1;
    hashIndex = jenkinsHashIntVer(capacity,i);
    assert(hashIndex >= 0 && hashIndex < (int)capacity);
    hist4[(int)((double)hashIndex/1047.29)] += 1;
  }

  for (i = 0; i < 100; i++) {
    assert(hist[i] > 0);
    assert(hist2[i] > 0);
    assert(hist3[i] > 0);
    assert(hist4[i] > 0);
  }
}
