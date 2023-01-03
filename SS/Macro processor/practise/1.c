#include <stdio.h>

// C does not support reference data types
// void passByReference(int &new) {
    // new = 22;
// }

// we can still simulate passing-by-reference by explicitly passing pointer values
void passByPointer(int *new) {
    *new = 22;
}


int main() {
    int hi = 10;
    // passByReference(hi);
    passByPointer(&hi);
    printf("%d\n", hi);
}