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


// Updates the letrec bindings with actual values
Frame *evalLetrecBindings(Value *letrecBindings, Frame *env1, Frame *env2){
  Value *currentBinding;
  Value *addedBindings = makeNull();
  Value *letBindingsDefined = makeNull();

  while (!isNull(car(letrecBindings))){
    currentBinding = car(letrecBindings);
    Value *newBinding = makeBinding(currentBinding, env1);

    if (containsSymbol(letBindingsDefined, car(newBinding))){
      duplicateArgumentError(car(newBinding));
    }
    letBindingsDefined = cons(car(newBinding), letBindingsDefined);

    Value *bindingContainer;
    if (isNull(addedBindings)){
      bindingContainer = cons(newBinding, env1->bindings);
    }
    else {
      bindingContainer = cons(newBinding, addedBindings);
    }

    addedBindings = bindingContainer;
    letrecBindings = cdr(letrecBindings);
  }
  env1->bindings = addedBindings;
  return env1;
}

// Creates the temporary bindings for letrec
Frame *makeEmptyLetrecBindings(Value *letrecBindings, Frame *frame){
  Value *currentBinding;
  Value *addedBindings = makeNull();
  Value *letBindingsDefined = makeNull();

  while (!isNull(car(letrecBindings))){
    currentBinding = car(letrecBindings);
    Value *newBinding = talloc(sizeof(Value));
    newBinding->type = UNSPECIFIED_TYPE;

    if (containsSymbol(letBindingsDefined, car(currentBinding))){
      duplicateArgumentError(car(currentBinding));
    }
    letBindingsDefined = cons(cons(car(currentBinding), newBinding), letBindingsDefined);

    Value *bindingContainer;
    if (isNull(addedBindings)){
      bindingContainer = cons(newBinding, frame->bindings);
    }
    else {
      bindingContainer = cons(newBinding, addedBindings);
    }

    addedBindings = bindingContainer;
    letrecBindings = cdr(letrecBindings);
  }
  frame->bindings = addedBindings;
  return frame;
}

// Takes a letrec expression and a frame as input, and evaluates the expression
Value *evalLetRec(Value *args, Frame *frame){
  Value *bindings = car(args);
  Value *expression = cdr(args);

  if (bindings->type != CONS_TYPE){
    bindingError();
  }
  Frame *letrecFrame = talloc(sizeof(Frame));
  letrecFrame->parent = frame;
  letrecFrame->bindings = makeNull();
  letrecFrame = makeEmptyLetrecBindings(bindings, letrecFrame);
  letrecFrame = evalLetrecBindings(bindings, frame, letrecFrame);

  Value *returnValue = getLastElement(expression);
  if (isNull(returnValue)){
    letArgsError();
  }
  return eval(returnValue, letrecFrame);
}
