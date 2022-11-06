#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// char ** codeSplit(char str[], int *len){

//     char ** code = malloc(10 * sizeof(char *));
//     for (int i = 0; i < 10; ++i) 
//         code[i] = malloc(10 * sizeof(char *));

//     char *hi = strtok(str, " ");

//     while (hi != NULL) {
//         // memcpy(code[*len], hi, 10);
//         // strcpy(code[len], hi);
//         // hi = strtok(NULL, " ");
//         code[(*len)++] = hi;
//         hi = strtok(NULL, " ");
//     }
//     // printf("%s", code[len]);
//     // printf("%s %lu", hi, strlen(hi));

//     return code;
// }

// int codeSplit(char str[], char code[][10]) {

//     int len = 3;
//     strcpy(code[0], "one");
//     strcpy(code[1], "two");
//     strcpy(code[10], "three");

//     return len;
// }

char ** read_codeSplit(char str[], int *len){
    *len = 0;

    char ** code = malloc(10 * sizeof(char));
    for (int i = 0; i < 10; ++i) 
        code[i] = malloc(10 * sizeof(char));

    char *hi = strtok(str, " ");

    while (hi != NULL) {
        code[(*len)++] = hi;
        hi = strtok(NULL, " ");
    }
    for (int i = 0; i < *len; ++i)
        printf("%s ", code[i]);
    return code;
}


int main() {
    FILE *source = fopen("source.txt", "r");
    // char str[50] = "I am going to learn c++ soooooon oh yeah";
    // int now = 0X009749;
    // printf("%s\n%06X\n", str, now);

    int len;
        char str[100];
        fgets(str, 100, source);
        printf("%s", str);

    char **code;
    code = read_codeSplit(str, &len);

    printf("inside main\n");
    for (int i = 0; i < len; ++i)
        printf("%s ", code[i]);

    printf("%d", !strcmp(code[1], "START"));

    // char *code[10];
    // code[len] = strtok(str, " ");

    
    // while (code[len] != NULL) 
    //     code[++len] = strtok(NULL, " ");

    return 0;
}