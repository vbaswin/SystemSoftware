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

void saveSym(sym **SYMTAB, char label[], int LOCCTR, int forward) {
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
            memcpy(newForward->address, strLocctr,strlen(strLocctr));
            newForward->link = NULL;

            if (!(*SYMTAB)->link) {
                (*SYMTAB)->link = newForward;
                return;
            }
            for (temp1 = temp->link; temp1->link != NULL; temp1 = temp1->link);
            temp1->link = newForward;
        } else {
            memcpy(temp->address, strLocctr, strlen(strLocctr));
            for (temp1 = temp->link; temp1 != NULL; temp1 = temp1->link) {
                // if (temp1)
            }
        }
    }

    sym *newNode = (sym *)malloc(sizeof(sym));
    memcpy(newNode->labelName,label, strlen(label));
    memcpy(newNode->address, strLocctr, strlen(strLocctr));
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

void storeTextData(FILE *objectCode, char tRecord[][20], int *tCur, int *tStart, int current, int *tLen, int last) {
    char temp[30];
    if (!last)
        --*tCur;
    memset(temp, 0 , 30);
    memcpy(temp, tRecord[*tCur], strlen(tRecord[*tCur]));
    writeTextRecord(objectCode, tRecord, *tStart, *tCur, *tLen-strlen(tRecord[*tCur]));
    resetTextRecord(tRecord, &*tCur);
    memcpy(tRecord[(*tCur)++], temp, strlen(temp));
    *tStart = current;
    *tLen = strlen(temp);

}

int main() {
    FILE  *source, *OPTAB, *objectCode;
    int LOCCTR, startingAddress, current, len, programLen, tStartAddr, opcode, tCur = 0, tStart, address, tLen = 0;
    char code[100][20], temp[30], tRecord[20][20], addressString[10], previous[30];
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
    while (strcmp(code[1], "END")) {
        current = LOCCTR;
        if (strcmp(code[0], "**")) {
            if (strcmp(code[0], "-")) {
                if (searchSymtab(SYMTAB, code[0]) == -1)
                    saveSym(&SYMTAB, code[0], LOCCTR, 1);
            }
            if ((opcode = searchOptab(OPTAB, code[1])) != -1) {
                if (strcmp(code[2],"-") && strcmp(code[2],"BUFFER,X")) {
                    if ((address = searchSymtab(SYMTAB, code[2])) != -1) {
                        sprintf(addressString, "%04X", address);
                    }else {
                        saveSym(&SYMTAB, code[2], 0, 0);
                        memcpy(addressString, "0000", strlen(addressString));
                    }
                } else {
                    memcpy(addressString, "0000", strlen(addressString));
                }
                sprintf(tRecord[tCur], "%02X%s",opcode,addressString);
                tLen += strlen(tRecord[tCur++]);
                LOCCTR += 3;
            }
            else if (!strcmp(code[1], "WORD")) {
                char word[50];
                int val = (int)strtol(code[2], NULL, 10);
                sprintf(word,"%06X", val);
                memcpy(tRecord[tCur], word, strlen(word));
                tLen += strlen(tRecord[tCur++]);
                LOCCTR += 3;
            }
            else if (!strcmp(code[1], "RESW"))
                LOCCTR += (3 * atoi(code[2]));
            else if (!strcmp(code[1], "RESB"))
                LOCCTR += atoi(code[2]);
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
                    LOCCTR += strlen(new)/2;
                }
                tLen += strlen(tRecord[tCur++]);
            }
            else
                printf("Error in opcode!\n");
        }
        if (tLen > 60)
            storeTextData(objectCode, tRecord, &tCur, &tStart, current, &tLen, 0);
        len = parseSpTb(code, source);
    }
    if (tLen)
        storeTextData(objectCode, tRecord, &tCur, &tStart, LOCCTR, &tLen,1);
    fprintf(objectCode, "E‸%06X", startingAddress);

    // sym *temp;
    // for (temp = SYMTAB; temp != NULL; )

    fclose(source);
    fclose(OPTAB);
    fclose(objectCode);
    return 0;
}