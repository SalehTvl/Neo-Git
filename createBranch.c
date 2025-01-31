#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void createBranch(char* newBranch) {
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
    char allBranchesPath[MAX_PATH];
    sprintf(allBranchesPath, "%s\\ngit\\info\\branches.txt", repoPath);
    FILE* branchExistCheck=fopen(allBranchesPath, "r"); char Branch[20];
    while(fgets(Branch, sizeof(Branch), branchExistCheck)!=NULL) {
        Branch[strcspn(Branch, "\n")]='\0';
        if(strcmp(Branch, newBranch)==0) {
            printf("this branch <%s> already exists\n", newBranch);
            fclose(branchExistCheck);
            return;
        }
    }
    fclose(branchExistCheck);
    char currentBranchPath[MAX_PATH];
    strcpy(currentBranchPath, repoPath);
    strcat(currentBranchPath, "\\ngit\\info\\currentbranch.txt");
    FILE* currentBranchptr=fopen(currentBranchPath, "r");
    char currentbranch[20];
    fgets(currentbranch, sizeof(currentbranch), currentBranchptr); currentbranch[strcspn(currentbranch, "\n")] = '\0'; fclose(currentBranchptr);
    char curBranLastCommitPath [MAX_PATH];
    sprintf(curBranLastCommitPath, "%s\\ngit\\info\\%slastCommit.txt", repoPath, currentbranch);
    int lastCommit=0;
    FILE* curBraLastComptr=fopen(curBranLastCommitPath, "r"); fscanf(curBraLastComptr, "%d", &lastCommit); fclose(curBraLastComptr);
    char newBranchlastComPath[MAX_PATH];
    sprintf(newBranchlastComPath, "%s\\ngit\\info\\%slastCommit.txt", repoPath, newBranch);
    FILE* newBranchlastComptr=fopen(newBranchlastComPath, "w"); fprintf(newBranchlastComptr, "1"); fclose(newBranchlastComptr);
    FILE* allBranchesptr=fopen(allBranchesPath, "a"); fprintf(allBranchesptr, "%s\n", newBranch); fclose(allBranchesptr);
    char newBranchPath[MAX_PATH];
    sprintf(newBranchPath, "%s\\ngit\\branches\\%s", repoPath, newBranch); CreateDirectory(newBranchPath, NULL);
    strcat(newBranchPath, "\\commits"); CreateDirectory(newBranchPath, NULL);
    strcat(newBranchPath, "\\1"); CreateDirectory(newBranchPath, NULL);
    char curBranLastFolderPath[MAX_PATH]; sprintf(curBranLastFolderPath, "%s\\ngit\\branches\\%s\\commits\\%d", repoPath, currentbranch, lastCommit);
    FILE* batchFile = fopen("copydir.bat", "w");
    
    fprintf(batchFile, "@echo off\n");
    fprintf(batchFile, "set \"sourceDirectory=%s\"\n", curBranLastFolderPath);
    fprintf(batchFile, "set \"destinationDirectory=%s\"\n", newBranchPath);
    fprintf(batchFile, "xcopy \"%%sourceDirectory%%\" \"%%destinationDirectory%%\" /E /I /Q /Y > NUL\n");
    fprintf(batchFile, ")\n");

    fclose(batchFile);
    system("copydir.bat");
    char batFilePath[MAX_PATH]; sprintf(batFilePath, "%s\\copydir.bat", filePath);
    remove(batFilePath);
    printf("\033[32mbranch <%s> created successfully\033[0m\n", newBranch);
    return;
}

void listBranches() {
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
    char allBranchesPath[MAX_PATH];
    sprintf(allBranchesPath, "%s\\ngit\\info\\branches.txt", repoPath);
    FILE* allBranchesptr=fopen(allBranchesPath, "r");
    char branch[20];
    while(fscanf(allBranchesptr, "%s", branch)==1) {
        printf("%s\n", branch);
    }
    return;
}
