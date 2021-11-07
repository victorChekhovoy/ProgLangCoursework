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
    return makeNull();
  }
  else{
    return lookUpSymbol(targetSymbol, frame->parent);
  }
}
