#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct fileInfo {
    char name[20];
    struct fileInfo *link;
} files;

typedef struct dirInfo {
    char name[20];
    files *link;
} dirs;

typedef struct parentInfo {
    char name[20];
    int checked;
    dirs dir[50];
} parentI;

void newDir(parentI *parent, int *sz) {
    printf("\nEnter name of new dir: ");
    char name[20], str[] = "";
    scanf("%s", name);

    if (!(*parent).checked) {
        strcpy((*parent).name, name);
        (*parent).checked = 1;
        return;
    }
    strcpy((*parent).dir[*sz].name,name);
    (*parent).dir[(*sz)++].link = NULL;
}

void newFile(dirs dir[], int sz) {
    char name[20];
    printf("Enter name of dir: ");
    scanf("%s", name);
    int i;
    for (i = 0; i < sz; ++i) {
        if (!strcmp(name, dir[i].name)) 
            break;
    }
    if (i == sz) {
        printf("\nDir not found\n");
        return;
    }
    files *newNode = (files *)malloc(sizeof(files));
    printf("Enter name of file: ");
    scanf("%s", newNode->name);
    newNode->link = NULL;

    if (dir[i].link == NULL)  {
        dir[i].link = newNode;
        return;
    }       

    files *temp;
    for (temp = dir[i].link; temp->link != NULL; temp = temp->link);
    temp->link = newNode;
}

void display(dirs dir[], int sz) {
    printf("\ndName\tfNames..\n");
    for (int i = 0; i < sz; ++i) {
        printf("%s\t", dir[i].name);
        if (dir[i].link == NULL) {
            printf("emptyDir\n");
            continue;
        }
        for (files *temp = dir[i].link; temp != NULL; temp = temp->link) 
            printf("%s\t", temp->name);
        printf("\n");
    }   
    printf("\n");
}
void search(dirs dir[], int sz) {
    int present = 0;
    char name[20];
    printf("\nEnter name of file: ");
    scanf("%s", name);
    int i;
    for (i = 0; i < sz; ++i)  {
        for (files *temp = dir[i].link; temp != NULL; temp = temp->link) {
            if (!strcmp(temp->name,name)) {
                present = 1;
                break;
            }
        } 
        if (present)
            break;
    }

    if (present)
        printf("File is present in dir %s\n", dir[i].name);
    else 
        printf("File not present\n");
}

int main() {
    // freopen("2level.in", "rt", stdin);

    parentI parent;
    parent.checked = 0;
    printf("1) new Dir\n2) new file\n3) display\n4) search\n5) Exit\n");
    int choice, sz = 0;
    while (1) {
        printf("\nEnter one of options: ");
        scanf("%d", &choice);
        if (choice == 1)
            newDir(&parent, &sz);
        else if (choice == 2)
            newFile(parent.dir, sz);
        else if (choice == 3)
            display(parent.dir, sz);
        else if (choice == 4)
            search(parent.dir, sz);
        else if (choice == 5)
            break;
        else 
            printf("\nIncorrect input\n");
    }
    
}