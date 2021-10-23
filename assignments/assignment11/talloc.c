#include <stdio.h>
#include "talloc.h"


Value *activeList = NULL;

void *talloc(size_t size){
  void *outputPointer = malloc(size);
  if (activeList == NULL){
    activeList = malloc(sizeof(Value));
    activeList->type = CONS_TYPE;
    activeList->c.car = outputPointer;
    activeList->c.cdr = NULL;
  }
  else {
    Value *newNode = malloc(sizeof(Value));
    newNode->type = CONS_TYPE;
    newNode->c.car = outputPointer;
    newNode->c.cdr = activeList;
    activeList = newNode;
  }

  return outputPointer;
}

void cleanNode(Value *node){
  int type = node->type;
  switch (type){
    case STR_TYPE:
      free(node->s);
      break;
    case PTR_TYPE:
      free(node->p);
      break;
  }
  free(node);
  return;
}


void tfree(){
  if (activeList == NULL){
    return;
  }
  if (activeList->c.car == NULL){
    free(activeList);
    return;
  }
  else{
    cleanNode(activeList->c.car);
    activeList = activeList->c.cdr;
    tfree();
    return;
  }
}


void texit(int status){
  tfree();
  exit(status);
  return;
}
