#ifndef _ERRORCALL
#define _ERRORCALL

void bindingError();

void letArgsError();

void duplicateArgumentError(Value *symbol);

void symbolNotFoundError(Value *symbol);

void symbolNotFoundError(Value *symbol);

void evaluationError();

void ifArgumentError();

void defineArgumentError();

void functionArgumentError();

void bindingWrongTypeError();

void lambdaArgumentNumberError(int number);

void quoteArgumentNumberError(int number);

void defineArgumentNumberError(int number);

#endif
