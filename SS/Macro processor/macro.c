/*
    Program name: Macro processor
    Authur name: Aswin V B
    Rollno: 222
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct argtabStack {
    char labels[10][20];
    int argLen;
    struct argtabStack *link;
} ARGTAB;

typedef struct namtabStack {
    char name[20];
    int start;
    int end;
    struct namtabStack *link;
} NAMTAB;

int parseSpTb(char code[][20], FILE *source);
int GETLINE(char code[][20], FILE *source, FILE *defTAb, int *expanding);
void writeDeftab(FILE *defTab, int ***defPtr, char code[100][20], int len);
void writeExpanded(FILE *expanded, char code[100][20], int len, int *expanding);
int splitArguments(char input[100], char argumentsSplit[][20]);
int searchNamtab(FILE *namTab, char namTabCode[][20], char macroName[]);

void pushNam(NAMTAB ****nam, char name[], int start, int end) {
    NAMTAB *newNam = (NAMTAB *)malloc(sizeof(NAMTAB));
    memcpy(newNam->name,name,strlen(name)+1);
    newNam->start = start;
    newNam->end = end;
    newNam->link = NULL;
    if (!***nam) {
        ***nam = newNam;
        return;
    }
    newNam->link = ***nam;
    ***nam = newNam;
}

NAMTAB *popNam(NAMTAB ****nam) {
    if (!***nam)
        return NULL;
    NAMTAB *temp = ***nam;
    (***nam) = (***nam)->link;
    return temp;
}

void pushArg(ARGTAB ****arg, char argumentsSplit[][20], int argumentsNo) {
    ARGTAB *newNam = (ARGTAB *)malloc(sizeof(ARGTAB));
    for (int i = 0; i < argumentsNo; ++i)
        memcpy(newNam->labels[i], argumentsSplit[i],strlen(argumentsSplit[i])+1);
    newNam->argLen = argumentsNo;
    newNam->link = NULL;
    if (!***arg) {
        ***arg = newNam;
        return;
    }
    newNam->link = ***arg;
    ***arg = newNam;
}

void popArg(ARGTAB ****arg) {
    if (!***arg)
        return;
    (***arg) = (***arg)->link;
}

void substitutePositionalParamters(char code[][20], int len, ARGTAB ****arg) {
    if (!memcmp(code[2],"-",1))
        return;
    char ch, argument[20], newCode[20];
    int found, j, newCodeLen = 0, argumentLen = 0, curArg = 0;
    for (int i = 0; i < strlen(code[2]); ++i) {
        ch = code[2][i];
        if (ch == '&') {
            curArg = 1;
            continue;
        } else if (ch == '\'' || ch == ',' ) {
            argument[argumentLen] = 0;
            if (curArg) {
                found = 0;
                for (j = 0; j < (***arg)->argLen; ++j) {
                    if(!strcmp((***arg)->labels[j], argument)) {
                        found = 1;
                        break;
                    }
                }
                newCode[newCodeLen++] = '?';
                newCode[newCodeLen++] = '0'+ j + 1;
                argumentLen = 0;
                curArg = 0;
            }
        }
        if (curArg)
            argument[argumentLen++] = ch;
        else
            newCode[newCodeLen++] = ch;
    }
    if (curArg) {
        argument[argumentLen] = 0;
       found = 0;
        for (j = 0; j < (***arg)->argLen; ++j) {
            if(!strcmp((***arg)->labels[j], argument)) {
                found = 1;
                break;
            }
        }
        newCode[newCodeLen++] = '?';
        newCode[newCodeLen++] = '0'+ j + 1;
    }
    newCode[newCodeLen] = 0;
    memcpy(code[2],newCode,strlen(newCode)+1);
    return;
}

void DEFINE(FILE *namTab, FILE *defTab, FILE *source, char code[][20], int len, int **defPtr, NAMTAB ***nam, ARGTAB ***arg, int **expanding) {
    int level = 1, argumentsNo = 0;
    char argumentsSplit[10][20];
    pushNam(&nam, code[0], **defPtr, 0);

    writeDeftab(defTab, &defPtr, code, len);
    argumentsNo = splitArguments(code[2], argumentsSplit);
    pushArg(&arg, argumentsSplit, argumentsNo);

    while (level) {
        len = GETLINE(code, source, defTab, &**expanding);
        if (strcmp(code[0], "**")) {
            if (!strcmp(code[1],"MACRO")) {
                ++level;
                argumentsNo = splitArguments(code[2], argumentsSplit);
                substitutePositionalParamters(code, len, &arg);
                pushArg(&arg, argumentsSplit, argumentsNo);
                pushNam(&nam, code[0], **defPtr, 0);
                writeDeftab(defTab, &defPtr, code, len);
                continue;
            }
            else if (!strcmp(code[1],"MEND")) {
                --level;
                NAMTAB *temp = popNam(&nam);
                fprintf(namTab, "%s %d %d\n", temp->name, temp->start, **defPtr);
                popArg(&arg);
            }
            substitutePositionalParamters(code, len, &arg);
            writeDeftab(defTab, &defPtr, code, len);
        }
    }
}

void substituteArgumentsParamters(char code[][20], int len, ARGTAB ****arg) {
    if (!memcmp(code[3],"-",1))
        return;
    char ch, newCode[20];
    int newCodeLen = 0, idx = -1;
    for (int i = 0; i < strlen(code[3]); ++i) {
        ch = code[3][i];
        if (ch == '?') {
            idx = code[3][i+1] - '0';
        } else if (idx != -1) {
            for (int j = 0; j < strlen((***arg)->labels[idx-1]); ++j)
                newCode[newCodeLen++] = (***arg)->labels[idx-1][j];
            idx = -1;
        }
        else
            newCode[newCodeLen++] = ch;
    }
    newCode[newCodeLen] = 0;
    memcpy(code[3],newCode,strlen(newCode)+1);
    return;
}

void writeMacroHeader(FILE *expanded, char macroLabel[20], char macroName[20], char arguments[100]) {
    char macLabWithComment[20];
    if (strcmp(macroLabel,"-"))
        sprintf(macLabWithComment,".%s",macroLabel);
    else
        sprintf(macLabWithComment,".");
    fprintf(expanded, "%s\t", macLabWithComment);
    if (strlen(macLabWithComment) < 4)
        fprintf(expanded, "\t");
    fprintf(expanded, "%s\t", macroName);
    if (strlen(macroName) < 4)
        fprintf(expanded, "\t");
    fprintf(expanded, "%s\t", arguments);
    if (strlen(arguments) < 4)
        fprintf(expanded, "\t");
    fprintf(expanded, "\n");
}

void EXPAND(FILE *defTab, FILE *expanded, FILE *source, FILE *namTab, char namTabCode[][20], char code[][20], int **expanding, ARGTAB ***arg, NAMTAB ***nam) {
    **expanding = 1;
    char defTabCode[100][20], namStart[20], namEnd[20];
    int len, begin = 0, argumentsNo = 0;

    char argumentsSplit[10][20];
    argumentsNo = splitArguments(code[2], argumentsSplit);
    pushArg(&arg, argumentsSplit, argumentsNo);
    pushNam(&nam, namTabCode[0], atoi(namTabCode[1]), atoi(namTabCode[2]));

    fseek(defTab, 0, SEEK_SET);
    len = GETLINE(defTabCode,source, defTab, &**expanding);
    while (**nam) {
        sprintf(namStart, "%d", (**nam)->start);
        sprintf(namEnd, "%d", (**nam)->end);
        if (!strcmp(namStart, defTabCode[0])) {
            begin = 1;
            writeMacroHeader(expanded, code[0], code[1], code[2]);
            len = GETLINE(defTabCode,source, defTab, &**expanding);
            continue;
        } else if (!strcmp(namEnd, defTabCode[0])) {
            popArg(&arg);
            popNam(&nam);
            len = GETLINE(defTabCode,source, defTab, &**expanding);
            continue;
        }
        if (begin) {
            int found = searchNamtab(namTab, namTabCode, defTabCode[1]);
            if (found) {
                pushNam(&nam, namTabCode[0], atoi(namTabCode[1]), atoi(namTabCode[2]));
                substituteArgumentsParamters(defTabCode, len, &arg);
                argumentsNo = splitArguments(defTabCode[3], argumentsSplit);
                pushArg(&arg, argumentsSplit, argumentsNo);
                writeMacroHeader(expanded, "-", namTabCode[0], defTabCode[3]);
                len = GETLINE(defTabCode,source, defTab, &**expanding);
                continue;
            }
            substituteArgumentsParamters(defTabCode, len, &arg);
            writeExpanded(expanded, defTabCode, len, &**expanding);
        }
        len = GETLINE(defTabCode,source, defTab, &**expanding);
    }
    fseek(defTab, 0, SEEK_END);
    **expanding = 0;
}

void PROCESSLINE(FILE *namTab, FILE *expanded, FILE *defTab, FILE *source, char code[][20], int len, int *defPtr, NAMTAB **nam, ARGTAB **arg, int *expanding) {
    char namTabCode[10][20];
    int found = searchNamtab(namTab, namTabCode, code[1]);

    if (found) {
        EXPAND(defTab, expanded, source, namTab, namTabCode, code, &expanding, &arg, &nam);
    } else if (!strcmp(code[1], "MACRO")) {
        DEFINE(namTab, defTab, source, code, len, &defPtr, &nam, &arg, &expanding);
    } else {
        if (!strcmp(code[0], "**"))
            return;
        writeExpanded(expanded, code, len, &*expanding);
    }
}

void writeExpanded(FILE *expanded, char code[100][20], int len, int *expanding) {
    if (*expanding) {
        for (int i = 1; i < len -1; ++i) {
            fprintf(expanded, "%s\t", code[i]);
            if (strlen(code[i]) < 4)
                fprintf(expanded, "\t");
        }
        fprintf(expanded, "%s\n", code[len - 1]);
    } else {
        for (int i = 0; i < len -1; ++i) {
            fprintf(expanded, "%s\t", code[i]);
            if (strlen(code[i]) < 4)
                fprintf(expanded, "\t");
        }
        fprintf(expanded, "%s\n", code[len - 1]);
    }
}

int main() {
    char code[100][20];
    int len, expanding = 0, defPtr = 1;
    FILE *source, *defTab, *namTab, *expanded;
    NAMTAB *nam = NULL;
    ARGTAB *arg = NULL;

    source = fopen("source.txt", "r+");
    namTab = fopen("namTab.txt", "w+");
    defTab = fopen("defTab.txt", "w+");
    expanded = fopen("expanded.txt", "w+");


    len = GETLINE(code, source, defTab, &expanding);

    while (len) {
        PROCESSLINE(namTab, expanded, defTab, source, code, len, &defPtr, &nam, &arg, &expanding);
        len = GETLINE(code, source, defTab, &expanding);
    }

    fclose(source);
    fclose(defTab);
    fclose(namTab);
    fclose(expanded);
}

int searchNamtab(FILE *namTab, char namTabCode[][20], char macroName[]) {
    fseek(namTab, 0, SEEK_SET);
    int namLen, found = 0;
    namLen = parseSpTb(namTabCode, namTab);
    while (namLen) {
        if (!strcmp(namTabCode[0], macroName)) {
            found = 1;
            break;
        }
        namLen = parseSpTb(namTabCode, namTab);
    }
    fseek(namTab,0,SEEK_END);
    return found;
}

int GETLINE(char code[][20], FILE *source, FILE *defTab, int *expanding) {
    if (*expanding) {
        int len = parseSpTb(code, defTab);
        return len;
    } else
        return parseSpTb(code, source);
    return 0;
}

int splitArguments(char input[100], char argumentsSplit[][20]) {
    int len = 0,k = 0;
    char ch;
    for (int i = 0; i < strlen(input); ++i) {
        ch = input[i];
        if (ch == '&')
            continue;
        else if (ch == ',') {
            argumentsSplit[len++][k] = 0;
            k = 0;
        }
        else
            argumentsSplit[len][k++] = ch;
    }
            argumentsSplit[len++][k] = 0;
    return len;
}
void writeDeftab(FILE *defTab, int ***defPtr, char code[100][20], int len) {
    fprintf(defTab, "%d\t", (***defPtr)++);
    fprintf(defTab, "\t");
    for (int i = 0; i < len -1; ++i) {
        fprintf(defTab, "%s\t", code[i]);
        if (strlen(code[i]) < 4)
            fprintf(defTab, "\t");
    }
    fprintf(defTab, "%s\n", code[len -1]);
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