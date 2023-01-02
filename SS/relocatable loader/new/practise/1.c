#include <stdio.h>

char *passByReference() {
    static char new[50] = "akshay";
    return new;
}

int main() {
    char *hi = passByReference();
    printf("%s\n", hi);
    return 0;
}