#pragma once
#include "../../General/bool.h"
#include "../readTTL/readTurtleFile.h"
#include <check.h>

#ifdef EXTENSION
#include "../extension/postfix.h"
#endif

#ifdef INTERP
#include "../interpreter/interpreter.h"
#endif

bool isOp(Program * p);
bool isNumber(Program * p);
bool isVar(Program * p);
bool isVarnum(Program * p);
bool isPolish(Program * p);
bool isSet(Program * p);
bool isFD(Program * p);
bool isLT(Program * p);
bool isRT(Program * p);
bool isMain(Program * p);
bool isInstructLst(Program * p);
bool isInstruction(Program * p);
bool isDo(Program * p);
