//Id 205686538 Snir David Nahari
//

#include <stdio.h>
#include <string.h>
#include "ex2.h"

void checkFlag(char* flag, char* flagCode, char** flagList) {
    if (strcmp(flag,flagList[0]) == 0) {
        flagCode[0] = 0x0a;
    }
    if (strcmp(flag,flagList[1]) == 0) {
        flagCode[0] = 0x0d;
    }
    if (strcmp(flag,flagList[2]) == 0) {
        flagCode[0] = 0x0d;
        flagCode[1] = 0x0a;
    }
}

int checkBOM(char* bom) {

}

void twoArgs(char *oldFileName, char *newFileName){
    FILE *oldFilePtr, *newFilePtr;
    char buffer[1];
    oldFilePtr = fopen(oldFileName, "rb");
    newFilePtr = fopen(newFileName, "wb");
    while (fread(buffer, 1, 1, oldFilePtr) != 0){
        fwrite(buffer, 1, 1, newFilePtr);
    }
    fclose(newFilePtr);
    fclose(oldFilePtr);
}

void fourArgs(char *oldFileName, char *newFileName, char* srcOsFlag, char* destOsFlag){
    FILE *oldFilePtr, *newFilePtr;
    char srcCode[2] = {0}, destCode[2] = {0}, zeroPad = 0x00;
    char *flagsList[] = {"-unix", "-mac", "-win"};
    char buffer[1];
    checkFlag(srcOsFlag, srcCode, flagsList);
    checkFlag(destOsFlag, destCode, flagsList);
    oldFilePtr = fopen(oldFileName, "rb");
    newFilePtr = fopen(newFileName, "wb");
    while (fread(buffer, 1, 1, oldFilePtr) != 0){
        if (buffer[0] != srcCode[0]) {
            fwrite(buffer, 1, 1, newFilePtr);
        } else {
            if (strcmp(destOsFlag, flagsList[2])){
                fwrite(destCode, 1 , 1, newFilePtr);
            } else {
                fwrite(destCode, 1 , 1, newFilePtr);
                fwrite(&zeroPad, 1 , 1, newFilePtr);
                fwrite(destCode+1, 1 , 1, newFilePtr);
            }
        }
    }
    fclose(newFilePtr);
    fclose(oldFilePtr);
}
void fiveArgs(char *oldFileName, char *newFileName, char* srcOsFlag, char* destOsFlag, char* byteOrderFlag){

}

int main(int argc, char *argv[]) {
    switch (argc) {
        case 3:
            twoArgs(argv[1], argv[2]);
            break;
        case 5:
            fourArgs(argv[1], argv[2], argv[3], argv[4]);
            break;
        case 6:
            fiveArgs(argv[1], argv[2], argv[3], argv[4], argv[5]);
            break;
        default:
            break;
    }
    return 0;
}