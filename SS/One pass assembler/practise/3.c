#include <stdio.h>
#include <string.h>

int main() {
    char name[20] = "aswin", new[20];
    printf("%d\n", strlen(name)+1);
    memcpy(new, name, strlen(name)+1);
    printf("%s\n", new);
    return 0;
}