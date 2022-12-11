#include <stdio.h>
#include <string.h>

int parse (char code[][10], FILE *source) {
    int i = 0, j = 0, k = 0;
    int next_token = 0;
    char ch;
    while ((ch = fgetc(source)) != '\n') {
        if(ch == ' ' ||ch == '\t'){
            if(!next_token){
                next_token = 1;
                code[j][k] = 0;
                k = 0;
                j++;
            }
        }
        else{
            next_token = 0;
            code[j][k++] = ch;
        }
        i++;
    }
    code[j][k] = 0;
    return j+1;
}

int main() {
    FILE *source = fopen("aswin.txt", "rt");
    char code[100][10];
    int len;
    len = parse(code, source);
    printf("\ninside\n");
    for (int i = 0; i < len; ++i)
        printf("%s ", code[i]);

    return 0;
}