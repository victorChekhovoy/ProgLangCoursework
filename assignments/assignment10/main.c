// Tester for linked list.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "linkedlist.h"
#include "value.h"

void testForward(Value *head, int correctLength) {

  Value *value = head;
  assert(CONS_TYPE == value->type);
  assert(DOUBLE_TYPE == car(value)->type);
  assert(1 == car(value)->d);

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(STR_TYPE == car(value)->type);
  assert(!strcmp("2.0s", car(value)->s));

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(STR_TYPE == car(value)->type);
  assert(!strcmp("3.0s", car(value)->s));

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(DOUBLE_TYPE == car(value)->type);
  assert(4 == car(value)->d);

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(STR_TYPE == car(value)->type);
  assert(!strcmp("5.0s", car(value)->s));

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(DOUBLE_TYPE == car(value)->type);
  assert(6 == car(value)->d);

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(INT_TYPE == car(value)->type);
  assert(7 == car(value)->i);

  value = cdr(value);
  assert(NULL_TYPE == value->type);

  assert(correctLength == length(head));
  assert(!isNull(head));
}

void testBackward(Value *head, int correctLength) {
  Value *value = head;

  assert(CONS_TYPE == value->type);
  assert(INT_TYPE == car(value)->type);
  assert(7 == car(value)->i);

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(DOUBLE_TYPE == car(value)->type);
  assert(6 == car(value)->d);

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(STR_TYPE == car(value)->type);
  assert(!strcmp("5.0s", car(value)->s));

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(DOUBLE_TYPE == car(value)->type);
  assert(4 == car(value)->d);

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(STR_TYPE == car(value)->type);
  assert(!strcmp("3.0s", car(value)->s));

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(STR_TYPE == car(value)->type);
  assert(!strcmp("2.0s", car(value)->s));

  value = cdr(value);
  assert(CONS_TYPE == value->type);
  assert(DOUBLE_TYPE == car(value)->type);
  assert(1 == car(value)->d);

  value = cdr(value);
  assert(NULL_TYPE == value->type);

  assert(correctLength == length(head));
  assert(!isNull(head));
}


int main() {

  // 1. Create a null value (representing an empty list)
  printf("Test 1...\n");
  Value *head = makeNull();
  int correctLength = 0;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");

  // 2. Reverse the empty list
  printf("Test 2...\n");
  Value *reverseLengthZero = reverse(head);
  assert(length(reverseLengthZero) == correctLength);
  cleanup(reverseLengthZero);
  printf("\tPASSED\n");

  // 3. Cons a new cell at the head of the list
  printf("Test 3...\n");
  Value *val1 = malloc(sizeof(Value));
  val1->type = INT_TYPE;
  val1->i = 7;
  head = cons(val1, head);
  correctLength++;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");

  // 4. Reverse this list of one element
  printf("Test 4...\n");
  Value *reverseLengthOne = reverse(head);
  assert(length(reverseLengthOne) == correctLength);
  cleanup(reverseLengthOne);
  printf("\tPASSED\n");

  // 5. Cons a new cell at the front of the list
  printf("Test 5...\n");
  Value *val2 = malloc(sizeof(Value));
  val2->type = DOUBLE_TYPE;
  val2->d = 6.00;
  head = cons(val2,head);
  correctLength++;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");

  // 6. Cons a new cell at the front of the list
  printf("Test 6...\n");
  Value *val3 = malloc(sizeof(Value));
  val3->type = STR_TYPE;
  char *text = "5.0s";
  val3->s = malloc(sizeof(char)*(strlen(text) + 1));
  strcpy(val3->s,text);
  head = cons(val3,head);
  correctLength++;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");

  // 7. Cons a new cell at the front of the list
  printf("Test 7...\n");
  Value *val4 = malloc(sizeof(Value));
  val4->type = DOUBLE_TYPE;
  val4->d = 4.00000;
  head = cons(val4,head);
  correctLength++;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");

  // 8. Cons a new cell at the front of the list
  printf("Test 8...\n");
  Value *val5 = malloc(sizeof(Value));
  val5->type = STR_TYPE;
  text = "3.0s";
  val5->s = malloc(sizeof(char)*(strlen(text) + 1));
  strcpy(val5->s,text);
  head = cons(val5,head);
  correctLength++;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");
  
  // 9. Cons a new cell at the front of the list
  printf("Test 9...\n");
  Value *val6 = malloc(sizeof(Value));
  val6->type = STR_TYPE;
  text = "2.0s";
  val6->s = malloc(sizeof(char)*(strlen(text) + 1));
  strcpy(val6->s,text);
  head = cons(val6,head);
  correctLength++;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");

  // 10. Cons a new cell at the front of the list
  printf("Test 10...\n");
  Value *val7 = malloc(sizeof(Value));
  val7->type = DOUBLE_TYPE;
  val7->d = 1.0;
  head = cons(val7,head);
  correctLength++;
  assert(length(head) == correctLength);
  printf("\tPASSED\n");

  // 11. Display the contents of the list
  printf("Test 11...\n");
  display(head);
  printf("\tPASSED\n");

  // 12. Confirm that everything is in the expected order
  printf("Test 12...\n");
  testForward(head, correctLength);
  printf("\tPASSED\n");

  // 13. Reverse the entire list and display the result
  printf("Test 13...\n");
  Value *rev = reverse(head);
  display(rev);
  printf("\tPASSED\n");

  // 14. Work through the reversed list to ensure it is in the expected order
  printf("Test 14...\n");
  testBackward(rev, correctLength);
  printf("\tPASSED\n");

  // 15. Free all the memory
  cleanup(head);
  cleanup(rev);
  printf("\tPASSED\n");

  head = NULL;
  rev = NULL;
}
