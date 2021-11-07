#ifndef _ERRORCALL
#define _ERRORCALL

void bindingError();

void letArgsError();

void duplicateArgumentError(Value *symbol);

void symbolNotFoundError(Value *symbol);

void symbolNotFoundError(Value *symbol);

void evaluationError();

void ifBindingError();

#endif
