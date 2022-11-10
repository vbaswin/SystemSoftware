#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

char **codeSplit(char str[], int *len){
    *len = 0;
    printf("%s", str);

    char ** code = malloc(10 * sizeof(char));
    for (int i = 0; i < 10; ++i) 
        code[i] = malloc(10 * sizeof(char));

    char *hi = strtok(str, " \t");
    printf("%s", hi);

    while (hi != NULL) {
        code[(*len)++] = hi;
        hi = strtok(NULL, " \t");
        printf("%s %s\n",hi, code[*len]);
    }

    return code;
}


void listWrite(FILE *list, int startAddress, int objectCode) {

}

int main() {
    FILE  *OPTAB, *SYMTAB, *intermediate, *infoSave, *objectPrgm, *listing;
    int LOCCTR, startingAddress, current, len;
    char **code, str[200];

    OPTAB = fopen("optab.txt", "r");
    SYMTAB = fopen("symtab.txt", "r");
    intermediate = fopen("intermediate.txt", "r");
    infoSave = fopen("infoSave.txt", "r");
    objectPrgm = fopen("objectProgram.txt", "w+");
    listing = fopen("assemblyListing.txt", "w+");

    if (!intermediate)  {
        printf("Error opening file\n");
        return 0;
    }
    fgets(str, 200, intermediate);
    // printf("%s", str);
    code = codeSplit(str, &len);
    // printf("%s\n", code[0]);

    // char name[10] = " ", startAdd[10];
    // strcpy(startAdd, code[0]);
    // printf("%s", code[2]);
    // if (!strcmp(code[2], "START")) {
    //     // listing line 
    //     strcpy(name,code[1]);
    //     strcpy(startAdd, code[3]);
    //     fgets(str, 100, intermediate);
    //     code = codeSplit(str, &len);
    // }
    // fgets(str, 100, infoSave);
    // fgets(str, 100, infoSave);
    // code = codeSplit(str, &len);
    // char length[6];
    // strcpy(length, code[3]);
    // fprintf(objectPrgm, "H‸%s‸%s‸%s\n", name, startAdd,length);

    // while (strcmp(code[2], "END")) {
    //     current = LOCCTR;
    //     if (strcmp(code[0], "**")) {
    //         if (strcmp(code[0], "-")) {
    //             if (!searchSymtab(SYMTAB, code[0]))
    //                 saveSym(SYMTAB, code[0], LOCCTR); 
    //         }
    //         if (searchOptab(OPTAB, code[1])) 
    //             LOCCTR += 3;
    //         else if (!strcmp(code[1], "WORD")) 
    //             LOCCTR += 3;
    //         else if (!strcmp(code[1], "RESW"))
    //             LOCCTR += (3 * atoi(code[2]));
    //         else if (!strcmp(code[1], "RESB")) 
    //             LOCCTR += atoi(code[2]);
    //         else if (!strcmp(code[1], "BYTE")) 
    //             LOCCTR += bytecondition(code[2]);
    //         else {
    //             printf("Error in opcode!\n");
    //             fgets(str, 100, intermediate);
    //             code = codeSplit(str, &len);
    //             continue;
    //         }
    //     }
    //     else {
    //         fprintf(intermediate, "%s", code[0]);
    //         for (int i = 1; i < len; ++i)
    //             fprintf(intermediate, " %s", code[i]);
    //         fgets(str, 100, intermediate);
    //         code = codeSplit(str, &len);
    //         continue;
    //     }
    //     saveInter(intermediate, code, len, current);
    //     fgets(str, 100, intermediate);
    //     code = codeSplit(str, &len);
    // }
    // saveInter(intermediate, code, len, LOCCTR);

    // rewind(infoSave);
    // fgets(str, 100, infoSave);
    // code = codeSplit(str, &len);
    // printf("%s", code[1]);



    fclose(objectPrgm);
    fclose(listing);
    fclose(intermediate);
    fclose(OPTAB);
    fclose(SYMTAB);
    fclose(infoSave);
    return 0;
}