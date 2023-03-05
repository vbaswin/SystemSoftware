#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char code[100] = "0f";
    int n = (int)strtol(code,NULL,16);

    char word[10], newWord[10];
    sprintf(word,"%04X",n);

    printf("%d\n", n);
    printf("%s\n", word);

    memcpy(newWord,word,strlen(word)+1);
    printf("%s\n", newWord);
    return 0;
}