#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "diffCheck.h"
#include "commitCreator.h"

void mergeCommit(char* branch1, char* branch2) {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,currentPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result = strstr(currentPath, repoPath);
        if(result!=NULL) break;
    }
    fclose(reposfile);

    char branch1LastCommitPath[MAX_PATH]; sprintf(branch1LastCommitPath, "%s\\ngit\\info\\%slastCommit.txt", repoPath, branch1);
    FILE* branch1LastCommitptr=fopen(branch1LastCommitPath, "r"); int branch1LastCommit=0;
    if(branch1LastCommitptr==NULL) {
        printf("branch <%s> was not found\n", branch1);
        return;
    }
    fscanf(branch1LastCommitptr, "%d", &branch1LastCommit); fclose(branch1LastCommitptr);
    char branch2LastCommitPath[MAX_PATH]; sprintf(branch2LastCommitPath, "%s\\ngit\\info\\%slastCommit.txt", repoPath, branch2);
    FILE* branch2LastCommitptr=fopen(branch2LastCommitPath, "r"); int branch2LastCommit=0;
    if(branch2LastCommitptr==NULL) {
        printf("branch <%s> was not found\n", branch2);
        return;
    }
    fscanf(branch2LastCommitptr, "%d", &branch2LastCommit); fclose(branch2LastCommitptr);
    int flagCurrentBranch=1;
    char curBranchPath[MAX_PATH]; sprintf(curBranchPath, "%s\\ngit\\info\\currentbranch.txt", repoPath); char currentBranch[30];
    FILE* curBranchptr=fopen(curBranchPath, "r"); fscanf(curBranchptr, "%s", currentBranch); fclose(curBranchptr);
    if(strcmp(currentBranch, branch1)!=0) {
        if(strcmp(currentBranch, branch2)==0) {
            printf("you must enter your current branch in the first argument\n");
            return;
        }
        else {
            flagCurrentBranch=0;
        }
    }
    char listofBranch1FilesPath[MAX_PATH]; char listofBranch2FilesPath[MAX_PATH];
    sprintf(listofBranch1FilesPath, "%s\\ngit\\branches\\%s\\commits\\%d\\commitedfiles.txt", repoPath, branch1, branch1LastCommit);
    sprintf(listofBranch2FilesPath, "%s\\ngit\\branches\\%s\\commits\\%d\\commitedfiles.txt", repoPath, branch2, branch2LastCommit);
    FILE* listofBranch1Filesptr=fopen(listofBranch1FilesPath, "r"); FILE* listofBranch2Filesptr=fopen(listofBranch2FilesPath, "r");
    char placeofBranch1TempFile[MAX_PATH]; char placeofBranch2TempFile[MAX_PATH];
    char branch1TempFile[MAX_PATH]; char branch2TempFile[MAX_PATH]; char branch1TempType[MAX_PATH]; char branch2TempType[MAX_PATH];
    char branch1TempModif[MAX_PATH]; char branch2TempModif[MAX_PATH];
    int diffCheckResult=0; int conflictFlag=0;
    while(fscanf(listofBranch1Filesptr, "%s%s%s", branch1TempFile, branch1TempType, branch1TempModif)==3) {
        if(strcmp(branch1TempType, "d")==0) continue;
        if(strstr(branch1TempFile, ".txt")==NULL) continue;
        rewind(listofBranch2Filesptr);
        while(fscanf(listofBranch2Filesptr, "%s%s%s", branch2TempFile, branch2TempType, branch2TempModif)==3) {
            printf("branch2: %s\n", branch2TempFile);
            if(strcmp(branch1TempFile, branch2TempFile)==0) {
                printf("b1: %s b2: %s\n", branch1TempFile, branch2TempFile);
                char *match = strstr(branch1TempFile, repoPath); 
                memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
                sprintf(placeofBranch1TempFile, "%s\\ngit\\branches\\%s\\commits\\%d\\content%s", repoPath, branch1, branch1LastCommit, branch1TempFile);
                match = strstr(branch2TempFile, repoPath);
                memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
                sprintf(placeofBranch2TempFile, "%s\\ngit\\branches\\%s\\commits\\%d\\content%s", repoPath, branch2, branch2LastCommit, branch2TempFile);
                diffCheckResult=differenceCheck(placeofBranch1TempFile, placeofBranch2TempFile,1,100000,1,100000, 2);
                if(diffCheckResult==1) {
                    conflictFlag=1;
                }
                break;
            }
        }
    }
    if(conflictFlag==1) {
        printf("merging branches was cancelled because of above conflict\nplease resolve conflicts manually and merge again\n");
        fclose(listofBranch1Filesptr); fclose(listofBranch2Filesptr);
        return;
    }
    char newStagedFilesPath[MAX_PATH]; sprintf(newStagedFilesPath, "%s\\ngit\\info\\newstagedfiles.txt", repoPath);
    FILE* newStagedFilesptr=fopen(newStagedFilesPath, "w");
    rewind(listofBranch1Filesptr);
    while(fscanf(listofBranch1Filesptr, "%s%s%s", branch1TempFile, branch1TempType, branch1TempModif)==3) {
        fprintf(newStagedFilesptr, "%s %s %s\n", branch1TempFile, branch1TempType, branch1TempModif);
        if(strcmp(branch1TempType, "d")==0) {
            DIR* existDir=opendir(branch1TempFile);
            if(existDir==NULL) CreateDirectory(branch1TempFile, NULL);
            closedir(existDir);
        }
        else {
            char *match = strstr(branch1TempFile, repoPath); 
            memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
            sprintf(placeofBranch1TempFile, "%s\\ngit\\branches\\%s\\commits\\%d\\content%s", repoPath, branch1, branch1LastCommit, branch1TempFile);
            char fileStagingPlace[MAX_PATH]; sprintf(fileStagingPlace, "%s\\ngit\\stagingArea%s", repoPath, branch1TempFile);
            FILE *batchFile=fopen("copyfile.bat", "w");
            fprintf(batchFile, "@echo off\n");
            fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", placeofBranch1TempFile, fileStagingPlace);
            fprintf(batchFile, "exit /b 0\n");
            fclose(batchFile);
            system("copyfile.bat");
            char batFilePath[MAX_PATH]; strcpy(batFilePath, currentPath); strcat(batFilePath, "\\copyfile.bat");
            remove(batFilePath);
            char destinationofCopy[MAX_PATH]; sprintf(destinationofCopy, "%s%s", repoPath, branch1TempFile);
            batchFile=fopen("copyfile.bat", "w");
            fprintf(batchFile, "@echo off\n");
            fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", placeofBranch1TempFile, destinationofCopy);
            fprintf(batchFile, "exit /b 0\n");
            fclose(batchFile);
            system("copyfile.bat");
            strcpy(batFilePath, currentPath); strcat(batFilePath, "\\copyfile.bat");
            remove(batFilePath);
        }
    }
    rewind(listofBranch1Filesptr);
    rewind(listofBranch2Filesptr);
    int endofBranch1Files=0; int fileIsonlyInBranch2=0;
    while(fscanf(listofBranch2Filesptr, "%s%s%s", branch2TempFile, branch2TempType, branch2TempModif)==3) {
        while(fscanf(listofBranch1Filesptr, "%s%s%s", branch1TempFile, branch1TempType, branch1TempModif)==3) {
            if(strcmp(branch1TempFile, branch2TempFile)==0) {
                fileIsonlyInBranch2=1;
                rewind(listofBranch1Filesptr);
                break;
            }
        }
        if(fileIsonlyInBranch2==0) {
            fprintf(newStagedFilesptr, "%s %s %s\n", branch2TempFile, branch2TempType, branch2TempModif);
            if(strcmp(branch2TempType, "d")==0) {
                DIR* existDir=opendir(branch2TempFile);
                if(existDir==NULL) {
                    closedir(existDir);
                    CreateDirectory(branch2TempFile, NULL);           
                }
                closedir(existDir);
            }
            else {
                char *match = strstr(branch2TempFile, repoPath); 
                memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
                sprintf(placeofBranch2TempFile, "%s\\ngit\\branches\\%s\\commits\\%d\\content%s", repoPath, branch2, branch2LastCommit, branch2TempFile);
                char fileStagingPlace[MAX_PATH]; sprintf(fileStagingPlace, "%s\\ngit\\stagingArea%s", repoPath, branch2TempFile);
                FILE *batchFile=fopen("copyfile.bat", "w");
                fprintf(batchFile, "@echo off\n");
                fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", placeofBranch2TempFile, fileStagingPlace);
                fprintf(batchFile, "exit /b 0\n");
                fclose(batchFile);
                system("copyfile.bat");
                char batFilePath[MAX_PATH]; strcpy(batFilePath, currentPath); strcat(batFilePath, "\\copyfile.bat");
                remove(batFilePath);
                char destinationofCopy[MAX_PATH]; sprintf(destinationofCopy, "%s%s", repoPath, branch2TempFile);
                batchFile=fopen("copyfile.bat", "w");
                fprintf(batchFile, "@echo off\n");
                fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", placeofBranch2TempFile, destinationofCopy);
                fprintf(batchFile, "exit /b 0\n");
                fclose(batchFile);
                system("copyfile.bat");
                strcpy(batFilePath, currentPath); strcat(batFilePath, "\\copyfile.bat");
                remove(batFilePath);
            }
        }
        fileIsonlyInBranch2=0;
    }
    fclose(listofBranch1Filesptr); fclose(listofBranch2Filesptr); fclose(newStagedFilesptr);
    char stagedFilesPath[MAX_PATH]; sprintf(stagedFilesPath, "%s\\ngit\\info\\stagedfiles.txt", repoPath);
    SetFileAttributes(stagedFilesPath, FILE_ATTRIBUTE_NORMAL); remove(stagedFilesPath); rename(newStagedFilesPath, stagedFilesPath);
    curBranchptr=fopen(curBranchPath, "w"); fprintf(curBranchptr, "%s", currentBranch); fclose(curBranchptr);
    char totalCommitPath[MAX_PATH]; sprintf(totalCommitPath, "d:\\ANGP\\ngit-project\\totalCommitCount.txt"); int totalCommitCount=0;
    FILE* totalCommitCountptr=fopen(totalCommitPath, "r"); fscanf(totalCommitCountptr, "%d", &totalCommitCount); fclose(totalCommitCountptr);
    totalCommitCountptr=fopen(totalCommitPath, "w"); fprintf(totalCommitCountptr, "%d", totalCommitCount+90000000); fclose(totalCommitCountptr);
    commitCreator(0, "no message"); 
    totalCommitCountptr=fopen(totalCommitPath, "w"); fprintf(totalCommitCountptr, "%d", totalCommitCount+1); fclose(totalCommitCountptr);
    char mergedCommitsPath[MAX_PATH]; sprintf(mergedCommitsPath, "%s\\ngit\\info\\mergeHashes.txt", repoPath);
    FILE* mergedCommitsptr=fopen(mergedCommitsPath, "a"); fprintf(mergedCommitsptr, "%d %s %s\n", totalCommitCount+90000001, branch1, branch2); fclose(mergedCommitsptr);
    return;
} 