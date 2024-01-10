#include "specific.h"
#include "../assoc.h"
#include "../../.ADTs/General/bool.h"
#include <math.h>
void testAll();
void testKeycmp();
void testPrimeNoGeneration();
void testSetNode();
void testHashFunction();
void testMax();
void testKeyDataTypes();


bool isPrime(const int input);
int getNextPrime(const int current, const int scalefactor);
bool setNode(Node * newPair, void * key, void * data);
void checkMacro();
bool checkAssocInsertInputs(assoc ** a, void * key);
bool checkAssocLookupInputs(assoc * a, void * key);
int keycmp(void * key1, void * key2, unsigned int keysize);
int max(int a, int b);

int jenkinsHash(unsigned int capacity, void * input, unsigned int keysize);
int unnamedHash(unsigned int capacity, void * input, unsigned int keysize);

bool setIndices(int * index1, int * index2, const assoc * a, void * key);
bool insertToTable(assoc * a, void * key, void * data);
assoc * makeNewHashTable(assoc * a);
assoc * rehash(assoc * a);


assoc* assoc_init(int keysize) {
  assoc * a;

  if (keysize < 0) {
    fprintf(stderr, "assoc_init(int keysize): keysize should be >= 0\n");
    return NULL;
  }

  checkMacro();
  a = (assoc *)ncalloc(sizeof(assoc),1);
  a->keysize = keysize;
  a->pairCounter = 0;
  a->capacity = INITIAL_CAPACITY;
  a->hashTable = (Node *)ncalloc(sizeof(Node),a->capacity);
  return a;
}

void assoc_insert(assoc ** a, void * key, void * data) {
  double occupancy;
  if (checkAssocInsertInputs(a,key) == false) {
    return;
  }

  assert(insertToTable(*a,key,data) == true);

  occupancy = (double)assoc_count(*a)/((*a)->capacity);
  if (occupancy >= LOAD_FACTOR) {
    *a = rehash(*a);
  }
}

unsigned int assoc_count(assoc * a) {
  if (a == NULL) {
    fprintf(stderr, "assoc_count(assoc * a): a can't be NULL.\n");
    return 0;
  }
  return a->pairCounter;
}

void* assoc_lookup(assoc * a, void * key) {
  int index, decrement;

  if (checkAssocLookupInputs(a,key) == false) {
    return NULL;
  }

  setIndices(&index,&decrement,a,key);
  if (a->hashTable[index].key == NULL) {
    return NULL;
  }

  while (keycmp(key,a->hashTable[index].key,a->keysize) != 0) {
    index = index - decrement;
    if (index < 0) {
      index = index + a->capacity;
    }
    if (a->hashTable[index].key == NULL) {
      return NULL;
    }
  }
  return a->hashTable[index].data;
}

void assoc_free(assoc * a) {
  if (a == NULL) {
    return;
  }

  if (a->hashTable != NULL) {
    free(a->hashTable);
  }
  free(a);
}

bool checkAssocLookupInputs(assoc * a, void * key) {
  if (a == NULL) {
    fprintf(stderr, "assoc_lookup(assoc * a, void * key): a can't be NULL\n");
    return false;
  }

  if (key == NULL) {
    fprintf(stderr, "assoc_lookup(assoc * a, void * key): key can't be NULL\n");
    return false;
  }
  return true;
}

bool checkAssocInsertInputs(assoc ** a, void * key) {
  if (a == NULL) {
    fprintf(stderr, "assoc_insert(assoc ** a, void * key, void * data): a can't be NULL.\n");
    return false;
  }

  if (*a == NULL) {
    fprintf(stderr, "assoc_insert(assoc ** a, void * key, void * data): *a can't be NULL.\n");
    return false;
  }

  if (key == NULL) {
    fprintf(stderr, "assoc_insert(assoc ** a, void * key, void * data): key can't be NULL.\n");
    return false;
  }
  return true;
}

assoc * rehash(assoc * a) {
  unsigned int index;
  void * key, * data;
  assoc * b = makeNewHashTable(a);
  assert(a != NULL && b != NULL);

  for (index = 0; index < a->capacity; index++) {
    key = a->hashTable[index].key;
    data = a->hashTable[index].data;
    if (key != NULL) {
      insertToTable(b,key,data);
    }
  }
  assoc_free(a);
  return b;
}

assoc * makeNewHashTable(assoc * a) {
  assoc * b = ncalloc(sizeof(assoc),1);
  assert(a != NULL);
  b->keysize = a->keysize;
  b->capacity = getNextPrime(a->capacity,SCALE_FACTOR);
  assert(isPrime(b->capacity) == true);
  assert(b->capacity > a->capacity);
  b->hashTable = (Node *)ncalloc(sizeof(Node),b->capacity);
  return b;
}

bool insertToTable(assoc * a, void * key, void * data) {
  int index, decrement;
  if (a == NULL && key == NULL) {
    return false;
  }

  assert(setIndices(&index,&decrement,a,key) == true);
  decrement = max(1,decrement);

  while (a->hashTable[index].key != NULL) {
    if (keycmp(a->hashTable[index].key,key,a->keysize) == 0) {
      setNode((a->hashTable) + index,key,data);
      return true;
    } else {
      index = index - decrement;
      if (index < 0) {
        index = index + a->capacity;
      }
    }
  }
  setNode((a->hashTable) + index,key,data);
  a->pairCounter++;
  return true;
}

