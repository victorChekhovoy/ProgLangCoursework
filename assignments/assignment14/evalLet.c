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
  printf("Evaluation error: attempting to define duplicate argument ");
  printf("%s", symbol->s);
  printf(" in let expression\n");
  texit(0);
}

Value *makeBinding(Value *letBinding){
  if ((isNull(letBinding)) || (isNull(car(letBinding))) || (isNull(car(cdr(letBinding))))){
    bindingError();
  }

  Value *newBinding = talloc(sizeof(Value));
  newBinding->type = CONS_TYPE;
  newBinding->c.car = car(letBinding);
  newBinding->c.cdr = car(cdr(letBinding));
  return newBinding;
}

Frame *setVariables(Value *letBindings, Frame *frame){
  Value *currentBinding;
  while (!isNull(car(letBindings))){
    currentBinding = car(letBindings);

    /*if (!isNull(lookUpSymbol(car(currentBinding), frame))){
      duplicateArgumentError(car(currentBinding));
    }*/
    Value *newBinding = makeBinding(currentBinding);
    Value *bindingContainer = talloc(sizeof(Value));
    bindingContainer->type = CONS_TYPE;
    bindingContainer->c.car = newBinding;
    bindingContainer->c.cdr = frame->bindings;
    frame->bindings = bindingContainer;
    letBindings = cdr(letBindings);
  }
  return frame;
}

Value *evalLet(Value *args, Frame *frame){
  Value *bindings = car(args);
  Value *expression = cdr(args);
  frame = setVariables(bindings, frame);
  if (isNull(car(expression))){
    letArgsError();
  }
  return eval(car(expression), frame);
}
