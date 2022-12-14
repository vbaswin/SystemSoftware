#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char str[10] = "1010";
    int locctr = (int)strtol(str, NULL, 2);
    printf("%d\n", locctr);

    strcpy(str, "4096");
    locctr = (int)strtol(str, NULL, 10);
    printf("%d\n", locctr);

    char new[20] = "EOF";
    int num = (int)strtol(new, NULL, 16);
    // int num = atoi(new);
    printf("%d\n", num);
    return 0;
}