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
#include "errorCall.h"

Value *evalIf(Value *args, Frame *frame){
    Value *bindings = car(args);
    Value *expression = cdr(args);

    if (length(args) != 3) {
        ifArgumentError();
    }

    Value *boolExp = eval(car(args), frame);
    Value *trueResult = car(cdr(args));
    Value *falseResult = car(cdr(cdr(args)));

    if (boolExp->type == BOOL_TYPE && boolExp->i == 0) {
        return eval(falseResult, frame);
    } else {
        return eval(trueResult, frame);
    }
    return args;

}
