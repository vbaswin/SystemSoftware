#include <stdio.h>
#include <string.h>

int main() {
    char code[10] = "X'FfF", ch;

    char new[10] = {0};
    for (int i = 1; i < strlen(code); ++i) {
        ch = code[i];
        if (ch == '\'')
            continue;
        strcat(new, &ch); 
    }
    printf("%s\n", new);
}