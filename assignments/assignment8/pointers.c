/* Program demonstrating pointers in C. */

#include <stdio.h>

int main() {

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // IMPORTANT: DRAW THE EXECUTION OUT BEFORE YOU RUN!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int a = 5;
    int b = 6;

    // p and q are pointers to a and b, respectively
    int *p = &a;  // p is p, not *p; p's type is "int *"
    int *q = &b;  // Likewise with q

    // pp is a pointer to p
    int **pp = &p;  // int ** indicates "int pointer to int pointer"

    // Now the fun starts
    *q += *p;
    (**pp)++;  // (**pp) means "dereference(dereference(pp))"
    q = p;  // Note the lack of asterisks here
    *pp = &b;
    *p += 3;
    *q *= 2;


    if (&b > &a){
      printf("higher\n");
    }
    else {
      printf("lower\n");
    }

    // What will this print? Draw before you run it!
    printf("Final values of a and b: %i, %i\n", a, b);
}
