#include <stdio.h>
#include <string.h>

int main() {
    char name[20];
    int roll = 20;
    snprintf(name, 3,"%d", roll);
    printf("%s\n", name);
}