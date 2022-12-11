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

void saveInter(FILE *intermediate, char code[][20], int len, int LOCCTR) {
    fprintf(intermediate, "%04X\t%s", LOCCTR, code[0]);
    for (int i = 1; i < 3; ++i) {
        if (strlen(code[i-1]) < 4) 
            fprintf(intermediate,"\t");
        fprintf(intermediate,"\t%s", code[i]);
    }
    fprintf(intermediate,"\n");
}
void saveSym(FILE *file, char label[], int LOCCTR) {
    if (strlen(label) < 4)
        fprintf(file, "%s\t\t%04X\n", label, LOCCTR);
    else 
        fprintf(file, "%s\t%04X\n", label, LOCCTR);
}

int searchSymtab(FILE *SYMTAB, char key[]) {
    rewind(SYMTAB);
    char label[20], address[20];
    while (fscanf(SYMTAB, "%s %s", label, address) != EOF) {
        if (!strcmp(key, label)) 
            return 1;
    }
    return 0;
}

int searchOptab(FILE *OPTAB, char key[]) {
    rewind(OPTAB);
    char op[20];
    int opcode;
    while (fscanf(OPTAB, "%s %06X", op, &opcode) != EOF) {
        if (!strcmp(op, key))
            return 1;
    }
    return 0;
}

int bytecondition(char str[]) {
    if (str[0] == 'C') {
        return strlen(str) - 3;
    }
    else if (str[0] == 'X') 
        return ceil((int)((strlen(str)-2)/2));
    return 0;
}

int main() {
    FILE  *source, *OPTAB, *SYMTAB, *intermediate, *infoSave;
    int LOCCTR, startingAddress, current, len;
    char code[100][20];

    source = fopen("source.txt", "r");
    OPTAB = fopen("optab.txt", "r");
    SYMTAB = fopen("symtab.txt", "w+");
    intermediate = fopen("intermediate.txt", "w+");
    infoSave = fopen("infoSave.txt", "w+");

    if (!source)  {
        printf("Error opening file\n");
        return 0;
    }

    len = parseSpTb(code, source);
    if (!strcmp(code[1], "START")) {
        // convert string to base 16
        LOCCTR = (int)strtol(code[2], NULL, 16);
        saveInter(intermediate, code, len, LOCCTR);
    }
    else {
        LOCCTR = 0;
        rewind(source);
    }
    startingAddress = LOCCTR;

    len = parseSpTb(code, source);
    while (strcmp(code[1], "END")) {
        current = LOCCTR;
        printf("%s %s %s\n", code[0], code[1], code[2]);
        if (strcmp(code[0], "**")) {
            if (strcmp(code[0], "-")) {
                if (!searchSymtab(SYMTAB, code[0]))
                    saveSym(SYMTAB, code[0], LOCCTR); 
            }
            if (searchOptab(OPTAB, code[1])) 
                LOCCTR += 3;
            else if (!strcmp(code[1], "WORD")) 
                LOCCTR += 3;
            else if (!strcmp(code[1], "RESW"))
                LOCCTR += (3 * atoi(code[2]));
            else if (!strcmp(code[1], "RESB")) 
                LOCCTR += atoi(code[2]);
            else if (!strcmp(code[1], "BYTE")) 
                LOCCTR += bytecondition(code[2]);
            else {
                printf("Error in opcode!\n");
                len = parseSpTb(code, source);
                continue;
            }
        }
        else { 
            len = parseSpTb(code, source);
            continue;
        }
        saveInter(intermediate, code, len, current);
        len = parseSpTb(code, source);
    }
    fprintf(infoSave, "ProgramLength: %4X", (LOCCTR-startingAddress));

    fclose(source);
    fclose(OPTAB);
    fclose(SYMTAB);
    fclose(intermediate);
    fclose(infoSave);
    return 0;
}