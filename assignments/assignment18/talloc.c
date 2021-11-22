#include <stdio.h>
#include "talloc.h"
#include "linkedlist.h"


Value *activeList = NULL;

// Allocates memory and keeps track of it for garbage collection
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

// Cleans a Value type variable
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
      case SYMBOL_TYPE:
        if (node->s != NULL){
          free(node->s);
        }
        node->s = NULL;
        break;
    }
    free(node);
    node = NULL;
  }
}

// Frees activeList and all values in it
void tfree(){
  cleanNode(activeList);
  activeList = NULL;
}

// Frees all variables and exits the execution process
void texit(int status){
  tfree();
  exit(status);
}
