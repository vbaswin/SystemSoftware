#include <stdio.h>
#include <string.h>

// struct data {
//     char code[10][10];
// }

int main() {
    FILE *intermediate = fopen("intermediate.txt", "r");

    char c;
    char code[10][10];

    for (int j = 0;j < 10; ++j) {
        strcpy(code[j],"");
    }

    int j =0; 
    int len = 0;
    memset(code[len],0,10);
    while((c=fgetc(intermediate)) != '\n') {
        printf("%c", c);
        if ((c == ' ' || c == '\t' )) {
            if (strlen(code[len]) > 0) {
                printf("%s\n", code[len]);
                ++len;
                memset(code[len],0,10);
            }
            else 
                continue;
        }
        strcat(code[len], &c);
    }
    printf("\n");
}