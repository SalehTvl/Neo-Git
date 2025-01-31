#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>

char fileStageState(char* filePath, char* repoPath) {
    char *match = strstr(filePath, repoPath);
    memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
    strcat(repoPath, "\\ngit\\stagingArea");
    strcat(repoPath, filePath);
    FILE* isFileStaged=fopen(repoPath, "r");
    if(isFileStaged==NULL) return '-';
    return '+';
}

char fileModificationState(char* filePath, char* repoPath, int fileDepth, char stageState) {
    strcat(repoPath, "\\ngit\\info\\contents\\");
    char newFilespath[MAX_PATH];
    char oldFilespath[MAX_PATH];
    sprintf(newFilespath, "%s%d\\newfiles.txt", repoPath, fileDepth);
    sprintf(oldFilespath, "%s%d\\oldfiles.txt", repoPath, fileDepth);
    int new=0; int old=0;
    char randomFile[MAX_PATH];
    FILE* newFilesptr=fopen(newFilespath, "r");
    FILE* oldFilesptr=fopen(oldFilespath, "r");
    while(fgets(randomFile, sizeof(randomFile), newFilesptr) != NULL) {
        size_t len = strlen(randomFile);
        if (len > 0 && randomFile[len - 1] == '\n') {
            randomFile[len - 1] = '\0';
        }
        if(strcmp(randomFile, filePath)==0) {
            new=1;
            break;
        }
    }
    while(fgets(randomFile, sizeof(randomFile), oldFilesptr) != NULL) {
        size_t len = strlen(randomFile);
        if (len > 0 && randomFile[len - 1] == '\n') {
            randomFile[len - 1] = '\0';
        }
        if(strcmp(randomFile, filePath)==0) {
            old=1;
            break;
        }
    }
    fclose(newFilesptr);
    fclose(oldFilesptr);
    ///////////////////////////// modified ro hanooz kari nakardam inja
    if(stageState=='+') {
        if(old==0 && new==1)
            return 'A';
        if(old==1 && new==0)
            return 'D';
        if(old==0 && new==0)
            return 'D';
        if(old==1 && new==1) {
            // boro bebin ba sha256 modify shodanesh moshakhas mishe ya na
            return 'N'; // N yani not changed
        }
    }
    else {
        if(old==0 && new==1)
            return 'U';
    }
}

void directoryStatus() {
    char directoryPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, directoryPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result = strstr(directoryPath, repoPath);
        if(result!=NULL) {
            break;
        } 
    }
    fclose(reposfile);

    char dirPathcopy[MAX_PATH];
    strcpy(dirPathcopy, directoryPath);        
    char *match = strstr(dirPathcopy, repoPath);
    memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
    int idx=0;
    char* piece=strtok(dirPathcopy, "\\");
    while(piece!=NULL) {
        piece=strtok(NULL,"\\");
        idx++;
    }
    int fileDepth=idx;

    char filePath[MAX_PATH];
    DIR *dir = opendir(directoryPath);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR) {
            strcpy(filePath, directoryPath);
            strcat(filePath, "\\");
            strcat(filePath, entry->d_name);
            char stageState=fileStageState(filePath, repoPath);
            char modificationState=fileModificationState(filePath, repoPath, fileDepth, stageState);
            printf("%s %c%c\n", entry->d_name, stageState, modificationState);
        }
    }
    closedir(dir);
}

