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

Frame *bindVariable(Value *param, Value *value, Frame *frame){
  if ((isNull(param)) || (isNull(value))){
    functionArgumentError();
  }
  Value *newVariable = cons(param, value);
  Value *bindingContainer = cons(newVariable, frame->bindings);
  frame->bindings = bindingContainer;
  return frame;
}

Frame *bindEach(Value *paramNames, Value *paramValues, Frame *frame){
  frame = bindVariable(car(paramNames), car(paramValues), frame);
  return bindEach(cdr(paramNames), cdr(paramValues), frame);
}

Value *apply(Value *closure, Value *arguments){
  Frame *functionFrame = talloc(sizeof(Frame));
  functionFrame->parent = closure->cl.frame;
  functionFrame->bindings = makeNull();
  functionFrame = bindEach(closure->cl.paramNames, arguments, functionFrame);
  return eval(closure->cl.functionCode, functionFrame);
}
