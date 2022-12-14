#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void intputFile(FILE *input, FILE *output) {
    printf("Enter load address: ");
    int start_addr;
    scanf("%X", &start_addr);
    char ch, head[20], name[20];
    int num, start, p_len;
    fscanf(input, "%s %s %X %X", head, name, &start, &p_len);
    start += start_addr;
    int len = start;

    int cur = 2;
    while ((ch = getc(input)) != EOF) {
        if (ch == ' ' || ch == '\n')
            continue;
        if (ch == 'E')
            break;
        if (cur == 2) {
            if (len != start) {
                fprintf(output, "\n");
            }
            fprintf(output, "%X\t", len);
            cur = 0;
            len += 1;
        }
        if (ch == 'T') {
            fscanf(input, "%s %s", name, name);
            continue;
        }
        if (ch == ' ' || ch == '\n')
            continue;
        fprintf(output, "%c", ch);
        ++cur;
    }
}

int main() {
    FILE *hi, *out;
    hi = fopen("input.txt", "r+");
    out = fopen("output.txt", "w+");

    char name[100];
    rewind(hi);
    intputFile(hi, out);
    fclose(hi);
    fclose(out);

    return 0;
}