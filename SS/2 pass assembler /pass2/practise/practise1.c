#include <stdio.h>

int main() {
    int num = 4096;
    char word[100];
    sprintf(word, "Hi the value is: %010X\n", num);
    printf("%s", word);
    snprintf(word,50, "Hi the value is: %X", num);
    printf("%s", word);
}