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
#define INITIALS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!$%&*/:<=>?~_^\"\'"
#define SUBSEQUENT " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!$%&*/:<=>?~_^1234567890.+-;,"

// peek at the next character and returns the next character
char peek(){
  char output = (char) fgetc(stdin);
  if (output != EOF){
    ungetc(output, stdin);
  }
  return output;
}

// makes a new token of OPEN_TYPE
Value *makeOpen(){
  Value *newOpen = (Value *)talloc(sizeof(Value));
  newOpen->type = OPEN_TYPE;
  return newOpen;
}

// makes a new token of CLOSED_TYPE
Value *makeClosed(){
  Value *newClosed = (Value *)talloc(sizeof(Value));
  newClosed->type = CLOSE_TYPE;
  return newClosed;
}

// makes a new token of BOOL_TYPE
Value *makeBool(char *value){
  Value *newBool = (Value *)talloc(sizeof(Value));
  newBool->type = BOOL_TYPE;
  if (value[1] == 't'){
    newBool->i = 1;
  }
  if (value[1] == 'f'){
    newBool->i = 0;
  }
  return newBool;
}

// makes a new token of SYMBOL_TYPE
Value *makeSymbol(char *value){
  Value *newSymbol = talloc(sizeof(Value));
  newSymbol->type = SYMBOL_TYPE;
  newSymbol->s = value;
  return newSymbol;
}

// makes a new token of INT_TYPE
Value *makeInt(char *value){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  int numericValue = strtol(value, NULL, 10);
  newSymbol->type = INT_TYPE;
  newSymbol->i = numericValue;
  return newSymbol;
}

// makes a new token of DOUBLE_TYPE
Value *makeDouble(char *value){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  double numericValue = 0.0;
  numericValue = strtod(value, NULL);
  newSymbol->d = numericValue;
  newSymbol->type = DOUBLE_TYPE;
  return newSymbol;
}

// makes a new token of STR_TYPE
Value *makeString(char *rawValue, int length){
  Value *newSymbol = (Value *)talloc(sizeof(Value));
  newSymbol->type = STR_TYPE;
  newSymbol->s = talloc(sizeof(char)*(length+1));
  for (int i = 0; i < length + 1; i++){
    newSymbol->s[i] = '\0';
  }
  for (int i = 1; i < length; i++){
    newSymbol->s[i-1] = rawValue[i];
  }
  newSymbol->s[length] = '\0';
  return newSymbol;
}

// makes a new token of ERROR_TYPE
Value *makeError(char *errorMessage){
  Value *newSymbol = (Value *) talloc(sizeof(Value));
  newSymbol->type = ERROR_TYPE;
  newSymbol->s = errorMessage;
  return newSymbol;
}

// takes in a pointer to the length of the string and reads through until it finds a
// terminating character, then returns the whole string
char *readString(int *length){
  char *output = talloc(MAX_STR_LEN);
  for (int i = 0; i < MAX_STR_LEN; i++){
    output[i] = '\0';
  }
  char current = (char) fgetc(stdin);
  *length = 0;
  output[(*length)++] = '"';
  while (current != EOF && current != '"' && current != '\n'){
    output[(*length)++] = current;
    current = (char) fgetc(stdin);
  }
  output[*length] = '"';
  return output;
}

// reads through until the end of the comment and returns it
char *readComment(){
  char *output = talloc(MAX_STR_LEN);
  for (int i = 0; i < MAX_STR_LEN; i++){
    output[i] = '\0';
  }
  char current = (char) fgetc(stdin);
  int index = 0;
  output[index++] = ';';
  while (current != EOF && current != '\n'){
    output[index] = current;
    index++;
    current = (char) fgetc(stdin);
  }
  return output;
}

