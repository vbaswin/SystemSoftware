#include <stdio.h>

int *passByReference() {
    static int new[10] = {20, 30, 40};
    return new;
}

int main() {
    int *hi = passByReference();
    printf("%d\n", hi[0]);
    return 0;
}