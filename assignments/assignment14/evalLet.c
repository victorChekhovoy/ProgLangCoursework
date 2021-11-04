#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>
#include "tokenizer.h"
#include <assert.h>
#include "parser.h"
#include "evalLet.h"
#include "lookUpSymbol.h"
#include "interpreter.h"

void displayBinding(Value *binding){
  printf("Symbol: %s, Value: ", car(car(binding))->s);
  Value *variableValue = cdr(car(binding));
  switch(variableValue->type){
      case INT_TYPE:
        printf("%i\n", variableValue->i);
        break;
      case BOOL_TYPE:
        if (variableValue->i == 1){
          printf("#t\n");
        }
        if (variableValue->i == 0){
          printf("#f\n");
        }
        break;
      case STR_TYPE:
        printf("\"%s\"\n", variableValue->s);
        break;
      case DOUBLE_TYPE:
        printf("%f\n", variableValue->d);
        break;
      default:
        printf("\n");
        break;
  }
}

void displayFrame(Frame *frame){
  Value *currentBinding = frame->bindings;
  while (!isNull(currentBinding)){
    displayBinding(currentBinding);
    currentBinding = cdr(currentBinding);
  }
}

void bindingError(){
  printf("Evaluation error: improper binding in let\n");
  texit(0);
}

void letArgsError(){
  printf("Evaluation error: no args following bindings in let\n");
  texit(0);
}

void duplicateArgumentError(Value *symbol){
  printf("Evaluation error: attempting to define duplicate argument %s in let expression\n", symbol->s);
  texit(0);
}

void symbolNotFoundError(Value *symbol){
  printf("Evaluation error: symbol %s not found in the frame.\n", symbol->s);
  texit(0);
}

bool containsSymbol(Value *linkedList, Value *symbolNode){
  assert(symbolNode->type == SYMBOL_TYPE && "containsSymbol can only be run on a node of type SYMBOL_TYPE");
  while (!isNull(linkedList)){
    if (strcmp(car(linkedList)->s, symbolNode->s) == 0){
      return true;
    }
    linkedList = cdr(linkedList);
  }
  return false;
}

Value *getLastElement(Value *linkedList){
  Value *last = makeNull();
  while (!isNull(cdr(linkedList))){
    last = car(linkedList);
    linkedList = cdr(linkedList);
  }
  return last;
}

Value *makeBinding(Value *letBinding, Frame *frame){
  if ((isNull(letBinding)) || (isNull(car(letBinding))) || (isNull(car(cdr(letBinding))))){
    bindingError();
  }
  if (car(letBinding)->type != SYMBOL_TYPE){
    bindingError();
  }

  Value *newBinding = talloc(sizeof(Value));
  newBinding->type = CONS_TYPE;
  newBinding->c.car = car(letBinding);

  Value *variableValue = car(cdr(letBinding));
  if (variableValue->type == SYMBOL_TYPE){
    newBinding->c.cdr = lookUpSymbol(variableValue, frame);
    if (isNull(newBinding->c.cdr)){

    }
  }
  else {
    newBinding->c.cdr = variableValue;
  }
  return newBinding;
}

Frame *setVariables(Value *letBindings, Frame *frame){
  Value *currentBinding;
  Value *addedBindings = makeNull();
  Value *symbolsBound = makeNull();
  while (!isNull(car(letBindings))){
    currentBinding = car(letBindings);

    Value *newBinding = makeBinding(currentBinding, frame);
    if (containsSymbol(symbolsBound, car(newBinding))){
      duplicateArgumentError(car(newBinding));
    }
    symbolsBound = cons(car(newBinding), symbolsBound);

    Value *bindingContainer = talloc(sizeof(Value));
    bindingContainer->type = CONS_TYPE;
    bindingContainer->c.car = newBinding;

    if (isNull(addedBindings)){ //we do this so that all vars from let bindings get added at once
      bindingContainer->c.cdr = frame->bindings;
      addedBindings = bindingContainer;
    }
    else {
      bindingContainer->c.cdr = addedBindings;
    }
    addedBindings = bindingContainer;
    letBindings = cdr(letBindings);
  }
  frame->bindings = addedBindings;
  return frame;
}

Value *evalLet(Value *args, Frame *frame){
  Value *bindings = car(args);
  Value *expression = cdr(args);

  if (bindings->type != CONS_TYPE){
    bindingError();
  }
  frame = setVariables(bindings, frame);
  Value *returnValue = getLastElement(expression);
  if (isNull(returnValue)){
    letArgsError();
  }
  return eval(returnValue, frame);
}
