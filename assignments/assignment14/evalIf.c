#include <stdlib.h>
#include <stdio.h>
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "evalIf.h"
#include "lookUpSymbol.h"
#include "interpreter.h"
#include "errorCall"

Value *evalIf(Value *args, Frame *frame){
    Value *bindings = car(args);
    Value *expression = cdr(args);
    if (bindings->type != CONS_TYPE){
        ifBindingError();
    }

    Value *bool_exp = eval(car(args), frame);
    Value *true_result = car(cdr(args));
    Value *false_result = car(cdr(cdr(args)));

    if (bool_exp->type == BOOL_TYPE && bool_exp->i == 0) {
        return eval(false_result, frame);
    } else {
        return eval(true_result, frame);
    }
    return args;

}
