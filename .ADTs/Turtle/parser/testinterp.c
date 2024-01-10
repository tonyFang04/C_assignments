#include "parser.h"
START_TEST(test_FD_RT_LT_revisited) {
  int i;
  Program * p = programInit();
  programInsert(p,"FD");
  programInsert(p,"36.7");
  ck_assert_int_eq(isFD(p),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).x,36.7),(int)true);
  p->currentWord++;
  programInsert(p,"RT");
  programInsert(p,"45");
  ck_assert_int_eq(isRT(p),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).angle,-(M_PI/4.0)),(int)true);
  p->currentWord++;
  programInsert(p,"LT");
  programInsert(p,"45");
  ck_assert_int_eq(isLT(p),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).angle,0),(int)true);
  p->currentWord++;
  programInsert(p,"FD");
  programInsert(p,"-36.7");
  ck_assert_int_eq(isFD(p),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).x,0),(int)true);
  p->currentWord++;
  (p->turtle).x = 66.6;
  (p->turtle).y = -55.5;
  for (i = 0; i < 5; i++) {
    programInsert(p,"FD");
    programInsert(p,"58.2");
    ck_assert_int_eq(isFD(p),(int)true);
    p->currentWord++;
    programInsert(p,"RT");
    programInsert(p,"72");
    ck_assert_int_eq(isRT(p),(int)true);
    p->currentWord++;
  }
  ck_assert_int_eq(doublesEqual((p->turtle).x,66.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-55.5),(int)true);
  programInsert(p,"ft");
  ck_assert_int_eq(isFD(p),(int)false);
  ck_assert_int_eq(doublesEqual((p->turtle).x,66.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-55.5),(int)true);
  ck_assert_int_eq(isLT(p),(int)false);
  ck_assert_int_eq(doublesEqual((p->turtle).x,66.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-55.5),(int)true);
  ck_assert_int_eq(isRT(p),(int)false);
  ck_assert_int_eq(doublesEqual((p->turtle).x,66.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-55.5),(int)true);
  programFree(p);
} END_TEST

START_TEST(test_isPolish_revisited) {
  double number;
  Program * p = programInit();

  programInsert(p,"8");programInsert(p,"2");
  programInsert(p,"5");programInsert(p,"*");
  programInsert(p,";");
  ck_assert_int_eq(isPolish(p),(int)true);
  stack_peek(p->s,&number);
  ck_assert_int_eq(doublesEqual(10,number),(int)true);
  p->currentWord++;

  programInsert(p,"+");programInsert(p,";");
  ck_assert_int_eq(isPolish(p),(int)true);
  stack_peek(p->s,&number);
  ck_assert_int_eq(doublesEqual(18,number),(int)true);
  p->currentWord++;

  programInsert(p,"1");programInsert(p,"3");
  programInsert(p,"2");programInsert(p,"*");
  programInsert(p,";");
  ck_assert_int_eq(isPolish(p),(int)true);
  stack_peek(p->s,&number);
  ck_assert_int_eq(doublesEqual(6,number),(int)true);
  p->currentWord++;

  programInsert(p,"+");programInsert(p,"2.5");
  programInsert(p,"-");programInsert(p,";");
  ck_assert_int_eq(isPolish(p),(int)true);
  stack_peek(p->s,&number);
  ck_assert_int_eq(doublesEqual(4.5,number),(int)true);
  p->currentWord++;

  programInsert(p,"/");programInsert(p,";");
  ck_assert_int_eq(isPolish(p),(int)true);
  stack_peek(p->s,&number);
  ck_assert_int_eq(doublesEqual(4,number),(int)true);
  p->currentWord++;

  programInsert(p,"ff");
  ck_assert_int_eq(isPolish(p),(int)false);
  programFree(p);
} END_TEST

