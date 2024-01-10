#include "postfix.h"

void infixToPostfix(Program * p) {
  unsigned int i, currentWord;
  double pop;
  stack * postfix = stack_init();
  i = currentWord = p->currentWord;
  while (strcmp(p->wordsArray[p->currentWord],";") != 0) {
    if (isVarnum(p) == true) {
      if (p->currentWord > i) {
        strcpy(p->wordsArray[i],p->wordsArray[p->currentWord]);
      }
      i++;
    }

    if (isOp(p) == true) {
      while (stack_peek(postfix,&pop) == true && precedence((char)pop) >= precedence(*((char*)(p->wordsArray[p->currentWord])))) {
        stack_pop(postfix,&pop);
        *((char*)(p->wordsArray[i])) = (char)pop;
        *((char*)(p->wordsArray[i]) + 1) = '\0';
        i++;
      }
      stack_push(postfix,*((char*)(p->wordsArray[p->currentWord])));
    }
    p->currentWord++;
  }

  while (stack_peek(postfix,&pop) == true) {
    stack_pop(postfix,&pop);
    *((char*)(p->wordsArray[i])) = (char)pop;
    *((char*)(p->wordsArray[i]) + 1) = '\0';
    i++;
  }

  stack_free(postfix);
}

int precedence(char operator) {
  if (operator == '+' || operator == '-') {
    return 1;
  }
  if (operator == '*' || operator == '/') {
    return 2;
  }
  return 0;
}

#ifdef TEST_POSTFIX
START_TEST(test_precedence) {
  ck_assert_int_eq(precedence('+'),1);
  ck_assert_int_eq(precedence('-'),1);
  ck_assert_int_eq(precedence('*'),2);
  ck_assert_int_eq(precedence('/'),2);
  ck_assert_int_eq(precedence('a'),0);
} END_TEST

START_TEST(test_infixToPostfix) {
  Program * p = programInit();
  programInsert(p,"8.5");
  programInsert(p,"+");
  programInsert(p,"3.6");
  programInsert(p,";");
  infixToPostfix(p);
  p->currentWord = 0;
  ck_assert_str_eq(p->wordsArray[p->currentWord],"8.5");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 1],"3.6");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 2],"+");

  p->currentWord = 4;
  programInsert(p,"8.5");
  programInsert(p,"3.6");
  programInsert(p,"+");
  programInsert(p,";");
  infixToPostfix(p);
  p->currentWord = 4;
  ck_assert_str_eq(p->wordsArray[p->currentWord],"8.5");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 1],"3.6");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 2],"+");

  p->currentWord = 8;
  programInsert(p,"8.5");
  programInsert(p,"+");
  programInsert(p,"3.6");
  programInsert(p,"*");
  programInsert(p,"M_PI");
  programInsert(p,";");
  infixToPostfix(p);
  p->currentWord = 8;
  ck_assert_str_eq(p->wordsArray[p->currentWord],"8.5");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 1],"3.6");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 2],"M_PI");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 3],"*");
  ck_assert_str_eq(p->wordsArray[p->currentWord + 4],"+");

  p->currentWord = 14;
  programInsert(p,"8.5");
  programInsert(p,"+");
  programInsert(p,"3.6");
  programInsert(p,"*");
  programInsert(p,"M_PI");
  programInsert(p,"-");
  programInsert(p,"4.5");
  programInsert(p,"/");
  programInsert(p,"36.7");
  programInsert(p,";");
  infixToPostfix(p);
  p->currentWord = 14;
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"8.5");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"3.6");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"M_PI");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"*");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"+");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"4.5");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"36.7");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"/");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"-");
  p->currentWord = 24;
  programInsert(p,"8.5");
  programInsert(p,"3.6");
  programInsert(p,"M_PI");
  programInsert(p,"*");
  programInsert(p,"+");
  programInsert(p,"4.5");
  programInsert(p,"36.7");
  programInsert(p,"/");
  programInsert(p,"-");
  programInsert(p,";");
  infixToPostfix(p);
  p->currentWord = 24;
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"8.5");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"3.6");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"M_PI");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"*");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"4.5");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"36.7");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"/");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"+");
  ck_assert_str_eq(p->wordsArray[p->currentWord++],"-");
  programFree(p);
} END_TEST

Suite * prog_suite(void) {
  Suite * s;
  TCase * basic;

  s = suite_create("postfix");
  basic = tcase_create("Basic");
  tcase_add_test(basic, test_precedence);
  tcase_add_test(basic, test_infixToPostfix);
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
