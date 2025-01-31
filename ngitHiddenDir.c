#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "syntaxErrorRecognizer.h"

int makeHiddenNgitDir() {
    char *folderName = "ngit";
    char currentDirectory[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, currentDirectory) != 0) {
        strcat(currentDirectory, "\\");
        strcat(currentDirectory, folderName);
        if (CreateDirectory(currentDirectory, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
            if (GetLastError() == ERROR_ALREADY_EXISTS) {
                printf("directory is already set as a ngit repository.");
            } 
            else if (SetFileAttributes(currentDirectory, FILE_ATTRIBUTE_DIRECTORY)) {
                printf("directory is succesfully initialized as a ngit repository.\n");
                FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt", "a");
                char currentDirectory[MAX_PATH];
                GetCurrentDirectory(MAX_PATH, currentDirectory);
                fputs(currentDirectory, reposfile);
                fputs("\n", reposfile);
                fclose(reposfile);
                char curDirpathCopy[MAX_PATH];
                strcat(currentDirectory, "\\ngit");
                strcpy(curDirpathCopy, currentDirectory);
                strcat(currentDirectory, "\\info");
                CreateDirectory(currentDirectory, NULL);
                strcat(currentDirectory, "\\contents");
                CreateDirectory(currentDirectory, NULL);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\info");
                FILE* userInfofile = fopen("d:\\ANGP\\ngit-project\\userInfo.txt", "r+");
                char content[100];
                char username[100];
                char useremail[100];
                fgets(content, sizeof(content), userInfofile);
                strcpy(username, content);
                fgets(content, sizeof(content), userInfofile);
                strcpy(useremail, content);
                fclose(userInfofile);
                strcat(currentDirectory, "\\localuser.txt");
                FILE* fptr=fopen(currentDirectory, "w");
                fputs(username, fptr);
                fputs(useremail, fptr);
                //fputs("1\n", fptr); // tedade branch
                //fputs("0", fptr); // tedade commit
                fclose(fptr);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\info\\masterlastCommit.txt");
                FILE* fptr4=fopen(currentDirectory, "w");
                fprintf(fptr4,"0");
                fclose(fptr4);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\info\\stagedfiles.txt");
                FILE* fptr3=fopen(currentDirectory, "w");
                fclose(fptr3);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\info\\begstagedfiles.txt");
                FILE* fptr5=fopen(currentDirectory, "w");
                fclose(fptr5);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\info\\branches.txt");
                FILE* fptr6=fopen(currentDirectory, "w");
                fprintf(fptr6, "master\n");
                fclose(fptr6);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\info\\currentbranch.txt");
                FILE* fptr7=fopen(currentDirectory, "w");
                fprintf(fptr7, "master\n");
                fclose(fptr7);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\info\\allCommits.txt");
                FILE* fptr2=fopen(currentDirectory, "w");
                fclose(fptr2);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\branches");
                CreateDirectory(currentDirectory, NULL);
                strcat(currentDirectory, "\\master");
                CreateDirectory(currentDirectory, NULL);
                strcat(currentDirectory, "\\commits");
                CreateDirectory(currentDirectory, NULL);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\stagingArea");
                CreateDirectory(currentDirectory, NULL);
                strcpy(currentDirectory, curDirpathCopy);
                strcat(currentDirectory, "\\stashingArea");
                CreateDirectory(currentDirectory, NULL);
                FILE* curbranchfile=fopen("d:\\ANGP\\ngit-project\\currentbranch.txt", "r");
                if(curbranchfile==NULL) {
                    fclose(curbranchfile);
                    curbranchfile=fopen("d:\\ANGP\\ngit-project\\currentbranch.txt", "w");
                    fputs("master", curbranchfile);
                }
                else {
                    fclose(curbranchfile);
                    remove("d:\\ANGP\\ngit-project\\currentbranch.txt");
                    curbranchfile=fopen("d:\\ANGP\\ngit-project\\currentbranch.txt", "w");
                    fputs("master", curbranchfile);
                }
                fclose(curbranchfile);
            } 
        } 
        else {
            printf("systemic error creating repository.");
            return 0;
        }
    }
    return 1;
}

