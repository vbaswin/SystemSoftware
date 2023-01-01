#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct nxtfrwd {
    char address[20];
    struct nxtfrwd *link;
} nfrwd;

typedef struct symtab {
    char labelName[20];
    char address[20];
    int present;
    nfrwd *link;
    struct symtab *next;
} sym;

int parseSpTb(char code[][20], FILE *source);
void findAndReplace(sym ***SYMTAB, char find[20], char replace[20], FILE ***objectCode, char tRecord[][20], int tCur, int tStart, int addSize[][15], int row);
void saveSym(sym **SYMTAB, char label[], int LOCCTR, int forward, FILE **objectCode, char tRecord[][20], int tCur, int tStart, int addSize[][15], int row);
int searchSymtab(sym *SYMTAB, char key[]);
int searchOptab(FILE *OPTAB, char key[]);
int bytecondition(char str[]);
void writeTextRecord(FILE *objectCode, char tRecord[][20], int tStart, int tCur, int tLen);
void resetTextRecord(char tRecord[][20], int *tCur);
void resetAddSize(int addSize[][15]);
void storeTextData(FILE *objectCode, char tRecord[][20], int *tCur, int *tStart, int current, int *tLen, int last);
void writeHeaderEndRecord(FILE **objectCode, char name[], int startingAddress, int LOCCTR);

int main() {
    FILE  *source, *OPTAB, *objectCode;
    int LOCCTR, startingAddress, current, len, programLen, tStartAddr, opcode, tCur = 0, tStart, address, tLen = 0, addSize[10][15], row = 0;

    char code[100][20], temp[30], tRecord[20][20], addressString[10], previous[30], data[200];
    sym *SYMTAB = NULL;

    source = fopen("source.txt", "r+");
    OPTAB = fopen("optab.txt", "r+");
    objectCode = fopen("objectCode.txt", "w+");

    if (!source)  {
        printf("Error opening file\n");
        return 0;
    }
    len = parseSpTb(code, source);

    char name[10] = "";
    if (!strcmp(code[1], "START")) {
        if (strcmp(code[1],"-"))
            strcpy(name,code[1]);
        LOCCTR = (int)strtol(code[2], NULL, 16);
        len = parseSpTb(code, source);
    } else
        LOCCTR = 0;
    startingAddress = LOCCTR;
    tStart = startingAddress;
    resetAddSize(addSize);
    while (strcmp(code[1], "END")) {
        current = LOCCTR;
        if (strcmp(code[0], "**")) {
            if (strcmp(code[0], "-")) {
                if (searchSymtab(SYMTAB, code[0]) == -1)
                    saveSym(&SYMTAB, code[0], LOCCTR, 0, &objectCode, tRecord, tCur, tStart, addSize, row);
            }
            if ((opcode = searchOptab(OPTAB, code[1])) != -1) {
                if (strcmp(code[2],"-") && strcmp(code[2],"BUFFER,X")) {
                    if ((address = searchSymtab(SYMTAB, code[2])) != -1) {
                        sprintf(addressString, "%04X", address);
                    }else {
                        saveSym(&SYMTAB, code[2], LOCCTR, 1, &objectCode, tRecord, tCur, tStart, addSize, row);
                        memcpy(addressString, "0000\0", 5);
                    }
                } else {
                    memcpy(addressString, "0000\0", 5);
                }
                sprintf(tRecord[tCur], "%02X%s",opcode,addressString);
                tLen += strlen(tRecord[tCur]);
                addSize[row][tCur++] += 3;
                LOCCTR += 3;
            }
            else if (!strcmp(code[1], "WORD")) {
                char word[50];
                int val = (int)strtol(code[2], NULL, 10);
                sprintf(word,"%06X", val);
                memcpy(tRecord[tCur], word, strlen(word));
                tLen += strlen(tRecord[tCur]);
                LOCCTR += 3;
                addSize[row][tCur++] += 3;
            }
            else if (!strcmp(code[1], "RESW")) {
                addSize[row][tCur] += (3 * atoi(code[2]));
                LOCCTR += (3 * atoi(code[2]));
            }
            else if (!strcmp(code[1], "RESB")) {
                addSize[row][tCur] += atoi(code[2]);
                LOCCTR += atoi(code[2]);
            }
            else if (!strcmp(code[1], "BYTE")) {
                if (code[2][0] == 'C') {
                    char byteAscii[100] = {0};
                    char asciiCode[3], ch;
                    for (int i = 1; i < strlen(code[2]); ++i) {
                        ch = code[2][i];
                        if (ch == '\'')
                            continue;
                        sprintf(asciiCode, "%0X", ch);
                        strcat(byteAscii, asciiCode);
                    }
                    memcpy(tRecord[tCur], byteAscii, strlen(byteAscii));
                    addSize[row][tCur] += strlen(byteAscii)/2;
                    LOCCTR += strlen(byteAscii)/2;
                } else if (code[2][0] == 'X') {
                    char new[10], ch;
                    memset(new, 0, 10);
                    int j = 0;
                    for (int i = 1; i < strlen(code[2]); ++i) {
                        ch = code[2][i];
                        if (ch == '\'')
                            continue;
                        new[j++] = ch;
                    }
                    memcpy(tRecord[tCur], new, strlen(new));
                    addSize[row][tCur] += strlen(new)/2;
                    LOCCTR += strlen(new)/2;
                }
                tLen += strlen(tRecord[tCur++]);
            }
            else
                printf("Error in opcode!\n");
        }
        if (tLen > 60) {
            int tmp = addSize[row][tCur -1];
            addSize[row][tCur -1] = 0;
            storeTextData(objectCode, tRecord, &tCur, &tStart, current, &tLen, 0);
            addSize[++row][0] = tmp;
        }
        len = parseSpTb(code, source);
    }
    if (tLen) {
        int tmp = addSize[row][tCur -1];
        storeTextData(objectCode, tRecord, &tCur, &tStart, LOCCTR, &tLen,1);
        addSize[++row][0] = tmp;
    }
    writeHeaderEndRecord(&objectCode, name, startingAddress, LOCCTR);

    fclose(source);
    fclose(OPTAB);
    fclose(objectCode);
    return 0;
}

