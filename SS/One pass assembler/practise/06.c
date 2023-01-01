#include <stdio.h>

void passByReference(int hi[]) {
    hi[0] = 100;
    hi[5] = 27;
}

int main() {
    int hi[10] = {22, 23, 24, 25, 26};
    passByReference(hi);
    for (int i = 0; i < 10; ++i)
        printf("%d ", hi[i]);
    printf("\n");
    return 0;
}