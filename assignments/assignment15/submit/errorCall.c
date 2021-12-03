#include "value.h"
#include "talloc.h"
#include <stdlib.h>
#include <stdio.h>

//Throws an error if the bindings in let aren't properly formatted
void bindingError(){
  printf("Evaluation error: improper binding in let\n");
  texit(0);
}

//Throws an error if there are no arguments after bindings in let
void letArgsError(){
  printf("Evaluation error: no args following bindings in let\n");
  texit(0);
}

//Throws an error if the same variable appears twice in a binding in let
void duplicateArgumentError(Value *symbol){
  printf("Evaluation error: attempting to define duplicate argument %s in let expression\n", symbol->s);
  texit(0);
}

//Throws an error if a symbol wasn't found in the frame
void symbolNotFoundError(Value *symbol){
  printf("Evaluation error: symbol %s not found in the frame.\n", symbol->s);
  texit(0);
}

//Throws an error if the program was unable to evaluate an expression
void evaluationError(){
  printf("Evaluation Error: unable to evaluate expression.\n");
  texit(0);
}

// Throws an error if there was no code following the boolean expression in if
void ifArgumentError(){
    printf("Evaluation error: no consequent following an if\n");
  texit(0);
}
