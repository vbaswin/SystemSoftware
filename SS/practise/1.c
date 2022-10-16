#include <stdio.h>

int main() {
    FILE *fptr;
    fptr = fopen("aswin.txt", "r+");
    char str[50];
    while (fgets(str, 50, fptr) != NULL) 
        printf("%s", str);
    fseek(fptr, 0, SEEK_SET);
    while (fgets(str, 50, fptr) != NULL) 
        printf("%s", str);
    fclose(fptr);
    return 0;
}