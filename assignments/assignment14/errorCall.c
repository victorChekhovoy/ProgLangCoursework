#include "value.h"
#include "talloc.h"

void bindingError(){
  printf("Evaluation error: improper binding in let\n");
  texit(0);
}

void letArgsError(){
  printf("Evaluation error: no args following bindings in let\n");
  texit(0);
}

void duplicateArgumentError(Value *symbol){
  printf("Evaluation error: attempting to define duplicate argument %s in let expression\n", symbol->s);
  texit(0);
}

void symbolNotFoundError(Value *symbol){
  printf("Evaluation error: symbol %s not found in the frame.\n", symbol->s);
  texit(0);
}

void symbolNotFoundError(Value *symbol){
  printf("Evaluation Error: Symbol %s not found in the lookup table.\n", symbol->s);
  texit(0);
}

void evaluationError(){
  printf("Evaluation Error: unable to evaluate expression.\n");
  texit(0);
}
