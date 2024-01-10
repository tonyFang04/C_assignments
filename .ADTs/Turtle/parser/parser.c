#include "parser.h"

bool isOp(Program * p) {
  if (strcmp(p->wordsArray[p->currentWord],"+") == 0) {
    return true;
  }
  if (strcmp(p->wordsArray[p->currentWord],"-") == 0) {
    return true;
  }
  if (strcmp(p->wordsArray[p->currentWord],"*") == 0) {
    return true;
  }
  if (strcmp(p->wordsArray[p->currentWord],"/") == 0) {
    return true;
  }
  return false;
}

bool isNumber(Program * p) {
  double temp;
  if (sscanf(p->wordsArray[p->currentWord],"%lf",&temp) == 1) {
    return true;
  }
  return false;
}

bool isVar(Program * p) {
  if (strcmp(p->wordsArray[p->currentWord],"Z") <= 0 &&
      strcmp(p->wordsArray[p->currentWord],"A") >= 0) {
    return true;
  }
  return false;
}

bool isVarnum(Program * p) {
  if (isVar(p) == true) {
    return true;
  }
  if (isNumber(p) == true) {
    return true;
  }
  return false;
}

bool isPolish(Program * p) {
  #ifdef INTERP
  double number;
  #endif
  if (strcmp(p->wordsArray[p->currentWord],";") == 0) {
    return true;
  }
  if (isOp(p) == true) {
    #ifdef INTERP
    number = postfixCalculation(p);
    stack_push(p->s,number);
    #endif
    p->currentWord++;
    if (isPolish(p) == true) {
      return true;
    }
  }
  if (isVarnum(p) == true) {
    #ifdef INTERP
    number = fetchNumber(p);
    stack_push(p->s,number);
    #endif
    p->currentWord++;
    if (isPolish(p) == true) {
      return true;
    }
  }
  return false;
}

bool isSet(Program * p) {
  #ifdef INTERP
  Node * var_address;
  double number;
  #endif
  if (strcmp(p->wordsArray[p->currentWord],"SET") == 0 &&
      strcmp(p->wordsArray[p->currentWord + 2],":=") == 0) {
    p->currentWord++;
    if (isVar(p) == true) {
      #ifdef INTERP
      var_address = getVarAddress(p);
      #endif
      p->currentWord = p->currentWord + 2;
      if (isPolish(p) == true) {
        #ifdef INTERP
        if(stack_pop(p->s,&number) == false) {
          printf("Warning: stack empty before pop.\n");
        }
        setVariable(var_address,number);
        #endif
        return true;
      }
    }
  }
  return false;
}

bool isFD(Program * p) {
  #ifdef INTERP
  double distance;
  Turtle newTurtle;
  #endif
  if (strcmp(p->wordsArray[p->currentWord],"FD") == 0) {
    p->currentWord++;
    if (isVarnum(p) == true) {
      #ifdef INTERP
      newTurtle = p->turtle;
      distance = fetchNumber(p);
      turtleMove(&newTurtle,distance);
      #ifdef ADDSDL
      displayTurtleTrajectory(p,&newTurtle);
      #endif
      p->turtle = newTurtle;
      #endif
      return true;
    }
  }
  return false;
}

bool isLT(Program * p) {
  #ifdef INTERP
  double angleOffset;
  #endif
  if (strcmp(p->wordsArray[p->currentWord],"LT") == 0) {
    p->currentWord++;
    if (isVarnum(p) == true) {
      #ifdef INTERP
      angleOffset = fetchNumber(p);
      turtleRotate(&(p->turtle),angleOffset);
      #endif
      return true;
    }
  }
  return false;
}

bool isRT(Program * p) {
  #ifdef INTERP
  double angleOffset;
  #endif
  if (strcmp(p->wordsArray[p->currentWord],"RT") == 0) {
    p->currentWord++;
    if (isVarnum(p) == true) {
      #ifdef INTERP
      angleOffset = fetchNumber(p);
      turtleRotate(&(p->turtle),-angleOffset);
      #endif
      return true;
    }
  }
  return false;
}

bool isMain(Program * p) {
  if (strcmp(p->wordsArray[p->currentWord],"{") == 0) {
    p->currentWord++;
    if (isInstructLst(p) == true) {
      return true;
    }
  }
  return false;
}

bool isInstructLst(Program * p) {
  if (strcmp(p->wordsArray[p->currentWord],"}") == 0) {
    return true;
  }
  if (isInstruction(p) == true) {
    p->currentWord++;
    if (isInstructLst(p) == true) {
      return true;
    }
  }
  return false;
}

bool isInstruction(Program * p) {
  if (isFD(p) == true ||
      isLT(p) == true ||
      isRT(p) == true ||
      isSet(p) == true ||
      isDo(p) == true) {
    return true;
  }
  return false;
}

