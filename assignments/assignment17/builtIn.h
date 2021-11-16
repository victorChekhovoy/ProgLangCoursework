#ifndef _BUILNIT
#define _INTERPRETER

Value *builtInAdd(Value *args);

Value *builtInCar(Value *args);

Value *builtInCdr(Value *args);

void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame);

Value *builtInNull(Value *args);

#endif
