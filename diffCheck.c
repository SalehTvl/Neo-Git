#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

int differenceCheck(char* file1Path, char* file2Path, int line1Beg, int line1End, int line2Beg, int line2End, int state) {
    char file1Line[1000];  char file2Line[1000]; int beginflag=0; int line1Counter=0; int line2Counter=0;
    if(state==1) {
        char currentPath[MAX_PATH];
        GetCurrentDirectory(MAX_PATH,currentPath);
        char tempFilePath[MAX_PATH]; sprintf(tempFilePath, "%s\\%s", currentPath, file1Path); file1Path=tempFilePath;
        char tempFilePath1[MAX_PATH]; sprintf(tempFilePath1, "%s\\%s", currentPath, file2Path); file2Path=tempFilePath1;
    }
    char file1PathCopy[MAX_PATH]; 
    if(state==2) {
        strcpy(file1PathCopy, file1Path);
        char* contentBeginig = strstr(file1PathCopy, "\\content");
        char* ngitBegining = strstr(file1PathCopy, "\\ngit");
        if (ngitBegining != NULL && contentBeginig != NULL) {
            size_t removeLength = contentBeginig - ngitBegining + strlen("\\content");
            strcpy(ngitBegining, contentBeginig + strlen("\\content"));
        }
    }
    if(state==3) {
        strcpy(file1PathCopy, file1Path);
    }
    FILE* file1ptr=fopen(file1Path, "r"); FILE* file2ptr=fopen(file2Path, "r");
    if(file1ptr==NULL || file2ptr==NULL) {
        printf("files were not found\n");
        return 0;
    }
    char falseLines1[1000]; char falseLines2[1000]; 
    for(int i=0; i<line1Beg-1; i++) {
        fgets(falseLines1, sizeof(falseLines1), file1ptr); line1Counter++;
    }
    for(int i=0; i<line2Beg-1; i++) {
        fgets(falseLines2, sizeof(falseLines2), file2ptr); line2Counter++;
    }
    char* piece1=strtok(file1Path, "\\"); char file1NameLastPiece[20];
    while(piece1!=NULL) {
        strcpy(file1NameLastPiece, piece1);
        piece1=strtok(NULL, "\\");
    }
    char* piece2=strtok(file2Path, "\\"); char file2NameLastPiece[20];
    while(piece2!=NULL) {
        strcpy(file2NameLastPiece, piece2);
        piece2=strtok(NULL, "\\");
    }
    int diffResult=0;
    while(fgets(file1Line, sizeof(file1Line), file1ptr)!=NULL) {
        line1Counter++;
        file1Line[strcspn(file1Line, "\n")]='\0';
        if(strcmp(file1Line, "")==0) continue;
        while(fgets(file2Line, sizeof(file2Line), file2ptr)!=NULL) {
            line2Counter++;
            file2Line[strcspn(file2Line, "\n")]='\0';
            if(strcmp(file2Line, "")==0) continue;
            if(strcmp(file1Line, file2Line)!=0) {
                if(beginflag==0 && state!=2) {
                    printf("<<<<<<<<<<\n");
                    beginflag=1;
                }
                diffResult=1;
                if(state!=2 && state!=3) printf("%s-line: %d\n", file1NameLastPiece, line1Counter);
                else printf("<<<<<%s>>>>>\nline: %d\n", file1PathCopy, line1Counter);
                HANDLE hConsole1 = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO consoleInfo1;
                WORD savedAttributes1;
                GetConsoleScreenBufferInfo(hConsole1, &consoleInfo1);
                savedAttributes1 = consoleInfo1.wAttributes;
                SetConsoleTextAttribute(hConsole1, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("%s\n", file1Line);
                SetConsoleTextAttribute(hConsole1, savedAttributes1);
                if(state!=2 && state!=3) printf("%s-line: %d\n", file2NameLastPiece, line2Counter);
                else printf("<<<<<%s>>>>>\nline: %d\n", file1PathCopy, line2Counter);
                HANDLE hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO consoleInfo2;
                WORD savedAttributes2;
                GetConsoleScreenBufferInfo(hConsole2, &consoleInfo2);
                savedAttributes2 = consoleInfo2.wAttributes;
                SetConsoleTextAttribute(hConsole2, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("%s\n", file2Line);
                SetConsoleTextAttribute(hConsole2, savedAttributes2);
                
            }
            break;
        }
        if(line1Counter>=line1End || line2Counter>=line2End) break;
    }
    if(beginflag==1 && state!=2) {
        printf(">>>>>>>>>>\n");
    }
    else if(state==1 && beginflag==0) {
        printf("there was no difference between file <%s> and file <%s>\\n", file1NameLastPiece, file2NameLastPiece);
    }
    else if(beginflag==0 && state!=2) return 0;
    else if(state==2 && diffResult==1) return 1;
    else if(state==2 && diffResult==0) return 0;
    return 1;
}