void saveSym(sym **SYMTAB, char label[], int LOCCTR, int forward, FILE **objectCode, char tRecord[][20], int tCur, int tStart, int addSize[][15], int row) {

    char strLocctr[20];
    sprintf(strLocctr,"%04X", LOCCTR);

    sym *temp;

    for (temp = *SYMTAB; temp != NULL; temp = temp->next) {
        if (!strcmp(temp->labelName, label))
            break;
    }
    if (temp) {
        nfrwd *temp1;
        if (forward) {
            nfrwd *newForward = (nfrwd *)malloc(sizeof(nfrwd));
            memcpy(newForward->address, strLocctr,strlen(strLocctr)+1);
            newForward->link = NULL;

            if (!temp->link) {
                temp->link = newForward;
                return;
            }
            for (temp1 = temp->link; temp1->link != NULL; temp1 = temp1->link);
            temp1->link = newForward;
        } else {
            memcpy(temp->address, strLocctr, strlen(strLocctr)+1);
            for (temp1 = temp->link; temp1 != NULL; temp1 = temp1->link)
                findAndReplace(&SYMTAB, temp1->address, temp->address, &objectCode, tRecord, tCur, tStart, addSize, row);
            temp->present = 1;
        }
        return;
    }

    sym *newNode = (sym *)malloc(sizeof(sym));
    memcpy(newNode->labelName,label, strlen(label)+1);
    memcpy(newNode->address, strLocctr, strlen(strLocctr)+1);
    if (forward) {
        nfrwd *newForward1 = (nfrwd *)malloc(sizeof(nfrwd));
        memcpy(newForward1->address, strLocctr,strlen(strLocctr));
        newForward1->link = NULL;
        newNode->present = 0;
        newNode->link = newForward1;
    }
    else {
        newNode->present = 1;
        newNode->link = NULL;
    }
    newNode->next = NULL;
    if (!*SYMTAB) {
        *SYMTAB = newNode;
        return;
    }
    for (temp = *SYMTAB; temp->next != NULL; temp = temp->next);
    temp->next = newNode;

}

