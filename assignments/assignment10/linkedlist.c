#include <stdbool.h>
#include "value.h"

#ifndef _LINKEDLIST
#define _LINKEDLIST

typedef enum {CONS_TYPE, INT_TYPE, DOUBLE_TYPE, STR_TYPE, NULL_TYPE} valueType;

struct Value {
    valueType type;
    union {
        int i;
        double d;
        char *s;
        struct ConsCell {
            struct Value *car;
            struct Value *cdr;
        } c;
    };
};

typedef struct Value Value;

// Create a new NULL_TYPE value node.
Value *makeNull(){
  Value node;
  node.type = NULL_TYPE;
  return node;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr){
  Value node;
  node.type = CONS_TYPE;
  node.c.car = newCar;
  node.c.cdr = newCdr;
  return node;
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list){
  switch (list->type) {
      case INT_TYPE:
          printf("%i\n", list->i);
          break;
      case DOUBLE_TYPE:
          printf("%g\n", list->d);
          break;
      case STR_TYPE:
          printf("%s\n", list->s);
          break;
      case CONS_TYPE:
          display(list->c.car);
          display(list->c.cdr);
          break;
      case NULL_TYPE:
          printf("NULL\n");
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
Value *reverse(Value *list){
  // base case, node is null -> return null node
  // last node left (cdr is null) -> true base case

  // recursive case, take the car and have it point to the recursive call
  // on the cdr
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
void cleanup(Value *list);

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list);

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list);

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value);

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value);


#endif
