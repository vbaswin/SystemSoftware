#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
    freopen("input.in", "rt", stdin);
    double a, b;
    scanf("%lf %lf", &a, &b);
    int c = ceil( a/b );
    printf("%d\n", c);

    char name[20];
    scanf("%s", name);
    printf("%s", name);
    return 0;
}