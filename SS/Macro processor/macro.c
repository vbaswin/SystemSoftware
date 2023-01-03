#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct argtabStack {
    char values[10][20];
    int len;
    struct ARGTAB *link;
} ARGTAB;

typedef struct namtabStack {
    char name[20];
    int start;
    struct namtabStack *link;
} NAMTAB;

// void pushArg(char label[][20], )
int parseSpTb(char code[][20], FILE *source);
int GETLINE(char code[][20], FILE *source, FILE *defTAb, int *expanding);
void writeDeftab(FILE *defTab, int ***defPtr, char code[100][20], int len);
void writeExpanded(FILE *expanded, char code[100][20], int len, int *expanding);

void pushNam(NAMTAB ****nam, char name[], int start) {
    NAMTAB *newNam = (NAMTAB *)malloc(sizeof(NAMTAB));
    memcpy(newNam->name,name, strlen(name)+1);
    newNam->start = start;
    if (!***nam) {
        ***nam = newNam;
        return;
    }
    NAMTAB *temp;
    for (temp = ***nam; temp->link != NULL; temp = temp->link);
    temp->link = newNam;
}

NAMTAB *popNam(NAMTAB ****nam) {
    if (!***nam)
        return NULL;
    NAMTAB *temp, *temp1 = temp;
    for (temp = ***nam; temp->link != NULL; temp1 = temp, temp = temp->link);
    if (temp == ***nam) {
        ***nam = NULL;
        // printf("temp: %s tmp1: %s\n", temp->name,temp->name);
        return temp;
    }
    // printf("temp: %s tmp1: %s\n", temp->name,temp->name);
    temp1->link = NULL;
    // printf("tmp1: %s\n", temp1->name);
    return temp;
}

void DEFINE(FILE *namTab, FILE *defTab, FILE *source, char code[][20], int len, int **defPtr, NAMTAB ***nam, int **expanding) {
    int level = 1;
    pushNam(&nam, code[0], **defPtr);

    writeDeftab(defTab, &defPtr, code, len);

    while (level) {
        len = GETLINE(code, source, defTab, &**expanding);
        if (strcmp(code[0], "**")) {
            // substitute positional parameters
            writeDeftab(defTab, &defPtr, code, len);
            if (!strcmp(code[1],"MACRO")) {
                ++level;
                pushNam(&nam, code[0], (**defPtr)-1);
            }
            else if (!strcmp(code[1],"MEND")) {
                --level;
                NAMTAB *temp = popNam(&nam);
                fprintf(namTab, "%s %d %d\n", temp->name, temp->start, (**defPtr)-1);
            }
        }
    }
}

void EXPAND(FILE *defTab, FILE *expanded, FILE *source, char namTabCode[][20], char code[][20], int **expanding) {
    **expanding = 1;
    char defTabCode[100][20];
    int len, begin = 0;
    fseek(defTab, 0, SEEK_SET);
    len = GETLINE(defTabCode,source, defTab, &**expanding);
    while (len) {
        if (!strcmp(namTabCode[1], defTabCode[0])) {
            begin = 1;
            len = GETLINE(defTabCode,source, defTab, &**expanding);
            continue;
        }
        else if (!strcmp(namTabCode[2], defTabCode[0]))
            begin = 0;
        if (begin) {
            writeExpanded(expanded, defTabCode, len, &**expanding);
        }
        len = GETLINE(defTabCode,source, defTab, &**expanding);
    }
    fseek(defTab, 0, SEEK_END);
    **expanding = 0;
}


void PROCESSLINE(FILE *namTab, FILE *expanded, FILE *defTab, FILE *source, char code[][20], int len, int *defPtr, NAMTAB **nam, ARGTAB **arg, int *expanding) {
    fseek(namTab, 0, SEEK_SET);
    char namTabCode[10][20];
    int namLen, found = 0;
    namLen = parseSpTb(namTabCode, namTab);
    while (namLen) {
        if (!strcmp(namTabCode[0], code[1])) {
            found = 1;
            break;
        }
        namLen = parseSpTb(namTabCode, namTab);
    }
    if (found) {
        EXPAND(defTab, expanded, source, namTabCode, code, &expanding);
    } else if (!strcmp(code[1], "MACRO")) {
        DEFINE(namTab, defTab, source, code, len, &defPtr, &nam, &expanding);
    } else {
        if (!strcmp(code[0], "**"))
            return;
        writeExpanded(expanded, code, len, &*expanding);
    }
}

void writeExpanded(FILE *expanded, char code[100][20], int len, int *expanding) {
    if (!strcmp(code[2], "MACRO"))
        return;
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
        // for (int i = 0; i < len; ++i)
            // printf("%s\t", code[i]);
        // printf("\n");
        PROCESSLINE(namTab, expanded, defTab, source, code, len, &defPtr, &nam, &arg, &expanding);
        len = GETLINE(code, source, defTab, &expanding);
    }

    fclose(source);
    fclose(defTab);
    fclose(namTab);
    fclose(expanded);
}

int GETLINE(char code[][20], FILE *source, FILE *defTab, int *expanding) {
    if (*expanding) {
        int len = parseSpTb(code, defTab);
        return len;
        // substitute arguments from argtab
    } else
        return parseSpTb(code, source);
    return 0;
}

void writeDeftab(FILE *defTab, int ***defPtr, char code[100][20], int len) {
    // printf("defPtr: %d\n", ***defPtr);
    fprintf(defTab, "%d\t", (***defPtr)++);
    fprintf(defTab, "\t");
    for (int i = 0; i < len -1; ++i) {
        // printf("%s ", code[i]);
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