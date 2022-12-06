#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void readSource(FILE *source, char code[10][10]) {

    char str[100];
    fgets(str, 100, source);
    int next = 0;

    int j = 0, k = 0, i = 0;
    while (str[i] != 0) {
        if (str[i] == ' ' || str[i] == '\t') {
            if (!next) {
                next = 1;
                code[j][k] = 0;
                k = 0;
                ++j;
            }
        }
        else {
                next = 0;
                code[j][k++] = str[i];
        }
        ++i;
    }
    code[j][k] = 0;

    for (int i = 0; i < j+1; ++i)
        printf("%s ", code[i]);
}

// void saveInter(FILE *intermediate, codeL code, int len, int LOCCTR) {
//     fprintf(intermediate, "%04X %s %s %s", LOCCTR, code.label, code.opcode, code.operand);

// }
// void saveSym(FILE *file, char label[], int LOCCTR) {
//     if (strlen(label) < 4)
//         fprintf(file, "%s\t\t%04X\n", label, LOCCTR);
//     else 
//         fprintf(file, "%s\t%04X\n", label, LOCCTR);
// }

// int searchSymtab(FILE *SYMTAB, char key[]) {
//     rewind(SYMTAB);
//     char label[20], address[20];
//     while (fscanf(SYMTAB, "%s %s", label, address) != EOF) {
//         if (!strcmp(key, label)) 
//             return 1;
//     }
//     return 0;
// }

// int searchOptab(FILE *OPTAB, char key[]) {
//     rewind(OPTAB);
//     char op[20];
//     int opcode;
//     while (fscanf(OPTAB, "%s %06X", op, &opcode) != EOF) {
//         if (!strcmp(op, key))
//             return 1;
//     }
//     return 0;
// }

// int bytecondition(char str[]) {
//     if (str[0] == 'C') {
//         return strlen(str) - 3;
//     }
//     else if (str[0] == 'X') 
//         return ceil((int)((strlen(str)-2)/2));
//     return 0;
// }

int main() {
    FILE  *source, *OPTAB, *SYMTAB, *intermediate, *infoSave;
    int LOCCTR, startingAddress, current, len;
    char **code;

    source = fopen("source.txt", "r");
    OPTAB = fopen("optab.txt", "r");
    SYMTAB = fopen("symtab.txt", "w+");
    intermediate = fopen("intermediate.txt", "w+");
    infoSave = fopen("infoSave.txt", "w+");

    if (!source)  {
        printf("Error opening file\n");
        return 0;
    }
    readSource(source, code);

        for (int i = 0; i < 3; ++i)
        printf("%s ", code[i]);

    // printf("today: %s %s %s\n", d.code[0], d.code[1], d.code[2]);

    // if (!strcmp(code.opcode, "START")) {
    //     // convert string to base 16
    //     LOCCTR = (int)strtol(code.operand, NULL, 16);
    //     saveInter(intermediate, code, len, LOCCTR);
    // }
    // else {
    //     LOCCTR = 0;
    //     rewind(source);
    // }
    // startingAddress = LOCCTR;

    // readSource(source, &code);
    // printf("%s %s %s", code.label, code.opcode, code.operand);
    // while (strcmp(code.opcode, "END")) {
    //     // current = LOCCTR;
    //     printf("%s %s %s", code.label, code.opcode, code.operand);
    //     if (strcmp(code.label, "**")) {
    //         if (strcmp(code.label, "-")) {
    //             if (!searchSymtab(SYMTAB, code.label))
    //                 saveSym(SYMTAB, code.label, LOCCTR); 
    //         }
    //         if (searchOptab(OPTAB, code.opcode)) 
    //             LOCCTR += 3;
    //         else if (!strcmp(code.opcode, "WORD")) 
    //             LOCCTR += 3;
    //         else if (!strcmp(code.opcode, "RESW"))
    //             LOCCTR += (3 * atoi(code.operand));
    //         else if (!strcmp(code.opcode, "RESB")) 
    //             LOCCTR += atoi(code.operand);
    //         else if (!strcmp(code.opcode, "BYTE")) 
    //             LOCCTR += bytecondition(code.operand);
    //         else {
    //             printf("Error in opcode!\n");
    //             readSource(source, &code);
    //             continue;
    //         }
    //     }
    //     else { 
    //         readSource(source, &code);
    //         continue;
    //     }
    //     saveInter(intermediate, code, len, current);
    //     readSource(source, &code);
    // }
    // fprintf(infoSave, "ProgramLength: %4X", (LOCCTR-startingAddress));

    fclose(source);
    fclose(OPTAB);
    fclose(SYMTAB);
    fclose(intermediate);
    fclose(infoSave);
    return 0;
}

// owlcity - fireflies
// zootopia - last song
// counting stars
// 7 years