void findAndReplace(sym ***SYMTAB, char find[20], char replace[20], FILE ***objectCode, char tRecord[][20], int tCur, int tStart, int addSize[][15], int row) {
    int len;
    char code[100][20];

    int findHx, replaceHx, start, cur, curNext, idx, curRow;
    findHx = (int)strtol(find,NULL,16);
    replaceHx = (int)strtol(replace,NULL,16);

    idx = -1;
    if (findHx >= tStart) {
        cur = tStart;
        for (int i = 0; i < tCur; ++i) {
            if (findHx == cur)
                idx = i;
            else if (findHx < cur)
                idx = i -1;
            if (idx != -1) {
                for (int k = 0; k < 4; ++k)
                    tRecord[idx][2 + k] = replace[k];
                break;
            }
            cur += addSize[row][i];
        }
    }
    else {
        FILE *tempFile = fopen("temp.txt", "w+");
        curRow = 0;
        fseek(**objectCode, 0, SEEK_SET);
        len = parseSpTb(code, **objectCode);
        while (len) {
            if (idx == -1) {
                start = (int)strtol(code[1],NULL,16);
                cur = start;
                for (int i = 3; i < len; ++i) {
                    if (findHx == cur)
                        idx = i;
                    else if (findHx < cur)
                        idx = i -1;
                    if (idx != -1) {
                        for (int k = 0; k < 4; ++k)
                            code[idx][2 + k] = replace[k];
                        break;
                    }
                    cur += addSize[curRow][i-3];
                }
                ++curRow;
            }
            for (int i = 0; i < len -1; ++i)
                fprintf(tempFile, "%s ",code[i]);
            fprintf(tempFile,"%s\n", code[len -1]);
            len = parseSpTb(code, **objectCode);
        }
        freopen("objectCode.txt", "w+", **objectCode);

        fseek(tempFile, 0, SEEK_SET);
        char data[200];
        while (fgets(data, 200, tempFile))
            fputs(data, **objectCode);
        fclose(tempFile);
    }
}

void writeHeaderEndRecord(FILE **objectCode, char name[], int startingAddress, int LOCCTR) {
    char data[200];
    fseek(*objectCode, 0, SEEK_SET);
    FILE *tempFile = fopen("temp.txt", "w+");
    fprintf(tempFile, "H %-6s %06X %06X\n", name, startingAddress, (LOCCTR - startingAddress));
    while (fgets(data, 200, *objectCode))
        fputs(data, tempFile);
    fprintf(tempFile, "E %06X", startingAddress);

    fseek(tempFile, 0, SEEK_SET);
    freopen("objectCode.txt", "w+", *objectCode);
    while (fgets(data, 200, tempFile))
        fputs(data, *objectCode);

    fclose(tempFile);
    remove("temp.txt");
}

void storeTextData(FILE *objectCode, char tRecord[][20], int *tCur, int *tStart, int current, int *tLen, int last) {
    char temp[30];
    if (!last)
        --*tCur;
    memset(temp, 0 , 30);
    memcpy(temp, tRecord[*tCur], strlen(tRecord[*tCur])+1);
    writeTextRecord(objectCode, tRecord, *tStart, *tCur, *tLen-strlen(tRecord[*tCur]));
    resetTextRecord(tRecord, &*tCur);
    memcpy(tRecord[(*tCur)++], temp, strlen(temp)+1);
    *tStart = current;
    *tLen = strlen(temp);

}

int searchSymtab(sym *SYMTAB, char key[]) {
    int address;
    sym *temp;
    for (temp = SYMTAB; temp != NULL; temp = temp->next) {
        if (!strcmp(key, temp->labelName) && temp->present) {
            address = (int)strtol(temp->address,NULL,16);
            return address;
        }
    }
    if (!temp)
        return -1;
    return 0;
}

int searchOptab(FILE *OPTAB, char key[]) {
    rewind(OPTAB);
    char op[20];
    int opcode;
    while (fscanf(OPTAB, "%s %06X", op, &opcode) != EOF) {
        if (!strcmp(op, key))
            return opcode;
    }
    return -1;
}

int bytecondition(char str[]) {
    if (str[0] == 'C') {
        return strlen(str) - 3;
    }
    else if (str[0] == 'X')
        return ceil((int)((strlen(str)-2)/2));
    return 0;
}

void writeTextRecord(FILE *objectCode, char tRecord[][20], int tStart, int tCur, int tLen) {
    fprintf(objectCode, "T %06X %02X", tStart, tLen/2);
    for (int i = 0; i < tCur; ++i)
        fprintf(objectCode, " %s", tRecord[i]);
    fprintf(objectCode, "\n");
}

void resetTextRecord(char tRecord[][20], int *tCur) {
    for (int i = 0;i < 20; ++i) {
        memset(tRecord[i], 0, 20);
    }
    *tCur = 0;
}

void resetAddSize(int addSize[][15]) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 15; ++j)
            addSize[i][j] = 0;
    }
}

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