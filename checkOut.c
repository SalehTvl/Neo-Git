#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void deleterofDirectory(const char *dirPath) {
    DIR *directory = opendir(dirPath);
    struct dirent *entry;

    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);

            if (entry->d_type == DT_DIR) {
                deleterofDirectory(path);
            } else {
                remove(path) != 0;
            }
        }
    }

    closedir(directory);
    rmdir(dirPath);
}

void checkoutBranch(char* branchName, int state) {
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

    char lastCommitBranPath[MAX_PATH]; sprintf(lastCommitBranPath, "%s\\ngit\\info\\%slastCommit.txt", repoPath, branchName);
    int lastCommit=0;
    FILE* lastCommitBranptr=fopen(lastCommitBranPath, "r"); 
    if(lastCommitBranptr==NULL) {
        printf("branch <%s> does not exist\n", branchName); return;
    }

    fscanf(lastCommitBranptr, "%d", &lastCommit); fclose(lastCommitBranptr);
    if(state!=0) lastCommit-=state;
    char branchLastComContPath[MAX_PATH]; sprintf(branchLastComContPath, "%s\\ngit\\branches\\%s\\commits\\%d", repoPath, branchName, lastCommit);
    char branchCommitedFilesPath[MAX_PATH]; sprintf(branchCommitedFilesPath, "%s\\commitedfiles.txt", branchLastComContPath);
    char stagedFilesPath[MAX_PATH]; sprintf(stagedFilesPath, "%s\\ngit\\info\\stagedfiles.txt", repoPath);
    FILE* stagedFilesptr=fopen(stagedFilesPath, "r"); char tempsubFile[MAX_PATH]; char tempType[5]; char tempModif[25];
    while(fscanf(stagedFilesptr, "%s%s%s", tempsubFile, tempType, tempModif)==3) {
        if(strcmp(tempType, "d")==0) {
            deleterofDirectory(tempsubFile);
        }
        else {
            SetFileAttributes(tempsubFile, FILE_ATTRIBUTE_NORMAL);
            DeleteFile(tempsubFile);
        }
    }
    fclose(stagedFilesptr);
    char checkoutCommithash[9];
    char commitDetailPath[MAX_PATH]; sprintf(commitDetailPath, "%s\\commitDetail.txt", branchLastComContPath);
    FILE* commitDetailptr=fopen(commitDetailPath, "r"); fscanf(commitDetailptr, "%s", checkoutCommithash); fclose(commitDetailptr);
    char curCommithashPath[MAX_PATH]; sprintf(curCommithashPath, "%s\\ngit\\info\\curCommithash.txt", repoPath);
    FILE* curCommithashptr=fopen(curCommithashPath, "w"); fprintf(curCommithashptr, "%s", checkoutCommithash); fclose(curCommithashptr);
    strcat(branchLastComContPath, "\\content");
    char tempFilePathtoCopy[MAX_PATH];
    char tempFiletypetoCopy[5];
    char tempFileModiftoCopy[25];
    FILE* commitedFilesptr=fopen(branchCommitedFilesPath, "r");
    while(fscanf(commitedFilesptr, "%s%s%s", tempFilePathtoCopy, tempFiletypetoCopy, tempFileModiftoCopy)==3) {
        //if(strcmp(tempFiletypetoCopy, "d")==0) //continue;
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
        sprintf(tempFilePathsource, "%s", branchLastComContPath);
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
            strcpy(batFilePath, filePath);
            strcat(batFilePath, "\\copyfile.bat");
            remove(batFilePath);
        } 
    }
    fclose(commitedFilesptr);
    FILE *batchFile=fopen("copyfile.bat", "w");
    fprintf(batchFile, "@echo off\n");
    fprintf(batchFile, "set \"sourceFile=%s\"\n", branchCommitedFilesPath);
    fprintf(batchFile, "set \"destinationFile=%s\"\n", stagedFilesPath);
    fprintf(batchFile, ")\n");
    fprintf(batchFile, "type \"%%sourceFile%%\" > \"%%destinationFile%%\"\n");
    fprintf(batchFile, "exit /b 0\n");
    fclose(batchFile);
    system("copyfile.bat");
    char batFilePath[MAX_PATH];
    strcpy(batFilePath, filePath);
    strcat(batFilePath, "\\copyfile.bat");
    remove(batFilePath);
    char currentbranchPath[MAX_PATH]; sprintf(currentbranchPath, "%s\\ngit\\info\\currentbranch.txt", repoPath);
    FILE* curBranchptr=fopen(currentbranchPath, "w"); fprintf(curBranchptr, "%s", branchName); fclose(curBranchptr);
    char currentbranchMainPath[MAX_PATH]; sprintf(currentbranchMainPath, "d:\\ANGP\\ngit-project\\currentbranch.txt");
    FILE* curBranchMainptr=fopen(currentbranchMainPath, "w"); fprintf(curBranchMainptr, "%s", branchName); fclose(curBranchMainptr);
    return;
}

void checkoutHash(char* hash, int state) {
    int flag=0;
    if(strcmp(hash, "HEAD")==0) flag=1;
    int number=0;
    if(state==1) {
        number=atoi(hash);
    }
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
    char allCommitsPath[MAX_PATH]; sprintf(allCommitsPath, "%s\\ngit\\info\\allCommits.txt", repoPath);
    FILE* allCommitptr=fopen(allCommitsPath, "r");
    char line[100];
    char commitData[7][100];
    int hashExists=0;
    while (fgets(line, sizeof(line), allCommitptr)) {
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
        number--;
        if(strcmp(hash, commitData[0])==0 || flag==1 || (state==1 && number==0)) {
            hashExists=1;
            break;
        }
    }
    if(hashExists==0) {
        printf("invalid hash\n");
        return;
    }
    if(flag==1) hash=commitData[0];
    char branchOfHash[20]; strcpy(branchOfHash, commitData[1]);
    int count=0;
    rewind(allCommitptr);
    while (fgets(line, sizeof(line), allCommitptr)) {
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
        if(strcmp(hash, commitData[0])==0) break;
        if(strcmp(branchOfHash, commitData[1])==0) count++;
    }
    fclose(allCommitptr);
    checkoutBranch(branchOfHash, count);
}

void checkoutHead() {
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

    char removedFiles[MAX_PATH]; sprintf(removedFiles, "%s\\ngit\\info\\removedFiles.txt", repoPath);
    char lastStaged[MAX_PATH]; sprintf(lastStaged, "%s\\ngit\\info\\lastStaged.txt", repoPath);
    FILE* removedFilesptr=fopen(removedFiles, "w"); fclose(removedFilesptr);
    FILE* lastStagedptr=fopen(lastStaged, "w"); fclose(lastStagedptr);
    return;
}