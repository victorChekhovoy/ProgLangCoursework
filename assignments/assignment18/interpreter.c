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
#include "printResult.h"


// A utility function that creates a blank frame
Frame *makeFrame(){
  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->parent = NULL;
  newFrame->bindings = makeNull();
  return newFrame;
}

// Creates proper bindings in the global frame for all primitive functions
void bindPrimitives(Frame *frame){
  bindPrimitiveFunction("car", &builtInCar, frame);
  bindPrimitiveFunction("cdr", &builtInCdr, frame);
  bindPrimitiveFunction("+", &builtInAdd, frame);
  bindPrimitiveFunction("null?", &builtInNull, frame);
  bindPrimitiveFunction("cons", &builtInCons, frame);
  bindPrimitiveFunction("-", &builtInMinus, frame);
  bindPrimitiveFunction("*", &builtInMultiply, frame);
  bindPrimitiveFunction("/", &builtInDivide, frame);
  bindPrimitiveFunction("=", &builtInEquals, frame);
  bindPrimitiveFunction("eq?", &builtInEquals, frame);
  bindPrimitiveFunction("<", &builtInLess, frame);
  bindPrimitiveFunction(">", &builtInGreater, frame);
  bindPrimitiveFunction("modulo", &builtInModulo, frame);
}

// A utility function that creates a Value of type VOID_TYPE
Value *makeVoid(){
  Value *newValue = talloc(sizeof(Value));
  newValue->type = VOID_TYPE;
  return newValue;
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
