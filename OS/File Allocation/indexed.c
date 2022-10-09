#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct fileInfo {
    char name[20];
    int size;
    int idxBlkNo;
} file;

void newFile(int blocks[], double block, int blockNo, int blkAll[][blockNo], file files[], int *fileNo) {
    srand(time(NULL));
    char name[20];
    double size;
    printf("Enter name of file: ");
    scanf("%s", name);
    printf("Enter size of file: ");
    scanf("%lf", &size);

    int fileSize = ceil( size/block );

    int randIdx;
    while(1) {
        randIdx = rand() % blockNo;
        if (blocks[randIdx] == -1)
           break; 
    }
    printf("\nrandidx: %d", randIdx);
    strcpy(files[*fileNo].name, name);
    files[*fileNo].size = fileSize;
    files[(*fileNo)++].idxBlkNo = randIdx;
    blocks[randIdx] = 1;

    int idxBlk = randIdx; // random no is saved to variable
    int j = 0;
    while(j < fileSize) {
        randIdx = rand() % blockNo; // new random no. for allocating blocks of file 
        if (blocks[randIdx] != -1)
            continue;
        blocks[randIdx] = 1;
        blkAll[idxBlk][randIdx] = 1;
        ++j; 
    }
}

void search(int blockNo, file files[],int blkAll[][blockNo]) {
    printf("Enter file name: ");
    char name[20];
    scanf("%s", name);
    int idx = -1;

    for (int i = 0; i < blockNo; ++i) {
        if (!strcmp(files[i].name,name)) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("\nFile not found\n");
        return;
    }
    int idxBlk = files[idx].idxBlkNo;
    printf("\nFile is present\nIndex block no: %d\n", idxBlk);
    printf("The blocks in which the file located: ");
    int val;
    for (int i = 0; i < blockNo; ++i) {
        val = blkAll[idxBlk][i];
        if (val == 1)
            printf("%d ", i);
    }
    printf("\n");
}

void display(int blockNo, file files[]) {
    printf("\nName\tsize\tIndxBlkNo\n");
    for (int i = 0; i < blockNo; ++i) {
        if (files[i].idxBlkNo == -1)
            continue;
        printf("%s\t%d\t%d\n", files[i].name, files[i].size,files[i].idxBlkNo);
    }
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
    int blkAll[blockNo][blockNo];

    int fileNo = 0, blocks[blockNo];
    for (int i = 0; i< blockNo; ++i) 
        blocks[i] = -1, files[i].idxBlkNo = -1;

    int choice;
    printf("\n1) New file\n2) Search file\n3) display\n4) exit\n");

    while(1) {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) 
            newFile(blocks, block, blockNo, blkAll, files, &fileNo);
        else if (choice == 2)
            search(blockNo, files, blkAll);
        else if (choice == 3)
            display(blockNo, files);
        else if (choice == 4)
            break;
        else 
            printf("\nIncorrect input\n");
    }
    return 0;
}