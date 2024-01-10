#include "interpreter.h"
#define TOLERANCE 0.000001
/*TODO: add SDL, then done*/
bool cmpInts(int first, int second) {
  if (first > second) {
    return true;
  }
  return false;
}

double ctrlNot(double input, bool flag) {
  if (flag == true) {
    return -input;
  }
  return input;
}

bool loopExecute(Program * p, Node * address, double start, double end) {
  double counter;
  bool flag;
  int cw = p->currentWord;
  flag = cmpInts(start,end);
  for (counter = ctrlNot(start,flag); counter <= ctrlNot(end,flag); counter++) {
    p->currentWord = cw;
    setVariable(address,ctrlNot(counter,flag));
    if (isMain(p) == false) {
      return false;
    }
  }
  return true;
}

bool doublesEqual(double dub1, double dub2) {
  if (fabs(dub1 - dub2) < TOLERANCE) {
    return true;
  }
  return false;
}

Node * getVarAddress(Program * p) {
  return (p->varArray) + *((char*)(p->wordsArray[p->currentWord])) - 'A';
}

bool turtleRotate(Turtle * t, double angleOffset) {
  if (t == NULL) {
    return false;
  }
  t->angle = t->angle + (2.0 * M_PI / 360.0) * angleOffset;
  return true;
}

double fetchNumber(Program * p) {
  double number;
  Node * var_address;
  if (isNumber(p) == true) {
    assert(sscanf(p->wordsArray[p->currentWord],"%lf",&number) == 1);
  }
  if (isVar(p) == true) {
    var_address = getVarAddress(p);
    if (var_address->set == false) {
      fprintf(stderr,"Warning. Variable not set.\n");
    }
    number = var_address->number;
  }
  return number;
}

#ifdef ADDSDL
bool displayTurtleTrajectory(Program * p, Turtle * t) {
  int xOffset, yOffset;
  if (p == NULL || t == NULL) {
    return false;
  }
  xOffset = WWIDTH / 2; yOffset = WHEIGHT / 2;
  SDL_RenderDrawLine((p->sw).renderer, (int)((p->turtle).x + xOffset),
                                  (int)((p->turtle).y + yOffset),
                                  (int)(t->x + xOffset),
                                  (int)(t->y + yOffset));
  SDL_Delay(MILLISECONDDELAY);
  Neill_SDL_UpdateScreen(&(p->sw));
  Neill_SDL_Events(&(p->sw));
  return true;
}
#endif

bool turtleMove(Turtle * t, double distance) {
  if (t == NULL) {
    return false;
  }
  t->x = t->x + distance * cos(t->angle);
  t->y = t->y + distance * sin(t->angle);
  return true;
}

bool setVariable(Node * address, double number) {
  if (address == NULL) {
    return false;
  }
  address->number = number;
  address->set = true;
  return true;
}

double postfixCalculation(Program * p) {
  double first, second;
  if (stack_pop(p->s,&second) == false) {
    printf("Warning: stack empty before pop.\n");
  }
  if (stack_pop(p->s,&first) == false) {
    printf("Warning: stack empty before pop.\n");
  }
  if (strcmp(p->wordsArray[p->currentWord],"+") == 0) {
    first = first + second;
  }
  if (strcmp(p->wordsArray[p->currentWord],"-") == 0) {
    first = first - second;
  }
  if (strcmp(p->wordsArray[p->currentWord],"*") == 0) {
    first = first * second;
  }
  if (strcmp(p->wordsArray[p->currentWord],"/") == 0) {
    first = first / second;
  }
  return first;
}

#ifdef TEST_INTERP
#define TOLERANCE 0.000001
START_TEST(test_loopExecute) {
  Node * var_address;
  Program * p = programInit();
  programInsert(p,"F");
  var_address = getVarAddress(p);
  p->currentWord++;
  (p->turtle).x = 3.6;(p->turtle).y = -2.7;
  programInsert(p,"{");
  programInsert(p,"FD");programInsert(p,"48.6");
  programInsert(p,"RT");programInsert(p,"72");
  programInsert(p,"}");

  ck_assert_int_eq(loopExecute(p,var_address,1.6,5.9),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).x,3.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-2.7),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);
  ck_assert_int_eq(doublesEqual(var_address->number,5.6),(int)true);

  p->currentWord = 1;
  ck_assert_int_eq(loopExecute(p,var_address,5.9,1.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).x,3.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-2.7),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);
  ck_assert_int_eq(doublesEqual(var_address->number,1.9),(int)true);

  p->currentWord = 1;
  ck_assert_int_eq(loopExecute(p,var_address,-5.9,-1.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).x,3.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-2.7),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);
  ck_assert_int_eq(doublesEqual(var_address->number,-1.9),(int)true);

  p->currentWord = 1;
  ck_assert_int_eq(loopExecute(p,var_address,-1.6,-5.9),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).x,3.6),(int)true);
  ck_assert_int_eq(doublesEqual((p->turtle).y,-2.7),(int)true);
  ck_assert_int_eq(var_address->set,(int)true);
  ck_assert_int_eq(doublesEqual(var_address->number,-5.6),(int)true);

  ck_assert_int_eq(loopExecute(p,var_address,-1.6,-5.9),(int)false);
  programFree(p);
} END_TEST

START_TEST(test_cmpInts) {
  ck_assert_int_eq(cmpInts(1,0),(int)true);
  ck_assert_int_eq(cmpInts(1,1),(int)false);
  ck_assert_int_eq(cmpInts(0,1),(int)false);
} END_TEST

