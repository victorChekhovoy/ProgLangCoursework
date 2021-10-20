#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"
#include <string.h>
#include <assert.h>
// Create a new NULL_TYPE value node.
Value *makeNull(){
  Value *node = malloc(sizeof(Value));
  node->type = NULL_TYPE;
  return node;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr){
  Value *node = malloc(sizeof(Value));
  node->type = CONS_TYPE;
  (node->c).car = newCar;
  (node->c).cdr = newCdr;
  return node;
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list){
  switch (list->type) {
      case INT_TYPE:
          printf("%i -> ", list->i);
          break;
      case DOUBLE_TYPE:
          printf("%g -> ", list->d);
          break;
      case STR_TYPE:
          printf("%s -> ", list->s);
          break;
      case CONS_TYPE:
          display(list->c.car);
          display(list->c.cdr);
          break;
      case NULL_TYPE:
          printf("NULL\n");
          break;
  }
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory whatsoever between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
//
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.




Value *reverseHelper(Value *list, Value *reversed_list){
  if (isNull(list)) {
    return reversed_list;
  }
  else {
      Value *current_car = list->c.car;
      Value *new_car = malloc(sizeof(Value));
      int current_type = current_car->type;
      new_car->type = current_type;
      if (current_type == INT_TYPE){
        int new_car_value = current_car->i;
        new_car->i = new_car_value;
      }
      if (current_type == DOUBLE_TYPE){
        double new_car_value = current_car->d;
        new_car->d = new_car_value;
      }
      if (current_type == STR_TYPE){
        char *new_car_value = malloc(sizeof(char) * (strlen(current_car->s) + 1));
        strcpy(new_car_value, current_car->s);
        new_car->s = new_car_value;
      }
      reversed_list = cons(new_car, reversed_list);
      Value *remainder = cdr(list);
      Value *output = reverseHelper(remainder, reversed_list);
      return output;
    }
}

Value *reverse(Value *list){
  Value *reversed_list = makeNull();
  reversed_list = reverseHelper(list, reversed_list);
  return reversed_list;

}

// Frees up all memory directly or indirectly referred to by list. This includes strings.
//
// FAQ: What if a string being pointed to is a string literal? That throws an
// error when freeing.
//
// ANS: Don't put a string literal into the list in the first place. All strings
// added to this list should be able to be freed by the cleanup function.
//
// FAQ: What if there are nested lists inside that list?
//
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
void cleanup(Value *list){
  if (!isNull(list)){
    if (car(list)->type == STR_TYPE){
      free(car(list)->s);
    }
    free(car(list));
    cleanup(cdr(list));
    free(list);
  }
  else{
    free(list);
  }
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list){
  assert(list != NULL);
  if (list->type == CONS_TYPE){
    return list->c.car;
  }
  else {
    return makeNull();
  }
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list){
  assert(list != NULL);
  if (list->type == CONS_TYPE){
    return list->c.cdr;
  }
  else {
    return makeNull();
  }
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value){
  assert(value != NULL);
  return value->type == NULL_TYPE;
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value){
  assert(value != NULL);
  Value* current_car = car(value);
  Value* current_cdr = cdr(value);
  if (isNull(current_car)){
    free(current_car);
    free(current_cdr);
    return 0;
  }
  int output_length = 1 + length(current_cdr);
  return output_length;
}
