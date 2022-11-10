#include <stdio.h>
#include <string.h>

int main() {
    FILE *intermediate = fopen("intermediate.txt", "r");

    char c;
    char code[10][10];

    for (int j = 0;j < 10; ++j) {
        strcpy(code[j],"");
    }

    int j =0; 
    int len = 0;
    while((c=fgetc(intermediate)) != '\n') {
        printf("%c ", c);
        if (c == ' ' || c == '\t') 
            ++len;
        strcat(code[len], &c);
        printf("%s\n", code[len]);
    }
}