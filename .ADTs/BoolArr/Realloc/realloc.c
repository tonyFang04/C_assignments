#include "specific.h"
#include "../boolarr.h"
#include <math.h>
/* All elements initialised to "0" */
boolarr * boolarr_init(void) {
  boolarr * ba = (boolarr * )ncalloc(sizeof(boolarr), 1);
  ba->a = (datatype * )ncalloc(sizeof(datatype), FIXEDSIZE);
  ba->size = 0;
  ba->numOfBits = 0;
  ba->capacity= FIXEDSIZE;
  return ba;
}

/* Set nth bit on/off */
bool boolarr_set(boolarr* ba, const unsigned int n, const bool b) {
  if (ba == NULL) {
    return false;
  }

  if (b == true) {

    if (ba->numOfBits < n + 1) {
      ba->numOfBits = n + 1;
    }
    ba->size = n / BITS_PER_BYTE + 1;

    while (ba->size >= ba->capacity) {
      ba->a = (datatype * ) nrecalloc(ba->a, sizeof(datatype) * ba->capacity, sizeof(datatype) * ba->capacity * SCALEFACTOR);
      ba->capacity = ba->capacity * SCALEFACTOR;
    }
    ba->a[ba->size - 1] += (datatype)pow(2,n % BITS_PER_BYTE);
    return true;
  }

  if (b == false) {
    while (n / BITS_PER_BYTE >= ba->capacity) {
      ba->a = (datatype * ) nrecalloc(ba->a, sizeof(datatype) * ba->capacity, sizeof(datatype) * ba->capacity * SCALEFACTOR);
      ba->capacity = ba->capacity * SCALEFACTOR;
    }
    ba->a[n / BITS_PER_BYTE] -= (datatype)pow(2,n % BITS_PER_BYTE);
    return true;
  }
  return true;
}
/* Get nth bit */
bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b) {
  datatype i, j;
  if (ba == NULL) {
    return false;
  }
  i = (datatype)pow(2,n % BITS_PER_BYTE);
  j = (ba->a[n / BITS_PER_BYTE]);
  if (i & j) {
    *b = true;
  } else {
    *b = false;
  }

  return true;
}

/* Clears all space */
bool boolarr_free(boolarr* p) {
  if(p == NULL){
    return true;
  }
  free(p->a);
  free(p);
  return true;
}

/* Create boolarr based on string e.g. "1100001" */
boolarr* boolarr_initstr(const char* str) {
  boolarr * ba;
  int i;
  if (str == NULL) {
    return NULL;
  }

  ba = boolarr_init();
  for (i = 0; i < (int)strlen(str); i++) {
    if (str[i] == '1') {
      boolarr_set(ba,(int)strlen(str) - 1 - i,true);
    }
  }
  return ba;
}

/* Return a deep copy */
boolarr* boolarr_clone(const boolarr* ba) {
  boolarr* new_boolarr = boolarr_init();
  new_boolarr->a = nremalloc(new_boolarr->a,sizeof(datatype) * ba->capacity);
  memcpy(new_boolarr->a,ba->a,ba->capacity);
  new_boolarr->size = ba->size;
  new_boolarr->capacity = ba->capacity;
  new_boolarr->numOfBits = ba->numOfBits;
  return new_boolarr;
}

/* Get number of bits in array */
unsigned int boolarr_size(const boolarr* ba) {
  if (ba == NULL) {
    return 0;
  }
  return ba->numOfBits;
}

unsigned int count_1s_in_a_byte(datatype input) {
  unsigned int quotient, modulus, counter;
  counter = 0;
  if (input == 0) {
    return counter;
  }

  while (input != 0) {
    quotient = input / 2;
    modulus = input % 2;
    if (modulus > 0) {
      counter++;
    }
    input = quotient;
  }
  return counter;
}

/* Return number of bits that are set true */
unsigned int boolarr_count1s(const boolarr* ba) {
  unsigned int counter,i;

  if (ba == NULL) {
    return 0;
  }

  counter = 0; i = ba->size;
  while (i > 0) {
    counter += count_1s_in_a_byte(ba->a[i]);
    i--;
  }
  counter += count_1s_in_a_byte(ba->a[i]);
  return counter;
}

/* Return if two arrays are the same (bitwise) */
bool boolarr_issame(const boolarr* b1, const boolarr* b2) {
  if (b1 == NULL && b2 == NULL) {
    return true;
  }
  if (b1 == NULL || b2 == NULL) {
    return false;
  }
  if (memcmp(b1->a,b2->a,b1->capacity) == 0) {
    if (b1->size == b2->size && b1->capacity == b2->capacity && b1->numOfBits == b2->numOfBits)
    return true;
  }
  return false;
}

/* Store to string - rightmost bit is LSB */
bool boolarr_tostring(const boolarr* ba, char* str) {
  int i,j;
  bool b;
  if (ba == NULL || str == NULL) {
    return false;
  }
  i = ba->numOfBits - 1; j = 0;
  while (i >= 0) {
    boolarr_get(ba,i,&b);
    str[j] = (datatype)b + '0';
    i--;
    j++;
  }
  str[j] = '\0';
  return true;
}

/* Flip all bits */
bool boolarr_negate(boolarr* ba) {
  unsigned int i;
  bool b;
  for (i = 0; i < ba->numOfBits; i++) {
    boolarr_get(ba,i,&b);
    if (b == true) {
      boolarr_set(ba,i,false);
    }
    if (b == false) {
      boolarr_set(ba,i,true);
    }
  }
  return true;
}

/* Functions dealing with 2 bitwise-arrays */
/* Must be the same length */
datatype bitwise_and(datatype i, datatype j) {
  return i & j;
}

datatype bitwise_or(datatype i, datatype j) {
  return i | j;
}

datatype bitwise_xor(datatype i, datatype j) {
  return i ^ j;
}

boolarr* boolarr_bitwise(const boolarr* ba1, const boolarr* ba2, const logicalop l) {
  boolarr * bc;
  unsigned int i;
  if (ba1 == NULL || ba2 == NULL) {
    return NULL;
  }

  bc = boolarr_clone(ba1);
  switch(l) {
    case and:
      for (i = 0; i <= bc->size; i++) {
        bc->a[i] = bitwise_and(ba1->a[i],ba2->a[i]);
      }
      return bc;
    case or:
      for (i = 0; i <= bc->size; i++) {
        bc->a[i] = bitwise_or(ba1->a[i],ba2->a[i]);
      }
      return bc;
    case xor:
      for (i = 0; i <= bc->size; i++) {
        bc->a[i] = bitwise_xor(ba1->a[i],ba2->a[i]);
      }
      return bc;
    default:
      return NULL;
  }
}
