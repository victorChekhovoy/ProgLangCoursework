#ifndef _SPECIAL
#define _SPECIAL

Value *processQuote(Value *args);

Frame *processDefine(Value *args, Frame *frame);

Value *processLambda(Value *args, Frame *frame);

Value *applyFunction(Value *function, Value *args, Frame *frame);

Frame *processSet(Value *args, Frame *frame);

Value *processBegin(Value *args, Frame *frame);

Value *processAnd(Value *args, Frame *frame);

Value *processOr(Value *args, Frame *frame);

Value *processCond(Value *args, Frame *frame);

#endif
