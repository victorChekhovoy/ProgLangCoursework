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
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = frame->parent;
  newFrame->bindings = frame->bindings;

  while (!isNull(car(letBindings))){
    Frame *iterationFrame = talloc(sizeof(Frame));
    iterationFrame->parent = newFrame;
    currentBinding = car(letBindings);
    Value *newBinding = makeBinding(currentBinding, newFrame);
    Value *bindingContainer = cons(newBinding, makeNull());
    iterationFrame->bindings = bindingContainer;
    letBindings = cdr(letBindings);
    newFrame = iterationFrame;
  }
  return newFrame;
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
  
  if (isNull(expression)){
    letArgsError();
  }
  while(!isNull(cdr(expression))){
    eval(car(expression), letFrame);
    expression = cdr(expression);
  }
  return eval(car(expression), letFrame);
}
