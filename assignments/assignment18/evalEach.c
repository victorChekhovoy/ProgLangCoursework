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
#include "interpreter.h"

//Evaluates each expression in a linked list
Value *evalEach(Value *argsList, Frame *frame){
  if(isNull(argsList)){
    return makeNull();
  }
  return cons(eval(car(argsList), frame), evalEach(cdr(argsList), frame));
}
