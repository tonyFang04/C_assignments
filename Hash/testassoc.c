#include "specific.h"
#include "assoc.h"
#include <math.h>
#define ARRSIZE 15
#define WORDS 370119
#define NUMRANGE 100000

struct testSubject {
  int i;
  double k[3][3];
};
typedef struct testSubject testSubject;

char* strduprev(char* str);

int main(void)
{
   FILE *fp;
   char* tstr;
   void *p;
   unsigned int lngst;
   unsigned int j;
   assoc* a;
   static int i[WORDS];
   int unique_numbers;
   int hist[NUMRANGE];
   static char strs[WORDS][50]={{0}};
   testSubject * k;
   a = assoc_init(0);
   fp = nfopen("Data/Words/eng_370k_shuffle.txt", "rt");
   for(j=0; j<WORDS; j++){
      assert(assoc_count(a)==j);
      i[j] = j;
      if(fscanf(fp, "%s", strs[j])!=1){
         on_error("Failed to scan in a word?");
      }
      assoc_insert(&a, strs[j], &i[j]);
   }
   fclose(fp);
   lngst = 0;
   for(j=0; j<WORDS; j++){
      if(strlen(strs[j]) > lngst){
         tstr = strduprev(strs[j]);
         if(strcmp(tstr, strs[j])){
            if((p = assoc_lookup(a, tstr))!=NULL){
               lngst = strlen(tstr);
               printf("%s <-> %s = %d (%d in the file)\n", tstr, strs[j], lngst, *(int*)p);
            }
         }
         free(tstr);
      }
   }
   assoc_free(a);
   memset(hist,0,sizeof(int) * NUMRANGE);
   k = (testSubject*)ncalloc(sizeof(testSubject),WORDS);
   srand(time(NULL));
   a = assoc_init(sizeof(testSubject));
   for(j=0; j<NUMRANGE; j++){
      k[j].i = rand()%NUMRANGE;
      assoc_insert(&a, &k[j], NULL);
      hist[k[j].i] += 1;
   }

   unique_numbers = 0;
   for (j=0; j<NUMRANGE; j++) {
     if (hist[j] > 0) {
       unique_numbers++;
     }
   }
   /*
   for (j=0; j<NUMRANGE; j++) {
     printf("%d,",hist[j]);
   }
   printf("\n%d\n",unique_numbers);
   */
   assert(unique_numbers = assoc_count(a));
   printf("%d unique numbers out of %d\n", assoc_count(a), j);
   /*
   for (j=0; j<INITIAL_CAPACITY; j++) {
     if (a->table[0][j].key != NULL)
       printf("%d,",*(int*)(a->table[0][j].key));
   }
   printf("\n");
   for (j=0; j<INITIAL_CAPACITY; j++) {
     if (a->table[1][j].key != NULL)
       printf("%d,",*(int*)(a->table[1][j].key));
   }
   printf("\n");
   */
   assoc_free(a);
   free(k);
   return 0;
}

/* Make a copy, reversed */
char* strduprev(char* str)
{
   int i, j;
   char* t;
   j = strlen(str);
   t = ncalloc(j+1, 1); /* Add null char */
   strcpy(t, str);
   for(i=0, j--; i<j; i++,j--){
      /* Swap using bit-twiddling */
      t[i] ^= t[j];
      t[j] ^= t[i];
      t[i] ^= t[j];
   }
   return t;
}
