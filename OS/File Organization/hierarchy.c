#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct fileInfo {
    char name[20];
    struct fileInfo *link;
} files;

typedef struct dirInfo {
    char name[20];
    struct dirInfo *linkDir;
    struct dirInfo *childDir;
    files *childFls;
} dirs;

void search(dirs *dir, char name[], dirs **ptr) {
    if (dir == NULL || *ptr != NULL)
        return;
    if (!strcmp(dir->name, name)) {
        *ptr = dir;
        return;
    }
    search(dir->linkDir, name, ptr);
    search(dir->childDir, name, ptr);
}

dirs *newDir(dirs *dir) {
    dirs *newNode = (dirs *)malloc(sizeof(dirs));
    printf("\nEnter name of new dir: ");
    scanf("%s", newNode->name);

    newNode->linkDir = NULL;
    newNode->childDir = NULL;
    newNode->childFls = NULL;
    if (dir == NULL) 
        dir = newNode;
    else {
        int option = 0;
        printf("Is it child of another dir[1=>yes]: ");
        scanf("%d", &option);
        if (!option) {
            dirs *temp;
            if (dir->childDir == NULL) {
                dir->childDir = newNode;
            }
            else {
                for (temp = dir->childDir; temp->linkDir != NULL; temp = temp->linkDir);
                temp->linkDir = newNode;
            }
        }
        else {

            char name[20];
            printf("Enter name of dir: ");
            scanf("%s", name);


            dirs *ptr = NULL;
            search(dir, name, &ptr);
            if (!ptr) {
                printf("\nDir not found\n");
                return dir;
            }
            else  {
                if (ptr->childDir == NULL) {
                    ptr->childDir = newNode;
                }
                else {
                    dirs *temp;
                    for (temp = ptr->childDir; temp->linkDir != NULL; temp = temp->linkDir);
                    temp->linkDir = newNode;
                }
            }
        }
    }
    printf("\nDir created successfully\n");
    return dir;
}


void newFile(dirs **dir) {
    char name[20];
    printf("Enter name of dir: ");
    scanf("%s", name);
    dirs *ptr = NULL;
    dirs *me = *dir;
    search(me, name, &ptr);
    if (!ptr) {
        printf("\nDir not found\n");
        return;
    }
    
    files *newNode = (files *)malloc(sizeof(files));
    printf("Enter name of file: ");
    scanf("%s", newNode->name);
    newNode->link = NULL;

    if (ptr->childFls == NULL)  
        ptr->childFls = newNode;
    else { 
        files *temp;
        for (temp = ptr->childFls; temp->link != NULL; temp = temp->link);
        temp->link = newNode;
    }
    printf("File created successfully\n");
    return;
}

void display(dirs *dir) {

    if (dir == NULL)
        return;

    printf("\nName: %s\tCldDrs: ", dir->name);
    for (dirs *temp = dir->childDir; temp != NULL; temp = temp->linkDir)
        printf("%s ", temp->name);

    printf("\tCldFls: ");
    for (files *temp = dir->childFls; temp != NULL; temp = temp->link)
        printf("%s ", temp->name);

    display(dir->linkDir);
    display(dir->childDir);
}


void flSearch(dirs *dir, char name[], dirs **parent) {
    if (dir == NULL || *parent != NULL)
        return;
    if (dir->childFls) {
        for (files *temp = dir->childFls; temp != NULL; temp = temp->link) {
            if (!strcmp(temp->name, name)) {
                *parent = dir;
                return;
            }
        }
    }
    flSearch(dir->linkDir, name, parent);
    flSearch(dir->childDir, name, parent);
}
void searchFile(dirs *dir) {
    printf("Enter name of file: ");
    char name[20];
    scanf("%s", name);
    dirs *parent = NULL;
    flSearch(dir, name, &parent);
    if (!parent) {
        printf("\nFile not found\n");
        return;
    }
    printf("\nFile present in dir: %s\n", parent->name);
}


int main() {
    freopen("hie.in", "rt", stdin);

    dirs *dir = NULL;
    printf("1) new Dir\n2) new file\n3) display\n4) search\n5) Exit\n");
    int choice, sz = 0;
    while (1) {
        printf("\nEnter one of options: ");
        scanf("%d", &choice);
        if (choice == 1)
            dir = newDir(dir);
        else if (choice == 2)
            newFile(&dir);
        else if (choice == 3) {
            display(dir);
            printf("\n");
        }
        else if (choice == 4) 
            searchFile(dir);
        else if (choice == 5)
            break;
        else 
            printf("\nIncorrect input\n");
    }
    
}