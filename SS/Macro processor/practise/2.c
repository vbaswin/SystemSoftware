#include <stdio.h>

void passByPointer(FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    fprintf(fp,"top\n");
    fseek(fp, 0, SEEK_END);
    fprintf(fp,"doing fine\n");
    // fseek(fp, 0, SEEK_SET);

}

int main() {
    FILE *fp = fopen("hi.txt", "w+");

    fprintf(fp,"hi\nAswin here I am doing programming\n");
    passByPointer(fp);
    fprintf(fp,"middle\n");

    fclose(fp);
}