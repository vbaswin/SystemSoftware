/*
    Program name: Relocatable loader
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

void convertHexToBinaryString(int hxBits, char binary[]) {
    int quo, rem, cur = 9;
    binary[10] = 0;
    for (int i = hxBits; i >= 2; i = quo) {
        quo = i / 2;
        rem = i % 2;
        binary[cur--] = rem + '0';
    }
    binary[cur--] = quo + '0';
    while (cur != -1)
        binary[cur--] = '0';
   return;
}

int main() {
    FILE *objectCode, *output;
    char code[100][20], binary[11], addressField[5];
    int hxbits, LOCCTR, bit, address, loadLocation;

    objectCode = fopen("objectCode.txt", "r+");
    output = fopen("output.txt", "w+");

    printf("Enter loading address: ");
    scanf("%X", &loadLocation);

    int len = parseSpTb(code, objectCode);
    LOCCTR = (int)strtol(code[2], NULL, 16);
    len = parseSpTb(code, objectCode);

    while (len && strcmp(code[0],"E")) {
        bit = 0;
        hxbits = (int)strtol(code[3], NULL, 16);
        convertHexToBinaryString(hxbits, binary);

        for (int i = 4; i < len; ++i) {
            if (binary[bit++] == '1') {
                for (int j = 0; j < 4; ++j)
                    addressField[j] = code[i][2 + j];
                address = (int)strtol(addressField, NULL, 16);
                address += loadLocation;
                fprintf(output, "%04X %c%c%04X\n", LOCCTR, code[i][0], code[i][1], address);
            }
            else
                fprintf(output, "%04X %s\n", LOCCTR, code[i]);
            LOCCTR += strlen(code[i])/2;
        }
        len = parseSpTb(code, objectCode);
    }


    fclose(objectCode);
    fclose(output);
}