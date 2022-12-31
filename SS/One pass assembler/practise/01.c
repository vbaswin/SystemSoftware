#include <stdio.h>

void anotherFunction(FILE *source) {
    rewind(source);
    char first[20], second[20], third[20];
    fscanf(source, "%s %s %s", first, second, third);
    printf("%s %s %s\n", first, second, third);

    fseek(source,0,SEEK_SET);
    fprintf(source, "I am Aswin V B\n");
    int now;
    scanf("%d", &now);

    fseek(source,0,SEEK_END);
    fprintf(source, "%d\n", now);

    fprintf(source, "I also like smiling :)\n");
    fclose(source);
}

int main() {
    FILE *source = fopen("source.txt", "w+");
    fprintf(source, "I am Aswin\nI love programming\n");
    // fclose(source);
    // fprintf(source, "Good \n");
    anotherFunction(source);

    fprintf(source, "To check if it actually closed or not");
    fclose(source);
}