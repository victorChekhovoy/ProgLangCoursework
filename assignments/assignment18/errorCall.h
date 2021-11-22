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

void lambdaNonSymbolArguments();

void lambdaNoCode();

void functionArgumentNumberError(int params, int values);

void lambdaDuplicateArgumentError(Value *symbol);

void nullNoArgumentsError();

void nullTooManyArgumentsError();

void builtInAddArgumentTypeError();

void carTooManyArgumentsError(int inputLength);

void carNoArgumentsError();

void cdrTooManyArgumentsError(int inputLength);

void cdrNoArgumentsError();

void consTooManyArgumentsError(int inputLength);

void consTooFewArgumentsError();

void consNoArgumentsError();

void carInvalidArgumentTypeError();

void cdrInvalidArgumentTypeError();

void setNoArgumentsError();

void setTooFewArgumentsError();

void setTooManyArgumentsError(int inputLength);

#endif
