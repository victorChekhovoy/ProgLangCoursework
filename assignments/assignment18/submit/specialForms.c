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
#include "interpreter.h"
#include "apply.h"
#include "evalEach.h"


// Checks if an expression has the same argument for the car and cdr
void checkDuplicateArgs(Value *args){
  if (containsSymbol(cdr(args), car(args))){
    lambdaDuplicateArgumentError(car(args));
  }
  if (!isNull(args)){
    return checkDuplicateArgs(cdr(args));
  }
}

// Creates a new closure from the given args, code, and frame
Value *makeClosure(Value *args, Value* code, Frame *frame){
  Value *newClosure = talloc(sizeof(Value));
  checkDuplicateArgs(args);
  newClosure->type = CLOSURE_TYPE;
  newClosure->cl.paramNames = args;
  newClosure->cl.functionCode = code;
  newClosure->cl.frame = frame;
  return newClosure;
}

// Creates a new variable and makes it the binding for the frame then returns the frame
Frame *defineVariable(Value *symbol, Value *variableValue, Frame *frame){
  if ((isNull(symbol)) || (isNull(variableValue))){
    defineArgumentError();
  }
  Value *newVariable = cons(symbol, variableValue);
  Value *bindingContainer = cons(newVariable, frame->bindings);
  frame->bindings = bindingContainer;
  return frame;
}

// Checks if each element in a list is a symbol
bool checkSymbolEach(Value *list){
  if (isNull(list)){
    return true;
  } else if (car(list)->type != SYMBOL_TYPE){
    return false;
  }
  return checkSymbolEach(cdr(list));
}

// Does error checking and returns the result of calling quote
Value *processQuote(Value *args){
  if (length(args) > 1){
    quoteArgumentNumberError(length(args));
  }
  return car(args);
}

// Does error checking and returns the result of calling define
Frame *processDefine(Value *args, Frame *frame){
  if (length(args) != 2){
    defineArgumentNumberError(length(args));
  }
  Value *variable = car(args);
  Value *value = eval(car(cdr(args)), frame);
  if (variable->type != SYMBOL_TYPE){
    bindingWrongTypeError();
  }
  return defineVariable(variable, value, frame);
}

// Does error checking and returns the result of calling lambda
Value *processLambda(Value *args, Frame *frame){
  if (!checkSymbolEach(car(args))){
    lambdaNonSymbolArguments();
  }
  if (isNull(car(cdr(args)))){
    lambdaNoCode();
  }
  if (length(args) > 3){
    lambdaArgumentNumberError(length(args));
  }
  return makeClosure(car(args), car(cdr(args)), frame);
}

// Return the result of calling a function
Value *applyFunction(Value *function, Value *args, Frame *frame){
  Value *evaluatedOperator = eval(function, frame);
  Value *evaluatedArgs = evalEach(args, frame);
  return apply(evaluatedOperator, evaluatedArgs);
}

// Does error checking and returns the result of calling set!
Frame *processSet(Value *args, Frame *frame){
  int inputLength = length(args);
  if (inputLength == 0){
    setNoArgumentsError();
  } else if (inputLength == 1){
    setTooFewArgumentsError();
  } else if (inputLength > 2){
    setTooManyArgumentsError(inputLength);
  }
  Value *lookUp = car(args);
  Value *replace = eval(car(cdr(args)), frame);
  return replaceSymbol(lookUp, replace, frame);
}

// Return the result of calling begin
Value *processBegin(Value *args, Frame *frame){
  Value *result;
  if(isNull(args)){
    result = makeNull();
    result->type = VOID_TYPE;
    return result;
  }
  while(!isNull(cdr(args))){
    eval(car(args), frame);
    args = cdr(args);
  }
  return eval(car(args), frame);
}

// Return the result of calling and
Value *processAnd(Value *args, Frame *frame){
  Value *result;
  Value *firstTerm;
  result = talloc(sizeof(Value));
  result->type = BOOL_TYPE;
  if(args->type == NULL_TYPE){
    andNoArgs();
  }
  while(args->type != NULL_TYPE){
    firstTerm = eval(car(args), frame);
    if(firstTerm->i == 0){
      result->i = 0;
      return result;
    }
    args = cdr(args);
  }
  result->i = 1;
  return result;
}

// Return the result of call or
Value *processOr(Value *args, Frame *frame){
  Value *result;
  Value *firstTerm;
  result = talloc(sizeof(Value));
  result->type = BOOL_TYPE;
  result->i = 0;
  if(args->type == NULL_TYPE){
    orNoArgs();
  }
  while(args->type != NULL_TYPE){
    firstTerm = eval(car(args), frame);
    if(firstTerm->i == 1){
      result->i = 1;
      return result;
    }
    args = cdr(args);
  }
  return result;
}

// Return the result of calling cond
Value *processCond(Value *args, Frame *frame){
    Value *conditionStatement;
    Value *voidValue;
    if (length(args) == 0) {
      condNoArgs();
    }
    while (args->type != NULL_TYPE) {
      conditionStatement = car(car(args));
      if (conditionStatement->type == SYMBOL_TYPE && !strcmp(conditionStatement->s, "else")) {
        if (cdr(args)->type != NULL_TYPE) {
          condElseNotLast();
        }
        return eval(car(cdr(car(args))), frame);
      }
      conditionStatement = eval(conditionStatement, frame);
      if (conditionStatement->type != BOOL_TYPE) {
        condReturnNotBool();
      } else if (conditionStatement->i) {
        return eval(car(cdr(car(args))), frame);
      }
      args = cdr(args);
    }
    voidValue = makeNull();
    voidValue->type = VOID_TYPE;
    return voidValue;
}
