#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>


#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "1234567890"
#define MAX_STR_LEN 301

char peek(){
  char output = (char) fgetc(stdin);
  if (output != EOF){
    ungetc(output, stdin);
  }
  return output;
}

Value *makeOpen(){
  Value *newOpen = (Value *)talloc(sizeof(Value));
  newOpen->type = OPEN_TYPE;
  return newOpen;
}

Value *makeClosed(){
  Value *newClosed = (Value *)talloc(sizeof(Value));
  newClosed->type = CLOSE_TYPE;
  return newClosed;
}

Value *makeBool(int value){
  Value *newBool = (Value *)talloc(sizeof(Value));
  newBool->type = BOOL_TYPE;
  if ((value == 0) || (value == 1)){
    newBool->i = value;
  }
  else{
    newBool->i = -1;
  }
  return newBool;
}

Value *makeSymbol(char* value){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  newSymbol->type = SYMBOL_TYPE;
  newSymbol->s = talloc(sizeof(value));
  strcpy(newSymbol->s, value);
  return newSymbol;
}

Value *makeInt(int value){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  newSymbol->type = INT_TYPE;
  newSymbol->i = value;
  return newSymbol;
}

Value *makeDouble(double value){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  newSymbol->type = DOUBLE_TYPE;
  newSymbol->d = value;
  return newSymbol;
}

char *readMultiChar(){
  return "a";
}

int determineType(char *symbol){
  return NULL_TYPE;
}
// Read source code that is input via stdin, and return a linked list consisting of the
// tokens in the source code. Each token is represented as a Value struct instance, where
// the Value's type is set to represent the token type, while the Value's actual value
// matches the type of value, if applicable. For instance, an integer token should have
// a Value struct of type INT_TYPE, with an integer value stored in struct variable i.
// See the assignment instructions for more details.
Value *tokenize(){
    Value *tokens = makeNull();
    char nextChar = (char)fgetc(stdin);

    while (nextChar != EOF){
        if (nextChar == ')'){
          tokens = cons(makeClosed(), tokens);
        }
        if (nextChar == '('){
          tokens = cons(makeOpen(), tokens);
        }
        else{
          char *currentSymbol = talloc(MAX_STR_LEN);
          strcpy(currentSymbol, readMultiChar());
          int type = determineType(currentSymbol);


        }
        nextChar = (char)fgetc(stdin);
    }

    // Reverse the tokens list, to put it back in order
    Value *reversedList = reverse(tokens);
    return reversedList;
}


// Display the contents of the list of tokens, along with associated type information.
// The tokens are displayed one on each line, in the format specified in the instructions.
void displayTokens(Value *list){
  if(!isNull(car(list))){
    switch (car(list)->type) {
      case INT_TYPE:
        printf("%i:integer\n", car(list)->i);
        break;
      case BOOL_TYPE:
        printf("%i:boolean\n", car(list)->i);
        break;
      case OPEN_TYPE:
        printf("(:open\n");
        break;
      case CLOSE_TYPE:
        printf("):close\n");
        break;
      case STR_TYPE:
        printf("%s:integer\n", car(list)->s);
        break;
      case DOUBLE_TYPE:
        printf("%f:double\n", car(list)->d);
        break;
      case SYMBOL_TYPE:
        printf("%s:symbol\n", car(list)->s);
        break;
      default:
        printf("WRONG TYPE\n");
    }
    return displayTokens(cdr(list));
  }
}
