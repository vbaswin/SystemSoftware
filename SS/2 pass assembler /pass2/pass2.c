/*
    Program name: Pass 2 of 2 pass assembler
    Authur name: Aswin V B
    Rollno: 222
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int parseSpTb (char code[][20], FILE *source) {
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

int searchSymtab(FILE *SYMTAB, char key[]) {
    rewind(SYMTAB);
    char label[20];
    int address;
    while (fscanf(SYMTAB, "%s %04X", label, &address) != EOF) {
        if (!strcmp(key, label))
            return address;
    }
    return -1;
}

int searchOptab(FILE *OPTAB, char key[]) {
    rewind(OPTAB);
    char op[20];
    int opcode;
    while (fscanf(OPTAB, "%s %X", op, &opcode) != EOF) {
        if (!strcmp(op, key)) {
            return opcode;
        }
    }
    return -1;
}

void writeTextRecord(FILE *objectCode, char tRecord[][20], int tStart, int tCur, int tLen) {
    fprintf(objectCode, "T‸%06X‸%02X", tStart, tLen/2);
    for (int i = 0; i < tCur; ++i)
        fprintf(objectCode, "‸%s", tRecord[i]);
    fprintf(objectCode, "\n");
}

void resetTextRecord(char tRecord[][20], int *tCur) {
    for (int i = 0;i < 20; ++i) {
        memset(tRecord[i], 0, 20);
    }
    *tCur = 0;
}

void storeTextData(FILE *objectCode, char tRecord[][20], int *tCur, int *tStart, int *tLen, char code[100][20], int last) {
    char temp[30];
    if (!last)
        --*tCur;
    memset(temp, 0 , 30);
    memcpy(temp, tRecord[*tCur], strlen(tRecord[*tCur]));
    writeTextRecord(objectCode, tRecord, *tStart, *tCur, *tLen-strlen(tRecord[*tCur]));
    resetTextRecord(tRecord, &*tCur);
    memcpy(tRecord[(*tCur)++], temp, strlen(temp)+1);
    *tStart = strtol(code[0], NULL, 16);
    *tLen = strlen(temp);

}
int main() {
    FILE  *OPTAB, *SYMTAB, *intermediate, *infoSave, *objectCode;
    int startingAddress, current, len, programLen, tStartAddr, opcode, tCur = 0, tStart, address, tLen = 0;
    char code[100][20], temp[30], tRecord[20][20], addressString[10], previous[30];

    intermediate = fopen("intermediate.txt", "r+");
    OPTAB = fopen("optab.txt", "r+");
    SYMTAB = fopen("symtab.txt", "r+");
    infoSave = fopen("infoSave.txt", "r+");
    objectCode = fopen("objectCode.txt", "w+");

    if (!intermediate)  {
        printf("Error opening file\n");
        return 0;
    }

    len = parseSpTb(code, intermediate);
    char name[10];
    startingAddress = 0;
    if (!strcmp(code[2], "START")) {
        if (strcmp(code[1],"-"))
            strcpy(name,code[1]);
        // convert string to base 16
        startingAddress = (int)strtol(code[3], NULL, 16);
        len = parseSpTb(code, intermediate);
    }
    tStartAddr = (int)strtol(code[0],NULL, 16);
    resetTextRecord(tRecord, &tCur);

    fscanf(infoSave, "%s %X", temp, &programLen);
    fprintf(objectCode, "H‸%-6s‸%06X‸%06X\n", name, startingAddress, programLen);

    tStart = startingAddress;

    while (strcmp(code[2], "END")) {
        if ((opcode = searchOptab(OPTAB, code[2])) != -1) {
            if (strcmp(code[3],"-")) {
                if ((address = searchSymtab(SYMTAB, code[3])) != -1) {
                    sprintf(addressString, "%04X", address);
                }else
                    memcpy(addressString, "0000\0",5);
            } else {
                memcpy(addressString, "0000\0", 5);
            }
            sprintf(tRecord[tCur], "%02X%s",opcode,addressString);
            tLen += strlen(tRecord[tCur++]);
        } else if (!strcmp(code[2], "BYTE")) {
            if (code[3][0] == 'C') {
                char byteAscii[100] = {0};
                char asciiCode[3], ch;
                for (int i = 1; i < strlen(code[3]); ++i) {
                    ch = code[3][i];
                    if (ch == '\'')
                        continue;
                    sprintf(asciiCode, "%0X", ch);
                    strcat(byteAscii, asciiCode);
                }
                memcpy(tRecord[tCur], byteAscii, strlen(byteAscii));
            } else if (code[3][0] == 'X') {
                char new[10], ch;
                memset(new, 0, 10);
                int j = 0;
                for (int i = 1; i < strlen(code[3]); ++i) {
                    ch = code[3][i];
                    if (ch == '\'')
                        continue;
                    new[j++] = ch;
                }
                memcpy(tRecord[tCur], new, strlen(new));
            }
            tLen += strlen(tRecord[tCur++]);
        } else if (!strcmp(code[2], "WORD")) {
            char word[50];
            int val = (int)strtol(code[3], NULL, 10);
            sprintf(word,"%06X", val);
            memcpy(tRecord[tCur], word, strlen(word));
            tLen += strlen(tRecord[tCur++]);
        }
        if (tLen > 60)
            storeTextData(objectCode, tRecord, &tCur, &tStart, &tLen, code, 0);
        len = parseSpTb(code, intermediate);
    }
    if (tLen)
        storeTextData(objectCode, tRecord, &tCur, &tStart, &tLen, code, 1);
    fprintf(objectCode, "E‸%06X", startingAddress);

    fclose(OPTAB);
    fclose(SYMTAB);
    fclose(intermediate);
    fclose(infoSave);
    return 0;
}