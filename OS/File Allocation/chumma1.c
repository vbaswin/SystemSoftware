#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(0));
    while (1) {
        printf("%d", time(0));
        int c = rand() % 21;
        printf("%d\nEnter choice: ", c);
        int choice;
        scanf("%d", &choice);
        if (choice == 2)
            break;
    }
    return 0;
}