#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct fileInfo {
    char name[20];
    int size;
    int startIdx;
} file;

void newFile(int blocks[], double block, int blockNo, file files[], int *fileNo) {
    srand(time(NULL));
    char name[20];
    double size;
    printf("Enter name of file: ");
    scanf("%s", name);
    printf("Enter size of file: ");
    scanf("%lf", &size);

    int fileSize = ceil( size/block );

    int randIdx, available;
    while(1) {
        available = 1;
        randIdx = rand() % blockNo;
        for (int i = 0; i < fileSize; ++i) {
            if (blocks[randIdx+i] != -1) {
                available = 0;
                break;
            }
        }
        if (available)
            break;
    }
    strcpy(files[*fileNo].name, name);
    files[*fileNo].startIdx = randIdx;
    files[(*fileNo)++].size = fileSize;
    for (int i = 0; i < fileSize; ++i) 
        blocks[randIdx+i] = 1;
}

void search(int fileNo, file files[]) {
    printf("Enter file name: ");
    char name[20];
    scanf("%s", name);
    int idx = -1;

    for (int i = 0; i < fileNo; ++i) {
        if (!strcmp(files[i].name,name)) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("\nFile not found\n");
        return;
    }
    printf("\nFile is present\nStart block no: %d\n", files[idx].startIdx);
}

void display(file files[], int fileNo) {
    printf("\nName\tsize\tstartIdx\n");
    for (int i = 0; i < fileNo; ++i) 
        printf("%s\t%d\t%d\n", files[i].name, files[i].size,files[i].startIdx);
    printf("\n");
}

int main() {
    freopen("c.in", "rt", stdin);
    double disk, block;
    printf("Enter the size of disk: ");
    scanf("%lf", &disk);

    printf("Enter block size: ");
    scanf("%lf", &block);

    int blockNo = ceil(disk / block);

    file files[blockNo];

    int fileNo = 0, blocks[blockNo];
    for (int i = 0; i< blockNo; ++i) 
        blocks[i] = -1;

    int choice;
    printf("\n1) New file\n2) Search file\n3) display\n4) exit\n");

    while(1) {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) 
            newFile(blocks, block, blockNo, files, &fileNo);
        else if (choice == 2)
            search(fileNo, files);
        else if (choice == 3)
            display(files, fileNo);
        else if (choice == 4)
            break;
        else 
            printf("\nIncorrect input\n");
    }
    return 0;
}