START_TEST(test_ctrlNot) {
  ck_assert_int_eq(doublesEqual(1.65,ctrlNot(1.65,false)),(int)true);
  ck_assert_int_eq(doublesEqual(-1.65,ctrlNot(1.65,true)),(int)true);
  ck_assert_int_eq(doublesEqual(-1.65,ctrlNot(-1.65,false)),(int)true);
  ck_assert_int_eq(doublesEqual(1.65,ctrlNot(-1.65,true)),(int)true);
} END_TEST

START_TEST(test_postfixCalculation) {
  Program * p = programInit();
  stack_push(p->s,27.5);stack_push(p->s,63.9);
  *((char*)(p->wordsArray[p->currentWord])) = '+';
  ck_assert_int_eq(doublesEqual(postfixCalculation(p),27.5 + 63.9),(int)true);
  stack_push(p->s,27.5);stack_push(p->s,63.9);
  *((char*)(p->wordsArray[p->currentWord])) = '-';
  ck_assert_int_eq(doublesEqual(postfixCalculation(p),27.5 - 63.9),(int)true);
  stack_push(p->s,27.5);stack_push(p->s,63.9);
  *((char*)(p->wordsArray[p->currentWord])) = '*';
  ck_assert_int_eq(doublesEqual(postfixCalculation(p),27.5 * 63.9),(int)true);
  stack_push(p->s,27.5);stack_push(p->s,63.9);
  *((char*)(p->wordsArray[p->currentWord])) = '/';
  ck_assert_int_eq(doublesEqual(postfixCalculation(p),27.5 / 63.9),(int)true);
  doublesEqual(postfixCalculation(p),27.5 / 63.9);
  programFree(p);
} END_TEST

START_TEST(test_setVariable) {
  Node variable;
  ck_assert_int_eq(setVariable(NULL,57.2),(int)false);
  variable.set = false;
  variable.number = 0;
  ck_assert_int_eq(setVariable(&variable,57.2),(int)true);
} END_TEST

START_TEST(test_doublesEqual) {
  ck_assert_int_eq(doublesEqual(M_PI,M_PI),(int)true);
  ck_assert_int_eq(doublesEqual(0,-(M_PI/4.0)),(int)false);
} END_TEST

START_TEST(test_turtleMove) {
  Turtle t;
  ck_assert_int_eq(turtleMove(NULL,36.7),(int)false);
  turtleRotate(&t,45);
  ck_assert_int_eq(turtleMove(&t,36.7),(int)true);
  ck_assert_int_eq(doublesEqual(t.x,36.7/sqrt(2)),(int)true);
  ck_assert_int_eq(doublesEqual(t.y,36.7/sqrt(2)),(int)true);
  turtleRotate(&t,-180);
  ck_assert_int_eq(turtleMove(&t,36.7),(int)true);
  ck_assert_int_eq(doublesEqual(t.x,0.0),(int)true);
  ck_assert_int_eq(doublesEqual(t.y,0.0),(int)true);
} END_TEST

START_TEST(test_fetchNumber) {
  Program * p = programInit();
  programInsert(p,"10.5");
  ck_assert_int_eq(doublesEqual(10.5,fetchNumber(p)),(int)true);
  p->currentWord++;
  programInsert(p,"C");
  p->varArray['C'-'A'].number = 24.5;
  ck_assert_int_eq(doublesEqual(24.5,fetchNumber(p)),(int)true);
  p->varArray['C'-'A'].set = true;
  ck_assert_int_eq(doublesEqual(24.5,fetchNumber(p)),(int)true);
  programFree(p);
} END_TEST

START_TEST(test_turtleRotate) {
  Turtle t;
  ck_assert_int_eq(turtleRotate(NULL,0.5),(int)false);
  ck_assert_int_eq(turtleRotate(&t,45),(int)true);
  ck_assert_int_eq(doublesEqual(M_PI / 4.0,t.angle),(int)true);
} END_TEST

START_TEST(test_getVarAddress) {
  Node * var_address;
  Program * p = programInit();
  programInsert(p,"C");
  p->varArray['C'-'A'].number = 24.5;
  var_address = getVarAddress(p);
  ck_assert_int_eq(doublesEqual(var_address->number,24.5),(int)true);
  ck_assert_int_eq(var_address->set,(int)false);
  programFree(p);
} END_TEST

START_TEST(dryrun) {
  Program * p = programInit();
  double i,j;
  ck_assert_int_eq(sizeof(double),sizeof(stacktype));
  for (i = 0.1; i < 100.1; i++) {
    stack_push(p->s,i);
  }
  for (i = 99.1; i >= 0.1; i--) {
    stack_pop(p->s,&j);
    ck_assert_int_eq(doublesEqual(i,j),(int)true);
  }
  programFree(p);
} END_TEST

Suite * prog_suite(void) {
  Suite * s;
  TCase * basic;

  s = suite_create("Interpreter");
  basic = tcase_create("Basic");
  tcase_set_timeout(basic,240);
  tcase_add_test(basic, dryrun);
  tcase_add_test(basic, test_doublesEqual);
  tcase_add_test(basic, test_getVarAddress);
  tcase_add_test(basic, test_turtleRotate);
  tcase_add_test(basic, test_fetchNumber);
  tcase_add_test(basic, test_turtleMove);
  tcase_add_test(basic, test_setVariable);
  tcase_add_test(basic, test_postfixCalculation);
  tcase_add_test(basic, test_cmpInts);
  tcase_add_test(basic, test_ctrlNot);
  tcase_add_test(basic, test_loopExecute);
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
