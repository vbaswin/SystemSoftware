#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct symtab {
    char labelName[20];
    char address[20];
    int present;
    struct symtab *link;
    struct symtab *next;
} sym;

void saveSym(sym **SYMTAB, char label[], int LOCCTR) {
    char strLocctr[20];
    sprintf(strLocctr,"%04X", LOCCTR);

    sym *newNode = (sym *)malloc(sizeof(sym));
    memcpy(newNode->labelName,label, strlen(label));
    memcpy(newNode->address, strLocctr, strlen(strLocctr));
    newNode->present = 1;
    newNode->link = NULL;
    newNode->next = NULL;
    if (!*SYMTAB) {
        *SYMTAB = newNode;
        return;
    }
    sym *temp;
    for (temp = *SYMTAB; temp->next != NULL; temp = temp->next);
    temp->next = newNode;
}
int main() {
    sym *SYMTAB = NULL;
    saveSym(&SYMTAB, "chumma", 4096);

    saveSym(&SYMTAB, "second", 1435);
    sym *temp;
    for (temp = SYMTAB; temp != NULL; temp = temp->next)
            printf("%s %s\n", temp->labelName, temp->address);
    return 0;
}