#include "value.h"
#include "talloc.h"
#include <stdlib.h>
#include <stdio.h>

void bindingError(){
  printf("Evaluation Error: improper binding in let\n");
  texit(0);
}

void letArgsError(){
  printf("Evaluation Error: no args following bindings in let\n");
  texit(0);
}

void duplicateArgumentError(Value *symbol){
  printf("Evaluation Error: attempting to define duplicate argument %s in let expression\n", symbol->s);
  texit(0);
}

void symbolNotFoundError(Value *symbol){
  printf("Evaluation Error: symbol %s not found in the frame.\n", symbol->s);
  texit(0);
}

void evaluationError(){
  printf("Evaluation Error: unable to evaluate expression.\n");
  texit(0);
}

void ifArgumentError(){
  printf("Evaluation Error: no consequent following an if\n");
  texit(0);
}

void defineArgumentError(){
  printf("Evaluation Error: improper variable binding in define\n");
  texit(0);
}

void functionArgumentError(){
  printf("Evaluation Error: improper variables in function call\n");
  texit(0);
}

void bindingWrongTypeError(){
  printf("Evaluation Error: define must bind to a symbol\n");
}

void lambdaArgumentNumberError(int number){
  if (number > 2){
    printf("Evaluation Error: too many arguments (%i) passed to lambda, when 2 are required\n", number);
  } else if (number == 0){
    printf("Evaluation Error: no arguments passed to lambda, when 2 are required\n");
  } else if (number < 2){
    printf("Evaluation Error: too few arguments (%i) passed to lambda, when 2 are required\n", number);
  }
  texit(0);
}

void quoteArgumentNumberError(int number){
  if (number > 1){
    printf("Evaluation Error: too many arguments (%i) passed to quote, when 1 is required\n", number);
  } else if (number == 0){
    printf("Evaluation Error: no arguments passed to quote, when 2 are required\n");
  }
  texit(0);
}

void defineArgumentNumberError(int number){
  if (number > 2){
    printf("Evaluation Error: too many arguments (%i) passed to define, when 2 are required\n", number);
  } else if (number == 0){
    printf("Evaluation Error: no arguments passed to define, when 2 are required\n");
  } else if (number < 2){
    printf("Evaluation Error: too few arguments (%i) passed to define, when 2 are required\n", number);
  }
  texit(0);
}

void lambdaNonSymbolArguments(){
  printf("Evaluation Error: non-symbol arguments in lambda\n");
  texit(0);
}

void lambdaNoCode(){
  printf("Evaluation Error: no code following lambda\n");
  texit(0);
}

void functionArgumentNumberError(int params, int values){
  if (params < values){
    printf("Evaluation Error: too many arguments passed (%i) to a function, expected %i\n", values, params);
  } else{
    printf("Evaluation Error: too few arguments passed (%i) to a function, expected %i\n", values, params);
  }
  texit(0);
}

void lambdaDuplicateArgumentError(Value *symbol){
  printf("Evaluation Error: duplicate identifier %s in lambda\n", symbol->s);
  texit(0);
}

void nullNoArgumentsError(){
  printf("Evaluation Error: no arguments passed to null?, 1 required\n");
  texit(0);
}

void nullTooManyArgumentsError(){
  printf("Evaluation Error: too many arguments passed to null?, 1 required\n");
  texit(0);
}

void builtInAddArgumentTypeError(){
  printf("Evaluation Error: wrong type passed to +, expecting integer or double\n");
  texit(0);
}

void carTooManyArgumentsError(int inputLength){
  printf("Evaluation Error: too many arguments (%i) passed to car, 1 expected\n", inputLength);
  texit(0);
}

void carNoArgumentsError(){
  printf("Evaluation Error: no arguments passed to car, 1 required\n");
  texit(0);
}

void carInvalidArgumentTypeError(){
  printf("Evaluation Error: wrong type passed to car, expected list\n");
  texit(0);
}

void cdrTooManyArgumentsError(int inputLength){
  printf("Evaluation Error: too many arguments (%i) passed to cdr, 1 expected\n", inputLength);
  texit(0);
}

void cdrNoArgumentsError(){
  printf("Evaluation Error: no arguments passed to cdr, 1 required\n");
  texit(0);
}

void cdrInvalidArgumentTypeError(){
  printf("Evaluation Error: wrong type passed to cdr, expected list\n");
  texit(0);
}

void consTooManyArgumentsError(int inputLength){
  printf("Evaluation Error: too many arguments (%i) passed to cons, 2 expected\n", inputLength);
  texit(0);
}

void consNoArgumentsError(){
  printf("Evaluation Error: no arguments passed to cons, 2 required\n");
  texit(0);
}

void consTooFewArgumentsError(){
  printf("Evaluation Error: too few arguments (1) passed to cons, 2 expected\n");
  texit(0);
}

void setNoArgumentsError(){
  printf("Evaluation Error: no arguments passed to set!, 2 required\n");
  texit(0);
}

void setTooFewArgumentsError(){
  printf("Evaluation Error: too few arguments (1) passed to set!, 2 expected\n");
  texit(0);
}

void setTooManyArgumentsError(int inputLength){
  printf("Evaluation Error: too many arguments (%i) passed to set!, 2 expected\n", inputLength);
  texit(0);
}

void andNoArgs(){
  printf("Evaluation Error: no arguments passed to and\n");
  texit(0);
}

void orNoArgs(){
  printf("Evaluation Error: no arguments passed to or\n");
  texit(0);
}

void condNoArgs(){
  printf("Evaluation Error: no arguments passed to cond\n");
  texit(0);
}

void condElseNotLast(){
  printf("Evaluation Error: else expeceted as last case in cond\n");
  texit(0);
}

void condReturnNotBool(){
  printf("Evaluation Error: boolean expected as return in cond\n");
  texit(0);
}

void builtInMultipyArgumentTypeError(){
  printf("Evaluation Error: wrong type passed to *, expecting integer or double\n");
  texit(0);
}

void letrecBindingOrderError(Value *symbol){
  printf("Evaluation Error: variable %s in binding expression referenced before assignment\n", symbol->s);
  texit(0);
}

void lessWrongTypeError(){
  printf("Evaluation Error: wrong type passed to <\n");
  texit(0);
}

void greaterWrongTypeError(){
  printf("Evaluation Error: wrong type passed to >\n");
  texit(0);
}

void primitiveTooManyArgsError(char *name, int inputLength){
  printf("Evaluation Error: too many arguments (%i) passed to %s, 2 expected\n", inputLength, name);
  texit(0);
}

void primitiveNoArgsError(char *name){
  printf("Evaluation Error: no arguments passed to %s, 2 required\n", name);
  texit(0);
}

void primitiveTooFewArgsError(char *name){
  printf("Evaluation Error: too few arguments (1) passed to %s, 2 expected\n", name);
  texit(0);
}
void builtInModuloWrongNumberOfArgs(){
  printf("Evaluation Error: wrong number of arguments passed to modulo. Expecting 2 arguments\n");
  texit(0);
}

void builtInModuloArgumentTypeError(){
  printf("Evaluation Error: wrong type of arguments passed to modulo. Expecting integers");
  texit(0);
}

void letrecUsedTooSoonError(Value *symbol){
  printf("Evaluation Error: letrec used a variable %s before all were computed.\n", symbol->s);
  texit(0);
}

void setNoVariableError(Value *lookUp){
  printf("Evaluation Error: <set!> couldn't find the variable %s in the frame; did you mean to use <define>?\n", lookUp->s);
  texit(0);
}
