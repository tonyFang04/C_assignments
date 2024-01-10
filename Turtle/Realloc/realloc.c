#include "realloc.h"

#ifdef TEST_REALLOC /*need to have specific name for each .c file to test*/
#include <check.h>
#define WORDS 370119
#define STRMAXLEN 200
#endif

Program * programInit() {
  Program * prog = (Program *)ncalloc(sizeof(Program),1);
  prog->size = 0;
  prog->currentWord = 0;
  prog->capacity = INITIAL_CAPACITY;
  prog-> wordsArray = n2dcalloc(INITIAL_CAPACITY,MAXWORDSIZE,sizeof(char));
  #ifdef INTERP
  prog->s = stack_init();
  #endif
  return prog;
}

void programFree(Program * prog) {
  if (prog == NULL) {
    return;
  }
  n2dfree(prog->wordsArray,prog->capacity);
  #ifdef INTERP
  stack_free(prog->s);
  #endif
  free(prog);
}

unsigned int programSize(const Program * prog) {
  assert(prog != NULL);
  return prog->size;
}

void programInsert(Program * prog, const char * word) {
  assert(prog != NULL);
  strcpy(prog->wordsArray[programSize(prog)],word);
  prog->size++;
  if (prog->size >= prog->capacity) {
    prog->wordsArray = n2drecalloc(prog->wordsArray,prog->capacity,prog->capacity * SCALE_FACTOR,MAXWORDSIZE,MAXWORDSIZE,sizeof(char));
    prog->capacity = prog->capacity * SCALE_FACTOR;
  }
}

#ifdef TEST_REALLOC
START_TEST(test_dtype_Prog) {
  Program * p;
  FILE *fp;
  unsigned int j;
  char buffer[200];
  p = NULL;
  programFree(p);
  p = programInit();
  fp = nfopen("../../../Data/Words/eng_370k_shuffle.txt", "rt");
  for(j=0; j<WORDS; j++){
     ck_assert_int_eq(programSize(p),j);
     if(fscanf(fp, "%s", buffer)!=1){
        on_error("Failed to scan in a word?");
     }
     programInsert(p, buffer);
  }
  fclose(fp);
  fp = nfopen("../../../Data/Words/eng_370k_shuffle.txt", "rt");
  for(j=0; j<WORDS; j++) {
    ck_assert_int_eq(fscanf(fp, "%s", buffer),1);
    ck_assert_str_eq(buffer,p->wordsArray[j]);
  }
  programFree(p);
  fclose(fp);
} END_TEST

Suite * prog_suite(void) {
  Suite * s;
  TCase * basic;

  s = suite_create("Program_Dtype");
  basic = tcase_create("Basic");
  tcase_set_timeout(basic,240);
  tcase_add_test(basic, test_dtype_Prog);
  suite_add_tcase(s, basic);

  return s;
}


int main(void) {
  int no_failed = 0;
  Suite *s;
  SRunner *runner;

  s = prog_suite();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif
