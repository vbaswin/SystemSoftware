#include <stdio.h>
#include <string.h>

void chumma(char code[10][20]) {
    printf("%s\n", code[0]);
    strcpy(code[0], "akshay");
}

int main() {
    char code[10][20] = {"aswin"};
    chumma(code);
    printf("%s", code[0]);
}