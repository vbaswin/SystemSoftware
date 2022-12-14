#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int convertTobinary(int rBits) {
    int quo, rem, binary = 0, place = 0, i;
    char strBit[20];
    int len = 0;
    for (i = rBits; i >= 2; i = quo) {
        quo = i / 2;
        rem = i % 2;
        strBit[len++] = '0' + rem;
    }
    strBit[len++] = '0' + rem;
    char tmp;
    /*
        for (int i = 0; i < len/2; ++i) {
            tmp = strBit[i];
            strBit[i] = strbit[len -1 -i];
            strBit[len -1 -i] = tmp;
        }
    */
   printf("binary: %s\n", strBit);
   return 0;
}

void inputFile(FILE *input, FIle *output) {
    printf("Enter load address: ");
    int start_addr;
    scafn("%X", &start_addr);
    char ch, head[20], name[20];
    int num, start, p_len;
    fscanf(input, "%s %s %X %X", head, name, &start, &p_len);
    start += start_addr;
    int len = start;

    int cur = 2;
    while ((ch = getc(input)) != EOF) {
        if (ch == ' ' || ch == '\n')
            continue;
        if (ch == 'E')
            break;
        if (ch == 'T') {
            char startAddr[20], lenght[20];
            int rBitsHex, rbitsBianary;
            fscanf(input, "%s %s %X", startAddr, length, &rBitsHex);
            rBitsBianry = convertTobinary(rBitsHex);
        }
        if (cur == 2) {
            if (len != start){
                fprintf(output, "\n");
            }
            fprintf(output, "%x\t", len);
            cur = 0;
            len += 1;
        }
        if (ch == ' ' || ch == '\n') 
            continue;
        fprintf(output, "%c", ch);
        ++cur;
    }
}

int main() {
    FILE *hi, *out;
    hi = fopen("input.txt", "r+");
    out = fopen("output.txt", "w+");

    char name[100];
    rewind(hi);
    inputFile(hi, out);
    fclose(hi);
    fclose(out);

    return 0;
}