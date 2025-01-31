#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "diffCheck.h"

void stashCreator(char* message){
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

    char currentBranchPath[MAX_PATH]; sprintf(currentBranchPath, "%s\\ngit\\info\\currentbranch.txt", repoPath);
    char curCommithashPath[MAX_PATH]; sprintf(curCommithashPath, "%s\\ngit\\info\\curCommithash.txt", repoPath);
    char curCommithash[9]; char curBranch[20];
    FILE* currentBranchptr=fopen(currentBranchPath, "r"); fscanf(currentBranchptr, "%s", curBranch); fclose(currentBranchptr);
    FILE* curCommithashptr=fopen(curCommithashPath, "r"); 
    if(curCommithashptr==NULL) {
        printf("you must first create a commit to stash changes\n");
        fclose(curCommithashptr);
        return;
    }
    fscanf(curCommithashptr, "%s", curCommithash); fclose(curCommithashptr);
    char lastStashPath[MAX_PATH]; sprintf(lastStashPath, "%s\\ngit\\info\\lastStash.txt", repoPath); int lastStash=0;
    FILE* lastStashptr=fopen(lastStashPath, "r"); 
    if(lastStashptr==NULL) {
        fclose(lastStashptr);
        lastStashptr=fopen(lastStashPath, "w"); fprintf(lastStashptr, "%d", lastStash); fclose(lastStashptr);
    }
    else {
        fscanf(lastStashptr, "%d", &lastStash); fclose(lastStashptr); lastStash++;
        lastStashptr=fopen(lastStashPath, "w"); fprintf(lastStashptr, "%d", lastStash); fclose(lastStashptr);
    }
    char totalStashCountPath[MAX_PATH]; sprintf(totalStashCountPath, "d:\\ANGP\\ngit-project\\totalStashCount.txt"); int totalStashCount=0;
    FILE* totalStashCountptr=fopen(totalStashCountPath, "r"); fscanf(totalStashCountptr, "%d", &totalStashCount); fclose(totalStashCountptr); totalStashCount++;
    totalStashCountptr=fopen(totalStashCountPath, "w"); fprintf(totalStashCountptr, "%d", totalStashCount); fclose(totalStashCountptr);    

    char newStashDirectoryPath[MAX_PATH]; sprintf(newStashDirectoryPath, "%s\\ngit\\stashingArea\\%d", repoPath, lastStash);
    CreateDirectory(newStashDirectoryPath, NULL);
    strcat(newStashDirectoryPath, "\\content"); CreateDirectory(newStashDirectoryPath, NULL);
    char stashedFilesPath[MAX_PATH]; sprintf(stashedFilesPath, "%s\\ngit\\stashingArea\\%d\\stashedFiles.txt", repoPath, lastStash);
    char stagedFilesPath[MAX_PATH]; sprintf(stagedFilesPath, "%s\\ngit\\info\\stagedfiles.txt", repoPath);
    FILE *batchFile=fopen("copyfile.bat", "w");
    fprintf(batchFile, "@echo off\n");
    fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", stagedFilesPath, stashedFilesPath);
    fprintf(batchFile, "exit /b 0\n");
    fclose(batchFile);
    system("copyfile.bat");
    char batFilePath[MAX_PATH]; strcpy(batFilePath, currentPath); strcat(batFilePath, "\\copyfile.bat");
    remove(batFilePath);

    FILE* stagedFilesptr=fopen(stagedFilesPath, "r"); 
    char stagedTempFilePlace[MAX_PATH]; char stagedTempType[5]; char stagedTempModif[25]; char destinationofFile[MAX_PATH]; char tempFIlePiece[MAX_PATH];
    while(fscanf(stagedFilesptr, "%s%s%s", stagedTempFilePlace, stagedTempType, stagedTempModif)==3) {
        if(strcmp(stagedTempType, "d")==0) {
            strcpy(tempFIlePiece, stagedTempFilePlace);
            char *match = strstr(tempFIlePiece, repoPath); 
            memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
            sprintf(destinationofFile, "%s\\ngit\\stashingArea\\%d\\content%s", repoPath, lastStash, tempFIlePiece); 
            DIR* dirExists=opendir(destinationofFile);
            if(dirExists==NULL) {
                closedir(dirExists);
                CreateDirectory(destinationofFile, NULL);
            }
            closedir(dirExists);
        }
        else {
            strcpy(tempFIlePiece, stagedTempFilePlace);
            char *match = strstr(tempFIlePiece, repoPath); 
            memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
            sprintf(destinationofFile, "%s\\ngit\\stashingArea\\%d\\content%s", repoPath, lastStash, tempFIlePiece); 
            batchFile=fopen("copyfile.bat", "w");
            fprintf(batchFile, "@echo off\n");
            fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", stagedTempFilePlace, destinationofFile);
            fprintf(batchFile, "exit /b 0\n");
            fclose(batchFile);
            system("copyfile.bat");
            strcpy(batFilePath, currentPath); strcat(batFilePath, "\\copyfile.bat");
            remove(batFilePath);
        }
    }
    fclose(stagedFilesptr);
    char allStashesPath[MAX_PATH]; sprintf(allStashesPath, "%s\\ngit\\info\\allStashes.txt", repoPath);
    char newAllStashesPath[MAX_PATH]; sprintf(newAllStashesPath, "%s\\ngit\\info\\newAllStashes.txt", repoPath);
    FILE* allStashesptr=fopen(allStashesPath, "r"); FILE* newAllStashesptr=fopen(newAllStashesPath, "w");
    char line[100]; char stashData[5][73];
    fprintf(newAllStashesptr, "%02d\n%d\n%s\n%s\n%s\n", totalStashCount, lastStash, curCommithash, curBranch, message);
    printf("stashed successfully\nstash index:%02d\non commit with hash: %s\non branch: %s\nstash message: %s\n", totalStashCount, curCommithash, curBranch, message);
    while (fgets(line, sizeof(line), allStashesptr)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(stashData[0], line);
        for (int i = 1; i < 5; i++) {
            if (!fgets(line, sizeof(line), allStashesptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(stashData[i], line);
        }
        fprintf(newAllStashesptr, "%s\n%s\n%s\n%s\n%s\n", stashData[0], stashData[1], stashData[2], stashData[3], stashData[4]);
    }
    fclose(newAllStashesptr); fclose(allStashesptr);
    SetFileAttributes(allStashesPath, FILE_ATTRIBUTE_NORMAL); remove(allStashesPath); rename(newAllStashesPath, allStashesPath);
    return;
}

void stashList() {
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

    char allStashesPath[MAX_PATH]; sprintf(allStashesPath, "%s\\ngit\\info\\allStashes.txt", repoPath);
    FILE* allStashesptr=fopen(allStashesPath, "r"); char line[100]; char stashData[5][73];
    if(allStashesptr==NULL) {
        printf("there is no stash to show\n");
        return;
    }
    while (fgets(line, sizeof(line), allStashesptr)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(stashData[0], line);
        for (int i = 1; i < 5; i++) {
            if (!fgets(line, sizeof(line), allStashesptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(stashData[i], line);
        }
        printf("stash idndex: %s\non branch: %s\nstash message: %s\n/////////////////////////////////////////\n", stashData[0], stashData[3], stashData[4]);
    }
    fclose(allStashesptr);
    return;
}

void stashShow(char* stashIndex) {
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

    char stashCommithash[9]; char stashBranch[25]; char stashNumber[3];
    char allStashesPath[MAX_PATH]; sprintf(allStashesPath, "%s\\ngit\\info\\allStashes.txt", repoPath);
    FILE* allStashesptr=fopen(allStashesPath, "r"); char line[100]; char stashData[5][73]; int flag=0;
    if(allStashesptr==NULL) {
        printf("there is no stash to show\n");
        return;
    }
    while (fgets(line, sizeof(line), allStashesptr)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(stashData[0], line);
        for (int i = 1; i < 5; i++) {
            if (!fgets(line, sizeof(line), allStashesptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(stashData[i], line);
        }
        if(strcmp(stashIndex, stashData[0])==0) {
            strcpy(stashBranch, stashData[3]);
            strcpy(stashNumber, stashData[1]);
            strcpy(stashCommithash, stashData[2]);
            flag=1;
            break;
        }
    }
    fclose(allStashesptr);
    if(flag==0) {
        printf("stash index <%s> is not valid\n", stashIndex);
        return;
    }
    char allCommitsPath[MAX_PATH]; sprintf(allCommitsPath, "%s\\ngit\\info\\allCommits.txt", repoPath);
    FILE* allCommitsptr=fopen(allCommitsPath, "r"); char commitData[7][40];
    int count=0;
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
        if(strcmp(stashCommithash, commitData[0])==0) break;
        if(strcmp(stashBranch, commitData[1])==0) count++;
    }
    fclose(allCommitsptr);
    char branchLastCommitPath[MAX_PATH]; sprintf(branchLastCommitPath, "%s\\ngit\\info\\%slastCommit.txt", repoPath, stashBranch); int branchLastCommit=0;
    FILE* branchLastCommitptr=fopen(branchLastCommitPath, "r"); fscanf(branchLastCommitptr, "%d", &branchLastCommit); fclose(branchLastCommitptr);
    branchLastCommit-=count; int stashNumint=atoi(stashNumber);
    char stashedFilesPath[MAX_PATH]; sprintf(stashedFilesPath, "%s\\ngit\\stashingArea\\%d\\stashedFiles.txt", repoPath, stashNumint);
    char commitedFilesPath[MAX_PATH]; sprintf(commitedFilesPath, "%s\\ngit\\branches\\%s\\commits\\%d\\commitedfiles.txt", repoPath, stashBranch, branchLastCommit);
    char piceseofFilePath[MAX_PATH]; char stashFilePlace[MAX_PATH]; char commitedFilePlace[MAX_PATH];
    char commitedTempFile[MAX_PATH]; char commitedTempType[5]; char commitedTempModif[25];
    char stashedTempFile[MAX_PATH]; char stashedTempType[5]; char stashedTempModif[25]; int flag2=0;
    FILE* stashedFilesptr=fopen(stashedFilesPath, "r"); FILE* commitedFilesptr=fopen(commitedFilesPath, "r");
    while(fscanf(stashedFilesptr, "%s%s%s", stashedTempFile, stashedTempType, stashedTempModif)==3) {
        if(strcmp(stashedTempType, "d")==0) continue;
        if(strstr(stashedTempFile, ".txt")==NULL) continue;
        rewind(commitedFilesptr);
        while(fscanf(commitedFilesptr, "%s%s%s", commitedTempFile, commitedTempType, commitedTempModif)==3) {
            if(strcmp(stashedTempFile, commitedTempFile)==0) {
                strcpy(piceseofFilePath, stashedTempFile);
                char *match = strstr(piceseofFilePath, repoPath);
                memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
                sprintf(stashFilePlace, "%s\\ngit\\stashingArea\\%d\\content%s", repoPath, stashNumint, piceseofFilePath);
                sprintf(commitedFilePlace, "%s\\ngit\\branches\\%s\\commits\\%d\\content%s", repoPath, stashBranch, branchLastCommit, piceseofFilePath);
                printf("%s %s\n", commitedFilePlace, stashFilePlace);
                int returnValue=differenceCheck(commitedFilePlace, stashFilePlace,1,100000,1,100000, 2);
                if(returnValue==0) flag2=1;
                break;
            }
        }
    }
    if(flag2==0) {
        printf("there were no conflict to represent diffrences\n");
    }
    return;
}

void stashPop(char* stashIndex, int state) {
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

    char stashCommithash[9]; char stashBranch[25]; char stashNumber[3];
    char allStashesPath[MAX_PATH]; sprintf(allStashesPath, "%s\\ngit\\info\\allStashes.txt", repoPath);
    FILE* allStashesptr=fopen(allStashesPath, "r"); char line[100]; char stashData[5][73]; int flag=0;
    if(allStashesptr==NULL) {
        printf("there is no stash to show\n");
        return;
    }
    while (fgets(line, sizeof(line), allStashesptr)) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(stashData[0], line);
        for (int i = 1; i < 5; i++) {
            if (!fgets(line, sizeof(line), allStashesptr)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line[strcspn(line, "\n")] = '\0';
            strcpy(stashData[i], line);
        }
        if(state==1) {
            strcpy(stashBranch, stashData[3]);
            strcpy(stashNumber, stashData[1]);
            strcpy(stashCommithash, stashData[2]);
            stashIndex=stashData[0];
            flag=1;
            break;
        }
        else if(strcmp(stashIndex, stashData[0])==0) {
            strcpy(stashBranch, stashData[3]);
            strcpy(stashNumber, stashData[1]);
            strcpy(stashCommithash, stashData[2]);
            flag=1;
            break;
        }
    }
    fclose(allStashesptr); 
    if(flag==0 && state==0) {
        printf("stash index <%s> is not valid\n", stashIndex);
        return;
    }
    int stashNumint=atoi(stashNumber);
    char stashedFilesPath[MAX_PATH]; sprintf(stashedFilesPath, "%s\\ngit\\stashingArea\\%d\\stashedFiles.txt", repoPath, stashNumint);
    char stagedFilesPath[MAX_PATH]; sprintf(stagedFilesPath, "%s\\ngit\\info\\stagedfiles.txt", repoPath);
    FILE* stagedFilesptr=fopen(stagedFilesPath, "r"); FILE* stashedFilesptr=fopen(stashedFilesPath, "r");
    char stagedTempFilePlace[MAX_PATH]; char stashedTempFilePlace[MAX_PATH];
    char stagedTempFile[MAX_PATH]; char stashedTempFile[MAX_PATH]; char stagedTempType[5]; char stashedTempType[5];
    char stagedTempModif[25]; char stashedTempModif[25];
    int diffCheckResult=0; int conflictFlag=0;
    while(fscanf(stagedFilesptr, "%s%s%s", stagedTempFile, stagedTempType, stagedTempModif)==3) {
        if(strcmp(stagedTempType, "d")==0) continue;
        if(strstr(stagedTempFile, ".txt")==NULL) continue;
        while(fscanf(stashedFilesptr, "%s%s%s", stashedTempFile, stashedTempType, stashedTempModif)==3) {
            if(strcmp(stagedTempFile, stashedTempFile)==0) {
                char *match = strstr(stashedTempFile, repoPath); 
                memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
                sprintf(stashedTempFilePlace, "%s\\ngit\\stashingArea\\%d\\content%s", repoPath, stashNumint, stashedTempFile);
                strcpy(stagedTempFilePlace, stagedTempFile);
                diffCheckResult=differenceCheck(stagedTempFilePlace, stashedTempFilePlace,1,100000,1,100000, 3);
                if(diffCheckResult==1) {
                    conflictFlag=1;
                }
                rewind(stashedFilesptr);
                break;
            }
        }
    }
    if(conflictFlag==1) {
        printf("poping stash was cancelled because of above conflict\nplease resolve conflicts manually and merge again\n");
        fclose(stagedFilesptr); fclose(stashedFilesptr);
        return;
    }
    fclose(stagedFilesptr);
    char tempFilePathtoCopy[MAX_PATH];
    char tempFiletypetoCopy[5];
    char tempFileModiftoCopy[25];
    rewind(stashedFilesptr);
    while(fscanf(stashedFilesptr, "%s%s%s", tempFilePathtoCopy, tempFiletypetoCopy, tempFileModiftoCopy)==3) {
        if(strcmp(tempFiletypetoCopy, "d")==0) continue;
        char lastPieceofFiletoCopy[MAX_PATH];
        char piceseofFilePath[10][20];
        strcpy(lastPieceofFiletoCopy, tempFilePathtoCopy);
        char *match = strstr(lastPieceofFiletoCopy, repoPath);
        memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
        int idx1=0;
        char* piece1=strtok(lastPieceofFiletoCopy, "\\");
        while(piece1!=NULL) {
            strcpy(piceseofFilePath[idx1], piece1);
            piece1=strtok(NULL,"\\");
            idx1++;
        }
        char tempFilePathsource[MAX_PATH];
        sprintf(tempFilePathsource, "%s\\ngit\\stashingArea\\%d\\content", repoPath, stashNumint);
        char destFilePathsource[MAX_PATH];
        sprintf(destFilePathsource, "%s", repoPath);
        for(int i=0; i<idx1-1; i++) {
            strcat(tempFilePathsource, "\\");
            strcat(tempFilePathsource, piceseofFilePath[i]);
            strcat(destFilePathsource, "\\");
            strcat(destFilePathsource, piceseofFilePath[i]);
            DIR* dirptr=opendir(destFilePathsource);
            if(dirptr==NULL) {
                closedir(dirptr);
                CreateDirectory(destFilePathsource, NULL);
            }
            closedir(dirptr);
        }
        strcat(tempFilePathsource, "\\");
        strcat(tempFilePathsource, piceseofFilePath[idx1-1]);
        strcat(destFilePathsource, "\\");
        strcat(destFilePathsource, piceseofFilePath[idx1-1]);
        if(strcmp(tempFiletypetoCopy, "d")==0) {
            DIR* dirptr=opendir(destFilePathsource);
            if(dirptr==NULL) {
                closedir(dirptr);
                CreateDirectory(destFilePathsource, NULL);
            }
            closedir(dirptr);
        }
        else {
            FILE *batchFile=fopen("copyfile.bat", "w");
            fprintf(batchFile, "@echo off\n");
            fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", tempFilePathsource, destFilePathsource);
            fprintf(batchFile, "exit /b 0\n");
            fclose(batchFile);
            system("copyfile.bat");
            char batFilePath[MAX_PATH];
            strcpy(batFilePath, currentPath);
            strcat(batFilePath, "\\copyfile.bat");
            remove(batFilePath);
        } 
    }
    fclose(stashedFilesptr); SetFileAttributes(stagedFilesPath, FILE_ATTRIBUTE_NORMAL);
    FILE *batchFile=fopen("copyfile.bat", "w");
    fprintf(batchFile, "@echo off\n");
    fprintf(batchFile, "copy /Y \"%s\" \"%s\" > NUL \n", stashedFilesPath, stagedFilesPath);
    fprintf(batchFile, "exit /b 0\n");
    fclose(batchFile);
    system("copyfile.bat");
    char batFilePath[MAX_PATH];
    strcpy(batFilePath, currentPath);
    strcat(batFilePath, "\\copyfile.bat");
    remove(batFilePath);
    sprintf(allStashesPath, "%s\\ngit\\info\\allStashes.txt", repoPath);
    char newAllStashesPath[MAX_PATH]; sprintf(newAllStashesPath, "%s\\ngit\\info\\newAllStashes.txt", repoPath);
    FILE* allStashesptr1=fopen(allStashesPath, "r"); FILE* newAllStashesptr1=fopen(newAllStashesPath, "w");
    char line1[100]; char stashData1[5][73];
    while (fgets(line1, sizeof(line1), allStashesptr1)) {
        line1[strcspn(line1, "\n")] = '\0';
        strcpy(stashData1[0], line1);
        for (int i = 1; i < 5; i++) {
            if (!fgets(line1, sizeof(line1), allStashesptr1)) {
                printf("Error: Unexpected end of file\n");
                return;
            }
            line1[strcspn(line1, "\n")] = '\0';
            strcpy(stashData1[i], line1);
        }
        if(strcmp(stashIndex, stashData1[0])!=0)
            fprintf(newAllStashesptr1, "%s\n%s\n%s\n%s\n%s\n", stashData1[0], stashData1[1], stashData1[2], stashData1[3], stashData1[4]);
    }
    fclose(newAllStashesptr1); fclose(allStashesptr1); SetFileAttributes(allStashesPath, FILE_ATTRIBUTE_NORMAL);
    remove(allStashesPath); rename(newAllStashesPath, allStashesPath);
    printf("stash was poped successfully\n");
    return;
}

void deleteofDirectory(const char *dirPath) {
    DIR *directory = opendir(dirPath);
    struct dirent *entry;

    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);

            if (entry->d_type == DT_DIR) {
                deleteofDirectory(path);
            } else {
                remove(path) != 0;
            }
        }
    }
    closedir(directory);
    rmdir(dirPath);
}

void stashClear() {
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

    char allStashesPath[MAX_PATH]; sprintf(allStashesPath, "%s\\ngit\\info\\allStashes.txt", repoPath);
    FILE* allStashesptr=fopen(allStashesPath, "w"); fclose(allStashesptr);
    char stashingAreaPath[MAX_PATH]; sprintf(stashingAreaPath, "%s\\ngit\\stashingArea", repoPath);
    deleteofDirectory(stashingAreaPath); CreateDirectory(stashingAreaPath, NULL);
    printf("removed all stashes\n");
    return;
}