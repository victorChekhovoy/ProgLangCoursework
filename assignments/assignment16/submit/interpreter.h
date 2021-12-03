#ifndef _INTERPRETER
#define _INTERPRETER

void interpret(Value *tree);

Frame makeFrame();

Value *eval(Value *expr, Frame *frame);

#endif
