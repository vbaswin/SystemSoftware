/*
    Program name: Absolute Loader
    Authur name: Aswin V B
    Rollno: 222
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parseSpTb(char code[][20], FILE *source) {
    int j = 0, k = 0, sz = 0;
    char ch;
    while ((ch = fgetc(source)) != '\n' && ch != EOF) {
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
    FILE *objectCode, *output;
    char code[100][20];
    int LOCCTR, loadLocation;

    objectCode = fopen("objectCode.txt", "r+");
    output = fopen("output.txt", "w+");

    printf("Enter loading address: ");
    scanf("%X", &loadLocation);

    int len = parseSpTb(code, objectCode);
    LOCCTR = (int)strtol(code[2], NULL, 16);
    len = parseSpTb(code, objectCode);

    while (len && strcmp(code[0],"E")) {
        for (int i = 3; i < len; ++i) {
            fprintf(output, "%04X %s\n", LOCCTR, code[i]);
            LOCCTR += strlen(code[i])/2;
        }
        len = parseSpTb(code, objectCode);
    }


    fclose(objectCode);
    fclose(output);
}