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

// Binds a single argument in a function call to a parameter in the closure
Frame *bindVariable(Value *param, Value *value, Frame *frame){
  if (isNull(param)){
    functionArgumentError();
  }
  Value *newVariable = cons(param, value);
  Value *bindingContainer = cons(newVariable, frame->bindings);
  frame->bindings = bindingContainer;
  return frame;
}

// Binds each argument passed in a function call to a respective parameter in the closure
// Throws an error if the number of arguments is incorrect
Frame *bindEach(Value *paramNames, Value *paramValues, Frame *frame){
  int numberPassedValues = length(paramValues);
  if ((paramValues->type == CONS_TYPE) && (isNull(car(paramValues))) && (isNull(cdr(paramValues)))){
    numberPassedValues = 1;
  }
  if (length(paramNames) != numberPassedValues){
    functionArgumentNumberError(length(paramNames), length(paramValues));
  }
  if (isNull(paramNames)){
    return frame;
  }
  frame = bindVariable(car(paramNames), car(paramValues), frame);
  return bindEach(cdr(paramNames), cdr(paramValues), frame);
}

// Evalues the result of a built-in function
Value *applyBuiltIn(Value *function, Value *args){
  return (*(function->pf))(args);
}

// Given a closure and a list of arguments, evaluates the function given by the closure
Value *apply(Value *closure, Value *args){
  if (closure->type == PRIMITIVE_TYPE){
    return applyBuiltIn(closure, args);
  }
  Frame *functionFrame = talloc(sizeof(Frame));
  functionFrame->parent = closure->cl.frame;
  functionFrame->bindings = makeNull();
  functionFrame = bindEach(closure->cl.paramNames, args, functionFrame);
  return eval(closure->cl.functionCode, functionFrame);
}
