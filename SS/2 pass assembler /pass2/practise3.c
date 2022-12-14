#include <stdio.h>

int main() {
    FILE *SYMTAB = fopen("symtab.txt", "r+");
    rewind(SYMTAB);
    char label[20], address[20];
    while (fscanf(SYMTAB, "%s %04X", label, address) != EOF) {
        if (!strcmp("EXIT", label)) 
            return address;
    }
    return -1;
}