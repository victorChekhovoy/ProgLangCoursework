#ifndef _BUILNIT
#define _BUILNIT

Value *builtInAdd(Value *args);

Value *builtInCar(Value *args);

Value *builtInCdr(Value *args);

void bindPrimitiveFunction(char *name, Value *(*function)(struct Value *), Frame *frame);

Value *builtInNull(Value *args);

Value *builtInCons(Value *args);

Value *builtInMinus(Value *args);

Value *builtInMultiply(Value *args);

#endif
