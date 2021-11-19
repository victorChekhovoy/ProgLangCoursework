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

// Helper function with Null? to check both the car and cdr
bool checkInside(Value *args) {
  if (isNull(args)){
    return true;
  } else if (args->type == CONS_TYPE){
    return checkInside(car(args)) && checkInside(cdr(args));
  } else {
    return false;
  }
}

// Takes in the arguments for a Null? operation and performs it, then returns the result
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

// Helper function to check for a dotted pair
bool isDottedPair(Value *input){
  if ((car(input)->type != CONS_TYPE) && (!isNull(car(input)))){
    if ((cdr(input)->type != CONS_TYPE) && (!isNull(cdr(input)))){
      return true;
    }
  }
  return false;
}

// Takes in the arguments for a car operation and performs it, then returns the result
Value *builtInCar(Value *args) {
  Value *input = car(args);
  int inputLength = length(args);
  if (inputLength > 1) {
    carTooManyArgumentsError(inputLength);
  } else if (inputLength == 0) {
    carNoArgumentsError();
  }
  if (input->type != CONS_TYPE){
    carInvalidArgumentTypeError();
    return NULL;
  } else if (isDottedPair(input)) {
    return car(input);
  } else {
    return car(car(input));
  }
}

// Takes in the arguments for a cdr operation and performs it, then returns the result
Value *builtInCdr(Value *args) {
  Value *input = car(args);
  int inputLength = length(input);
  if (inputLength > 1) {
    cdrTooManyArgumentsError(inputLength);
  } else if (inputLength == 0) {
    cdrNoArgumentsError();
  }
  if (input->type != CONS_TYPE){
    cdrInvalidArgumentTypeError();
    return NULL;
  } else if (isDottedPair(input)) {
    return cdr(input);
  } else {
    return cons(cdr(car(input)), makeNull());
  }
}

// Takes in the arguments for a cons operation and performs it, then returns the result
Value *builtInCons(Value *args) {
  int inputLength = length(args);
  if (inputLength > 2) {
    consTooManyArgumentsError(inputLength);
  } else if (inputLength == 0) {
    consNoArgumentsError();
  }
  Value *firstArgument = car(args);
  Value *secondArgument = car(cdr(args));
  int secondArgType = secondArgument->type;
  Value *newNode = makeNull();
  if (secondArgType == NULL_TYPE){
    printf("Evaluation Error\n");
  } else if (secondArgType == CONS_TYPE){
    newNode = cons(cons(firstArgument, car(secondArgument)), makeNull());
  } else {
    newNode = cons(cons(firstArgument, secondArgument), makeNull());
  }
  return newNode;
}

// Takes in the arguments for a + operation and performs the arithmatic, then returns the result
Value *builtInAdd(Value *args) {
  Value *result = talloc(sizeof(Value));
  result->type = INT_TYPE;
  int integerSum = 0;
  double doubleSum = 0;

  while(args->type != NULL_TYPE){
    if (car(args)->type == INT_TYPE){
      integerSum += car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE){
      doubleSum = doubleSum + (double)integerSum + car(args)->d;
      integerSum = 0;
      result->type = DOUBLE_TYPE;
    } else {
      builtInAddArgumentTypeError();
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
