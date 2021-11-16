#include <stdlib.h>
#include <stdio.h>
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>
#include <assert.h>
#include "evalLet.h"
#include "evalIf.h"
#include "lookUpSymbol.h"
#include "evalLet.h"
#include "errorCall.h"
#include "parser.h"
#include "apply.h"
#include "evalEach.h"
#include "interpreter.h"
#include "tokenizer.h"

bool checkInside(Value *args) {
  if (isNull(args)){
    return true;
  } else if (args->type == CONS_TYPE){
    return checkInside(car(args)) && checkInside(cdr(args));
  } else {
    return false;
  }
}

Value *builtInNull(Value *args) {
  int numberOfArgs = length(args);
  if (isNull(args)){
    nullNoArgumentsError();
    return NULL;
  } else if (numberOfArgs > 1){
    nullTooManyArgumentsError(numberOfArgs);
    return NULL;
  } else if (args->type == CONS_TYPE){
    Value *isItNull = talloc(sizeof(Value));
    isItNull->type = BOOL_TYPE;
    isItNull->i = checkInside(args);
    return isItNull;
  } else{
    Value *isItNull = talloc(sizeof(Value));
    isItNull->type = BOOL_TYPE;
    isItNull->i = isNull(args);
    return isItNull;
  }
}

Value *builtInCar(Value *args) {
   return car(args);
}

Value *builtInCdr(Value *args) {
   return cdr(args);
}

Value *builtInAdd(Value *args) {
  Value *first = car(args);
  Value *second = car(cdr(args));
  Value *remainder = cdr(cdr(args));
  Value *firstTwoSummed = makeNull();
  if ((first->type == INT_TYPE) && (second->type == INT_TYPE)){
    firstTwoSummed = talloc(sizeof(Value));
    firstTwoSummed->type = INT_TYPE;
    firstTwoSummed->d = first->i + second->i;
  }
  else {
    double firstValue, secondValue;
    if (first->type == INT_TYPE){
      firstValue = (double) first->i;
    } else {
      firstValue = first->d;
    }

    if (second->type == INT_TYPE){
      secondValue = (double) second->i;
    } else {
      secondValue = second->d;
    }

    double addedValue = firstValue + secondValue;
    firstTwoSummed = talloc(sizeof(Value));
    firstTwoSummed->type = DOUBLE_TYPE;
    firstTwoSummed->d = addedValue;
  }

  if (isNull(remainder)){
    return firstTwoSummed;
  }
  return builtInAdd(cons(firstTwoSummed, remainder));

}

void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame) {
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;
    Value *functionName = makeSymbol(name);
    Value *bindingContainer = cons(functionName, value);
    frame->bindings = cons(bindingContainer, frame->bindings);
}
