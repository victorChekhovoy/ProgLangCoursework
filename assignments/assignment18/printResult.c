#include <stdlib.h>
#include <stdio.h>
#include "talloc.h"
#include <string.h>
#include <stdbool.h>
#include "value.h"
#include "linkedlist.h"


//Prints a single element in a tree
void printE(Value *tree, bool printWhitespace){
  switch (tree->type){
    case INT_TYPE:
        printf("%i", tree->i);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case DOUBLE_TYPE:
        printf("%g", tree->d);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case STR_TYPE:
        printf("\"%s\"", tree->s);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case PTR_TYPE:
        printf("<%p>", tree->p);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case BOOL_TYPE:
        if (tree->i == 1){
          printf("#t");
        }
        else{
          printf("#f");
        }
        if (printWhitespace){
          printf(" ");
        }
        break;
    case SYMBOL_TYPE:
        printf("%s", tree->s);
        if (printWhitespace){
          printf(" ");
        }
        break;
    case CLOSURE_TYPE:
      printf("#<procedure>");
      if (printWhitespace){
        printf(" ");
      }
    default:
        break;
  }
}

// Prints a linked list inside the tree
void printLL(Value **tree, bool printParens, bool endWhitespace){
  if (!isNull(*tree)){
    if (printParens){
      printf("(");
    }
    Value *node = car(*tree);
    Value *nextNode = car(cdr(*tree));
    bool isNextNull = isNull(nextNode);
    int nodeType = node->type;
    if (nodeType == CONS_TYPE){
      if (isNextNull){
        printLL(&node, true, false);
      }
      else{
        printLL(&node, true, true);
      }
    } else if ((cdr(*tree)->type != CONS_TYPE) && (!isNull(cdr(*tree))) && (!isNull(node))) {
      printE(node, true);
      printf(". ");
      printE(cdr(*tree), false);
    }
    else{
      if (isNextNull){
        printE(node, false);
      }
      else{
        printE(node, true);
      }
    }
    *tree = cdr(*tree);
    printLL(tree, false, false);
    if (printParens){
      printf(")");
      if (endWhitespace){
        printf(" ");
      }
    }
  }
}

//Prints the result of a Scheme function
void printResult(Value *result){
  if (isNull(result)){
    return;
  }
  while (!isNull(result)){
    switch (result->type){
      case CONS_TYPE:
        printLL(&result, true, false);
        break;
      default:
        printE(result, false);
        break;
    }
    result = cdr(result);
  }
  printf("\n");
}
