#define HASH_TYPE "Realloc"
#define SCALE_FACTOR 2
#define INITIAL_CAPACITY 17
#define LOAD_FACTOR 0.65

struct Node {
  void * key;
  void * data;
};
typedef struct Node Node;

struct assoc {
   Node * hashTable;
   unsigned int keysize;
   unsigned int pairCounter;
   unsigned int capacity;
};
typedef struct assoc assoc;
