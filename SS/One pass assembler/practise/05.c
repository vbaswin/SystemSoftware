#include <stdio.h>
#include <string.h>

int parseSpTb(char code[][20], FILE *source) {
    int j = 0, k = 0, sz = 0;
    char ch;
    printf("\n");
    while ((ch = fgetc(source)) != '\n' && ch != EOF) {
        printf("%c ", ch);
        if(ch == ' ' || ch == '\t'){
            if (sz) {
                sz = 0;
                code[j][k] = 0;
                k = 0;
                ++j;
            } else
                continue;
        } else {
            sz = 1;
            code[j][k++] = ch;
        }
    }
    if (!sz)
        return j;
    code[j][k] = 0;
    return j+1;
}

int main() {
    FILE *objectCode = fopen("objectCode.txt", "r+");
    char code[100][20];

    int len = parseSpTb(code, objectCode);
    printf("\nlen: %d\n", len);
    for (int i = 0; i < len; ++i)
        printf("%s ", code[i]);
    printf("\n");

    while (strcmp(code[0], "E") && len) {
        // cur = (int)strtol(code[1],NULL,16);
        // textSz = (int)strtol(code[2],NULL,16);
        // max = cur + textSz;
        // printf("%X\n", max);

        len = parseSpTb(code, objectCode);
        printf("\nlen: %d\n", len);
        for (int i = 0; i < len; ++i)
            printf("%s ", code[i]);
        printf("\n");
    }
    return 0;
}