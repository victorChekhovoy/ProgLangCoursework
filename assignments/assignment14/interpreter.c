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
#include "errorCall"

Frame *makeFrame(){
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = NULL;
  newFrame->bindings = makeNull();
  return newFrame;
}

void displaySymbol(Value *result){
  switch(result->type){
      case INT_TYPE:
        printf("%i\n", result->i);
        break;
      case BOOL_TYPE:
        if (result->i == 1){
          printf("#t\n");
        }
        if (result->i == 0){
          printf("#f\n");
        }
        break;
      case STR_TYPE:
        printf("\"%s\"\n", result->s);
        break;
      case DOUBLE_TYPE:
        printf("%f\n", result->d);
        break;
      default:
        printf("\n");
        break;
  }
}

Value *eval(Value *tree, Frame *frame) {
  switch (tree->type)  {
   case SYMBOL_TYPE: {
      Value *symbol = lookUpSymbol(tree, frame);
      if (isNull(symbol)){
        symbolNotFoundError(tree);
      }
      return symbol;
   }
   case CONS_TYPE: {
      Value *first = car(tree);
      Value *args = cdr(tree);
      Value *result = talloc(sizeof(Value));

      if (!strcmp(first->s, "if")) {
          result = evalIf(args, frame);
      } else if (!strcmp(first->s, "let")){
        return evalLet(args, frame);
      } else {
         evaluationError();
         return NULL;
      }
   }
   default: {
     return tree;
   }
  }
}

void interpret(Value *tree){
  Value *currentS = car(tree);
  while (!isNull(currentS)){
    Value *result = eval(currentS, makeFrame());
    tree = cdr(tree);
    currentS = car(tree);
    displaySymbol(result);
  }
}
