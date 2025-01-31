#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

void listDirectories(int isOld) {
    int dirDepth=0;
    char outputPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,outputPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len1 = strlen(repoPath);
        if (len1 > 0 && repoPath[len1 - 1] == '\n') {
            repoPath[len1 - 1] = '\0';
        }
        char* result = strstr(outputPath, repoPath);
        if(result!=NULL) break;
    }
    fclose(reposfile);
    strcpy(outputPath, repoPath);
    outputPath[strcspn(outputPath, "\n")] = '\0';
    strcat(outputPath, "\\ngit\\info\\contents");
    char outputfilepath[MAX_PATH];
    char extralstpath[MAX_PATH];
    char allAddress[MAX_PATH];
    strcpy(allAddress, outputPath);
    switch (isOld) {
        case 1 :
            strcat(allAddress, "\\oldAll.txt");
            break;
        case 0 :
            strcat(allAddress, "\\newAll.txt");
            break;
    }
    FILE* allFileptr = fopen(allAddress, "w");
    while(1) {
        sprintf(outputfilepath,"%s\\%d", outputPath, dirDepth);
        CreateDirectory(outputfilepath, NULL);
        strcpy(extralstpath, outputfilepath);
        switch (isOld) {
            case 1 :
                strcat(outputfilepath, "\\oldDirs.txt");
                break;
            case 0 :
                strcat(outputfilepath, "\\newDirs.txt");
                break;
        }
        FILE* outputfile = fopen(outputfilepath, "w");
        struct dirent *entry;
        int subDirsCount=0;
        if(dirDepth==0) {
            char dirPath[MAX_PATH];
            strcpy(dirPath, repoPath);
            dirPath[strcspn(dirPath, "\n")] = '\0';
            DIR* dirptr = opendir(dirPath);
            while ((entry = readdir(dirptr)) != NULL) {
                if (entry->d_type == DT_DIR) {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "ngit") != 0) {
                        fprintf(outputfile, "%s\\%s\n", dirPath, entry->d_name);
                        fprintf(allFileptr, "%s\\%s ", dirPath, entry->d_name);
                        fprintf(allFileptr, "d ");
                        char filename[MAX_PATH];
                        strcpy(filename, dirPath);
                        strcat(filename, "\\");
                        strcat(filename, entry->d_name);                       
                        struct stat file_info;
                        stat(filename, &file_info);
                        time_t modification_time = file_info.st_mtime;
                        char modified_time_str[20];
                        strftime(modified_time_str, sizeof(modified_time_str), "%Y-%m-%d%H:%M:%S", localtime(&modification_time));
                        fprintf(allFileptr, "%s\n", modified_time_str);
                        subDirsCount++;
                    }
                }
            }
            closedir(dirptr);
        }
        else {
            char dirPath[MAX_PATH];
            char Dirsfile[MAX_PATH];
            sprintf(Dirsfile,"%s\\%d", outputPath, dirDepth-1);
            switch (isOld) {
                case 1 :
                    strcat(Dirsfile, "\\oldDirs.txt");
                    break;
                case 0 :
                    strcat(Dirsfile, "\\newDirs.txt");
                    break;
            }
            FILE* dirsfileptr=fopen(Dirsfile, "r");
            while(fgets(dirPath, sizeof(dirPath), dirsfileptr)!=NULL) {
                size_t len2 = strlen(dirPath);
                if (len2 > 0 && dirPath[len2 - 1] == '\n') {
                    dirPath[len2 - 1] = '\0';
                }
                dirPath[strcspn(dirPath, "\n")] = '\0';
                DIR* dirptr = opendir(dirPath);
                while ((entry = readdir(dirptr)) != NULL) {
                    if (entry->d_type == DT_DIR) {
                        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "ngit") != 0) {
                            fprintf(outputfile, "%s\\%s\n", dirPath, entry->d_name);
                            subDirsCount++;
                            fprintf(allFileptr, "%s\\%s ", dirPath, entry->d_name);
                            fprintf(allFileptr, "d ");
                            char filename[MAX_PATH];
                            strcpy(filename, dirPath);
                            strcat(filename, "\\");
                            strcat(filename, entry->d_name);                       
                            struct stat file_info;
                            stat(filename, &file_info);
                            time_t modification_time = file_info.st_mtime;
                            char modified_time_str[20];
                            strftime(modified_time_str, sizeof(modified_time_str), "%Y-%m-%d%H:%M:%S", localtime(&modification_time));
                            fprintf(allFileptr, "%s\n", modified_time_str);
                        }
                    }
                }
                closedir(dirptr);
            }
            fclose(dirsfileptr);           
        }
        fclose(outputfile);
        dirDepth++;
        if(subDirsCount<1) {
            break; 
        }
    } 
    fclose(allFileptr);
    repoPath[strcspn(repoPath, "\n")] = '\0';
    strcat(repoPath, "\\ngit\\info\\maxDepth.txt");
    FILE* maxDepthfile=fopen(repoPath, "w");
    fprintf(maxDepthfile, "%d", dirDepth-1);
    fclose(maxDepthfile);  
}

