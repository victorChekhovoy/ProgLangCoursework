#include "value.h"
#include "talloc.h"
#include <stdlib.h>
#include <stdio.h>

//Throws an Error if the bindings in let aren't properly formatted
void bindingError(){
  printf("Evaluation Error: improper binding in let\n");
  texit(0);
}

//Throws an Error if there are no arguments after bindings in let
void letArgsError(){
  printf("Evaluation Error: no args following bindings in let\n");
  texit(0);
}

//Throws an Error if the same variable appears twice in a binding in let
void duplicateArgumentError(Value *symbol){
  printf("Evaluation Error: attempting to define duplicate argument %s in let expression\n", symbol->s);
  texit(0);
}

//Throws an Error if a symbol wasn't found in the frame
void symbolNotFoundError(Value *symbol){
  printf("Evaluation Error: symbol %s not found in the frame.\n", symbol->s);
  texit(0);
}

//Throws an Error if the program was unable to evaluate an expression
void evaluationError(){
  printf("Evaluation Error: unable to evaluate expression.\n");
  texit(0);
}

// Throws an Error if there was no code following the boolean expression in if
void ifArgumentError(){
  printf("Evaluation Error: no consequent following an if\n");
  texit(0);
}

void defineArgumentError(){
  printf("Evaluation Error: improper variable binding in define");
  texit(0);
}

void functionArgumentError(){
  printf("Evaluation Error: improper variables in function call");
  texit(0);
}
