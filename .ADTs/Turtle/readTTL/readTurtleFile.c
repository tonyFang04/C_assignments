#include "readTurtleFile.h"
#define TERMINAL_INPUT_NUMBER 2
#define STRMAXLEN 200
bool checkTerminalInputNumber(int argc) {
  if (argc == TERMINAL_INPUT_NUMBER) {
    return true;
  }
  return false;
}

bool checkFileName(char * filename) {
  int i;
  for (i = 0; i < (int)strlen(filename); i++) {
    if (filename[i] == '.') {
      if (i == (int)strlen(filename) - 4 &&
          filename[i + 1] == 't' &&
          filename[i + 2] == 't' &&
          filename[i + 3] == 'l') {
        return true;
      }
      return false;
    }
  }
  return false;
}

/*this is actually testable, can write some dummy files
and check the word length. Change the output to bool so easier to test*/
bool readFile(Program * p, char * filename) {
  int i;
  char buffer[STRMAXLEN];
  FILE * fp;
  if (!(fp = fopen(filename, "r"))) {
    fprintf(stderr, "Cannot open %s\n",filename);
    return false;
  }
  i = 0;
  while(fscanf(fp, "%s", buffer) == 1) {
    if ((int)strlen(buffer) > MAXWORDSIZE - 1) {
      fprintf(stderr, "Length of %s is longer than %d, the maximum length allowed for a word\n",buffer,MAXWORDSIZE - 1);
      return false;
    }
    programInsert(p, buffer);
    i++;
  }
  fclose(fp);
  return true;
}

#ifdef TEST_READTURTLEFILE

START_TEST(test_checkFileName) {
  ck_assert_int_eq(checkFileName("simp.ttl"),(int)true);
  ck_assert_int_eq(checkFileName("ecchishiyou.ttl"),(int)true);
  ck_assert_int_eq(checkFileName("simp.ttl.ttl"),(int)false);
  ck_assert_int_eq(checkFileName("simp.ttll"),(int)false);
  ck_assert_int_eq(checkFileName("simp.doc"),(int)false);
  ck_assert_int_eq(checkFileName("simp..tt"),(int)false);
  ck_assert_int_eq(checkFileName("simp"),(int)false);
} END_TEST

START_TEST(test_checkTerminalInputNumber) {
  ck_assert_int_eq(checkTerminalInputNumber(2),(int)true);
  ck_assert_int_eq(checkTerminalInputNumber(3),(int)false);
  ck_assert_int_eq(checkTerminalInputNumber(1),(int)false);
} END_TEST

START_TEST(test_readFile) {
  Program * p = programInit();
  ck_assert_int_eq(readFile(p,"simpaesg1.ttl"),(int)false);
  ck_assert_int_eq(readFile(p,"simp1.ttl"),(int)true);
  programFree(p);
  p = programInit();
  ck_assert_int_eq(readFile(p,"simp2.ttl"),(int)false);
  programFree(p);
} END_TEST

Suite * prog_suite(void) {
  Suite * s;
  TCase * basic;

  s = suite_create("readTTL");
  basic = tcase_create("Basic");
  tcase_set_timeout(basic,240);
  tcase_add_test(basic, test_checkFileName);
  tcase_add_test(basic, test_checkTerminalInputNumber);
  tcase_add_test(basic, test_readFile);
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
