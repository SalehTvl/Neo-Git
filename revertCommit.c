#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "checkOut.h"
#include "commitCreator.h"

void revertCommit(char* hash, char* message, int state) {
    char filePath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,filePath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result = strstr(filePath, repoPath);
        if(result!=NULL) break;
    }
    fclose(reposfile);

    int countX=-1;
    int Xcopy=-1;
    if(state==3 || state==5) {
        int len=strlen(hash);
        char number[len-4];
        for(int i=5; i<len; i++) {
            number[i-5]=hash[i];
        }
        countX=atoi(number);
        Xcopy=countX;
    }
    char commitMessage[73]; char commitHash[9]; char commitBranch[20];
    char mergeHashesPath[MAX_PATH]; sprintf(mergeHashesPath, "%s\\ngit\\info\\mergeHashes.txt", repoPath);
    char mergeHash[9]; char mergeFbranch[20]; char mergeMbranch[20];
    FILE* mergeHashesptr=fopen(mergeHashesPath, "r");
    char allCommitsPath[MAX_PATH]; sprintf(allCommitsPath, "%s\\ngit\\info\\allCommits.txt", repoPath);
    FILE* allCommitptr=fopen(allCommitsPath, "r");
    char line[100];
    char commitData[7][100];
    while (fgets(line, sizeof(line), allCommitptr)) {
        countX--;
        line[strcspn(line, "\n")] = '\0';
        strcpy(commitData[0], line);
        for (int i = 1; i < 7; i++) {
            if (!fgets(line, sizeof(line), allCommitptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(commitData[i], line);
        }

        if(strcmp(hash, commitData[0])==0 || countX==0) {
            strcpy(commitMessage, commitData[3]);
            strcpy(commitHash, commitData[0]);
            strcpy(commitBranch, commitData[1]);
            break;
        } 
    }
    rewind(allCommitptr);
    while (fgets(line, sizeof(line), allCommitptr)) {
        Xcopy--;
        line[strcspn(line, "\n")] = '\0';
        strcpy(commitData[0], line);
        for (int i = 1; i < 7; i++) {
            if (!fgets(line, sizeof(line), allCommitptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(commitData[i], line);
        }
        rewind(mergeHashesptr);
        while(fscanf(mergeHashesptr, "%s%s%s", mergeHash, mergeFbranch, mergeMbranch)==3) {
            if(strcmp(mergeHash, commitData[0])==0) {
                if(strcmp(mergeFbranch, commitBranch)==0 || strcmp(mergeMbranch, commitBranch)==0) {
                    printf("there was a merge in route to the commit\n");
                    return;
                }
                break;
            }
        }
        if(strcmp(hash, commitData[0])==0 || Xcopy==0) {
            break;
        } 
    }
    fclose(allCommitptr); fclose(mergeHashesptr);
    switch (state) {
        case 1:
            checkoutHash(commitHash,0);
            commitCreator(0, commitMessage);
            return;
        case 3:
            checkoutHash(commitHash,0);
            commitCreator(0, commitMessage);
            return;
        case 4:
            break;
        case 5:
            checkoutHash(commitHash,0);
            commitCreator(0, message);
            return;
    }
    char curCommithashPath[MAX_PATH]; sprintf(curCommithashPath, "%s\\ngit\\info\\curCommithash.txt", repoPath); char curBranch[20];
    FILE* curCommithashptr=fopen(curCommithashPath, "r"); fscanf(curCommithashptr, "%s", curBranch); fclose(curCommithashptr);
    checkoutHash(commitHash,0);
    curCommithashptr=fopen(curCommithashPath, "w"); fprintf(curCommithashptr, "%s", curBranch); fclose(curCommithashptr);
    return;
}