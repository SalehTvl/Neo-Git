#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void grepWord(char* fileName, char* targetWord, char* commitHash, int state) {
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
    char filePath[MAX_PATH]; sprintf(filePath, "%s\\%s", currentPath, fileName);
    char fileAbsolutePath[MAX_PATH]; 

    if(state==2 || state==4) {
        int flag=1;
        char commitBranch[30];
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
            if(strcmp(commitData[0], commitHash)==0) {
                strcpy(commitBranch, commitData[1]);
                flag=1;
                break;
            }
        }
        if(flag==0) {
            printf("the commit hash <%s> in invalid\n", commitHash);
            return;
        }
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
            if(strcmp(commitHash, commitData[0])==0) break;
            if(strcmp(commitBranch, commitData[1])==0) count++;
        }
        fclose(allCommitsptr);
        char lastCommitBranPath[MAX_PATH]; sprintf(lastCommitBranPath, "%s\\ngit\\info\\%slastCommit.txt", repoPath, commitBranch);
        int lastCommit=0;
        FILE* lastCommitBranptr=fopen(lastCommitBranPath, "r"); 
        fscanf(lastCommitBranptr, "%d", &lastCommit); fclose(lastCommitBranptr);
        lastCommit-=count;
        char branchLastComContPath[MAX_PATH]; sprintf(branchLastComContPath, "%s\\ngit\\branches\\%s\\commits\\%d", repoPath, commitBranch, lastCommit);
        char *match = strstr(filePath, repoPath); 
        memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
        sprintf(fileAbsolutePath, "%s\\content%s", branchLastComContPath, filePath);
    }
    else {
        strcpy(fileAbsolutePath, filePath);
    }
    int wordFoudFlag=0; int lineCounter=0;
    char fileLine[1000]; char oneLineWords[60][20];
    FILE* targetFileptr=fopen(fileAbsolutePath, "r");
    if(targetFileptr==NULL) {
        printf("the file <%s> was not found\n", fileName);
        fclose(targetFileptr);
        return;
    }
    while(fgets(fileLine, sizeof(fileLine), targetFileptr)!=NULL) {
        fileLine[strcspn(fileLine, "\n")] = '\0';
        const char *wordPosition = strstr(fileLine, targetWord);
        lineCounter++;
        if (wordPosition == NULL) {
            continue;
        }
        wordFoudFlag=1;
        size_t wordLength = strlen(targetWord);
        size_t firstPartLength = wordPosition - fileLine;
        char *firstPart = malloc(firstPartLength + 1);
        strncpy(firstPart, fileLine, firstPartLength);
        firstPart[firstPartLength] = '\0';
        size_t secondPartLength = strlen(fileLine) - (firstPartLength + wordLength);
        char *secondPart = malloc(secondPartLength + 1);
        strncpy(secondPart, wordPosition + wordLength, secondPartLength);
        secondPart[secondPartLength] = '\0';
        if(state>2) printf("%d. ", lineCounter);
        printf("%s", firstPart);
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD savedAttributes;
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        savedAttributes = consoleInfo.wAttributes;
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        printf("%s", targetWord);
        SetConsoleTextAttribute(hConsole, savedAttributes);
        printf("%s\n", secondPart);
        free(firstPart);
        free(secondPart);
        
    }
    fclose(targetFileptr);
    if(wordFoudFlag==0) {
        printf("the word <%s> was not found inside the file\n", targetWord);
    }
    return;
}
