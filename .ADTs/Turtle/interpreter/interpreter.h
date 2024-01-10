#pragma once
#include <check.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "../parser/parser.h"
#define MILLISECONDDELAY 10
#ifndef M_PI
#define M_PI acos(-1.0)
#endif

bool doublesEqual(double dub1, double dub2);
Node * getVarAddress(Program * p);
bool turtleRotate(Turtle * t, double angleOffset);
double fetchNumber(Program * p);
bool turtleMove(Turtle * t, double distance);
double postfixCalculation(Program * p);
bool setVariable(Node * address, double number);
bool loopExecute(Program * p, Node * address, double start, double end);
bool displayTurtleTrajectory(Program * p, Turtle * t);