void listFiles(int isOld) {
    int dirDepth=0;
    char outputPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,outputPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len3 = strlen(repoPath);
        if (len3 > 0 && repoPath[len3 - 1] == '\n') {
            repoPath[len3 - 1] = '\0';
        }
        char* result = strstr(outputPath, repoPath);
        if(result!=NULL) break;
    }
    fclose(reposfile);
    int maxDepth=0;
    char maxDepthaddress[MAX_PATH];
    repoPath[strcspn(repoPath, "\n")] = '\0';
    sprintf(maxDepthaddress, "%s\\ngit\\info\\maxDepth.txt", repoPath);
    FILE* maxDepthfile=fopen(maxDepthaddress, "r");
    fscanf(maxDepthfile, "%d", &maxDepth);
    fclose(maxDepthfile);
    strcpy(outputPath, repoPath);
    outputPath[strcspn(outputPath, "\n")] = '\0';
    strcat(outputPath, "\\ngit\\info\\contents");
    char outputfilepath[MAX_PATH];
    char extralstpath[MAX_PATH];
    char allAddress[MAX_PATH];
    strcpy(allAddress, outputPath);
    switch (isOld) {
        case 1 :
            strcat(allAddress, "\\oldAll.txt");
            break;
        case 0 :
            strcat(allAddress, "\\newAll.txt");
            break;
    }
    FILE* allFileptr=fopen(allAddress, "a");
    while(1) {
        sprintf(outputfilepath,"%s\\%d", outputPath, dirDepth);
        //CreateDirectory(outputfilepath, NULL);
        //strcpy(extralstpath, outputfilepath);
        switch (isOld) {
            case 1 :
                strcat(outputfilepath, "\\oldfiles.txt");
                break;
            case 0 :
                strcat(outputfilepath, "\\newfiles.txt");
                break;
        }
        FILE* outputfile = fopen(outputfilepath, "w");
        struct dirent *entry;
        int subDirsCount=0;
        if(dirDepth==0) {
            char dirPath[MAX_PATH];
            strcpy(dirPath, repoPath);
            dirPath[strcspn(dirPath, "\n")] = '\0';
            DIR* dirptr = opendir(dirPath);
            while ((entry = readdir(dirptr)) != NULL) {
                if (entry->d_type == DT_REG) {
                    fprintf(outputfile, "%s\\%s\n", dirPath, entry->d_name);
                    fprintf(allFileptr, "%s\\%s ", dirPath, entry->d_name);
                    fprintf(allFileptr, "f ");
                    char filename[MAX_PATH];
                    strcpy(filename, dirPath);
                    strcat(filename, "\\");
                    strcat(filename, entry->d_name);                       
                    struct stat file_info;
                    stat(filename, &file_info);
                    time_t modification_time = file_info.st_mtime;
                    char modified_time_str[20];
                    strftime(modified_time_str, sizeof(modified_time_str), "%Y-%m-%d%H:%M:%S", localtime(&modification_time));
                    fprintf(allFileptr, "%s\n", modified_time_str);
                }
            }
            closedir(dirptr);
        }
        else {
            char dirPath[MAX_PATH];
            char Dirsfile[MAX_PATH];
            sprintf(Dirsfile,"%s\\%d", outputPath, dirDepth-1);
            switch (isOld) {
                case 1 :
                    strcat(Dirsfile, "\\oldDirs.txt");
                    break;
                case 0 :
                    strcat(Dirsfile, "\\newDirs.txt");
                    break;
            }
            FILE* dirsfileptr=fopen(Dirsfile, "r");
            while(fgets(dirPath, sizeof(dirPath), dirsfileptr)) {
                size_t len4 = strlen(repoPath);
                if (len4 > 0 && repoPath[len4 - 1] == '\n') {
                    repoPath[len4 - 1] = '\0';
                }
                dirPath[strcspn(dirPath, "\n")] = '\0';
                DIR* dirptr = opendir(dirPath);
                while ((entry = readdir(dirptr)) != NULL) {
                    if (entry->d_type == DT_REG) {
                        fprintf(outputfile, "%s\\%s\n", dirPath, entry->d_name);
                        fprintf(allFileptr, "%s\\%s ", dirPath, entry->d_name);
                        fprintf(allFileptr, "f ");
                        char filename[MAX_PATH];
                        strcpy(filename, dirPath);
                        strcat(filename, "\\");
                        strcat(filename, entry->d_name);                       
                        struct stat file_info;
                        stat(filename, &file_info);
                        time_t modification_time = file_info.st_mtime;
                        char modified_time_str[20];
                        strftime(modified_time_str, sizeof(modified_time_str), "%Y-%m-%d%H:%M:%S", localtime(&modification_time));
                        fprintf(allFileptr, "%s\n", modified_time_str);
                    }
                }
                closedir(dirptr);
            }
            fclose(dirsfileptr);           
        }
        fclose(outputfile);
        dirDepth++;
        if(maxDepth+1==dirDepth) {
            break; 
        }
    }   
    fclose(allFileptr);
}

