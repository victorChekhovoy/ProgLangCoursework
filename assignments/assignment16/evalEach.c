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

Value *evalEach(Value *argsList, Frame *frame){
  return cons(eval(car(argsList), frame), evalEach(cdr(argsList), frame));
}
