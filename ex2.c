//Id 205686538 Snir David Nahari
//

#include <stdio.h>
#include <string.h>
#include "ex2.h"

/* check Flag getting flag, flag Code and flag List - change the flag code return void.
 * current flag list - win, unix, mac */
void checkFlag(char *flag, char *flagCode, char **flagList) {
    //checking if the flag is -win, -mac, -unix
    //if unix - flag code = '\n'
    if (strcmp(flag, flagList[0]) == 0) {
        flagCode[0] = 0x0a;
    }
    //if mac - flag code = '\r'
    if (strcmp(flag, flagList[1]) == 0) {
        flagCode[0] = 0x0d;
    }
    //if win - flag code = '\r\n'
    if (strcmp(flag, flagList[2]) == 0) {
        flagCode[0] = 0x0d;
        flagCode[1] = 0x0a;
    }
}

/* check BOM bytes getting isBig (boolean), buffer to read into, source file and destination file
 * writing the dest file in opposite endian from source file */
void checkBomAndSwap(int *isBig, char *buffer, FILE *oldFile, FILE *newFile) {
    char firstChar = 0xFE, secondChar = 0xFF;
    //reading first byte in file
    fread(buffer, 1, 1, oldFile);
    //if first char is FE - text in Big Endian
    if (buffer[0] == firstChar) {
        //Swap to Little Endian
        fwrite(&secondChar, 1, 1, newFile);
        //reading second byte - so it won't be written
        fread(buffer, 1, 1, oldFile);
        fwrite(&firstChar, 1, 1, newFile);
        //changing isBig to 1 - source file is in Big Endian
        isBig = 1;
    } else {     //if first char is FF - text in Little Endian
        //Swap to Big Endian
        fwrite(&firstChar, 1, 1, newFile);
        //reading second byte - so it won't be written
        fread(buffer, 1, 1, oldFile);
        fwrite(&secondChar, 1, 1, newFile);
        //changing isBig to 0 - source file is in Little Endian
        isBig = 0;
    }
}

/* reading source file and writing it to a new file */
void ReadAndSave(char *oldFileName, char *newFileName) {
    FILE *oldFilePtr, *newFilePtr;
    //buffer for reading from file
    char buffer[1];
    //opening file in right mode
    oldFilePtr = fopen(oldFileName, "rb");
    //if there is no file with this name
    if (oldFilePtr == NULL) {
        return;
    }
    newFilePtr = fopen(newFileName, "wb");
    //while there is bytes to read - read and writing to the new file
    while (fread(buffer, 1, 1, oldFilePtr) != 0) {
        fwrite(buffer, 1, 1, newFilePtr);
    }
    //closing files
    fclose(newFilePtr);
    fclose(oldFilePtr);
}

/* reading source file in src flag coding and writing it to a new file in dest flag coding  */
void changeCodingFile(char *oldFileName, char *newFileName, char *srcOsFlag, char *destOsFlag) {
    FILE *oldFilePtr, *newFilePtr;
    char srcCode[2] = {0}, destCode[2] = {0}, zeroPad = 0x00,
            *flagsList[] = {"-unix", "-mac", "-win"}, buffer[1];
    //checking src and dest flags
    checkFlag(srcOsFlag, srcCode, flagsList);
    checkFlag(destOsFlag, destCode, flagsList);
    //opening file in right mode
    oldFilePtr = fopen(oldFileName, "rb");
    //if there is no file with this name
    if (oldFilePtr == NULL) {
        return;
    }
    newFilePtr = fopen(newFileName, "wb");
    //while there is bytes to read - read and writing to the new file
    while (fread(buffer, 1, 1, oldFilePtr) != 0) {
        //checking if byte is not new line in src flag - if not write it to new file
        if (buffer[0] != srcCode[0]) {
            fwrite(buffer, 1, 1, newFilePtr);
        } else { //if byte is new line in src flag - change it to the dst flag
            //checking in dest flag is not win
            if (strcmp(destOsFlag, flagsList[2])) {
                fwrite(destCode, 1, 1, newFilePtr);
                //checking if source flag is win - read more 2 bytes for 00, \n
                if (strcmp(srcOsFlag, flagsList[2]) == 0) {
                    fread(buffer, 1, 1, oldFilePtr);
                    fread(buffer, 1, 1, oldFilePtr);
                }
            } else { //dest flag is win - write 3 byte - '\r' '00' '\n'
                fwrite(destCode, 1, 1, newFilePtr);
                fwrite(&zeroPad, 1, 1, newFilePtr);
                fwrite(destCode + 1, 1, 1, newFilePtr);
            }
        }
    }
    //closing files
    fclose(newFilePtr);
    fclose(oldFilePtr);
}

