#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "diffCheck.h"

int lastCommitFinder(char* commitBranch, char* commitHash, char* repoPath) {
    char allCommitsPath[MAX_PATH]; sprintf(allCommitsPath, "%s\\ngit\\info\\allCommits.txt", repoPath);
    FILE* allCommitsptr=fopen(allCommitsPath, "r"); char commitData[7][40]; char line[100];
    int count=0;
    rewind(allCommitsptr);
    while (fgets(line, sizeof(line), allCommitsptr)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(commitData[0], line);
        for (int i = 1; i < 7; i++) {
            if (!fgets(line, sizeof(line), allCommitsptr)) {
                printf("Error: Unexpected end of file\n");
                return 0;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(commitData[i], line);
        }
        if(strcmp(commitHash, commitData[0])==0) break;
        if(strcmp(commitBranch, commitData[1])==0) count++;
    }
    fclose(allCommitsptr);
    return count;
}

void conflictThreat(char* commit1Hash, char* commit2Hash, int state) {
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
    int flag1=1;
    char commit1Branch[30]; char commit2Branch[30];
    char allCommitsPath[MAX_PATH]; sprintf(allCommitsPath, "%s\\ngit\\info\\allCommits.txt", repoPath);
    FILE* allCommitsptr=fopen(allCommitsPath, "r"); char commitData[7][40]; char line[100];
    while (fgets(line, sizeof(line), allCommitsptr)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(commitData[0], line);
        for (int i = 1; i < 7; i++) {
            if (!fgets(line, sizeof(line), allCommitsptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(commitData[i], line);
        }
        if(strcmp(commitData[0], commit1Hash)==0) {
            strcpy(commit1Branch, commitData[1]);
            flag1=1;
            break;
        }
    }
    if(flag1==0) {
        printf("the commit hash <%s> is invalid\n", commit1Hash);
        return;
    }
    flag1=0;
    rewind(allCommitsptr);
    while (fgets(line, sizeof(line), allCommitsptr)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(commitData[0], line);
        for (int i = 1; i < 7; i++) {
            if (!fgets(line, sizeof(line), allCommitsptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(commitData[i], line);
        }
        if(strcmp(commitData[0], commit2Hash)==0) {
            strcpy(commit2Branch, commitData[1]);
            flag1=1;
            break;
        }
    }
    if(flag1==0) {
        printf("the commit hash <%s> is invalid\n", commit2Hash);
        return;
    }
    int count1=lastCommitFinder(commit1Branch, commit1Hash, repoPath); int count2=lastCommitFinder(commit2Branch, commit2Hash, repoPath);
    char lastCommitBranPath1[MAX_PATH]; sprintf(lastCommitBranPath1, "%s\\ngit\\info\\%slastCommit.txt", repoPath, commit1Branch);
    int lastCommit1=0;
    FILE* lastCommitBranptr1=fopen(lastCommitBranPath1, "r"); 
    fscanf(lastCommitBranptr1, "%d", &lastCommit1); fclose(lastCommitBranptr1);
    char lastCommitBranPath2[MAX_PATH]; sprintf(lastCommitBranPath2, "%s\\ngit\\info\\%slastCommit.txt", repoPath, commit2Branch);
    int lastCommit2=0;
    FILE* lastCommitBranptr2=fopen(lastCommitBranPath2, "r"); 
    fscanf(lastCommitBranptr2, "%d", &lastCommit2); fclose(lastCommitBranptr2);
    lastCommit1-=count1;
    lastCommit2-=count2;
    char commit1StagedFilesPath[MAX_PATH]; sprintf(commit1StagedFilesPath, "%s\\ngit\\branches\\%s\\commits\\%d\\commitedfiles.txt", repoPath, commit1Branch, lastCommit1);
    char commit2StagedFilesPath[MAX_PATH]; sprintf(commit2StagedFilesPath, "%s\\ngit\\branches\\%s\\commits\\%d\\commitedfiles.txt", repoPath, commit2Branch, lastCommit2);
    char commit1TempFile[MAX_PATH];
    char commit2TempFile[MAX_PATH];
    char commit1Type[5];
    char commit2Type[5];
    char commit1Modif[25];
    char commit2Modif[25];
    FILE* commit1StagedFilesptr=fopen(commit1StagedFilesPath, "r");
    FILE* commit2StagedFilesptr=fopen(commit2StagedFilesPath, "r");
    int flag2=0;
    while(fscanf(commit1StagedFilesptr, "%s%s%s", commit1TempFile, commit1Type, commit1Modif)==3) {
        if(strcmp(commit1Type, "f")==0) {
            if(strstr(commit1TempFile, ".txt")!=NULL) {  ///////////////// inja noe file ro age c ham bood lahaz mikonam
                rewind(commit2StagedFilesptr);
                while(fscanf(commit2StagedFilesptr, "%s%s%s", commit2TempFile, commit2Type, commit2Modif)==3) {
                    if(strcmp(commit1TempFile, commit2TempFile)==0) {
                        char commitedFiles[18]="commitedfiles.txt";
                        char commit1SourceFilePath[MAX_PATH]; strcpy(commit1SourceFilePath, commit1StagedFilesPath);
                        char *match = strstr(commit1SourceFilePath, commitedFiles);
                        memmove(match, match + strlen(commitedFiles), strlen(match + strlen(commitedFiles)) + 1); strcat(commit1SourceFilePath, "content");
                        match = strstr(commit1TempFile, repoPath);
                        memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1); strcat(commit1SourceFilePath, commit1TempFile);
                        char commit2SourceFilePath[MAX_PATH]; strcpy(commit2SourceFilePath, commit2StagedFilesPath);
                        match = strstr(commit2SourceFilePath, commitedFiles);
                        memmove(match, match + strlen(commitedFiles), strlen(match + strlen(commitedFiles)) + 1); strcat(commit2SourceFilePath, "content");
                        match = strstr(commit2TempFile, repoPath);
                        memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1); strcat(commit2SourceFilePath, commit2TempFile);
                        int returnValue=differenceCheck(commit1SourceFilePath, commit2SourceFilePath,1,100000,1,100000, 0);
                        if(returnValue==0) flag2=1;
                        break;
                    }
                }
            }
        } 
    }
    if(flag2==1) {
        printf("there were no conflict to represent diffrences\n");
    }
    return;
}