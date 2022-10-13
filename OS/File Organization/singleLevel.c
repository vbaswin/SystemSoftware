#include <stdio.h>
#include <string.h>

typedef struct fileInfo {
    char name[20];
    int sz;
} files;

typedef struct dirInfo {
    char name[20];
    files file[50];
} dirs;

void newFile(files file[], int *sz) {
    printf("\nEnter name of new file: ");
    scanf("%s", file[*sz].name);
    printf("Enter size of new file: ");
    scanf("%d", &file[(*sz)++].sz);
}

void display(files file[], int sz) {
    printf("\nName\tsz\n\n");
    for (int i = 0; i < sz; ++i) 
        printf("%s\t%d\n", file[i].name, file[i].sz);
    printf("\n");
}
void search(files file[], int sz) {
    int present = 0;
    char name[20];
    printf("\nEnter name of file: ");
    scanf("%s", name);
    for (int i = 0; i < sz; ++i)  {
        if (!strcmp(file[i].name,name)) {
            present = 1;
            break;
        }
    }

    if (present)
        printf("File is present\n");
    else 
        printf("File not present\n");
}

int main() {

    printf("Enter the name of dir: ");
    char name[20];
    dirs dir;
    scanf("%s", dir.name);

    printf("1) new file\n2) display\n3) search\n4) Exit\n");
    int choice, sz = 0;
    while (1) {
        printf("\nEnter one of options: ");
        scanf("%d", &choice);
        if (choice == 1) 
            newFile(dir.file, &sz);
        else if (choice == 2)
            display(dir.file, sz);
        else if (choice == 3) 
            search(dir.file, sz);
        else if (choice == 4)
            break;
        else 
            printf("\nIncorrect input\n");
    }
    
}