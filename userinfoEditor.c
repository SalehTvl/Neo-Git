#include <stdio.h>
#include <windows.h>
#include <string.h>

void userLocalInfoE(int argc, char* argv[], int info) {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentPath);
    char repoPath[100];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt", "r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!=NULL) {   
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result=strstr(currentPath, repoPath);
        if(result!=NULL) break;        
    }
    fclose(reposfile);
    strcat(repoPath, "\\ngit\\info\\localuser.txt");
    FILE* userInfofile=fopen(repoPath, "r");
    char name[100]; char email[100];
    fgets(name, sizeof(name), userInfofile);
    fgets(email, sizeof(email), userInfofile);
    fgets(email, sizeof(email), userInfofile);
    size_t lengthname = strlen(name);
        if (lengthname > 0 && name[lengthname - 1] == '\n') {
            name[lengthname - 1] = '\0';
        }
    size_t lengthemail = strlen(email);
        if (lengthemail > 0 && email[lengthemail - 1] == '\n') {
            email[lengthemail - 1] = '\0';
        }
    fclose(userInfofile);
    userInfofile=fopen(repoPath, "w");
    if(info==0) {
        fputs(argv[3], userInfofile);
        fputs("\n", userInfofile);
        fputs(email, userInfofile);
    }
    else {
        fputs(name, userInfofile);
        fputs("\n", userInfofile);
        fputs(argv[3], userInfofile);
    }
    fclose(userInfofile);
}

void userGlobalInfoE(int argc, char* argv[], int info) {
    FILE* userInfofile=fopen("d:\\ANGP\\ngit-project\\userInfo.txt", "r");
    char name[100]; char email[100];
    fgets(name, sizeof(name), userInfofile);
    fgets(email, sizeof(email), userInfofile);
    fgets(email, sizeof(email), userInfofile);
    size_t lengthname = strlen(name);
        if (lengthname > 0 && name[lengthname - 1] == '\n') {
            name[lengthname - 1] = '\0';
        }
    size_t lengthemail = strlen(email);
        if (lengthemail > 0 && email[lengthemail - 1] == '\n') {
            email[lengthemail - 1] = '\0';
        }
    fclose(userInfofile);
    userInfofile=fopen("d:\\ANGP\\ngit-project\\userInfo.txt", "w");
    if(info==0) {
        fputs(argv[4], userInfofile);
        fputs("\n", userInfofile);
        fputs(email, userInfofile);
    }
    else {
        fputs(name, userInfofile);
        fputs("\n", userInfofile);
        fputs(argv[4], userInfofile);
    }
    fclose(userInfofile);
}