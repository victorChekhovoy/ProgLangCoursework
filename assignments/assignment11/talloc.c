#include <stdio.h>
#include "talloc.h"
#include "linkedlist.h"


Value *activeList = NULL;

void *talloc(size_t size){
  if (activeList == NULL){
    activeList = malloc(sizeof(Value));
    activeList->type = NULL_TYPE;
  }
  void *outputPointer = malloc(size);
  Value *newNode = malloc(sizeof(Value));
  Value *pointerNode = malloc(sizeof(Value));
  pointerNode->type = PTR_TYPE;
  pointerNode->p = outputPointer;
  newNode->type = CONS_TYPE;
  newNode->c.car = pointerNode;
  newNode->c.cdr = activeList;
  activeList = newNode;

  return outputPointer;
}

void cleanNode(Value *node){
  if (node != NULL){
    int type = node->type;
    printf("Node type: %i\n", type);
    switch (type){
      case STR_TYPE:
        free(node->s);
        break;
      case PTR_TYPE:
        free(node->p);
        break;
      case CONS_TYPE:
        cleanNode(node->c.car);
        cleanNode(node->c.cdr);
        break;
    }
    free(node);
  }
}


void tfree(){
  /*display(activeList);
  if (activeList == NULL){
    return;
  }
  if (activeList->c.car == NULL){
    free(activeList);
    return;
  }
  else{
    cleanNode(activeList->c.car);
    //free(activeList);
    activeList = activeList->c.cdr;
    tfree();
    return;
  }*/
  cleanNode(activeList);
}


void texit(int status){
  tfree();
  exit(status);
  return;
}
