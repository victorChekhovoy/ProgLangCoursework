#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>
#include "tokenizer.h"
#include <assert.h>
#include "parser.h"
#include "lookUpSymbol.h"
#include "errorCall.h"

// Takes a frame and a symbol and goes through the bindings of the frame comparing to the target symbol
// then checks the parent frame if there is one
Value *lookUpSymbol(Value *targetSymbol, Frame *frame){
  Value *bindings = frame->bindings;
  while(!isNull(bindings)){
    Value *currentSymbol = car(car(bindings));
    if (strcmp(currentSymbol->s, targetSymbol->s) == 0){
      return cdr(car(bindings));
    }
    bindings = cdr(bindings);
  }
  if (frame->parent == NULL){
    symbolNotFoundError(targetSymbol);
    return NULL;
  }
  else{
    return lookUpSymbol(targetSymbol, frame->parent);
  }
}

// Finds a symbol in the frame and replaces its value with the given value
Frame *replaceSymbol(Value *lookUp, Value *replace, Frame *frame){
  Value *bindings = frame->bindings;
  Value *newBindings = makeNull();
  while(!isNull(bindings)){
    Value *currentSymbol = car(car(bindings));
    if (strcmp(currentSymbol->s, lookUp->s) == 0){
      newBindings = cons(cons(currentSymbol, replace), newBindings);
      cdr(car(bindings));
    } else{
      newBindings = cons(car(bindings), newBindings);
    }
    bindings = cdr(bindings);
  }
  frame->bindings = newBindings;
  return frame;
}
