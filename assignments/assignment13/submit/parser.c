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

// Checks if the current token is an opening paren
bool isOpen(Value *token){
  return (token->type == OPEN_TYPE);
}

// Checks if the current token is a closing paren
bool isClosed(Value *token){
  return (token->type == CLOSE_TYPE);
}

// Checks if the current token is a string
bool isString(Value *token){
  return (token->type == STR_TYPE);
}

// Pops a value from the stack
Value *pop(Value **stack){
  Value *poppedValue = talloc(sizeof(Value));
  poppedValue = car(*stack);
  *stack = cdr(*stack);
  return poppedValue;
}

// Pushes the value onto the stack
Value* push(Value *stack, Value *token){
  stack = cons(token, stack);
  return stack;
}

// Parses the stack by popping off tokens one by one and putting them into a separate expression
Value *parseStack(Value *stack, Value* tree){
  Value *expression = cons(makeNull(), makeNull());
  while ((!isOpen(car(stack))) && (!isNull(stack))) {
    if(isClosed(car(stack))){
      pop(&stack);
    }
    else{
      expression = cons(pop(&stack), expression);
    }
  }
  pop(&stack);
  stack = cons(expression, stack);
  return stack;
}

// Parses one S-expression
Value *parseExpression(Value *tree, Value **tokens, int *depth){
  Value *stack = makeNull();
  do{
    while ((!isClosed(car(*tokens))) && (!isNull(*tokens))){
      if (isOpen(car(*tokens))){
        (*depth)++;
      }
      stack = cons(pop(tokens), stack);
    }
    if (isClosed(car(*tokens))){
      (*depth)--;
      stack = cons(pop(tokens), stack);
      stack = parseStack(stack, tree);
    }
  } while (((*depth) != 0) && !isNull(*tokens));
  tree = cons(pop(&stack), tree);
  return tree;
}

// Parses the entire Scheme program if it's an atom
Value *parseAtom(Value *tree, Value **tokens){
  Value *atomList = makeNull();
  while (!isNull(*tokens)){
    atomList = cons(car(*tokens), atomList);
    *tokens = cdr(*tokens);
  }
  return atomList;
}

// Parses the entire Scheme program
Value *parse(Value *tokens) {

    assert(tokens != NULL && "Error (parse): null pointer");

    Value *tree = makeNull();
    int depth = 0;
    if (!isOpen(car(tokens))){
      tree = parseAtom(tree, &tokens);
    }
    while (!isNull(tokens)) {
        tree = parseExpression(tree, &tokens, &depth);
    }
    if (depth != 0) {
        syntaxError();
    }
    tree = reverse(tree);
    return tree;
}

// Prints the type of a node, for debugging and displaying
void printType(Value *node){
  switch(node->type){
    case INT_TYPE:
      printf("INT_TYPE\n");
      break;
    case DOUBLE_TYPE:
      printf("DOUBLE_TYPE\n");
      break;
    case STR_TYPE:
      printf("STR_TYPE\n");
      break;
    case CONS_TYPE:
      printf("CONS_TYPE\n");
      break;
    case NULL_TYPE:
      printf("NULL_TYPE\n");
      break;
    case PTR_TYPE:
      printf("PTR_TYPE\n");
      break;
    case OPEN_TYPE:
      printf("OPEN_TYPE\n");
      break;
    case CLOSE_TYPE:
      printf("CLOSE_TYPE\n");
      break;
    case BOOL_TYPE:
      printf("BOOL_TYPE\n");
      break;
    case SYMBOL_TYPE:
      printf("SYMBOL_TYPE\n");
      break;
    case ERROR_TYPE:
      printf("ERROR_TYPE\n");
      break;
    default:
      printf("TYPE NOT SUPPORTED\n");
      break;
    }
}

// Prints one element in the linked list
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
    default:
        break;
  }
}

// Prints a linked list
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

// Prints the parse tree
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
}
