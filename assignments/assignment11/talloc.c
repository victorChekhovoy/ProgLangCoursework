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

    switch (type){
      case STR_TYPE:
        if (node->s != NULL){
          free(node->s);
        }
        node->s = NULL;
        break;
      case PTR_TYPE:
        if (node->p != NULL){
          free(node->p);
        }
        node->p = NULL;
        break;
      case CONS_TYPE:
        cleanNode(node->c.car);
        cleanNode(node->c.cdr);
        node->c.car = NULL;
        node->c.cdr = NULL;
        break;
    }
    free(node);
    node = NULL;
  }
}


void tfree(){
  cleanNode(activeList);
  activeList = NULL;
}


void texit(int status){
  tfree();
  exit(status);
  return;
}
