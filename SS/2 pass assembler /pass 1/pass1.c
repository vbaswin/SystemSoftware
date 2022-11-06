#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct datas {
    char label[20];
    char opcode[20];
    char operand[20];
} data;

void read(FILE *file, data *d) {
    fscanf(file, "%s %s %s", (*d).label, (*d).opcode, (*d).operand);
}
void saveInter(FILE *file, data d, int LOCCTR) {
    fprintf(file, "%4X %s %s %s\n", LOCCTR, d.label, d.opcode, d.operand);
}
void saveSym(FILE *file, char label[], int LOCCTR) {
    fprintf(file, "%s %4X\n", label, LOCCTR);
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
    while (fscanf(OPTAB, "%s %X", op, &opcode) != EOF) {
        if (!strcmp(op, key))
            return 1;
    }
    return 0;
}

int main() {
    FILE  *source, *OPTAB, *SYMTAB, *intermediate, *infoSave;
    int LOCCTR, startingAddress, current;

    source = fopen("source.txt", "r");
    OPTAB = fopen("optab.txt", "r");
    SYMTAB = fopen("symtab.txt", "w+");
    intermediate = fopen("intermediate.txt", "w+");
    infoSave = fopen("infoSave.txt", "w+");

    data d;
    if (!source)  {
        printf("Error opening file\n");
        return 0;
    }
    read(source, &d);
    if (!strcmp(d.opcode, "START")) {
        // convert string to base 16
        LOCCTR = (int)strtol(d.operand, NULL, 16);
        saveInter(intermediate, d, LOCCTR);
    }
    else {
        LOCCTR = 0;
        rewind(source);
    }
    startingAddress = LOCCTR;

    char comment[200], f_comm[2000] = "";
    read(source, &d);
    while (strcmp(d.opcode, "END")) {
        current = LOCCTR;
        if (strcmp(d.label, "//")) {
            if (strcmp(d.label, "NULL")) {
                if (!searchSymtab(SYMTAB, d.label))
                    saveSym(SYMTAB, d.label, LOCCTR); 
            }
            if (searchOptab(OPTAB, d.opcode)) 
                LOCCTR += 3;
            else if (!strcmp(d.opcode, "WORD"))
                LOCCTR += 3;
            else if (!strcmp(d.opcode, "RESW"))
                LOCCTR += (3 * atoi(d.operand));
            else if (!strcmp(d.opcode, "RESB")) 
                LOCCTR += atoi(d.operand);
            else if (!strcmp(d.opcode, "BYTE")) 
                LOCCTR += strlen(d.operand);
            else {
                printf("Error in opcode!\n");
                read(source, &d);
                continue;
            }
            
        }
        else {
            fgets(comment, 200, source);
            snprintf(f_comm, 200, "%s %s %s %s", d.label, d.opcode, d.operand, comment);
            fputs(f_comm, intermediate);
            read(source, &d);
            continue;
        }
        saveInter(intermediate, d, current);
        read(source, &d);
        printf("%d\n", LOCCTR);
    }
    fprintf(infoSave, "LOCCTR: %X\nLength of program: %X", LOCCTR, (LOCCTR-startingAddress));

    fclose(source);
    fclose(OPTAB);
    fclose(SYMTAB);
    fclose(intermediate);
    fclose(infoSave);
    return 0;
}

