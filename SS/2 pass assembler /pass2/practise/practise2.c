#include <stdio.h>
#include <string.h>

int main() {
    char code[10] = "C'EOF'", ch;
    // printf("%lu\n", strlen(code));

    char byteAscii[100] = {0};
    char asciiCode[3];
    for (int i = 1; i < strlen(code); ++i) {
        ch = code[i]; 
        if (ch == '\'')
            continue;
        sprintf(asciiCode, "%0X", ch);
        strcat(byteAscii, asciiCode);
    }
    printf("%s\n", byteAscii);

}