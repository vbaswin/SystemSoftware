#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct fileInfo {
    char name[20];
    int size;
    struct blkLink *link1;
    struct fileInfo *link2;
} files;

typedef struct blkLink {
    int blockNo;
    struct blkLink *link;
} blkLk;



void newFile(int blocks[], double block, int blockNo,  files **start) {
    srand(time(NULL));
    char name[20];
    double size;
    printf("Enter name of file: ");
    scanf("%s", name);
    printf("Enter size of file: ");
    scanf("%lf", &size);

    int fileSize = ceil( size/block );

    files *newNode = (files *)malloc(sizeof(files));
    strcpy(newNode->name, name);
    newNode->size = size;
    newNode->link1 = NULL;
    newNode->link2 = NULL;

    if (*start == NULL) {
        *start = newNode;
    }
    else {
        files *i;
        for (i = *start; i->link2 != NULL; i = i->link2);
        i->link2 = newNode;
    }

    blkLk *head = NULL;
    blkLk *temp = head, *newnode;


    int randIdx, j = 0;
    while(j < fileSize) {
        randIdx = rand() % blockNo; // new random no. for allocating blocks of file 
        if (blocks[randIdx] != -1)
            continue;
        blocks[randIdx] = 1;

        newnode = (blkLk *)malloc(sizeof(blkLk));
        newnode->blockNo = randIdx;
        newnode->link = NULL;

        if (head == NULL) {
            head = newnode;
            newNode->link1 = head;
            temp = newnode;
            ++j;
            continue;
        }
        temp->link = newnode;
        temp = newnode;
        ++j; 
    }
}

void search(files *start) {
    printf("Enter file name: ");
    char name[20];
    scanf("%s", name);
    int idx = -1;

    files *i;
    for (i = start; i != NULL; i = i->link2) {
        if (!strcmp(i->name,name))
            break;  
    }

    if (i == NULL) {
        printf("\nFile not found\n");
        return;
    }
    printf("\nFile is present\n");
    printf("The blocks in which the file located: ");

    blkLk *j;
    for (j = i->link1; j!= NULL; j = j->link)
        printf("%d ", j->blockNo);
    printf("\n");
}

void display(files *start) {
    printf("\nName\tsize\n");
    files *i;
    for (i = start; i != NULL; i = i->link2)
        printf("%s\t%d\n", i->name, i->size);
}

int main() {
    freopen("c.in", "rt", stdin);
    double disk, block;
    printf("Enter the size of disk: ");
    scanf("%lf", &disk);

    printf("Enter block size: ");
    scanf("%lf", &block);

    int blockNo = ceil(disk / block);

    files *start = NULL;

    int fileNo = 0, blocks[blockNo];
    for (int i = 0; i< blockNo; ++i)
        blocks[i] = -1;

    int choice;
    printf("\n1) New file\n2) Search file\n3) display\n4) exit\n");

    while(1) {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) 
            newFile(blocks, block, blockNo, &start);
        else if (choice == 2)
            search(start);
        else if (choice == 3)
            display(start);
        else if (choice == 4)
            break;
        else 
            printf("\nIncorrect input\n");
    }
    return 0;
}