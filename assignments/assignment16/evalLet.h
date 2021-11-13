#include "value.h"

#ifndef _EVALLET
#define _EVALLET

bool containsSymbol(Value *linkedList, Value *symbolNode);

Value *evalLet(Value *args, Frame *frame);

#endif
