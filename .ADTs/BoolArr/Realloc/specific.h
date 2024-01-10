typedef unsigned char datatype;

#define BOOLARR_TYPE "Realloc"
#define FIXEDSIZE 1
#define BITS_PER_BYTE 8
#define SCALEFACTOR 2
struct boolarr {
   /* Underlying array */
   datatype* a;
   unsigned int size;
   unsigned int numOfBits;
   unsigned int capacity;
};
typedef struct boolarr boolarr;
