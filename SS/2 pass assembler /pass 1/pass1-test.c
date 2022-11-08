#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void saveInter(FILE *file, char **code, int len, int LOCCTR) {
    fprintf(file, "%04X", LOCCTR);
    for (int i = 0; i < len; ++i) {
        if (i > 0 && strlen(code[i-1]) < 4)
            fprintf(file, "\t\t%s", code[i]);
        else 
            fprintf(file, "\t%s", code[i]);

    } 
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

char **codeSplit(char str[], int *len){
    *len = 0;

    char ** code = malloc(10 * sizeof(char));
    for (int i = 0; i < 10; ++i) 
        code[i] = malloc(10 * sizeof(char));

    char *hi = strtok(str, " ");

    while (hi != NULL) {
        code[(*len)++] = hi;
        hi = strtok(NULL, " ");
    }

    return code;
}

int bytecondition(char str[]) {
    if (str[1] == 'C') 
        return strlen(str);
    else if (str[1] == 'X') {
        return ceil(strlen(str)/2);
    }
    return 0;
}

int main() {
    FILE  *source, *OPTAB, *SYMTAB, *intermediate, *infoSave;
    int LOCCTR, startingAddress, current, len;
    char **code, str[100];

    source = fopen("source.txt", "r");
    OPTAB = fopen("optab.txt", "r");
    SYMTAB = fopen("symtab.txt", "w+");
    intermediate = fopen("intermediate.txt", "w+");
    infoSave = fopen("infoSave.txt", "w+");

    if (!source)  {
        printf("Error opening file\n");
        return 0;
    }
    fgets(str, 100, source);
    code = codeSplit(str, &len);

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

    fgets(str, 100, source);
    code = codeSplit(str, &len);
    char comment[200], f_comm[2000] = "";
    while (strcmp(code[1], "END")) {
        current = LOCCTR;
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
                LOCCTR += bytecondition(code[1]);
            else {
                printf("Error in opcode!\n");
                fgets(str, 100, source);
                code = codeSplit(str, &len);
                continue;
            }
        }
        else {
            fprintf(intermediate, "%s", code[0]);
            for (int i = 1; i < len; ++i)
                fprintf(intermediate, " %s", code[i]);
            fgets(str, 100, source);
            code = codeSplit(str, &len);
            continue;
        }
        saveInter(intermediate, code, len, current);
        fgets(str, 100, source);
        code = codeSplit(str, &len);
        printf("%d\n", LOCCTR);
    }
    saveInter(intermediate, code, len, current);
    printf("%d\n", LOCCTR);

    fprintf(infoSave, "LOCCTR: %04X\nLength of program: %04X", LOCCTR, (LOCCTR-startingAddress));

    fclose(source);
    fclose(OPTAB);
    fclose(SYMTAB);
    fclose(intermediate);
    fclose(infoSave);
    return 0;
}

