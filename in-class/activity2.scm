#include <stdlib.h>
#include <stdio.h>

struct LinkedList {
    struct LinkedList *next;
    int node;
};

typedef struct LinkedList LinkedList;

int main() {
    // TODO
    // 1. Create three LinkedList instances: l1, l2, l3.
    // 2. Set their respective node values to 10, 20, 30.
    // 3. Set l1's next to point to l2, l2's next to point to l3,
    //    and l3's next to point to NULL.
    // 4. Print out l1's node.
    // 5. Print out l2 and l3's nodes, BUT ONLY BY TRAVERSING
    //    .next LINKS TO REACH THE NODES!
}
