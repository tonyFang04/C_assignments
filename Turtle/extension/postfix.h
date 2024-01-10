#pragma once
#include <check.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../interpreter/interpreter.h"
void infixToPostfix(Program * p);
int precedence(char operator);
