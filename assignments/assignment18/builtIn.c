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
  if (isNull(args)){
    nullNoArgumentsError();
    return NULL;
  } else if (!isNull(cdr(args))){
    nullTooManyArgumentsError();
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
  } else {
    return car(input);
  }
}

// Takes in the arguments for a cdr operation and performs it, then returns the result
Value *builtInCdr(Value *args) {
  //Value *input = car(args);
  int inputLength = length(args);
  if (inputLength > 1) {
    cdrTooManyArgumentsError(inputLength);
  } else if (inputLength == 0) {
    cdrNoArgumentsError();
  }
  if (args->type != CONS_TYPE){
    cdrInvalidArgumentTypeError();
    return NULL;
  } else {
    return cdr(car(args));
  }
}

// Takes in the arguments for a cons operation and performs it, then returns the result
Value *builtInCons(Value *args) {
  int inputLength = length(args);
  if (inputLength > 2) {
    consTooManyArgumentsError(inputLength);
  } else if (inputLength == 0) {
    consNoArgumentsError();
  } else if (inputLength == 1){
    consTooFewArgumentsError();
  }
  Value *firstArg = car(args);
  Value *secondArgument = car(cdr(args));
  int firstArgType = firstArg->type;
  int secondArgType = secondArgument->type;
  Value *newNode = makeNull();
  newNode = cons(firstArg, secondArgument);
  return newNode;
}

// Takes in the arguments for a + operation and performs the arithmetic, then returns the result
Value *builtInAdd(Value *args) {
  Value *output = talloc(sizeof(Value));
  output->type = INT_TYPE;
  int integerSum = 0;
  double doubleSum = 0;

  while(args->type != NULL_TYPE){
    if (car(args)->type == INT_TYPE){
      integerSum += car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE){
      doubleSum = doubleSum + (double)integerSum + car(args)->d;
      integerSum = 0;
      output->type = DOUBLE_TYPE;
    } else {
      builtInAddArgumentTypeError();
    }
    args = cdr(args);
  }
  if (output->type == DOUBLE_TYPE){
    output->d = doubleSum;
  } else if (output->type == INT_TYPE){
    output->i = integerSum;
  }
  return output;
}

// Takes in the arguments for a - operation and performs the arithmetic, then returns the result
Value *builtInMinus(Value *args) {
  int inputLength = length(args);
  if (inputLength > 2){
    primitiveTooManyArgsError("-", inputLength);
  } else if (inputLength == 0){
    primitiveNoArgsError("-");
  } else if (inputLength == 1){
    primitiveTooFewArgsError("-");
  }

  Value *firstArg = car(args);
  Value *secondArg = car(cdr(args));
  Value *output = talloc(sizeof(Value));

  double firstArgConverted = 0;
  if (firstArg->type == INT_TYPE){
    firstArgConverted = (double) firstArg->i;
  } else {
    firstArgConverted = firstArg->d;
  }

  double secondArgumentConverted = 0;
  if (secondArg->type == INT_TYPE){
    secondArgumentConverted = (double) secondArg->i;
  } else {
    secondArgumentConverted = secondArg->d;
  }

  double doubleResult = firstArgConverted - secondArgumentConverted;
  if ((firstArg->type == INT_TYPE) && (secondArg->type == INT_TYPE)){
    output->type = INT_TYPE;
    output->i = (int) doubleResult;
  } else{
    output->type = DOUBLE_TYPE;
    output->d = doubleResult;
  }
  return output;
}

// Takes in the arguments for a * operation and performs the arithmetic, then returns the result
Value *builtInMultiply(Value *args){
  Value *result = talloc(sizeof(Value));
  result->type = INT_TYPE;
  result->i = 1;

  while (!isNull(args)) {
    if (car(args)->type == INT_TYPE) {
      result->i *= car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE){
      if (result->type == INT_TYPE){
        result->type = DOUBLE_TYPE;
        result->d = (double) result->i;
      }
      result->d *= car(args)->d;
    } else {
      builtInMultipyArgumentTypeError();
    }
    args = cdr(args);
  }

  return result;
}

// Takes in the arguments for a / operation and performs the arithmetic, then returns the result
Value *builtInDivide(Value *args){
  int inputLength = length(args);
  if (inputLength > 2){
    primitiveTooManyArgsError("/", inputLength);
  } else if (inputLength == 0){
    primitiveNoArgsError("/");
  } else if (inputLength == 1){
    primitiveTooFewArgsError("/");
  }
  Value *firstArg = car(args);
  Value *secondArg = car(cdr(args));

  double firstArgConverted = 0;
  if (firstArg->type == INT_TYPE){
    firstArgConverted = (double) firstArg->i;
  } else {
    firstArgConverted = firstArg->d;
  }

  double secondArgumentConverted = 0;
  if (secondArg->type == INT_TYPE){
    secondArgumentConverted = (double) secondArg->i;
  } else {
    secondArgumentConverted = secondArg->d;
  }

  double result = firstArgConverted / secondArgumentConverted;
  Value *resultValue = talloc(sizeof(Value));
  if (result == ((double) (int) result)){
    resultValue->type = INT_TYPE;
    resultValue->i = (int) result;
  } else {
    resultValue->type = DOUBLE_TYPE;
    resultValue->d = result;
  }
  return resultValue;
}

