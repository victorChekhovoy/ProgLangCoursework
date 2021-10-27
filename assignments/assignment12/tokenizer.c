#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "talloc.h"
#include "linkedlist.h"
#include <string.h>


#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUMBERS "1234567890"
#define MAX_STR_LEN 301
#define TERMINATORS "() \n"

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

Value *makeBool(char *value){
  Value *newBool = (Value *)talloc(sizeof(Value));
  newBool->type = BOOL_TYPE;
  if (value[1] == 't'){
    newBool->i = 1;
  }
  if (value[1] == 'f'){
    newBool->i = 0;
  }
  //printf("Boolean value: %i\n", newBool->i);
  return newBool;
}

Value *makeSymbol(char *value){
  Value *newSymbol = talloc(sizeof(Value));
  newSymbol->type = SYMBOL_TYPE;
  newSymbol->s = value;
  return newSymbol;
}

Value *makeInt(char *value){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  int numericValue = strtol(value, NULL, 10);
  newSymbol->type = INT_TYPE;
  newSymbol->i = numericValue;
  return newSymbol;
}

Value *makeDouble(char *value){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  double numericValue = strtod(value, NULL);
  newSymbol->type = DOUBLE_TYPE;
  newSymbol->d = numericValue;
  return newSymbol;
}

Value *makeString(char *rawValue, int length){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  newSymbol->type = STR_TYPE;
  newSymbol->s = talloc(sizeof(char)*(length-1));
  for (int i = 1; i < length; i++){
    newSymbol->s[i-1] = rawValue[i];
  }
  newSymbol->s[length] = '\0';
  return newSymbol;
}

char* readString(int *length){
  char *output = talloc(MAX_STR_LEN);
  char current = (char) fgetc(stdin);
  *length = 0;
  output[(*length)++] = '"';
  while (current != EOF && current != '"' && current != '\n'){
    output[(*length)++] = current;
    current = (char) fgetc(stdin);
  }
  output[*length] = '"';
  output[(*length)+1] = '\0';
  return output;
}

char* readComment(){
  char *output = talloc(MAX_STR_LEN);
  char current = (char) fgetc(stdin);
  int index = 0;
  output[index++] = ';';
  while (current != EOF && current != '\n'){
    output[index] = current;
    index++;
    current = (char) fgetc(stdin);
  }
  output[index+1] = '\0';
  return output;
}

bool errorCheck(char *input, int length){
  return true; //TODO
}

char *readMultiChar(char currentChar, int *index){
  char *output;
  if (currentChar == '"'){
    output = readString(index);
  }
  else if (currentChar == ';'){
    output = readComment();
  }
  else{
    output = talloc(MAX_STR_LEN);
    *index = 0;
    while (currentChar != EOF && strchr(TERMINATORS, currentChar) == NULL){
      output[*index] = currentChar;
      (*index)++;
      currentChar = (char) peek();
      if ((currentChar != ')') && (currentChar != '(')){
        currentChar = (char) fgetc(stdin);
      }
    }
    output[(*index)+1] = '\0';
  }
  return output;
}

bool validNumber(char *symbol, int length, bool dots_allowed){
  int index = 0;
  int dots = dots_allowed;
  if (symbol[0] == '-'){
    if (length == 1){
      return false;
    }
    index++;
  }
  while (symbol[index] != '\0'){
    if (symbol[index] == '.'){
      dots--;
    }
    else if (strchr(NUMBERS, symbol[index]) == NULL){
      return false;
    }
    index++;
  }
  if (dots >= 0){
    return true;
  }
  else{
    return false;
  }
}

int determineType(char *symbol, int length){

  if ((symbol[0] == '"') && (symbol[length] == '"')){
    return STR_TYPE;
  }
  if (symbol[0] == ';'){
    return NULL_TYPE;
  }
  if (validNumber(symbol, length, false)){
    return INT_TYPE;
  }
  if (validNumber(symbol, length, true)){
    return DOUBLE_TYPE;
  }
  if ((length == 2) && (symbol[0] == '#') && ((symbol[1] == 't') || (symbol[1] == 'f'))){
    return BOOL_TYPE;
  }
  return SYMBOL_TYPE;
}

Value *makeNewSymbol(int type, char *rawSymbol, int length){
  Value *output;
  switch(type){
    case STR_TYPE:
      output = makeString(rawSymbol, length);
      break;
    case INT_TYPE:
      output = makeInt(rawSymbol);
      break;
    case DOUBLE_TYPE:
      output = makeDouble(rawSymbol);
      break;
    case BOOL_TYPE:
      output = makeBool(rawSymbol);
      break;
    case SYMBOL_TYPE:
      output = makeSymbol(rawSymbol);
      break;
    default:
      output = makeNull();
  }
  return output;
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
        else if (nextChar == '('){
          tokens = cons(makeOpen(), tokens);
        }
        else if ((nextChar != '\n') && (nextChar != ' ')){
          char *currentRawSymbol;
          int *symbolLength = talloc(sizeof(int));
          currentRawSymbol = readMultiChar(nextChar, symbolLength);
          int type = determineType(currentRawSymbol, *symbolLength);
          Value *currentSymbol = makeNewSymbol(type, currentRawSymbol, *symbolLength);
          if (!isNull(currentSymbol)){
            tokens = cons(currentSymbol, tokens);
          }
        }
        nextChar = (char)fgetc(stdin);
      }
    Value *reversedList = reverse(tokens);
    return reversedList;
}


// Display the contents of the list of tokens, along with associated type information.
// The tokens are displayed one on each line, in the format specified in the instructions.
void displayTokens(Value *list){
  Value *head = car(list);
  if(!isNull(head)){
    switch (head->type) {
      case INT_TYPE:
        printf("%i:integer\n", head->i);
        break;
      case BOOL_TYPE:
        if (head->i == 1){
          printf("#t:boolean\n");
        }
        if (head->i == 0){
          printf("#f:boolean\n");
        }
        break;
      case OPEN_TYPE:
        printf("(:open\n");
        break;
      case CLOSE_TYPE:
        printf("):close\n");
        break;
      case STR_TYPE:
        printf("\"%s\":string\n", head->s);
        break;
      case DOUBLE_TYPE:
        printf("%f:double\n", head->d);
        break;
      case SYMBOL_TYPE:
        printf("%s:symbol\n", head->s);
        break;
      case ERROR_TYPE:
        printf("Syntax Error: %s\n", head->s);
        break;
      default:
        printf("WRONG TYPE\n");
        break;
    }
    return displayTokens(cdr(list));
  }
}