bool isDo(Program * p) {
  #ifdef INTERP
  Node * var_address;
  double loop_start, loop_end;
  #endif
  if (strcmp(p->wordsArray[p->currentWord],"DO") == 0 &&
      strcmp(p->wordsArray[p->currentWord + 2],"FROM") == 0 &&
      strcmp(p->wordsArray[p->currentWord + 4],"TO") == 0) {
    p->currentWord++;
    if (isVar(p) == true) {
      #ifdef INTERP
      var_address = getVarAddress(p);
      #endif
      p->currentWord = p->currentWord + 2;
      if (isVarnum(p) == true) {
        #ifdef INTERP
        loop_start = fetchNumber(p);
        #endif
        p->currentWord = p->currentWord + 2;
        if (isVarnum(p) == true) {
          #ifdef INTERP
          loop_end = fetchNumber(p);
          #endif
          p->currentWord++;
          #ifdef PARSER
          if (isMain(p) == true) {
            return true;
          }
          #endif
          #ifdef INTERP
          if (loopExecute(p,var_address,loop_start,loop_end) == true) {
            return true;
          }
          #endif
        }
      }
    }
  }
  return false;
}

#ifdef TEST_PARSER
START_TEST(test_isMain) {
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

START_TEST(test_isInstructLst) {
  Program * p = programInit();
  programInsert(p,"FD");
  programInsert(p,"8");
  ck_assert_int_eq(isInstructLst(p),(int)false);
  programInsert(p,"FD");
  programInsert(p,"8");
  programInsert(p,"}");
  ck_assert_int_eq(isInstructLst(p),(int)true);
  p->currentWord++;
  programInsert(p,"RT");
  programInsert(p,"45");
  programInsert(p,"SET");
  programInsert(p,"A");
  programInsert(p,":=");
  programInsert(p,"70");
  programInsert(p,";");
  programInsert(p,"}");
  ck_assert_int_eq(isInstructLst(p),(int)true);
  programFree(p);
} END_TEST

START_TEST(test_isInstruction) {
  Program * p = programInit();
  programInsert(p,"FD");
  programInsert(p,"8");
  ck_assert_int_eq(isInstruction(p),(int)true);
  p->currentWord++;
  programInsert(p,"LT");
  programInsert(p,"10");
  ck_assert_int_eq(isInstruction(p),(int)true);
  p->currentWord++;
  programInsert(p,"RT");
  programInsert(p,"16");
  ck_assert_int_eq(isInstruction(p),(int)true);
  p->currentWord++;
  programInsert(p,"SET");
  programInsert(p,"A");
  programInsert(p,":=");
  programInsert(p,";");
  ck_assert_int_eq(isInstruction(p),(int)true);
  p->currentWord++;
  programInsert(p,"DO");
  programInsert(p,"A");
  programInsert(p,"FROM");
  programInsert(p,"10");
  programInsert(p,"TO");
  programInsert(p,"100");
  programInsert(p,"{");
  programInsert(p,"}");
  ck_assert_int_eq(isInstruction(p),(int)true);
  p->currentWord++;
  programInsert(p,"+");
  ck_assert_int_eq(isInstruction(p),(int)false);
  programFree(p);
} END_TEST

START_TEST(test_isDo) {
  Program * p = programInit();
  programInsert(p,"Do");
  ck_assert_int_eq(isDo(p),(int)false);
  p->currentWord++;
  programInsert(p,"DO");
  programInsert(p,"A");
  programInsert(p,"FRO");
  ck_assert_int_eq(isDo(p),(int)false);
  p->currentWord = p->currentWord + 3;
  programInsert(p,"DO");
  programInsert(p,"A");
  programInsert(p,"FROM");
  programInsert(p,"10");
  programInsert(p,"T");
  ck_assert_int_eq(isDo(p),(int)false);
  ck_assert_int_eq(p->currentWord,4);
  p->currentWord = p->currentWord + 5;
  programInsert(p,"DO");
  programInsert(p,"A");
  programInsert(p,"FROM");
  programInsert(p,"10");
  programInsert(p,"TO");
  programInsert(p,"100");
  programInsert(p,"}");
  ck_assert_int_eq(isDo(p),(int)false);
  ck_assert_str_eq(p->wordsArray[p->currentWord],"}");
  p->currentWord++;
  programInsert(p,"DO");
  programInsert(p,"A");
  programInsert(p,"FROM");
  programInsert(p,"10");
  programInsert(p,"TO");
  programInsert(p,"100");
  programInsert(p,"{");
  programInsert(p,"}");
  ck_assert_int_eq(isDo(p),(int)true);
  programFree(p);
} END_TEST

START_TEST(test_isFD_isRT_isLT) {
  Program * p = programInit();
  programInsert(p,"FD");
  programInsert(p,"A");
  ck_assert_int_eq(isFD(p),(int)true);
  p->currentWord--;
  ck_assert_int_eq(isRT(p),(int)false);
  ck_assert_int_eq(isLT(p),(int)false);
  p->currentWord = p->currentWord + 2;
  programInsert(p,"LT");
  programInsert(p,"A");
  ck_assert_int_eq(isFD(p),(int)false);
  ck_assert_int_eq(isRT(p),(int)false);
  ck_assert_int_eq(isLT(p),(int)true);
  p->currentWord++;
  programInsert(p,"RT");
  programInsert(p,"A");
  ck_assert_int_eq(isFD(p),(int)false);
  ck_assert_int_eq(isLT(p),(int)false);
  ck_assert_int_eq(isRT(p),(int)true);
  programFree(p);
} END_TEST

START_TEST(test_isSet) {
  Program * p = programInit();
  programInsert(p,"SET");
  programInsert(p,"A");
  programInsert(p,":=");
  programInsert(p,";");
  ck_assert_int_eq(isSet(p),(int)true);
  p->currentWord++;
  programInsert(p,"SeT");
  ck_assert_int_eq(isSet(p),(int)false);
  p->currentWord++;
  programInsert(p,"SET");
  programInsert(p,"A");
  programInsert(p,"=");
  programInsert(p,";");
  programFree(p);
} END_TEST

START_TEST(test_isOP) {
  Program * p = programInit();
  int i;
  int j = 0;
  *((char*)(p->wordsArray[p->currentWord])) = '+';
  ck_assert_int_eq(isOp(p),(int)true);
  *((char*)(p->wordsArray[p->currentWord])) = '-';
  ck_assert_int_eq(isOp(p),(int)true);
  *((char*)(p->wordsArray[p->currentWord])) = '*';
  ck_assert_int_eq(isOp(p),(int)true);
  *((char*)(p->wordsArray[p->currentWord])) = '/';
  ck_assert_int_eq(isOp(p),(int)true);
  for (i = 0; i <= 255; i++) {
    *((char*)(p->wordsArray[p->currentWord])) = i;
    if (isOp(p) == true) {
      j++;
    }
  }
  ck_assert_int_eq(i,256);
  ck_assert_int_eq(j,4);
  programFree(p);
} END_TEST

START_TEST(test_isNumber) {
  Program * p = programInit();
  programInsert(p,"1e5");
  ck_assert_int_eq(isNumber(p),(int)true);
  p->currentWord++;
  programInsert(p,".68");
  ck_assert_int_eq(isNumber(p),(int)true);
  p->currentWord++;
  programInsert(p,"45");
  ck_assert_int_eq(isNumber(p),(int)true);
  p->currentWord++;
  programInsert(p,"0");
  ck_assert_int_eq(isNumber(p),(int)true);
  p->currentWord++;
  programInsert(p,"-67.2");
  ck_assert_int_eq(isNumber(p),(int)true);
  p->currentWord++;
  programInsert(p,"dog");
  ck_assert_int_eq(isNumber(p),(int)false);
  p->currentWord++;
  programInsert(p,"+");
  ck_assert_int_eq(isNumber(p),(int)false);
  p->currentWord++;
  programInsert(p,"A");
  ck_assert_int_eq(isNumber(p),(int)false);
  p->currentWord++;
  programFree(p);
} END_TEST

START_TEST(test_isVar) {
  Program * p = programInit();
  int i;
  int j = 0;
  for (i = 'A'; i <= 'Z'; i++) {
    *((char*)(p->wordsArray[p->currentWord])) = i;
    ck_assert_int_eq(isVar(p),(int)true);
  }
  for (i = 0; i <= 255; i++) {
    *((char*)(p->wordsArray[p->currentWord])) = i;
    if (isVar(p) == true) {
      j++;
    }
  }
  ck_assert_int_eq(i,256);
  ck_assert_int_eq(j,26);
  programFree(p);
} END_TEST

START_TEST(test_isVarnum) {
  Program * p = programInit();
  int i;
  int j = 0;
  for (i = 0; i <= 255; i++) {
    *((char*)(p->wordsArray[p->currentWord])) = i;
    if (isVarnum(p) == true) {
      j++;
    }
  }
  ck_assert_int_eq(i,256);
  ck_assert_int_eq(j,36);
  programFree(p);
} END_TEST

START_TEST(test_isPolish) {
  Program * p = programInit();
  programInsert(p,";");
  ck_assert_int_eq(isPolish(p),(int)true);
  programInsert(p,"+");
  p->currentWord++;
  ck_assert_int_eq(isPolish(p),(int)false);
  programInsert(p,"A");
  ck_assert_int_eq(isPolish(p),(int)false);
  programInsert(p,"A");
  programInsert(p,"+");
  ck_assert_int_eq(isPolish(p),(int)false);
  programInsert(p,"A");
  programInsert(p,"+");
  programInsert(p,";");
  ck_assert_int_eq(isPolish(p),(int)true);
  programFree(p);
} END_TEST

Suite * prog_suite(void) {
  Suite * s;
  TCase * basic;

  s = suite_create("parser");
  basic = tcase_create("Basic");
  tcase_set_timeout(basic,240);
  tcase_add_test(basic, test_isOP);
  tcase_add_test(basic, test_isNumber);
  tcase_add_test(basic, test_isVar);
  tcase_add_test(basic, test_isVarnum);
  tcase_add_test(basic, test_isPolish);
  tcase_add_test(basic, test_isSet);
  tcase_add_test(basic, test_isFD_isRT_isLT);
  tcase_add_test(basic, test_isDo);
  tcase_add_test(basic, test_isInstruction);
  tcase_add_test(basic, test_isInstructLst);
  tcase_add_test(basic, test_isMain);
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
