#define HASH_TYPE "Cuckoo"
#define SCALE_FACTOR 2
#define INITIAL_CAPACITY 16
/*magic number for load factor came from the following
https://www.sciencedirect.com/science/article/abs/
pii/S0196677403001925?via%3Dihub*/
#define LOAD_FACTOR 0.416
#define TABLES_PER_ASSOC 2
struct Node {
  void * key;
  void * data;
};
typedef struct Node Node;

struct assoc {
   Node * table[2];
   unsigned int keysize;
   unsigned int pairCounter[2];
   unsigned int capacity;
};
typedef struct assoc assoc;