// checks if a number is a valid int or double
bool validNumber(char *symbol, int length, bool dots_allowed){
  int dots = dots_allowed;
  int i = 0;
  if (symbol[i] == '-'){
    if (length == 1){
      return false;
    }
    i++;
  }
  for (; i < length; i++){
    if (symbol[i] == '.'){
      dots--;
    }
    else if (strchr(NUMBERS, symbol[i]) == NULL){
      return false;
    }
  }
  if (dots >= 0){
    return true;
  }
  else{
    return false;
  }
}

// performs of a number of checks to see if there is invalid syntax
char *errorCheck(char *input, int length){
  if ((input[0] == '#') && ((input[1] == 'f') || (input[1] == 't'))){
    return NULL;
  }
  if ((input[0] == '#') && ((input[1] != 'f') || (input[1] != 't'))){
    return "Boolean was not #t or #f";
  }
  if ((length == 1) && ((input[0] == '+') || (input[0] == '-'))){
    return NULL;
  }
  if (validNumber(input, length, true) || validNumber(input, length, false)){
    return NULL;
  }
  if (strchr(INITIALS, input[0]) == NULL){
    char *errorOutput = talloc(MAX_STR_LEN + 50);
    for (int i = 0; i < MAX_STR_LEN + 50; i++){
      errorOutput[i] = '\0';
    }
    char *errorMessage = "Illegal initial symbol for token <";
    strcat(errorOutput, errorMessage);
    int i;
    for (i = 0; i < length; i++){
      errorOutput[i+34] = input[i];
    }
    errorOutput[i+34] = '>';
    return errorOutput;
  }
  for (int i = 1; i < length; i++){
    if (strchr(SUBSEQUENT, input[i]) == NULL){
      char *errorOutput = talloc(MAX_STR_LEN + 50);
      for (int i = 0; i < MAX_STR_LEN + 50; i++){
        errorOutput[i] = '\0';
      }
      char *errorMessage = "Illegal subsequent symbol <";
      strcat(errorOutput, errorMessage);
      errorOutput[27] = input[i];
      char *errorMessageCont = "> in token ";
      strcat(errorOutput, errorMessageCont);
      int i;
      for (i = 0; i < length; i++){
        errorOutput[i+40] = input[i];
      }
      return errorOutput;
    }
  }
  return NULL;
}

// reads a multi-character symbol from the input file
char *readMultiChar(char currentChar, int *index){
  char *output = NULL;
  if (currentChar == '"'){
    output = readString(index);
  }
  else if (currentChar == ';'){
    output = readComment();
  }
  else{
    output = talloc(MAX_STR_LEN);
    for (int i = 0; i < MAX_STR_LEN; i++){
      output[i] = '\0';
    }
    *index = 0;
    while (currentChar != EOF && strchr(TERMINATORS, currentChar) == NULL){
      output[*index] = currentChar;
      (*index)++;
      currentChar = (char) peek();
      if ((currentChar != ')') && (currentChar != '(')){
        currentChar = (char) fgetc(stdin);
      }
    }
  }
  return output;
}

//determines the type of the symbol, given the raw string input
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

// creates a new symbol, given its type and raw string value
Value *makeNewSymbol(int type, char *rawSymbol, int length){
  Value *output = NULL;
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
          char *currentRawSymbol = NULL;
          int *symbolLength = talloc(sizeof(int));
          *symbolLength = 0;
          currentRawSymbol = readMultiChar(nextChar, symbolLength);
          int type = determineType(currentRawSymbol, *symbolLength);
          Value *currentSymbol = makeNewSymbol(type, currentRawSymbol, *symbolLength);
          if (!isNull(currentSymbol)){
            char *errorMessage = errorCheck(currentRawSymbol, *symbolLength);
            if (errorMessage != NULL){
              tokens = cons(makeError(errorMessage), makeNull());
            }
            else{
              tokens = cons(currentSymbol, tokens);
            }
          }
        }
        nextChar = (char)fgetc(stdin);
        if (car(tokens)->type == ERROR_TYPE){
          nextChar = EOF;
        }
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
