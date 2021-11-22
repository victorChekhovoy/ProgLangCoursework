#include <stdlib.h>
#include <stdio.h>
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>
#include <assert.h>
#include "evalLet.h"
#include "lookUpSymbol.h"
#include "interpreter.h"
#include "errorCall.h"


// Takes a set of bindings from let and a frame, and adds all of the listed bindings to the frame
Frame *setLetStarVariables(Value *letBindings, Frame *frame){
  Value *currentBinding;
  Value *letBindingsDefined = makeNull();

  while (!isNull(car(letBindings))){
    currentBinding = car(letBindings);
    Value *newBinding = makeBinding(currentBinding, frame);
    Value *bindingContainer = cons(newBinding, frame->bindings);
    frame->bindings = bindingContainer;
    letBindings = cdr(letBindings);
  }
  return frame;
}

// Takes a let expression and a frame as input, and evaluates the expression
Value *evalLetStar(Value *args, Frame *frame){
  Value *bindings = car(args);
  Value *expression = cdr(args);

  if (bindings->type != CONS_TYPE){
    bindingError();
  }
  Frame *letFrame = talloc(sizeof(Frame));
  letFrame->parent = frame;
  letFrame->bindings = makeNull();
  letFrame = setLetStarVariables(bindings, letFrame);
  //frame = setVariables(bindings, frame);
  Value *returnValue = getLastElement(expression);
  if (isNull(returnValue)){
    letArgsError();
  }
  return eval(returnValue, letFrame);
}
