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
#include "errorCall.h"
#include "parser.h"
#include "apply.h"
#include "evalEach.h"

// A utility function that creates a blank frame
Frame *makeFrame(){
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = NULL;
  newFrame->bindings = makeNull();
  return newFrame;
}

Value *makeClosure(Value *args, Value* code, Frame *frame){
  Value *newClosure = talloc(sizeof(Value));
  newClosure->type = CLOSURE_TYPE;
  newClosure->cl.paramNames = args;
  newClosure->cl.functionCode = code;
  newClosure->cl.frame = frame;
  return newClosure;
}

Frame *defineVariable(Value *symbol, Value *variableValue, Frame *frame){
  if ((isNull(symbol)) || (isNull(variableValue))){
    defineArgumentError();
  }
  Value *newVariable = cons(symbol, variableValue);
  Value *bindingContainer = cons(newVariable, frame->bindings);
  frame->bindings = bindingContainer;
  return frame;
}

// Displays a Value for the output of interpret()
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

// Evaluates a single S-expression given in tree
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
        return evalIf(args, frame);
      } else if (!strcmp(first->s, "let")){
        return evalLet(args, frame);
      } else if (!strcmp(first->s, "quote")){
        if (length(args) != 1){
          evaluationError();
        }
        return args;
      } else if (!strcmp(first->s, "define")){
        if (length(args) != 2){
          evaluationError();
        }
        frame = defineVariable(car(args), car(cdr(args)), frame);
        return makeNull();
      } else if (!strcmp(first->s, "lambda")){
        if (length(args) != 2){
          evaluationError();
        }
        return makeClosure(car(args), car(cdr(args)), frame);
      }
      else {
        Value *evaluatedOperator = eval(first, frame);
        Value *evaluatedArgs = evalEach(args, frame);
        return apply(evaluatedOperator, evaluatedArgs);
      }
    }
   default: {
     return tree;
   }
  }
}

// Given a Scheme program in a linked list format, evaluates all S-expressions in it
void interpret(Value *tree){
  Value *currentS = car(tree);
  Frame *frame = makeFrame();
  while (!isNull(currentS)){
    Value *result = eval(currentS, frame);
    tree = cdr(tree);
    currentS = car(tree);
    printTree(result);

  }
}
