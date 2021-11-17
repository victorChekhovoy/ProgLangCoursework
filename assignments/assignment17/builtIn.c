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
  Value *result = talloc(sizeof(Value));
  result->type = INT_TYPE;
  Value *first = car(args);
  int integerSum = 0;
  double doubleSum = 0;
  
  while(args->type != NULL_TYPE){
    if (first->type == DOUBLE_TYPE){
      doubleSum = doubleSum + (double)integerSum + first->d;
      integerSum = 0;
      result->type = DOUBLE_TYPE;

    }else if(first->type == INT_TYPE){
      integerSum = integerSum + first->i;
    }else{
      printf("Evaluation Error: + must take numbers.\n");
      texit(0);
    }
    args = cdr(args);
  }
  if(result->type == DOUBLE_TYPE){
    result->d = doubleSum;
  }else if (result->type == INT_TYPE){
    result->i = integerSum;
  }
  return result;
}

void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame) {
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;
    Value *functionName = makeSymbol(name);
    Value *bindingContainer = cons(functionName, value);
    frame->bindings = cons(bindingContainer, frame->bindings);
}