bool setIndices(int * index1, int * index2, const assoc * a, void * key) {
  unsigned int size;
  if (a == NULL || index1 == NULL || index2 == NULL || key == NULL) {
    return false;
  }

  if (a->keysize == 0) {
    size = strlen(key);
  } else {
    size = a->keysize;
  }

  *index1 = jenkinsHash(a->capacity,key,size);
  *index2 = unnamedHash(a->capacity,key,size);
  return true;
}

int keycmp(void * key1, void * key2, unsigned int keysize) {
  assert(key1 != NULL && key2 != NULL);
  if (keysize == 0) {
    return strcmp(key1,key2);
  } else {
    return memcmp(key1,key2,keysize);
  }
}

void checkMacro() {
  assert(SCALE_FACTOR >= 2);
  assert(isPrime(INITIAL_CAPACITY) == true);
  assert(LOAD_FACTOR <= 0.8 && LOAD_FACTOR >= 0.5);
}

/*https://en.wikipedia.org/wiki/Jenkins_hash_function*/
int jenkinsHash(unsigned int capacity, void * input, unsigned int keysize) {
  unsigned char * c = input;
  unsigned long hash = 0;
  assert(capacity > 0 && input != NULL && keysize > 0);
  while (keysize--) {
    hash += (*c++);
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash % capacity;
}

/*https://stackoverflow.com/questions/664014/what-integer-
hash-function-are-good-that-accepts-an-integer-hash-key*/
int unnamedHash(unsigned int capacity, void * input, unsigned int keysize) {
  unsigned char * c = input;
  unsigned long hash = 0;
  assert(capacity > 0 && input != NULL && keysize > 0);
  while (keysize--) {
    hash = hash + (*c++);
  }
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

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

void testAll() {
  testKeycmp();
  testPrimeNoGeneration();
  testSetNode();
  testHashFunction();
  testMax();
  testKeyDataTypes();
  printf("Test successful.\n");
}

void testKeyDataTypes() {
  struct testSubject {
    int i;
    double k[3][3];
  };
  struct testSubject2 {
    double i;
    int k[3][3];
  };
  typedef struct testSubject testSubject;
  typedef struct testSubject2 testSubject2;
  assoc * a;
  testSubject * k; testSubject2 * k2;
  static const int NUMRANGE = 100000;
  int j;
  double i[100000];
  k = ncalloc(sizeof(testSubject),NUMRANGE);
  k2 = ncalloc(sizeof(testSubject2),NUMRANGE);
  srand(time(NULL));
  a = assoc_init(sizeof(double));
  for(j=0; j<NUMRANGE; j++){
     i[j] = (double)sqrt(rand()%NUMRANGE);
     assoc_insert(&a, &i[j], NULL);
  }
  printf("%d unique numbers out of %d\n", assoc_count(a), j);
  assoc_free(a);
  a = assoc_init(sizeof(testSubject));
  for(j=0; j<NUMRANGE; j++){
     k[j].i = rand()%NUMRANGE;
     assoc_insert(&a, &k[j], NULL);
  }
  printf("%d unique numbers out of %d\n", assoc_count(a), j);
  assoc_free(a);
  a = assoc_init(sizeof(testSubject2));
  for(j=0; j<NUMRANGE; j++){
     k2[j].i = (double)sqrt(rand()%NUMRANGE);
     assoc_insert(&a, &k2[j], NULL);
  }
  printf("%d unique numbers out of %d\n", assoc_count(a), j);
  assoc_free(a);
  free(k);
  free(k2);
}

void testKeycmp() {
  int i,j;
  assert(keycmp("dog","dog",0) == strcmp("cat","cat"));
  assert(keycmp("cat","dog",0) == strcmp("cat","dog"));
  assert(keycmp("dog","cat",0) == strcmp("dog","cat"));
  i = 0; j = 0;
  assert(keycmp(&i,&j,sizeof(int)) == memcmp(&i,&j,sizeof(int)));
  i = 1;
  assert(keycmp(&i,&j,sizeof(int)) == memcmp(&i,&j,sizeof(int)));
  i = 0; j = 1;
  assert(keycmp(&i,&j,sizeof(int)) == memcmp(&i,&j,sizeof(int)));
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

void testMax() {
  assert(max(1,0) == 1);
  assert(max(0,2) == 2);
  assert(max(2,2) == 2);
  assert(max(3,2) == 3);
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
        hashIndex = unnamedHash(capacity,str,3);
        assert(hashIndex >= 0 && hashIndex < (int)capacity);
        hist[(int)((double)hashIndex/1047.29)] += 1;
        hashIndex = jenkinsHash(capacity,str,3);
        assert(hashIndex >= 0 && hashIndex < (int)capacity);
        hist2[(int)((double)hashIndex/1047.29)] += 1;
      }
    }
  }
  for (i = 0; i < 1000000; i++) {
    hashIndex = unnamedHash(capacity,&i,sizeof(int));
    assert(hashIndex >= 0 && hashIndex < (int)capacity);
    hist3[(int)((double)hashIndex/1047.29)] += 1;
    hashIndex = jenkinsHash(capacity,&i,sizeof(int));
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
