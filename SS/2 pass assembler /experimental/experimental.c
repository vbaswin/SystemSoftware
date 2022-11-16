#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct line{
    int address;
    char label[10];
    char opcode[10];
    char operand[10];
}inputLine;

struct outLine{
    int startingAddress;
    int length;
    int opcode[10];
    int operandAdd[10];
}textRecord;

void readFrom(FILE *);
void writeTo(FILE *);
int search(char * key, FILE *);
void initializeTextRecord();
int calculateHexValue();
int calculateIntValue();

int main(){
    char programName[8];
    char temp[20];
    int outLineCounter=0;
    int found = 0;

    int pgmStartingAddress, locCtr, length, pgmLength;
    FILE *output, *intermediate, *symTab, *opTab, *infoSave;

    symTab = fopen("symTab.txt", "r");
    opTab = fopen("opTab.txt", "r");
    intermediate = fopen("intermediate.txt", "r");
    output = fopen("output.txt", "w");
    infoSave = fopen("infoSave.txt", "r");
    if(output == NULL){
        printf("\nERROR OPENING FILE");
        exit(1);
    }

    readFrom(intermediate);
    if(strcmp(inputLine.opcode, "START")==0){
        strcpy(programName, inputLine.label);    
        textRecord.length = 0;
        fscanf(infoSave, "%s %X", temp, &pgmLength);
        initializeTextRecord();
        pgmStartingAddress = (int)strtol(inputLine.operand, NULL, 16);
        textRecord.startingAddress = pgmStartingAddress;
        readFrom(intermediate);
    }
    fprintf(output, "H %-6s%0000006X %0000006X\n", programName, pgmStartingAddress, pgmLength);
    while(strcmp(inputLine.opcode, "END")!=0){
        if(textRecord.startingAddress == -1){
            textRecord.startingAddress = inputLine.address;
        }
        found = search(inputLine.opcode, opTab);
        if(found!=-1){
            textRecord.length += 3;
            textRecord.opcode[outLineCounter] = found;
            if(strcmp(inputLine.operand, "**")!=0){
                found = search(inputLine.operand, symTab);
                printf("\noperand: %s %d", inputLine.operand ,found);
                if(found==-1){
                    // printf("\n%s SYMBOL NOT FOUND", inputLine.operand);
                    readFrom(intermediate);
                    // exit(1);
                }
                else{
                    textRecord.operandAdd[outLineCounter] = found;
                }
            }else{
                textRecord.operandAdd[outLineCounter] = 0;
            }
        }else if(strcmp(inputLine.opcode, "BYTE")==0){
            if(inputLine.operand[0]=='C'){
                textRecord.length += 3;
                textRecord.opcode[outLineCounter] = -2;
                textRecord.operandAdd[outLineCounter] = calculateHexValue();
            }else if(inputLine.operand[0]=='X'){
                textRecord.opcode[outLineCounter] = -3;
                char tempStr[3];
                for(int i=2;inputLine.operand[i]!='\'';i++){
                    tempStr[i-2] = inputLine.operand[i];
                }
                textRecord.length += (strlen(tempStr)/2);
                textRecord.operandAdd[outLineCounter] = calculateIntValue();
            }
        }else if(strcmp(inputLine.opcode, "WORD")==0){
            textRecord.length += 3;
            textRecord.opcode[outLineCounter] = -2;
            textRecord.operandAdd[outLineCounter] = atoi(inputLine.operand);
        }
        outLineCounter++;
        if(outLineCounter>9){
            writeTo(output);
            initializeTextRecord(textRecord.operandAdd[outLineCounter]);
            outLineCounter = 0;
        }
        readFrom(intermediate);
    }
    if(outLineCounter < 10){
        writeTo(output);
        initializeTextRecord(textRecord.operandAdd[outLineCounter]);
        outLineCounter = 0;
    }
    fprintf(output, "E %0000006X", pgmStartingAddress);

    fclose(infoSave);
    fclose(output);
    fclose(symTab);
    fclose(opTab);
    fclose(intermediate);
    printf("\nPASS 2 SUCCESSFUL");
    return 0;
}

void initializeTextRecord(){
    textRecord.startingAddress = -1;
    textRecord.length = 0;
    memset(textRecord.opcode, -1, 10*sizeof(int));
}
void readFrom(FILE * fin){
    fscanf(fin, "%X %s %s %s", &inputLine.address ,inputLine.label, inputLine.opcode, inputLine.operand);
}

void writeTo(FILE * fout){
    fprintf(fout, "T %0000006X %002X", textRecord.startingAddress, textRecord.length);
    for(int i=0;textRecord.opcode[i]!=-1 && i<10;i++){
        if(textRecord.opcode[i] == -2){
            fprintf(fout, " %0000006X", textRecord.operandAdd[i]);
        }else if(textRecord.opcode[i] == -3){
            fprintf(fout, " %002X", textRecord.operandAdd[i]);
        }
        else{
            fprintf(fout, " %002X%00004X", textRecord.opcode[i], textRecord.operandAdd[i]);
        }
    }
    fprintf(fout, "\n");
}
int search(char * key, FILE * file){
    int flag = 1;
    for(int i=0; i<strlen(key); i++){
        if(key[i] == ','){
            key[i] = '\0';
            flag = 0;
        }
    }
    char fileKey[10];
    int fileKeyValue;
    rewind(file);
    while(fscanf(file, "%s %X", fileKey, &fileKeyValue)!=-1){
        if(strcmp(fileKey, key)==0){
            if(flag == 0){
                return (fileKeyValue+(8*4096));
            }
            return fileKeyValue;
        }
    }
    return -1;
}
int calculateHexValue(){
    FILE * fptr;
    fptr = fopen("text.txt", "w+");
    for(int i=2; inputLine.operand[i]!='\'';i++){
        fprintf(fptr,"%2X", inputLine.operand[i]);
    }
    fclose(fptr);
    fptr = fopen("text.txt", "r");
    int val ;
    fscanf(fptr, "%X", &val);
    fclose(fptr);
    remove("text.txt");
    return val;
}
int calculateIntValue(){
    FILE * fptr;
    fptr = fopen("text.txt", "w+");
    char temp[10];
    for(int i=2; inputLine.operand[i]!='\'';i++){
        temp[i-2]=inputLine.operand[i];
    }
    fprintf(fptr, "%s", temp);
    fclose(fptr);
    fptr = fopen("text.txt", "r");
    int val ;
    fscanf(fptr, "%X", &val);
    fclose(fptr);
    remove("text.txt");
    return val;
}