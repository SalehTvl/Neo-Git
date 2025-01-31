#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>

void deleteDirectory(const char *dirPath) {
    DIR *directory = opendir(dirPath);
    struct dirent *entry;

    if (directory == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);

            if (entry->d_type == DT_DIR) {
                deleteDirectory(path);
            } else {
                if (remove(path) != 0) {
                    perror("Unable to delete file");
                }
            }
        }
    }

    closedir(directory);

    if (rmdir(dirPath) != 0) {
        perror("Unable to delete directory");
    }
}

void resetStage(char* argv) {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result = strstr(currentPath, repoPath);
        if(result!=NULL) {
            break;
        } 
    }
    fclose(reposfile);
    
    char repoPathcopy1[MAX_PATH];
    strcpy(repoPathcopy1, repoPath);
    char repoPathcopy2[MAX_PATH];
    strcpy(repoPathcopy2, repoPath);
    strcat(repoPath, "\\ngit\\info\\stagedfiles.txt");
    strcat(repoPathcopy1, "\\ngit\\info\\newstagedfiles.txt");
    FILE* stagedfiles=fopen(repoPath, "r");
    FILE* newstagedfiles=fopen(repoPathcopy1, "w");
    char lastStagedfile[MAX_PATH]; sprintf(lastStagedfile, "%s\\%s", currentPath, argv);
    char subPath0[MAX_PATH];
    char subPath1[MAX_PATH];
    char subType0[5];
    char subType1[5];
    char subModified0[30];
    char subModified1[30];
    char stagedResetPath[MAX_PATH];
    if(strcmp(argv, "-undo")==0) {
        char lastStagedPath[MAX_PATH]; strcpy(lastStagedPath, repoPathcopy2); strcat(lastStagedPath, "\\ngit\\info\\lastStaged.txt");
        char lastStagedremPath[MAX_PATH]; strcpy(lastStagedremPath, repoPathcopy2); strcat(lastStagedremPath, "\\ngit\\info\\remlastStaged.txt");
        FILE* lastStagedptr=fopen(lastStagedPath, "r");  fscanf(lastStagedptr, "%s", stagedResetPath);
        FILE* lastStagedremptr=fopen(lastStagedremPath, "w");
        char tempLastStaged[MAX_PATH];
        while(fscanf(lastStagedptr, "%s", tempLastStaged)==1) {
            fprintf(lastStagedremptr, "%s\n", tempLastStaged);
        }
        fclose(lastStagedptr); fclose(lastStagedremptr);
        SetFileAttributes(lastStagedPath, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(lastStagedPath);                                  
        rename(lastStagedremPath, lastStagedPath);
    }
    else {
        strcpy(stagedResetPath, currentPath);
        strcat(stagedResetPath, "\\");
        strcat(stagedResetPath, argv);
        char lastStagedPath[MAX_PATH]; strcpy(lastStagedPath, repoPathcopy2); strcat(lastStagedPath, "\\ngit\\info\\lastStaged.txt");
        char lastStagedremPath[MAX_PATH]; strcpy(lastStagedremPath, repoPathcopy2); strcat(lastStagedremPath, "\\ngit\\info\\remlastStaged.txt");
        FILE* lastStagedptr=fopen(lastStagedPath, "r");
        FILE* lastStagedremptr=fopen(lastStagedremPath, "w");
        char tempLastStaged[MAX_PATH];
        while(fscanf(lastStagedptr, "%s", tempLastStaged)==1) {
            if(strcmp(lastStagedfile, tempLastStaged)!=0)
                fprintf(lastStagedremptr, "%s\n", tempLastStaged);
        }
        fclose(lastStagedptr); fclose(lastStagedremptr);
        SetFileAttributes(lastStagedPath, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(lastStagedPath);                                  
        rename(lastStagedremPath, lastStagedPath);
    }

    char needlePath[MAX_PATH];
    strcpy(needlePath, stagedResetPath);
    strcat(needlePath, "\\");
    while(fscanf(stagedfiles, "%s%s%s", subPath0, subType0, subModified0)==3) {
        if(strstr(subPath0, needlePath)==NULL && strcmp(stagedResetPath, subPath0)!=0) {
            fprintf(newstagedfiles, "%s %s %s\n", subPath0, subType0, subModified0);
        }
    }
    SetFileAttributes(repoPath, FILE_ATTRIBUTE_NORMAL);
    fclose(newstagedfiles);
    fclose(stagedfiles);
    remove(repoPath);
    rename(repoPathcopy1, repoPath);

    char *match = strstr(stagedResetPath, repoPathcopy2);
    memmove(match, match + strlen(repoPathcopy2), strlen(match + strlen(repoPathcopy2)) + 1);
    strcat(repoPathcopy2, "\\ngit\\stagingArea");
    strcat(repoPathcopy2, stagedResetPath);
    FILE* fptr=fopen(repoPathcopy2, "r");
    DIR* dptr=opendir(repoPathcopy2);
    if(fptr!=NULL) {
        fclose(fptr);
        closedir(dptr);
        SetFileAttributes(repoPathcopy2, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(repoPathcopy2);
    }
    else {
        closedir(dptr);
        fclose(fptr);
        char dirName[MAX_PATH];
        strcpy(dirName, repoPathcopy2);
        deleteDirectory(dirName);
    }                                  
}