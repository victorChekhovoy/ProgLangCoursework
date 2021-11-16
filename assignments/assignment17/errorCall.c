#include "value.h"
#include "talloc.h"
#include <stdlib.h>
#include <stdio.h>

//Throws an error if the bindings in let aren't properly formatted
void bindingError(){
  printf("Evaluation Error: improper binding in let\n");
  texit(0);
}

//Throws an error if there are no arguments after bindings in let
void letArgsError(){
  printf("Evaluation Error: no args following bindings in let\n");
  texit(0);
}

//Throws an error if the same variable appears twice in a binding in let
void duplicateArgumentError(Value *symbol){
  printf("Evaluation Error: attempting to define duplicate argument %s in let expression\n", symbol->s);
  texit(0);
}

//Throws an error if a symbol wasn't found in the frame
void symbolNotFoundError(Value *symbol){
  printf("Evaluation Error: symbol %s not found in the frame.\n", symbol->s);
  texit(0);
}

//Throws an error if the program was unable to evaluate an expression
void evaluationError(){
  printf("Evaluation Error: unable to evaluate expression.\n");
  texit(0);
}

// Throws an error if there was no code following the boolean expression in if
void ifArgumentError(){
  printf("Evaluation Error: no consequent following an if\n");
  texit(0);
}

//Throws an error if one of the arguments passed to define was null
void defineArgumentError(){
  printf("Evaluation Error: improper variable binding in define\n");
  texit(0);
}

//Throws an error if the program couldn't bind the variables in a function call
void functionArgumentError(){
  printf("Evaluation Error: improper variables in function call\n");
  texit(0);
}

//Throws an error if the user attemps to bind a value to a non-symbol value
void bindingWrongTypeError(){
  printf("Evaluation Error: define must bind to a symbol\n");
}

//Thows an error if the user passes an incorrect number of arguments to lambda
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

//Throws an error if the user passes an incorrect number of arguments to quote
void quoteArgumentNumberError(int number){
  if (number > 1){
    printf("Evaluation Error: too many arguments (%i) passed to quote, when 1 is required\n", number);
  } else if (number == 0){
    printf("Evaluation Error: no arguments passed to quote, when 2 are required\n");
  }
  texit(0);
}

//Throws an error if the user passes an incorrect number of arguments to define
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

//Throws an error if the user attemts to include non-symbol variables in almbda definition
void lambdaNonSymbolArguments(){
  printf("Evaluation Error: non-symbol arguments in lambda\n");
  texit(0);
}

//Throws an error if there is no code following variable declaration in lambda
void lambdaNoCode(){
  printf("Evaluation Error: no code following lambda\n");
  texit(0);
}

//Throws an error if the user passes an incorrec number of arguments in a function call
void functionArgumentNumberError(int params, int values){
  if (params < values){
    printf("Evaluation Error: too many arguments passed (%i) to a function, expected %i\n", values, params);
  } else{
    printf("Evaluation Error: too few arguments passed (%i) to a function, expected %i\n", values, params);
  }
  texit(0);
}

//Throws an error if there are multiple symbols in lambda variable declaration
void lambdaDuplicateArgumentError(Value *symbol){
  printf("Evaluation Error: duplicate identifier %s in lambda\n", symbol->s);
  texit(0);
}

//Throws an error if there were no arguments passed to null?
void nullNoArgumentsError(){
  printf("Evaluation Error: no arguments passed to null?, 1 required\n");
  texit(0);
}

//Throws an error if too many arguments were passed to null?
void nullTooManyArgumentsError(int argCount){
  printf("Evaluation Error: too many arguments (%i) passed to null?, 1 required\n", argCount);
  texit(0);
}