/* reading source file in src flag coding and writing it to a new file in dest flag coding.
 * also checking if flag is keep and swap. if swap than changing from one endian to another.*/
void
changeCodingFileWithSwap(char *oldFileName, char *newFileName, char *srcOsFlag, char *destOsFlag, char *byteOrderFlag) {
    FILE *oldFilePtr, *newFilePtr;
    char srcCode[2] = {0}, destCode[2] = {0}, zeroPad = 0x00,
            *flagsList[] = {"-unix", "-mac", "-win"}, keep[] = {"-keep"};
    int isBig;
    //checking src and dest flags
    checkFlag(srcOsFlag, srcCode, flagsList);
    checkFlag(destOsFlag, destCode, flagsList);
    //opening file in right mode
    oldFilePtr = fopen(oldFileName, "rb");
    //if there is no file with this name
    if (oldFilePtr == NULL) {
        return;
    }
    newFilePtr = fopen(newFileName, "wb");
    // if flags is keep
    if (strcmp(byteOrderFlag, keep) == 0) {
        char buffer[1];
        //while there is bytes to read - read and writing to the new file
        while (fread(buffer, 1, 1, oldFilePtr) != 0) {
            //checking if byte is not new line in src flag - if not write it to new file
            if (buffer[0] != srcCode[0]) {
                fwrite(buffer, 1, 1, newFilePtr);
            } else { //if byte is new line in src flag - change it to the dst flag
                //checking in dest flag is not win
                if (strcmp(destOsFlag, flagsList[2])) {
                    fwrite(destCode, 1, 1, newFilePtr);
                    //checking if source flag is win - read more 2 bytes for 00, \n
                    if (strcmp(srcOsFlag, flagsList[2]) == 0) {
                        fread(buffer, 1, 1, oldFilePtr);
                        fread(buffer, 1, 1, oldFilePtr);
                    }
                } else { //dest flag is win - write 3 byte - '\r' '00' '\n'
                    fwrite(destCode, 1, 1, newFilePtr);
                    fwrite(&zeroPad, 1, 1, newFilePtr);
                    fwrite(destCode + 1, 1, 1, newFilePtr);
                }
            }
        }
    } else { // if flag is swap
        char buffer[2];
        checkBomAndSwap(&isBig, buffer, oldFilePtr, newFilePtr);
        //while there is bytes to read - read and writing to the new file
        while (fread(buffer, 1, 2, oldFilePtr) != 0) {
            if (buffer[0] != srcCode[0] && buffer[1] != srcCode[0]) {
                //swap every two bytes
                fwrite(buffer + 1, 1, 1, newFilePtr);
                fwrite(buffer, 1, 1, newFilePtr);
            } else {
                //dest Flag - different from win
                if (strcmp(destOsFlag, flagsList[2])) {
                    //source flag is win - read more 2 bytes for 00, \n
                    if (strcmp(srcOsFlag, flagsList[2]) == 0) {
                        fread(buffer, 1, 1, oldFilePtr);
                        fread(buffer, 1, 1, oldFilePtr);
                    }
                    //if text is Big - swap to Little
                    if (isBig) {
                        fwrite(destCode, 1, 1, newFilePtr);
                        fwrite(&zeroPad, 1, 1, newFilePtr);
                    } else { // text is Little - swap to Big
                        fwrite(&zeroPad, 1, 1, newFilePtr);
                        fwrite(destCode, 1, 1, newFilePtr);
                    }
                } else { //dest Flag is win
                    //if text is Big - swap to Little
                    if (isBig) {
                        fwrite(destCode, 1, 1, newFilePtr);
                        fwrite(&zeroPad, 1, 1, newFilePtr);
                        fwrite(destCode + 1, 1, 1, newFilePtr);
                        fwrite(&zeroPad, 1, 1, newFilePtr);
                    } else { // text is Little - swap to Big
                        fwrite(&zeroPad, 1, 1, newFilePtr);
                        fwrite(destCode, 1, 1, newFilePtr);
                        fwrite(&zeroPad, 1, 1, newFilePtr);
                        fwrite(destCode + 1, 1, 1, newFilePtr);
                    }
                }
            }
        }

    }
}

/* main function - getting arguments from command line and checking case with switch.*/
int main(int argc, char *argv[]) {
    //checking cases according to argc
    switch (argc) {
        case 3:  // 2 argument (beside the program name)
            ReadAndSave(argv[1], argv[2]);
            break;

        case 5: // 4 argument (beside the program name)
            changeCodingFile(argv[1], argv[2], argv[3], argv[4]);
            break;
        case 6: // 5 argument (beside the program name)
            changeCodingFileWithSwap(argv[1], argv[2], argv[3], argv[4], argv[5]);
            break;
        default: // any other case - one file name, one flag...
            break;
    }
    return 0;
}