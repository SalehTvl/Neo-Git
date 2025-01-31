#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <time.h>
#include <shellapi.h>

void commitCreator(int state, char* message) {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,currentPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len1 = strlen(repoPath);
        if (len1 > 0 && repoPath[len1 - 1] == '\n') {
            repoPath[len1 - 1] = '\\'; repoPath[len1]='\0';
        }
        char* result = strstr(currentPath, repoPath);
        if(result!=NULL) break;
    }
    fclose(reposfile);
    if(state==1) {
        int flag=0;
        char commitsetMessagePath[MAX_PATH];
        strcpy(commitsetMessagePath, repoPath);
        strcat(commitsetMessagePath, "\\ngit\\info\\commitsetMessage.txt");
        FILE* commitsetFIleptr=fopen(commitsetMessagePath, "r");
        char shortcutName[40];
        char shortcMessage[80];
        while(fscanf(commitsetFIleptr, "%99s", shortcutName)==1) {
            fgets(shortcMessage, sizeof(shortcMessage), commitsetFIleptr);
            size_t len1 = strlen(shortcMessage);
            if (len1 > 0 && shortcMessage[len1 - 1] == '\n') {
                shortcMessage[len1 - 1] = '\0';
            }
            if(shortcMessage[0]==' ') {
                memmove(shortcMessage, shortcMessage + 1, strlen(shortcMessage));
                shortcMessage[strlen(shortcMessage)] = '\0';
            }
            if(strcmp(message, shortcutName)==0) {
                message=shortcMessage;
                flag=1;
                break;
            }
        }
        if(flag==0) {
            printf("shortcut <%s> does not exist", message);
            return ;
        }
    }
    
    FILE* totalCommmitptr=fopen("d:\\ANGP\\ngit-project\\totalCommitCount.txt", "r");
    int totalCommit=0;
    fscanf(totalCommmitptr, "%d", &totalCommit);
    fclose(totalCommmitptr);
    totalCommit++;
    totalCommmitptr=fopen("d:\\ANGP\\ngit-project\\totalCommitCount.txt", "w");
    fprintf(totalCommmitptr, "%d", totalCommit);
    fclose(totalCommmitptr);
    char curCommithashPath[MAX_PATH]; sprintf(curCommithashPath, "%s\\ngit\\info\\curCommithash.txt", repoPath);
    FILE* curCommithashptr=fopen(curCommithashPath, "w"); fprintf(curCommithashptr, "%08d", totalCommit); fclose(curCommithashptr);
    FILE* currentBranchptr=fopen("d:\\ANGP\\ngit-project\\currentbranch.txt", "r");
    char currentbranch[25];
    fscanf(currentBranchptr, "%s", currentbranch);
    fclose(currentBranchptr);
    char lastCommitFilePath[MAX_PATH];
    strcpy(lastCommitFilePath, repoPath);
    strcat(lastCommitFilePath, "\\ngit\\info\\");
    strcat(lastCommitFilePath, currentbranch);
    strcat(lastCommitFilePath, "lastCommit.txt");
    FILE* lastCommitptr=fopen(lastCommitFilePath, "r");
    int lastCommit=0;
    fscanf(lastCommitptr, "%d", &lastCommit);
    fclose(lastCommitptr);
    lastCommit++;
    lastCommitptr=fopen(lastCommitFilePath, "w");
    fprintf(lastCommitptr, "%d", lastCommit);
    fclose(lastCommitptr);
    char lasStagedPath[MAX_PATH]; sprintf(lasStagedPath, "%s\\ngit\\info\\lastStaged.txt", repoPath);
    FILE* lasStagedptr=fopen(lasStagedPath, "w"); fclose(lasStagedptr);

    char commitFolderPath[MAX_PATH];
    strcpy(commitFolderPath, repoPath);
    strcat(commitFolderPath, "\\ngit\\branches\\");
    char restofCommitFolderPath[MAX_PATH];
    sprintf(restofCommitFolderPath, "%s\\commits\\%d", currentbranch, lastCommit);
    strcat(commitFolderPath, restofCommitFolderPath);
    CreateDirectory(commitFolderPath, NULL);
    char commitContentFilePath[MAX_PATH];
    char stagingAreapath[MAX_PATH];
    strcpy(commitContentFilePath, commitFolderPath);
    strcat(commitContentFilePath, "\\content");
    strcpy(stagingAreapath, repoPath);
    strcat(stagingAreapath, "\\ngit\\stagingArea");
    FILE* batchFile = fopen("copydir.bat", "w");
    fprintf(batchFile, "@echo off\n");
    fprintf(batchFile, "set \"sourceDirectory=%s\"\n", stagingAreapath);
    fprintf(batchFile, "set \"destinationDirectory=%s\"\n", commitContentFilePath);
    fprintf(batchFile, ")\n");
    fprintf(batchFile, "xcopy \"%%sourceDirectory%%\" \"%%destinationDirectory%%\" /E /I /Q /Y >nul\n");
    fprintf(batchFile, "exit /b 0\n");
    fclose(batchFile);
    system("copydir.bat");
    char batFilePath[MAX_PATH];
    strcpy(batFilePath, currentPath);
    strcat(batFilePath, "\\copydir.bat");
    if(remove(batFilePath)!=0) printf("failed to remove batchfile\n");
    char localuserPath[MAX_PATH];
    strcpy(localuserPath, repoPath);
    strcat(localuserPath, "\\ngit\\info\\localuser.txt");
    FILE* localUserptr=fopen(localuserPath, "r");
    char username[50];
    char useremail[50];
    fgets(username, sizeof(username), localUserptr);
    size_t len2 = strlen(username);
    if (len2 > 0 && username[len2 - 1] == '\n') {
        username[len2 - 1] = '\0';
    }
    fgets(useremail, sizeof(useremail), localUserptr);
    size_t len3 = strlen(useremail);
    if (len3 > 0 && useremail[len3 - 1] == '\n') {
        useremail[len3 - 1] = '\0';
    }
    fclose(localUserptr);
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    char* dateandTime=buffer;
    char commitDetailPath[MAX_PATH];
    char stagedfilesPath[MAX_PATH];
    strcpy(stagedfilesPath, repoPath);
    strcat(stagedfilesPath, "\\ngit\\info\\stagedfiles.txt");
    FILE* stagedFilesptr2=fopen(stagedfilesPath, "r");
    int countofFiles=0;
    char subPath[MAX_PATH];
    char subType[5];
    char subModified[30];
    while(fscanf(stagedFilesptr2, "%s%s%s", subPath, subType, subModified)==3) {
        if(strcmp(subType, "f")==0) {
            countofFiles++;
        }
    }
    fclose(stagedFilesptr2);
    strcpy(commitDetailPath, commitFolderPath); 
    strcat(commitDetailPath, "\\commitDetail.txt");
    FILE* commitDetailptr=fopen(commitDetailPath, "w");
    fprintf(commitDetailptr, "%08d\n%s\n%d\n%s\n%s\n%s\n%s", totalCommit, currentbranch, countofFiles, message, username, useremail, dateandTime);
    fclose(commitDetailptr);
    printf("commited successfully\n%08d\n%s\n%s\n%s\n%s\n%s", totalCommit, currentbranch, message, username, useremail, dateandTime);
    char allCommitPath[MAX_PATH];
    strcpy(allCommitPath, repoPath);
    strcat(allCommitPath, "\\ngit\\info\\allCommits.txt");
    char commitHash[9];
    char commitBranch[20];
    char commitedFiles[6];
    char commitMessage[80];
    char commitAuthor[30];
    char commitAuthorEmail[30];
    char commitDate[30];
    char newallCommitPath[MAX_PATH];
    strcpy(newallCommitPath, repoPath);
    strcat(newallCommitPath, "\\ngit\\info\\newallCommits.txt");
    FILE* allCommitptr=fopen(allCommitPath, "r");
    FILE* newallCommitptr=fopen(newallCommitPath, "w");
    fprintf(newallCommitptr, "%08d\n%s\n%d\n%s\n%s\n%s\n%s\n", totalCommit, currentbranch, countofFiles, message, username, useremail, dateandTime);
    char line[100];
    char commitData[7][100];
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
        fprintf(newallCommitptr, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n", commitData[0], commitData[1], commitData[2], commitData[3], commitData[4], commitData[5], commitData[6]);
    }
    fclose(allCommitptr);
    fclose(newallCommitptr);
    SetFileAttributes(allCommitPath, FILE_ATTRIBUTE_NORMAL);
    DeleteFile(allCommitPath);
    rename(newallCommitPath, allCommitPath);
    char commitedfilesPath[MAX_PATH];
    strcpy(commitedfilesPath, commitFolderPath);
    strcat(commitedfilesPath, "\\commitedfiles.txt");
    char begstagedfilesPath[MAX_PATH];
    strcpy(begstagedfilesPath, repoPath);
    strcat(begstagedfilesPath, "\\ngit\\info\\begstagedfiles.txt");
    FILE* batchFile1=fopen("copyfile.bat", "w");
    fprintf(batchFile1, "@echo off\n");
    fprintf(batchFile1, "set \"sourceFile=%s\"\n", stagedfilesPath);
    fprintf(batchFile1, "set \"destinationFile=%s\"\n", commitedfilesPath);
    fprintf(batchFile1, ")\n");
    fprintf(batchFile1, "type \"%%sourceFile%%\" > \"%%destinationFile%%\"\n");
    fprintf(batchFile1, "exit /b 0\n");
    fclose(batchFile1);
    system("copyfile.bat");
    char batFile1Path[MAX_PATH];
    strcpy(batFile1Path, currentPath);
    strcat(batFile1Path, "\\copyfile.bat");
    if(remove(batFile1Path)!=0) printf("failed to remove batchfile1\n");
    char subPath0[MAX_PATH];
    char subType0[5];
    char subModified0[30];
    FILE* stagedFilesptr=fopen(stagedfilesPath, "r");
    FILE* begstagedFilesptr=fopen(begstagedfilesPath, "w");
    while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
        fprintf(begstagedFilesptr, "%s %s %s\n", subPath0, subType0, subModified0);
    }
    fclose(stagedFilesptr);
    fclose(begstagedFilesptr);
    char removedFilesPath[MAX_PATH]; sprintf(removedFilesPath, "%s\\ngit\\info\\removedFiles.txt", repoPath);
    FILE* removedFilesptr=fopen(removedFilesPath, "w"); fclose(removedFilesptr);
}