START_TEST(test_isSet_revisited) {
  Node * var_address;
  Program * p = programInit();
  programInsert(p,"SET");programInsert(p,"A");
  programInsert(p,":=");programInsert(p,"8.5");
  programInsert(p,"2");programInsert(p,"*");
  programInsert(p,";");
  ck_assert_int_eq(isSet(p),(int)true);
  ck_assert_int_eq(stack_pop(p->s,NULL),(int)false);
  p->currentWord = 1;
  var_address = getVarAddress(p);
  ck_assert_int_eq(doublesEqual(17,var_address->number),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);

  p->currentWord = 7;
  programInsert(p,"SET");programInsert(p,"B");
  programInsert(p,":=");programInsert(p,"A");
  programInsert(p,"2");programInsert(p,"*");
  programInsert(p,";");
  ck_assert_int_eq(isSet(p),(int)true);
  ck_assert_int_eq(stack_pop(p->s,NULL),(int)false);

  p->currentWord = 8;
  var_address = getVarAddress(p);
  ck_assert_int_eq(doublesEqual(34,var_address->number),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);

  p->currentWord = 14;
  programInsert(p,"SET");programInsert(p,"B");
  programInsert(p,":=");
  ck_assert_int_eq(isSet(p),(int)false);
  p->currentWord = 14;
  programInsert(p,";");
  ck_assert_int_eq(isSet(p),(int)true);
  programFree(p);
} END_TEST

START_TEST(test_isDo_revisited) {
  Program * p = programInit();
  Node * var_address;
  programInsert(p,"DO");
  programInsert(p,"F");programInsert(p,"FROM");
  programInsert(p,"1.6");programInsert(p,"TO");
  programInsert(p,"5.9");programInsert(p,"{");
  programInsert(p,"}");
  ck_assert_int_eq(isDo(p),(int)true);
  p->currentWord = 1;
  var_address = getVarAddress(p);
  ck_assert_int_eq(doublesEqual(var_address->number,5.6),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);

  p->currentWord = 8;
  programInsert(p,"DO");
  programInsert(p,"F");programInsert(p,"FROM");
  programInsert(p,"-1.6");programInsert(p,"TO");
  programInsert(p,"-5.9");programInsert(p,"{");
  ck_assert_int_eq(isDo(p),(int)false);
  p->currentWord = 8;programInsert(p,"}");
  ck_assert_int_eq(isDo(p),(int)true);
  ck_assert_int_eq(doublesEqual(var_address->number,-5.6),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);
  programFree(p);
} END_TEST

START_TEST(test_isMain_revisited) {
  Program * p = programInit();
  readFile(p,"../ttl_scripts/simp_c.ttl");
  ck_assert_int_eq(isMain(p),(int)true);
  p->currentWord++;
  readFile(p,"../ttl_scripts/simp.ttl");
  ck_assert_int_eq(isMain(p),(int)true);
  p->currentWord++;
  readFile(p,"../ttl_scripts/spiral1_c.ttl");
  ck_assert_int_eq(isMain(p),(int)true);
  p->currentWord++;
  readFile(p,"../ttl_scripts/spiral2_c.ttl");
  ck_assert_int_eq(isMain(p),(int)true);
  ck_assert_str_eq(p->wordsArray[p->currentWord+1],"");
  ck_assert_str_eq(p->wordsArray[p->currentWord],"}");
  ck_assert_str_eq(p->wordsArray[p->currentWord-1],"}");
  ck_assert_str_eq(p->wordsArray[p->currentWord-2],"}");
  programFree(p);
} END_TEST

Suite * prog_suite(void) {
  Suite * s;
  TCase * basic;

  s = suite_create("test_interp");
  basic = tcase_create("Basic");
  tcase_set_timeout(basic,240);
  tcase_add_test(basic, test_FD_RT_LT_revisited);
  tcase_add_test(basic, test_isPolish_revisited);
  tcase_add_test(basic, test_isSet_revisited);
  tcase_add_test(basic, test_isDo_revisited);
  tcase_add_test(basic, test_isMain_revisited);
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
