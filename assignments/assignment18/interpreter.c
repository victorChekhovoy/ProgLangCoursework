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
#include "specialForms.h"
#include "evalLetStar.h"
#include "evalLetRec.h"

// A utility function that creates a blank frame
Frame *makeFrame(){
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = NULL;
  newFrame->bindings = makeNull();
  return newFrame;
}

void bindPrimitives(Frame *frame){
  bindPrimitiveFunction("car", &builtInCar, frame);
  bindPrimitiveFunction("cdr", &builtInCdr, frame);
  bindPrimitiveFunction("+", &builtInAdd, frame);
  bindPrimitiveFunction("null?", &builtInNull, frame);
  bindPrimitiveFunction("cons", &builtInCons, frame);
  bindPrimitiveFunction("-", &builtInMinus, frame);
  bindPrimitiveFunction("*", &builtInMultiply, frame);
  bindPrimitiveFunction("*", &builtInMultiply, frame);
  bindPrimitiveFunction("=", &builtInEquals, frame);
  bindPrimitiveFunction("eq?", &builtInEquals, frame);
  bindPrimitiveFunction("<", &builtInLess, frame);
  bindPrimitiveFunction(">", &builtInGreater, frame);
}

// A utility function that creates a Value of type VOID_TYPE
Value *makeVoid(){
  Value *newValue = talloc(sizeof(Value));
  newValue->type = VOID_TYPE;
  return newValue;
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
      printE(node, true);
      printf(". ");
      printE(cdr(*tree), false);
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

//Prints the result of a Scheme function
void printResult(Value *result){
  while (!isNull(result)){
    switch (result->type){
      case CONS_TYPE:
        printLL(&result, true, false);
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
      return symbol;
   }
   case CONS_TYPE: {
      Value *first = car(tree);
      Value *args = cdr(tree);
      Value *result = talloc(sizeof(Value));
      char *specialSymbol = first->s;
      if (!strcmp(specialSymbol, "if")) {
        return evalIf(args, frame);
      } else if (!strcmp(specialSymbol, "let*")){
        return evalLetStar(args, frame);
      } else if (!strcmp(specialSymbol, "letrec")){
        return evalLetRec(args, frame);
      } else if (!strcmp(specialSymbol, "let")){
        return evalLet(args, frame);
      } else if (!strcmp(specialSymbol, "quote")){
        return processQuote(args);
      } else if (!strcmp(specialSymbol, "define")){
        frame = processDefine(args, frame);
        return makeVoid();      //define returns VOID_TYPE instead of NULL_TYPE because NULL_TYPE is sometimes returned by actual functions
      } else if (!strcmp(specialSymbol, "lambda")){
        return processLambda(args, frame);
      } else if (!strcmp(specialSymbol, "set!")){
        frame = processSet(args, frame);
        return makeVoid();
      } else if (!strcmp(specialSymbol, "begin")){
        return processBegin(args, frame);
      } else if (!strcmp(specialSymbol, "and")){
        return processAnd(args, frame);
      } else if (!strcmp(specialSymbol, "or")){
        return processOr(args, frame);
      } else if(!strcmp(specialSymbol, "cond")){
        return processCond(args, frame);
      }
      else {
        return applyFunction(first, args, frame);
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
  bindPrimitives(globalFrame);
  while (!isNull(currentS)){
    Value *result = eval(currentS, globalFrame);
    tree = cdr(tree);
    currentS = car(tree);
    if (isNull(result)){
      printResult(cons(result, makeNull()));
    } else{
      printResult(result);
    }

  }
}
