#pragma once
#include <string.h>
#include <stdio.h>
#include <check.h>
#include <assert.h>
#include <stdlib.h>
#include "../../General/bool.h"
#include "../Realloc/realloc.h"

bool checkTerminalInputNumber(int argc);
bool checkFileName(char * filename);
bool readFile(Program * p, char * filename);
