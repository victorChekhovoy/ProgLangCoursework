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
#include "builtIn.h"


// A utility function that creates a blank frame
Frame *makeFrame(){
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = NULL;
  newFrame->bindings = makeNull();
  return newFrame;
}

// Checks if an expression has the same argument for the car and cdr
void checkDuplicateArgs(Value *args){
  if (containsSymbol(cdr(args), car(args))){
    lambdaDuplicateArgumentError(car(args));
  }
  if (!isNull(args)){
    return checkDuplicateArgs(cdr(args));
  }
}

// Creates a new closure from the given args, code, and frame
Value *makeClosure(Value *args, Value* code, Frame *frame){
  Value *newClosure = talloc(sizeof(Value));
  checkDuplicateArgs(args);
  newClosure->type = CLOSURE_TYPE;
  newClosure->cl.paramNames = args;
  newClosure->cl.functionCode = code;
  newClosure->cl.frame = frame;
  return newClosure;
}

// Checks if each element in a list is a symbol
bool checkSymbolEach(Value *list){
  if (isNull(list)){
    return true;
  } else if (car(list)->type != SYMBOL_TYPE){
    return false;
  }
  return checkSymbolEach(cdr(list));
}

// Creates a new variable and makes it the binding for the frame then returns the frame
Frame *defineVariable(Value *symbol, Value *variableValue, Frame *frame){
  if ((isNull(symbol)) || (isNull(variableValue))){
    defineArgumentError();
  }
  Value *newVariable = cons(symbol, variableValue);
  Value *bindingContainer = cons(newVariable, frame->bindings);
  frame->bindings = bindingContainer;
  return frame;
}


//Prints a single element in a tree
void printE(Value *tree, bool printWhitespace){
  switch (tree->type){
    case INT_TYPE:
        printf("%i", tree->i);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case DOUBLE_TYPE:
        printf("%g", tree->d);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case STR_TYPE:
        printf("\"%s\"", tree->s);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case PTR_TYPE:
        printf("<%p>", tree->p);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case BOOL_TYPE:
        if (tree->i == 1){
          printf("#t");
        }
        else{
          printf("#f");
        }
        if (printWhitespace){
          printf(" ");
        }
        break;
    case SYMBOL_TYPE:
        printf("%s", tree->s);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case CLOSURE_TYPE:
      printf("#<procedure>");
      if (printWhitespace){
        printf(" ");
      }
    default:
        break;
  }
}

// Prints a linked list inside the tree
void printLL(Value **tree, bool printParens, bool endWhitespace){
  if (!isNull(*tree)){
    if (printParens){
      printf("(");
    }
    Value *node = car(*tree);
    Value *nextNode = car(cdr(*tree));
    bool isNextNull = isNull(nextNode);
    int nodeType = node->type;
    if (nodeType == CONS_TYPE){
      if (isNextNull){
        printLL(&node, true, false);
      }
      else{
        printLL(&node, true, true);
      }
    } else if ((cdr(*tree)->type != CONS_TYPE) && (!isNull(cdr(*tree))) && (!isNull(node))) {
      //printf("(");
      printE(node, true);
      printf(". ");
      printE(cdr(*tree), false);
      //printf(")");
    }
    else{
      if (isNextNull){
        printE(node, false);
      }
      else{
        printE(node, true);
      }
    }
    *tree = cdr(*tree);
    printLL(tree, false, false);
    if (printParens){
      printf(")");
      if (endWhitespace){
        printf(" ");
      }
    }
  }
}

//Prints the parse tree
void printResult(Value *result){
  while (!isNull(result)){
    switch (result->type){
      case CONS_TYPE:
        printLL(&result, false, false);
        break;
      default:
        printE(result, false);
        break;
    }
    result = cdr(result);
  }
  printf("\n");
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
        if (length(args) > 1){
          quoteArgumentNumberError(length(args));
        }
        return args;
      } else if (!strcmp(first->s, "define")){
        if (length(args) != 2){
          defineArgumentNumberError(length(args));
        }
        Value *variable = car(args);
        Value *value = eval(car(cdr(args)), frame);
        if (variable->type != SYMBOL_TYPE){
          bindingWrongTypeError();
        }
        frame = defineVariable(variable, value, frame);
        return makeNull();
      } else if (!strcmp(first->s, "lambda")){
        if (!checkSymbolEach(car(args))){
          lambdaNonSymbolArguments();
        }
        if (isNull(car(cdr(args)))){
          lambdaNoCode();
        }
        if (length(args) > 3){
          lambdaArgumentNumberError(length(args));
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
  Frame *globalFrame = makeFrame();
  bindPrimitiveFunction("car", &builtInCar, globalFrame);
  bindPrimitiveFunction("cdr", &builtInCdr, globalFrame);
  bindPrimitiveFunction("+", &builtInAdd, globalFrame);
  bindPrimitiveFunction("null?", &builtInNull, globalFrame);
  bindPrimitiveFunction("cons", &builtInCons, globalFrame);
  while (!isNull(currentS)){
    Value *result = eval(currentS, globalFrame);
    tree = cdr(tree);
    currentS = car(tree);
    printResult(result);

  }
}
