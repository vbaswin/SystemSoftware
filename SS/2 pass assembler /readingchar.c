#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DELIM ' '
// int read_codeSplit(FILE *source, char code[10][10]) {
//     int len = 0;

//     char c;
//     while ((c = fgetc(source)) != '\n') {
//         if (strlen(code[len][10]) != 0 && c == ' ')
//             ++len;
//         strcat(code[len][10], c);
//     }
    
//     return len;
//}

int strsplit(FILE *f, char *code[10]){
    // char c;
    // int len = 0;
    // while((c = fgetc(f)) != '\n'){
    //     printf("%c", c);
    //     if(c == ' ' && strlen(code[len]) != 0){
    //         ++len; 
    //         continue;
    //     }
    //     strcat(code[len],&c);
    //     printf("%s", code[len]);
    // }

    char str[100];
    fgets(str, 100, f);

    while ()

    return len;
}


int main() {
    FILE *source = fopen("source.txt", "r");


    int len;

    char *code[10];
    len = strsplit(source, code);

    printf("inside main\n");
    for (int i = 0; i < len; ++i)
        printf("%s ", code[i]);

    printf("%d", !strcmp(code[1], "START"));


    return 0;
}