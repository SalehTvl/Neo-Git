#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>

int fileExistsF(char filePath[]) {
    FILE* fileptr=fopen(filePath, "r");
    if(fileptr==NULL) 
        return 0;
    return 1;
}

char* currentRepoPathF() {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    int flag=0;
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result = strstr(currentPath, repoPath);
        if(result!=NULL) {
            flag=1;
            break;
        } 
    }
    fclose(reposfile);
    if(flag==0) return NULL;
    return repoPath;
}