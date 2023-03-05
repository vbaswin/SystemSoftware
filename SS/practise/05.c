#include <stdio.h>


int parseSpTb(FILE *intermediate,char code[10][100]) {
    int j = 0, k = 0, sz = 0;
    char ch;
    while ((ch = fgetc(intermediate)) != '\n' && ch != EOF) {
        if (ch == ' ' || ch == '\t' ) {
            if (sz) {
                code[j][k] = 0;
                sz = 0;
                k = 0;
                ++j;
            } else
                continue;
        } else {
            code[j][k++] = ch;
            sz =1;
        }
    }
    if (sz) {
        code[j][k] = 0;
        return ++j;
    } else
        return j;
    return 0;
}

int main() {
    char code[10][100];
    FILE *intermediate = fopen("intermediate.txt","r+");

    int len = parseSpTb(intermediate, code);
    while (len) {
        for (int i = 0; i < len; ++i)
            printf("%s ",code[i]);
        printf("\n");
        len = parseSpTb(intermediate, code);
    }

    fclose(intermediate);

    return 0;
}