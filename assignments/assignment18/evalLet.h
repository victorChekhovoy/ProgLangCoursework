#include "value.h"

#ifndef _EVALLET
#define _EVALLET

void checkNullBinding(Value *letBinding);

void checkProperBinding(Value *letBinding);

Value *getLastElement(Value *linkedList);

Value *makeBinding(Value *letBinding, Frame *frame);

Frame *setVariables(Value *letBindings, Frame *frame);

bool containsSymbol(Value *linkedList, Value *symbolNode);

Value *evalLet(Value *args, Frame *frame);

#endif
