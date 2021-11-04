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

Frame *makeFrame(){
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = NULL;
  newFrame->bindings = makeNull();
  return newFrame;
}

void symbolNotFound(Value *symbol){
  printf("Fatal Error: Symbol ");
  printf("%s", symbol->s);
  printf(" not found in the lookup table.\nThe program will now terminate.\n");
  texit(0);
}


void displayResult(Value *result){
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
        symbolNotFound(tree);
      }
      return symbol;
   }
   case CONS_TYPE: {
      Value *first = car(tree);
      Value *args = cdr(tree);
      Value *result = talloc(sizeof(Value));

      // Error checking on 'first' goes here...
      /*
      if (!strcmp(first->s, "if")) {
          result = evalIf(args, frame);
      }*/
      if (!strcmp(first->s, "let")){
        return evalLet(args, frame);
      }
      // Other special forms go here...

      /*else {
         // 'first' is not a recognized special form
         evaluationError();
      }*/
      return result;
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
    displayResult(result);
  }
}
