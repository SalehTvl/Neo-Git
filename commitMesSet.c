#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void commitMesSet(char* message, char* shortcutName) {
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
    char commitsetMessagesaddress[MAX_PATH];
    strcpy(commitsetMessagesaddress, repoPath);
    strcat(commitsetMessagesaddress, "\\ngit\\info\\commitsetMessage.txt");
    FILE* commitsetMessptr=fopen(commitsetMessagesaddress, "a");
    fprintf(commitsetMessptr, "%s %s\n", shortcutName, message);
    fclose(commitsetMessptr);
}

void commitMesReplace(char* message, char* shortcutName) {
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
    char commitsetMessagesaddress[MAX_PATH];
    strcpy(commitsetMessagesaddress, repoPath);
    strcat(commitsetMessagesaddress, "\\ngit\\info\\commitsetMessage.txt");
    char commitrepMessagesaddress[MAX_PATH];
    strcpy(commitrepMessagesaddress, repoPath);
    strcat(commitrepMessagesaddress, "\\ngit\\info\\repcommitsetMessage.txt");
    FILE* commitsetMessptr=fopen(commitsetMessagesaddress, "r");
    FILE* commitrepMesptr=fopen(commitrepMessagesaddress, "w");
    char tempShortcut[100];
    char tempMessage[100];
    int flag=0;
    while(fscanf(commitsetMessptr, "%99s", tempShortcut)==1) {
        fgets(tempMessage, sizeof(tempMessage), commitsetMessptr);
        size_t len1 = strlen(tempMessage);
        if (len1 > 0 && tempMessage[len1 - 1] == '\n') {
            tempMessage[len1 - 1] = '\0';
        }
        if(tempMessage[0]==' ') {
            memmove(tempMessage, tempMessage + 1, strlen(tempMessage));
            tempMessage[strlen(tempMessage)] = '\0';
        }
        if(strcmp(tempShortcut,shortcutName)==0) {
            flag=1;
            fprintf(commitrepMesptr, "%s %s\n", shortcutName, message);
        }
        else {
            fprintf(commitrepMesptr, "%s %s\n", tempShortcut, tempMessage); 
        }
    }
    fclose(commitsetMessptr);
    fclose(commitrepMesptr);
    if(flag==0) {
        printf("shortcut <%s> does not exist\n", shortcutName);
        SetFileAttributes(commitrepMessagesaddress, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(commitrepMessagesaddress);
    }
    else {
        printf("shortcut's message successfully changed to <%s>\n", message);
        SetFileAttributes(commitsetMessagesaddress, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(commitsetMessagesaddress);
        rename(commitrepMessagesaddress, commitsetMessagesaddress);
    }
}

void commitMesRemove(char* shortcutName) {
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
    char commitsetMessagesaddress[MAX_PATH];
    strcpy(commitsetMessagesaddress, repoPath);
    strcat(commitsetMessagesaddress, "\\ngit\\info\\commitsetMessage.txt");
    FILE* commitsetMessptr=fopen(commitsetMessagesaddress, "r");
    char commitrepMessagesaddress[MAX_PATH];
    strcpy(commitrepMessagesaddress, repoPath);
    strcat(commitrepMessagesaddress, "\\ngit\\info\\repcommitsetMessage.txt");
    FILE* commitrepMesptr=fopen(commitrepMessagesaddress, "w");
    char tempShortcut[100];
    char tempMessage[100];
    int flag=0;
    while(fscanf(commitsetMessptr, "%99s", tempShortcut)==1) {
        fgets(tempMessage, sizeof(tempMessage), commitsetMessptr);
        size_t len1 = strlen(tempMessage);
        if (len1 > 0 && tempMessage[len1 - 1] == '\n') {
            tempMessage[len1 - 1] = '\0';
        }
        if(tempMessage[0]==' ') {
            memmove(tempMessage, tempMessage + 1, strlen(tempMessage));
            tempMessage[strlen(tempMessage)] = '\0';
        }
        if(strcmp(tempShortcut,shortcutName)==0) {
            flag=1;
        }
        else {
            fprintf(commitrepMesptr, "%s %s\n", tempShortcut, tempMessage);
        }
    }
    fclose(commitsetMessptr);
    fclose(commitrepMesptr);
    if(flag==0) {
        printf("shortcut <%s> does not exist\n", shortcutName);
        SetFileAttributes(commitrepMessagesaddress, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(commitrepMessagesaddress);
    }
    else {
        printf("shortcut's message successfully removed");
        SetFileAttributes(commitsetMessagesaddress, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(commitsetMessagesaddress);
        rename(commitrepMessagesaddress, commitsetMessagesaddress);
    }
}