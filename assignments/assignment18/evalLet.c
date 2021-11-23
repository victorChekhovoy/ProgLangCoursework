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

// Takes a binding like that in let and prints it for debug purposes
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
      case UNSPECIFIED_TYPE:
        printf("UNSPECIFIED\n");
        break;
      default:
        printf("\n");
        break;
  }
}

// Takes a frame as input and prints the variables in it for debug purposes
void displayFrame(Frame *frame){
  Value *currentBinding = frame->bindings;
  while (!isNull(currentBinding)){
    displayBinding(currentBinding);
    currentBinding = cdr(currentBinding);
  }
}

// Takes a binding as input, and checks if any of the elements in it are NULL
void checkNullBinding(Value *letBinding){
  if ((isNull(letBinding)) || (isNull(car(letBinding))) || (isNull(car(cdr(letBinding))))){
    bindingError();
  }
}

// Checks if the variable in a binding is a symbol
void checkProperBinding(Value *letBinding){
  if (car(letBinding)->type != SYMBOL_TYPE){
    bindingError();
  }
}

// Checks if a linked list contains the given symbol. The *value argument being passed must be of type SYMBOL_TYPE
bool containsSymbol(Value *linkedList, Value *symbolNode){
  assert((symbolNode->type == SYMBOL_TYPE) || (symbolNode->type == NULL_TYPE) && "containsSymbol can only be run on a node of type SYMBOL_TYPE");
  while (!isNull(linkedList)){
    if (strcmp(car(linkedList)->s, symbolNode->s) == 0){
      return true;
    }
    linkedList = cdr(linkedList);
  }
  return false;
}

// Takes a linked list as input, and return the tail of that list
Value *getLastElement(Value *linkedList){
  Value *last = car(linkedList);
  while (!isNull(cdr(linkedList))){
    last = car(linkedList);
    linkedList = cdr(linkedList);
  }
  return last;
}

// Given a binding in raw format and a frame, creates a binding in the frame
Value *makeBinding(Value *letBinding, Frame *frame){
  checkNullBinding(letBinding);
  checkProperBinding(letBinding);

  Value *newBinding = talloc(sizeof(Value));
  newBinding->type = CONS_TYPE;
  newBinding->c.car = car(letBinding);

  Value *variableValue = eval(car(cdr(letBinding)), frame);
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

// Takes a set of bindings from let and a frame, and adds all of the listed bindings to the frame
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
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->bindings = addedBindings;
  newFrame->parent = frame->parent;
  return newFrame;
}

// Takes a let expression and a frame as input, and evaluates the expression
Value *evalLet(Value *args, Frame *frame){
  Value *bindings = car(args);
  Value *expression = cdr(args);

  if (bindings->type != CONS_TYPE){
    bindingError();
  }
  Frame *letFrame = talloc(sizeof(Frame));
  letFrame->parent = frame;
  letFrame->bindings = makeNull();
  letFrame = setVariables(bindings, letFrame);
  
  if (isNull(expression)){
    letArgsError();
  }
  while(!isNull(cdr(expression))){
    eval(car(expression), letFrame);
    expression = cdr(expression);
  }
  return eval(car(expression), letFrame);
}
