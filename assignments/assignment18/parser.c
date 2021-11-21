#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>
#include "tokenizer.h"
#include <assert.h>


// Throws a simple syntax error
void syntaxError(){
  printf("SYNTAX ERROR\n");
  texit(0);
}

// Takes in a tree, depth value, and token and adds the token to the tree
Value *updateTree(Value *tree, int *depth, Value *token) {
    if (token->type != CLOSE_TYPE) {
        Value *temp = cons(token, tree);
        tree = temp;
        if (token->type == OPEN_TYPE) {
            *depth = *depth + 1;
        }
    } else {
        if (tree->type == NULL_TYPE || *depth < 1) {
            syntaxError();
        }
        *depth = *depth - 1;
        Value *subtree = makeNull();
        while (car(tree)->type != OPEN_TYPE) {
            Value *temp = cons(car(tree), subtree);
            subtree = temp;
            tree = cdr(tree);
            if (tree->type == NULL_TYPE) {
                syntaxError();
            }
        }
        tree->c.car = subtree;
    }
    return tree;
}

// Takes a list of tokens and returns a pointer to the parse tree
Value *parse(Value *tokens) {
    Value *tree = makeNull();
    int depth = 0;
    Value *curNode = tokens;
    assert(curNode != NULL && "Parse error: token list is null");
    while (curNode->type != NULL_TYPE) {
        tree = updateTree(tree, &depth, car(curNode));
        Value *next = cdr(curNode);
        curNode = next;
    }
    if (depth != 0) {
        syntaxError();
    }
    return reverse(tree);
}

//Prints a single element in a parse tree
void printElement(Value *tree, bool printWhitespace){
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

// Prints a linked list inside the parse tree
void printLinkedList(Value **tree, bool printParens, bool endWhitespace){
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
        printLinkedList(&node, true, false);
      }
      else{
        printLinkedList(&node, true, true);
      }
    }
    else{
      if (isNextNull){
        printElement(node, false);
      }
      else{
        printElement(node, true);
      }
    }
    *tree = cdr(*tree);
    printLinkedList(tree, false, false);
    if (printParens){
      printf(")");
      if (endWhitespace){
        printf(" ");
      }
    }
  }
}

//Prints the parse tree
void printTree(Value *tree){
  while (!isNull(tree)){
    switch (tree->type){
      case CONS_TYPE:
        printLinkedList(&tree, false, false);
        break;
      default:
        printElement(tree, false);
        break;
    }
    tree = cdr(tree);
  }
  printf("\n");
}

/*
1. Put items from tokens into stack until we reach a )
2. Put items from the stack into a linked list until we reach a (
3. Put the linked list into the stack
4. Repeat until tokens is empty

*/