// Takes in the arguments for a = or eq? operation and performs the comparison, then returns the result
Value *builtInEquals(Value *args){
  int inputLength = length(args);
  if (inputLength > 2){
    primitiveTooManyArgsError("=", inputLength);
  } else if (inputLength == 0){
    primitiveNoArgsError("=");
  } else if (inputLength == 1){
    primitiveTooFewArgsError("=");
  }
  Value *firstArg = car(args);
  Value *secondArgument = car(cdr(args));
  Value *output = talloc(sizeof(Value));
  output->type = BOOL_TYPE;
  if (firstArg->type != secondArgument->type){
    output->i = 0;
  } else {
    switch(firstArg->type){
      case INT_TYPE: {
        output->i = (firstArg->i == secondArgument->i);
        break;
      }
      case DOUBLE_TYPE: {
        output->i = (firstArg->d == secondArgument->d);
        break;
      }
      case BOOL_TYPE: {
        output->i = (firstArg->i == secondArgument->i);
        break;
      }
      case STR_TYPE: {
        output->i = 1 - (strcmp(firstArg->s, secondArgument->s));
        break;
      }
      case SYMBOL_TYPE: {
        output->i = 1 - (strcmp(firstArg->s, secondArgument->s));
        break;
      }
      default: {
        output->i = 0;
        break;
      }
    }
  }
  return output;
}

// Takes in the arguments for a < operation and performs the comparison, then returns the result
Value *builtInLess(Value *args){
  int inputLength = length(args);
  if (inputLength > 2){
    primitiveTooManyArgsError("<", inputLength);
  } else if (inputLength == 0){
    primitiveNoArgsError("<");
  } else if (inputLength == 1){
    primitiveTooFewArgsError("<");
  }
  Value *firstArg = car(args);
  Value *secondArgument = car(cdr(args));
  Value *output = talloc(sizeof(Value));
  output->type = BOOL_TYPE;
  if ((firstArg->type != INT_TYPE) && (firstArg->type != DOUBLE_TYPE)){
    lessWrongTypeError();
  } else if ((secondArgument->type != INT_TYPE) && (secondArgument->type != DOUBLE_TYPE)){
    lessWrongTypeError();
  }

  double firstArgConverted = 0;
  if (firstArg->type == INT_TYPE){
    firstArgConverted = (double) firstArg->i;
  } else {
    firstArgConverted = firstArg->d;
  }

  double secondArgumentConverted = 0;
  if (secondArgument->type == INT_TYPE){
    secondArgumentConverted = (double) secondArgument->i;
  } else {
    secondArgumentConverted = secondArgument->d;
  }
  output->i = (firstArgConverted < secondArgumentConverted);
  return output;
}

// Takes in the arguments for a > operation and performs the comparison, then returns the result
Value *builtInGreater(Value *args){
  int inputLength = length(args);
  if (inputLength > 2){
    primitiveTooManyArgsError(">", inputLength);
  } else if (inputLength == 0){
    primitiveNoArgsError(">");
  } else if (inputLength == 1){
    primitiveTooFewArgsError(">");
  }
  Value *firstArg = car(args);
  Value *secondArgument = car(cdr(args));
  Value *output = talloc(sizeof(Value));
  output->type = BOOL_TYPE;
  if ((firstArg->type != INT_TYPE) && (firstArg->type != DOUBLE_TYPE)){
    lessWrongTypeError();
  } else if ((secondArgument->type != INT_TYPE) && (secondArgument->type != DOUBLE_TYPE)){
    lessWrongTypeError();
  }

  double firstArgConverted = 0;
  if (firstArg->type == INT_TYPE){
    firstArgConverted = (double) firstArg->i;
  } else {
    firstArgConverted = firstArg->d;
  }

  double secondArgumentConverted = 0;
  if (secondArgument->type == INT_TYPE){
    secondArgumentConverted = (double) secondArgument->i;
  } else {
    secondArgumentConverted = secondArgument->d;
  }

  output->i = (firstArgConverted > secondArgumentConverted);
  return output;
}

// Takes in the arguments for a modulo operation and performs the arithmetic, then returns the result
Value *builtInModulo(Value *args){
  Value *output = talloc(sizeof(Value));
  output->type = INT_TYPE;

  if (car(args)->type != INT_TYPE || car(cdr(args))->type != INT_TYPE){
    builtInModuloArgumentTypeError();
  } else if (length(args) != 2) {
    builtInModuloWrongNumberOfArgs();
  }
  output->i = (car(args)->i) % (car(cdr(args))->i);
	return output;
}

// Creates a PRIMITIVE_TYPE Value with the given name, code, and frame
void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame) {
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;
    Value *functionName = makeSymbol(name);
    Value *bindingContainer = cons(functionName, value);
    frame->bindings = cons(bindingContainer, frame->bindings);
}
