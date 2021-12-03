#include <stdlib.h>
#include <stdio.h>

struct LinkedList {
    struct LinkedList *next;
    int node;
};

typedef struct LinkedList LinkedList;

int main() {
    LinkedList *l1 = (LinkedList *)malloc(sizeof(LinkedList));
    LinkedList *l2 = (LinkedList *)malloc(sizeof(LinkedList));
    LinkedList *l3 = (LinkedList *)malloc(sizeof(LinkedList));
    l1->node = 10;
    l2->node = 20;
    l3->node = 30;

    l1->next = l2;
    l2->next = l3;
    l3->next = NULL;

    printf("%i\n", l1->node);
    printf("%i\n", l1->next->node);
    printf("%i\n", l1->next->next->node);

    // TODO
    // 1. Create three LinkedList instances: l1, l2, l3.
    // 2. Set their respective node values to 10, 20, 30.
    // 3. Set l1's next to point to l2, l2's next to point to l3,
    //    and l3's next to point to NULL.
    // 4. Print out l1's node.
    // 5. Print out l2 and l3's nodes, BUT ONLY BY TRAVERSING
    //    .next LINKS TO REACH THE NODES!
}
