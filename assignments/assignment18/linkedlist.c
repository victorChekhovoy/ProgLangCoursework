#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"
#include <string.h>
#include <assert.h>
#include "talloc.h"



// Create a pointer to a new NULL_TYPE Value (hint: where in memory will
// the value have to live?)
Value *makeNull(){
  Value *node = (Value *) talloc(sizeof(Value));
  node->type = NULL_TYPE;
  return node;
}
// Return whether the given pointer points at a NULL_TYPE Value. Use assertions
// to make sure that this is a legitimate operation. See the assignment
// instructions for further explanation on assertions.
bool isNull(Value *value){
  assert(value != NULL);
  return value->type == NULL_TYPE;
}
// Create a pointer to a new CONS_TYPE Value
Value *cons(Value *newCar, Value *newCdr){
  Value *node = (Value *) talloc(sizeof(Value));
  node->type = CONS_TYPE;
  (node->c).car = newCar;
  (node->c).cdr = newCdr;
  return node;
}
// Return a pointer to the car value for the cons cell at the head of the given
// linked list. Use assertions here to make sure that this is a legitimate operation
// (e.g., there is no car value at the head of an empty list). See the assignment
// instructions for further explanation.
Value *car(Value *list){
  assert(list != NULL);
  if (list->type == CONS_TYPE){
    return list->c.car;
  }
  else {
    return makeNull();
  }
}
// Return a pointer to the cdr value for the cons cell at the head of the given linked
// list. Again use assertions to make sure that this is a legitimate operation.
Value *cdr(Value *list){
  assert(list != NULL);
  if (list->type == CONS_TYPE){
    return list->c.cdr;
  }
  else {
    return makeNull();
  }
}

// Display the contents of the linked list to the screen in the
// format of a Scheme list -- e.g., ( 33 "lol" 9.9 ). It's okay
// to just use printf here, though you'll have to add the quotes in
// yourself, for strings.
void display_inner(Value* list){
  switch (list->type) {
      case INT_TYPE:
          printf("%i ", list->i);
          break;
      case DOUBLE_TYPE:
          printf("%g ", list->d);
          break;
      case STR_TYPE:
          printf("\"%s\" ", list->s);
          break;
      case CONS_TYPE:
          display_inner(list->c.car);
          display_inner(list->c.cdr);
          break;
      case NULL_TYPE:
          //printf("NULL\n");
          break;
      case PTR_TYPE:
          printf("<%p> ", list->p);
          break;
      case OPEN_TYPE:
          printf("(");
          break;
      case CLOSE_TYPE:
          printf(")");
          break;
      case BOOL_TYPE:
          if (list->i == 1){
            printf("#t ");
          }
          else{
            printf("#f ");
          }
          break;
      case SYMBOL_TYPE:
          printf("%s ", list->s);
          break;
      case ERROR_TYPE:
          printf("%s ", list->s);
          break;
      default:
          break;
  }
}


void display(Value *list){
  printf("(");
  display_inner(list);
  printf(")\n");
}


// Return a new list that is the reverse of the one that is passed in. None of
// the values in the original linked list should be copied this time. Instead,
// create a new linked list of CONS_TYPE nodes whose car values point to the
// corresponding car values in the original list.
Value *reverseHelper(Value *list, Value *reversed_list){
  if (isNull(list)) {
    return reversed_list;
  }
  else {
      Value *current_car = list->c.car;
      Value *new_car = (Value *) talloc(sizeof(Value));
      int current_type = current_car->type;
      new_car->type = current_type;
      if ((current_type == INT_TYPE) || (current_type == BOOL_TYPE)){
        int new_car_value = current_car->i;
        new_car->i = new_car_value;
      } else if (current_type == DOUBLE_TYPE){
        double new_car_value = current_car->d;
        new_car->d = new_car_value;
      } else if ((current_type == STR_TYPE) || (current_type == SYMBOL_TYPE) || (current_type == ERROR_TYPE)){
        char *new_car_value = (char *) talloc(sizeof(char) * (strlen(current_car->s) + 1));
        strcpy(new_car_value, current_car->s);
        new_car->s = new_car_value;
      } else if (current_type == CONS_TYPE){
        new_car->c.car = current_car->c.car;
        new_car->c.cdr = current_car->c.cdr;
      } else if (current_type == PRIMITIVE_TYPE){
        new_car->cl = current_car->cl;
      }
      reversed_list = cons(new_car, reversed_list);
      Value *remainder = cdr(list);
      Value *output = reverseHelper(remainder, reversed_list);
      return output;
    }
}

// Reverses a linked list
Value *reverse(Value *list){
  Value *reversed_list = makeNull();
  reversed_list = reverseHelper(list, reversed_list);
  return reversed_list;

}
// Return the length of the given list, i.e., the number of cons cells.
// Use assertions to make sure that this is a legitimate operation.
int length(Value *value){
  assert(value != NULL);
  if (isNull(value)){
    return 0;
  }
  int output_length = 1 + length(cdr(value));
  return output_length;
}
