Value *evalIf(Value *args, Frame *frame){
    Value *bindings = car(args);
    Value *expression = cdr(args);
    if (bindings->type != CONS_TYPE){
        bindingError();
    }

    Value *bool_exp = eval(car(args), frame);
    Value *true_result = car(cdr(args));
    Value *false_result = car(cdr(cdr(args)));

    if (bool_exp->type == BOOL_TYPE) {
        if (bool_exp->i == 1) {
            return eval(true_result, frame);
        }
        else {
            return eval(false_result, frame);
        }
    }
    else {
        //error function
    }
    return args;
}