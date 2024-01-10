#include "specific.h"
#include "../assoc.h"
#include "../../.ADTs/General/bool.h"
#include <math.h>
assoc * insertToTable(assoc * a, Node * strayPair);
assoc * rehash(assoc * a);
int keycmp(void * key1, void * key2, unsigned int keysize);
assoc * makeNewHashTable(assoc * a);
bool setNode(Node * newPair, void * key, void * data);
int jenkinsHash(unsigned int capacity, void * input, unsigned int keysize);
int unnamedHash(unsigned int capacity, void * input, unsigned int keysize);
void swapNode(Node * node1, Node * node2);
int updateKeysize(const assoc * a, const void * key);
assoc* assoc_init(int keysize) {
  assoc * a;
  assert(keysize >= 0);
  a = ncalloc(sizeof(assoc),1);
  a->keysize = keysize;
  a->capacity = INITIAL_CAPACITY;
  a->pairCounter[0] = a->pairCounter[1] = 0;
  a->table[0] = ncalloc(sizeof(Node),a->capacity);
  a->table[1] = ncalloc(sizeof(Node),a->capacity);
  return a;
}

void assoc_insert(assoc** a, void* key, void* data) {
  Node strayPair;
  double occupancy;
  int id;
  assert(a != NULL && *a != NULL && key != NULL);
  assert(setNode(&strayPair,key,data)==true);
  *a = insertToTable(*a,&strayPair);
  for (id = 0; id < TABLES_PER_ASSOC; id++) {
    occupancy = (double)((*a)->pairCounter[id])/((*a)->capacity);
    if (occupancy >= LOAD_FACTOR) {
      *a = rehash(*a);
      return;
    }
  }
}

unsigned int assoc_count(assoc* a) {
  assert(a != NULL);
  return a->pairCounter[0] + a->pairCounter[1];
}

void* assoc_lookup(assoc* a, void* key) {
  int id, keysize;
  int index[2];
  int(*f[])(unsigned int, void *, unsigned int) = {jenkinsHash,unnamedHash};
  assert(a != NULL && key != NULL);
  for (id = 0; id < TABLES_PER_ASSOC; id++) {
    keysize = updateKeysize(a,key);
    index[id] = f[id](a->capacity,key,keysize);
    if (a->table[id][index[id]].key != NULL) {
      if (keycmp(a->table[id][index[id]].key,key,keysize) == 0) {
        return a->table[id][index[id]].data;
      }
    }
  }
  return NULL;
}

void assoc_free(assoc* a) {
  assert(a != NULL);
  free(a->table[0]);
  free(a->table[1]);
  free(a);
}

assoc * insertToTable(assoc * a, Node * strayPair) {
  int iterator;
  int id = 0;
  int index[2];
  int keysize;
  int maxloop;
  int(*f[])(unsigned int, void *, unsigned int) = {jenkinsHash,unnamedHash};
  assert(a != NULL && strayPair != NULL);
  for (id = 0; id < TABLES_PER_ASSOC; id++) {
    keysize = updateKeysize(a,strayPair->key);
    index[id] = f[id](a->capacity,strayPair->key,keysize);
    if ((*(a->table+id)+index[id])->key != NULL) {
      if (keycmp((*(a->table+id)+index[id])->key,strayPair->key,keysize) == 0) {
        memcpy(*(a->table+id)+index[id],strayPair,sizeof(Node));
        return a;
      }
    }
  }
  id = 0;
  /*magic numbers for maxloop came from the following
  https://www.sciencedirect.com/science/article/abs/pii/
  S0196677403001925?via%3Dihub*/
  maxloop = (int)(log10(a->capacity)/log10(2)) * 6;
  for (iterator = 0; iterator < maxloop; iterator++) {
    keysize = updateKeysize(a,strayPair->key);
    index[id] = f[id](a->capacity,strayPair->key,keysize);
    if ((*(a->table+id)+index[id])->key == NULL) {
      memcpy(*(a->table+id)+index[id],strayPair,sizeof(Node));
      a->pairCounter[id] += 1;
      return a;
    }
    if ((*(a->table+id)+index[id])->key != NULL) {
      swapNode(*(a->table+id)+index[id],strayPair);
    }
    id = (id + 1) % TABLES_PER_ASSOC;
  }
  a = rehash(a);
  a = insertToTable(a,strayPair);
  return a;
}

assoc * rehash(assoc * a) {
  int id;
  unsigned int index;
  Node strayPair;
  void * key, * data;
  assoc * b = makeNewHashTable(a);
  assert(a != NULL);
  for (id = 0; id < TABLES_PER_ASSOC; id++) {
    for (index = 0; index < a->capacity; index++) {
      key = a->table[id][index].key;
      data = a->table[id][index].data;
      setNode(&strayPair,key,data);
      if(key != NULL) {
        b = insertToTable(b,&strayPair);
      }
    }
  }
  assoc_free(a);
  return b;
}

assoc * makeNewHashTable(assoc * a) {
  assoc * b = assoc_init(a->keysize);
  assert(a != NULL);
  free(b->table[0]);
  free(b->table[1]);
  b->capacity = a->capacity * SCALE_FACTOR;
  assert(b->capacity > a->capacity);
  b->table[0] = ncalloc(sizeof(Node),b->capacity);
  b->table[1] = ncalloc(sizeof(Node),b->capacity);
  return b;
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

int keycmp(void * key1, void * key2, unsigned int keysize) {
  assert(key1 != NULL && key2 != NULL);
  if (keysize == 0) {
    return strcmp(key1,key2);
  } else {
    return memcmp(key1,key2,keysize);
  }
}

bool setNode(Node * newPair, void * key, void * data) {
  if (key == NULL || newPair == NULL) {
    return false;
  }
  newPair->key = key;
  newPair->data = data;
  return true;
}

void swapNode(Node * node1, Node * node2) {
  Node temp;
  memcpy(&temp,node1,sizeof(Node));
  memcpy(node1,node2,sizeof(Node));
  memcpy(node2,&temp,sizeof(Node));
}

int updateKeysize(const assoc * a, const void * key) {
  if (a->keysize == 0) {
    return strlen(key);
  }
  else {
    return a->keysize;
  }
}
