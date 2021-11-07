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

void checkNullBinding(Value *letBinding){
  if ((isNull(letBinding)) || (isNull(car(letBinding))) || (isNull(car(cdr(letBinding))))){
    bindingError();
  }
}

void checkProperBinding(Value *letBinding){
  if (car(letBinding)->type != SYMBOL_TYPE){
    bindingError();
  }
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
  checkNullBinding(letBinding);
  checkProperBinding(letBinding);

  Value *newBinding = talloc(sizeof(Value));
  newBinding->type = CONS_TYPE;
  newBinding->c.car = car(letBinding);

  Value *variableValue = car(cdr(letBinding));
  if (variableValue->type == SYMBOL_TYPE){
    newBinding->c.cdr = lookUpSymbol(variableValue, frame);
    if (isNull(newBinding->c.cdr)){
      symbolNotFoundError(variableValue);
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
  Value *letBindingsDefined = makeNull();

  while (!isNull(car(letBindings))){
    currentBinding = car(letBindings);
    Value *newBinding = makeBinding(currentBinding, frame);

    if (containsSymbol(letBindingsDefined, car(newBinding))){
      duplicateArgumentError(car(newBinding));
    }
    letBindingsDefined = cons(car(newBinding), letBindingsDefined);


    Value *bindingContainer = NULL;
    if (isNull(addedBindings)){
      bindingContainer = cons(newBinding, frame->bindings);
    }
    else {
      bindingContainer = cons(newBinding, addedBindings);
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
