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


Frame *makeFrame(){
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = talloc(sizeof(Frame));
  newFrame->bindings = makeNull();
  return newFrame;
}


Value *eval(Value *tree, Frame *frame) {
   switch (tree->type)  {
     /*case SYMBOL_TYPE: {
        return lookUpSymbol(tree, frame);
        break;
     }*/
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
          result = evalLet(args, frame);
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
    //display(result);
    tree = cdr(tree);
    currentS = car(tree);
  }
}
