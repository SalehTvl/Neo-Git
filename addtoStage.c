#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

int static firstAdd=0;

int addtoStage(char argv[]) {
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
    char absolouteRepoPath[MAX_PATH]; strcpy(absolouteRepoPath, repoPath);
    if(firstAdd==0) {
        char lastStagedPath[MAX_PATH]; strcpy(lastStagedPath, repoPath); strcat(lastStagedPath, "\\ngit\\info\\lastStaged.txt");
        char lastStagedaddPath[MAX_PATH]; strcpy(lastStagedaddPath, repoPath); strcat(lastStagedaddPath, "\\ngit\\info\\lastStagedadd.txt");
        FILE* lastStagedptr=fopen(lastStagedPath, "r"); FILE* lastStagedaddptr=fopen(lastStagedaddPath, "w"); 
        char RecnetStagedFile[MAX_PATH]; sprintf(RecnetStagedFile, "%s\\%s", filePath, argv);
        fprintf(lastStagedaddptr, "%s\\%s\n", filePath, argv);
        char tempLastStaged[MAX_PATH];
        while(fscanf(lastStagedptr, "%s", tempLastStaged)==1) {
            if(strcmp(RecnetStagedFile, tempLastStaged)!=0)
                fprintf(lastStagedaddptr, "%s\n", tempLastStaged);
        }
        fclose(lastStagedptr);
        fclose(lastStagedaddptr);
        SetFileAttributes(lastStagedPath, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(lastStagedPath);                                  
        rename(lastStagedaddPath, lastStagedPath);
        firstAdd=1;
    }
    
    char repositoryPath[MAX_PATH];
    strcpy(repositoryPath, filePath);
    char repoPathcopy2[MAX_PATH];
    strcpy(repoPathcopy2, repoPath);
    strcat(repoPathcopy2, "\\ngit\\info\\contents\\newAll.txt");
    char stagedfileaddress[MAX_PATH];
    strcpy(stagedfileaddress, repoPath);
    strcat(stagedfileaddress, "\\ngit\\info\\stagedfiles.txt");
    char newstagedfileaddress[MAX_PATH];
    strcpy(newstagedfileaddress, repoPath);
    strcat(newstagedfileaddress, "\\ngit\\info\\newstagedfiles.txt");
    char repoPathcopy[MAX_PATH];
    strcpy(repoPathcopy, filePath);
    char filePathcopy[MAX_PATH];
    strcpy(filePathcopy, repoPath);
    strcat(filePath, "\\");
    strcat(filePath, argv);
    FILE* isfile=fopen(filePath, "r"); 
    DIR* isDir=opendir(filePath);       
    char *match = strstr(filePath, repoPath);
    memmove(match, match + strlen(repoPath), strlen(match + strlen(repoPath)) + 1);
    char piecesofFilepath[10][20]; 
    int idx=0;
    char* piece=strtok(filePath, "\\");
    while(piece!=NULL) {
        strcpy(piecesofFilepath[idx], piece);
        piece=strtok(NULL,"\\");
        idx++;
    }
    strcat(repoPath, "\\ngit\\stagingArea");
    char repoPathcopy3[MAX_PATH];
    strcpy(repoPathcopy3, filePathcopy);
    FILE* allNewptr=fopen(repoPathcopy2, "r");
    if(isfile==NULL && isDir!=NULL) {
        for(int i=0; i<idx; i++) {
            strcat(repoPath, "\\");               
            strcat(repoPath, piecesofFilepath[i]); 
            strcat(repoPathcopy3, "\\");               
            strcat(repoPathcopy3, piecesofFilepath[i]); 
                  
            DIR* dirptr=opendir(repoPath);
            if(dirptr==NULL) {
                CreateDirectory(repoPath, NULL); 
                firstAdd++;
            }
            char dirPath[MAX_PATH];
            char type[5];
            char dirModification[30]; char dirRecentModification[25];
            char newstagedfilesPath[MAX_PATH]; sprintf(newstagedfilesPath, "%s\\ngit\\info\\newstagedfiles.txt", filePathcopy);
            rewind(allNewptr); 
            FILE* newStagedFilesptr=fopen(newstagedfilesPath, "w"); FILE* stagedFilesptr=fopen(stagedfileaddress, "r");
            rewind(allNewptr);
            while(fscanf(allNewptr, "%s%s%s", dirPath, type, dirModification)==3) {     
                if(strcmp(dirPath, repoPathcopy3)==0) {
                    strcpy(dirRecentModification, dirModification);
                    break;
                }
            }
            int dirInStagedfile=0;
            while(fscanf(stagedFilesptr, "%s%s%s", dirPath, type, dirModification)==3) { 
                if(strcmp(dirPath, repoPathcopy3)==0) {
                    fprintf(newStagedFilesptr,"%s d %s\n", repoPathcopy3, dirRecentModification);
                    dirInStagedfile=1;
                }
                else
                    fprintf(newStagedFilesptr,"%s %s %s\n", dirPath, type, dirModification);
            }   
            if(dirInStagedfile==0) {
                fprintf(newStagedFilesptr,"%s d %s\n", repoPathcopy3, dirRecentModification);
            }
            fclose(newStagedFilesptr); fclose(stagedFilesptr); rewind(allNewptr); 
            SetFileAttributes(stagedfileaddress, FILE_ATTRIBUTE_NORMAL); remove(stagedfileaddress); rename(newstagedfilesPath, stagedfileaddress);       
        }
        rewind(allNewptr);
        strcat(repoPathcopy3, "\\");
        char subPath[MAX_PATH];
        char subType[5];
        char subModified[30];
        int round=0;
        while(fscanf(allNewptr,"%s%s%s", subPath, subType, subModified)==3) {
            if(strstr(subPath, repoPathcopy3)!=NULL) {
                char *match = strstr(subPath, repositoryPath);
                memmove(match, match + strlen(repositoryPath), strlen(match + strlen(repositoryPath)) + 1);
                addtoStage(subPath);          
            }
        } 
        return firstAdd; 
    }
    else {
        for(int i=0; i<idx-1; i++) {
            strcat(repoPath, "\\");               
            strcat(repoPath, piecesofFilepath[i]);
            strcat(repoPathcopy3, "\\");               
            strcat(repoPathcopy3, piecesofFilepath[i]);
            DIR* dirptr=opendir(repoPath);
            if(dirptr==NULL) {
                CreateDirectory(repoPath, NULL);
            }
            /*char dirPath[MAX_PATH];
            char type[5];
            char dirModification[30]; char dirRecentModification[25];
            char newstagedfilesPath[MAX_PATH]; sprintf(newstagedfilesPath, "%s\\ngit\\info\\newstagedfiles.txt", filePathcopy);
            FILE* newStagedFilesptr=fopen(newstagedfilesPath, "w"); FILE* stagedFilesptr=fopen(stagedfileaddress, "r");
            rewind(allNewptr); fclose(stagedfilesread); fclose(stagedfilesptr);
            while(fscanf(allNewptr, "%s%s%s", dirPath, type, dirModification)==3) {
                if(strcmp(dirPath, repoPathcopy3)==0) {
                    strcpy(dirRecentModification, dirModification);
                    break;
                }
            }
            while(fscanf(stagedFilesptr, "%s%s%s", dirPath, type, dirModification)==3) {
                if(strcmp(dirPath, repoPathcopy3)==0) {
                    fprintf(newStagedFilesptr,"%s d %s\n", repoPathcopy3, dirRecentModification);
                }
                else
                    fprintf(newStagedFilesptr,"%s %s %s\n", dirPath, type, dirModification);
            }    
            fclose(newStagedFilesptr); fclose(stagedFilesptr); rewind(allNewptr); 
            SetFileAttributes(stagedfileaddress, FILE_ATTRIBUTE_NORMAL); remove(stagedfileaddress); rename(newstagedfilesPath, stagedfileaddress);    */   
        }
        FILE* stagedfilesptr=fopen(stagedfileaddress, "a");
        FILE* stagedfilesread=fopen(stagedfileaddress, "r");
        strcat(repoPath, "\\");
        strcat(repoPath, piecesofFilepath[idx-1]);
        strcat(repoPathcopy3, "\\");               
        strcat(repoPathcopy3, piecesofFilepath[idx-1]);
        char subPath0[MAX_PATH];
        char subType0[5];
        char subModified0[30];
        char subPath1[MAX_PATH];
        char subType1[5];
        char subModified1[30];
        int flag1=0;
        int flag2=0;
        rewind(allNewptr);
        rewind(stagedfilesread);
        while(fscanf(allNewptr,"%s %s %s", subPath0, subType0, subModified0)==3) {
            if(strcmp(subPath0, repoPathcopy3)==0) {
                while(fscanf(stagedfilesread,"%s %s %s", subPath1, subType1, subModified1)==3) {
                    if(strcmp(subPath1, repoPathcopy3)==0) {
                        flag1=1;
                        if(strcmp(subModified0, subModified1)==0) {
                            fclose(stagedfilesread);
                            fclose(stagedfilesptr);
                            fclose(allNewptr);
                            return 1;
                        } 
                        break;
                    }
                }
                flag2=1;
                break;
            }
        }
        rewind(stagedfilesread);
        if(flag2==0) {
            while(fscanf(stagedfilesread,"%s %s %s", subPath1, subType1, subModified1)==3) {
                if(strcmp(subPath1, repoPathcopy3)==0) {
                    flag1=1;
                    break;
                }
            }
        }
        firstAdd++;
        fclose(allNewptr);
        if(flag2==1 || flag1==0) {
            strcat(repoPathcopy, "\\copyfile.bat");
            FILE *batchFile=fopen(repoPathcopy, "w");
            fprintf(batchFile, "@echo off\n");
            fprintf(batchFile, "set \"sourceFile=%s\"\n", repoPathcopy3);
            fprintf(batchFile, "set \"destinationFile=%s\"\n", repoPath);
            fprintf(batchFile, ")\n");
            fprintf(batchFile, "type \"%%sourceFile%%\" > \"%%destinationFile%%\"\n");
            fprintf(batchFile, "exit /b 0\n");
            fclose(batchFile);
            system("copyfile.bat");
            remove(repoPathcopy);
        }
        if(flag1==1 && flag2==1) {
            FILE* newStagedFilesptr=fopen(newstagedfileaddress, "w");
            rewind(stagedfilesread);
            while(fscanf(stagedfilesread,"%s %s %s", subPath1, subType1, subModified1)==3) {
                if(strcmp(subPath1, repoPathcopy3)==0) {
                    fprintf(newStagedFilesptr,"%s f %s\n", repoPathcopy3, subModified0);
                }
                else {
                    fprintf(newStagedFilesptr,"%s %s %s\n", subPath1, subType1, subModified1);
                }
            }
            fclose(stagedfilesread);
            fclose(newStagedFilesptr);
            fclose(stagedfilesptr);
            SetFileAttributes(stagedfileaddress, FILE_ATTRIBUTE_NORMAL);
            DeleteFile(stagedfileaddress);                                  
            rename(newstagedfileaddress, stagedfileaddress);
        }
        else if(flag2==1 && flag1==0){
            stagedfilesptr=fopen(stagedfileaddress, "a");
            fprintf(stagedfilesptr,"%s f %s\n", repoPathcopy3, subModified0);
            fclose(stagedfilesptr);
        }
        else if(flag2==0 && flag1==1) {
            FILE* newStagedFilesptr=fopen(newstagedfileaddress, "w");
            rewind(stagedfilesread);
            while(fscanf(stagedfilesread,"%s%s%s", subPath1, subType1, subModified1)==3) {
                if(strcmp(subPath1, repoPathcopy3)==0) {
                    SetFileAttributes(repoPath, FILE_ATTRIBUTE_NORMAL);
                    DeleteFile(repoPath);
                    char removedFilesPath[MAX_PATH]; sprintf(removedFilesPath, "%s\\ngit\\info\\removedFiles.txt", absolouteRepoPath);
                    FILE* removedFilesptr=fopen(removedFilesPath, "a"); fprintf(removedFilesptr, "%s %s %s\n", subPath1, subType1, subModified1);
                    fclose(removedFilesptr);
                }
                else {
                    fprintf(newStagedFilesptr,"%s %s %s\n", subPath1, subType1, subModified1);
                }
            }
            rewind(stagedfilesread);
            rewind(newStagedFilesptr);
            fclose(stagedfilesread);
            fclose(newStagedFilesptr);
            fclose(stagedfilesptr);
            SetFileAttributes(stagedfileaddress, FILE_ATTRIBUTE_NORMAL);
            DeleteFile(stagedfileaddress);                                  
            rename(newstagedfileaddress, stagedfileaddress);
        }
        fclose(stagedfilesread);
        fclose(stagedfilesptr);
        fclose(allNewptr);
    }   
    fclose(isfile);
    return firstAdd;
}

void stageDepth(int depth) {
    char currentpath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentpath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt", "r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result = strstr(currentpath, repoPath);
        if(result!=NULL) break;
    }
    fclose(reposfile);
    char stagedfilesaddress[MAX_PATH];
    strcpy(stagedfilesaddress, repoPath);
    strcat(stagedfilesaddress, "\\ngit\\info\\stagedfiles.txt");
    char maxDepthaddress[MAX_PATH];
    strcpy(maxDepthaddress, repoPath);
    strcat(maxDepthaddress, "\\ngit\\info\\maxDepth.txt");
    FILE* maxDepthfile=fopen(maxDepthaddress, "r");
    int maxDepth=0;
    fscanf(maxDepthfile, "%d", &maxDepth);
    fclose(maxDepthfile);
    if(depth>=maxDepth) depth=maxDepth;
    char contentsfileaddrress[MAX_PATH];
    strcpy(contentsfileaddrress, repoPath);
    strcat(contentsfileaddrress, "\\ngit\\info\\contents");
    char oldAlladdress[MAX_PATH];
    strcpy(oldAlladdress, repoPath);
    strcat(oldAlladdress, "\\ngit\\info\\contents\\oldAll.txt");
    char contentsfileaddrresscopy[MAX_PATH];
    strcpy(contentsfileaddrresscopy, contentsfileaddrress);    
    for(int i=0; i<=depth; i++) {
        sprintf(contentsfileaddrress, "%s\\%d", contentsfileaddrresscopy, i);
        char newDirsaddress[MAX_PATH];
        sprintf(newDirsaddress, "%s\\newDirs.txt", contentsfileaddrress);
        char oldDirsaddress[MAX_PATH];
        sprintf(oldDirsaddress, "%s\\oldDirs.txt", contentsfileaddrress);
        char existingdirectory[MAX_PATH];
        char stagedfile[MAX_PATH];
        int flag1=0;
        int flag2=0;
        int flag3=0;
        char directoryPath[MAX_PATH];
        char dirType[5];
        char dirModification[30];
        char olddirectoryPath[MAX_PATH];
        char olddirType[5];
        char olddirModification[30];
        char newAlladdress[MAX_PATH]; 
        char begStagedFilesPath[MAX_PATH];
        char newFilesPath[MAX_PATH];
        char oldFilesPath[MAX_PATH];
        sprintf(newFilesPath, "%s\\newfiles.txt", contentsfileaddrress);
        sprintf(oldFilesPath, "%s\\oldfiles.txt", contentsfileaddrress);
        sprintf(begStagedFilesPath, "%s\\ngit\\info\\begstagedfiles.txt", repoPath);
        strcpy(newAlladdress, repoPath);
        strcat(newAlladdress, "\\ngit\\info\\contents\\newAll.txt");
        char subPath0[MAX_PATH];
        char subPath1[MAX_PATH];
        char subPath2[MAX_PATH];
        char subPath3[MAX_PATH];
        char subType0[5];
        char subType1[5];
        char subType2[5];
        char subType3[5];
        char subModified0[30];
        char subModified1[30];
        char subModified2[30];
        char subModified3[30];
        int exists_innew=0;
        int exists_inold=0;
        int modif_state=0;
        int exists_instage=0;
        int exists_inBegstage=0;
        char oldDirPath[MAX_PATH]; 
        char newDirPath[MAX_PATH];
        FILE* newDirsptr=fopen(newDirsaddress, "r");
        FILE* oldDirsptr=fopen(oldDirsaddress, "r");
        FILE* stagedFilesptr=fopen(stagedfilesaddress, "r");
        FILE* begStagedFilesptr=fopen(begStagedFilesPath, "r");
        FILE* oldAllptr=fopen(oldAlladdress, "r");
        FILE* newAllptr=fopen(newAlladdress, "r");
        while(fscanf(newDirsptr, "%s", newDirPath)==1) {
            rewind(stagedFilesptr); rewind(begStagedFilesptr); rewind(newAllptr); rewind(oldAllptr);
            exists_inold=0; exists_instage=0; exists_inBegstage=0;
            while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
                if(strcmp(newDirPath, subPath0)==0) {
                    exists_instage=1;
                    break;
                }
            }
            if(exists_instage==0) {
                printf("%s -\n", newDirPath);
                continue;
            }
            while(fscanf(newAllptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
                if(strcmp(newDirPath, subPath1)==0) {
                    break;
                }      
            }    
            while(fscanf(oldAllptr, "%s%s%s", subPath2, subType2, subModified2)==3) {
                if(strcmp(newDirPath, subPath2)==0) {
                    exists_inold=1;
                    break;
                }
            }  
            if(exists_inold==0) {
                printf("%s +\n", newDirPath);
                continue;
            }
            if(strcmp(subModified0, subModified1)==0) {
                if(strcmp(subModified0, subModified2)!=0) {
                    printf("%s +\n", newDirPath);
                    continue;
                }
                else {
                    while(fscanf(begStagedFilesptr, "%s%s%s", subPath3, subType3, subModified3)==3) {
                        if(strcmp(newDirPath, subPath3)==0) {
                            printf("%s -\n", newDirPath);
                            exists_inBegstage=1;
                            break;
                        }
                    }
                    if(exists_inBegstage==0) {
                        printf("%s +\n", newDirPath);
                    }
                    continue;
                }
            }
            else {
                if(strcmp(subModified0, subModified2)!=0) printf("%s +", newDirPath);
                else {
                    while(fscanf(begStagedFilesptr, "%s%s%s", subPath3, subType3, subModified3)==3) {
                        if(strcmp(newDirPath, subPath3)==0) {
                            printf("%s -\n", newDirPath);
                            exists_inBegstage=1;
                            break;
                        }
                    }
                    if(exists_inBegstage==0) {
                        printf("%s +\n", newDirPath);
                    }
                    continue;
                }
            }
        }

        while(fscanf(oldDirsptr, "%s", oldDirPath)==1) {
            rewind(stagedFilesptr); rewind(begStagedFilesptr); rewind(newAllptr); rewind(oldAllptr); rewind(newDirsptr);
            exists_inold=0; exists_instage=0; exists_inBegstage=0; exists_innew=0;
            while(fscanf(newDirsptr, "%s", newDirPath)==1) {
                if(strcmp(oldDirPath, newDirPath)==0) {
                    exists_innew=1;
                    break;
                }
            }
            if(exists_innew==0) {
                while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
                    if(strcmp(oldDirPath, subPath0)==0) {
                        exists_instage=1;
                        break;
                    }
                    if(exists_instage==0) {
                        printf("%s -\n", oldDirPath);
                        break;
                    }
                    else {
                        while(fscanf(begStagedFilesptr, "%s%s%s", subPath3, subType3, subModified3)==3) {
                            if(strcmp(oldDirPath, subPath3)==0) {
                                if(strcmp(subModified0, subModified3)==0)
                                    printf("%s -\n", oldDirPath);
                                else 
                                    printf("%s +\n", oldDirPath);
                                exists_inBegstage=1;
                                break;
                            }
                        }
                        if(exists_inBegstage==0) {
                            printf("%s +\n", oldDirPath);
                        }
                        break;
                    }
                }
            }
        }
        fclose(newDirsptr); fclose(oldDirsptr);

        char newFilePath[MAX_PATH]; char oldFilePath[MAX_PATH];
        FILE* newFilesptr=fopen(newFilesPath, "r"); FILE* oldFilesptr=fopen(oldFilesPath, "r");
        while(fscanf(newFilesptr, "%s", newFilePath)==1) {
            rewind(stagedFilesptr); rewind(begStagedFilesptr); rewind(newAllptr); rewind(oldAllptr);
            exists_inold=0; exists_instage=0; exists_inBegstage=0;
            while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
                if(strcmp(newFilePath, subPath0)==0) {
                    exists_instage=1;
                    break;
                }
            }
            if(exists_instage==0) {
                printf("%s -\n", newFilePath);
                continue;
            }
            while(fscanf(newAllptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
                if(strcmp(newFilePath, subPath1)==0) {
                    break;
                }      
            }    
            while(fscanf(oldAllptr, "%s%s%s", subPath2, subType2, subModified2)==3) {
                if(strcmp(newFilePath, subPath2)==0) {
                    exists_inold=1;
                    break;
                }
            }  
            if(exists_inold==0) {
                printf("%s +\n", newFilePath);
                continue;
            }
            if(strcmp(subModified0, subModified1)==0) {
                if(strcmp(subModified0, subModified2)!=0) {
                    printf("%s +\n", newFilePath);
                    continue;
                }
                else {
                    while(fscanf(begStagedFilesptr, "%s%s%s", subPath3, subType3, subModified3)==3) {
                        if(strcmp(newFilePath, subPath3)==0) {
                            printf("%s -\n", newFilePath);
                            exists_inBegstage=1;
                            break;
                        }
                    }
                    if(exists_inBegstage==0) {
                        printf("%s +\n", newFilePath);
                    }
                    continue;
                }
            }
            else {
                if(strcmp(subModified0, subModified2)!=0) printf("17%s +", newFilePath);
                else {
                    while(fscanf(begStagedFilesptr, "%s%s%s", subPath3, subType3, subModified3)==3) {
                        if(strcmp(newFilePath, subPath3)==0) {
                            printf("%s -\n", newFilePath);
                            exists_inBegstage=1;
                            break;
                        }
                    }
                    if(exists_inBegstage==0) {
                        printf("%s +\n", newFilePath);
                    }
                    continue;
                }
            }
        }
        
        while(fscanf(oldFilesptr, "%s", oldFilePath)==1) {
            rewind(stagedFilesptr); rewind(begStagedFilesptr); rewind(newAllptr); rewind(oldAllptr); rewind(newFilesptr);
            exists_inold=0; exists_instage=0; exists_inBegstage=0; exists_innew=0;
            while(fscanf(newFilesptr, "%s", newFilePath)==1) {
                if(strcmp(oldFilePath, newFilePath)==0) {
                    exists_innew=1;
                    break;
                }
            }
            if(exists_innew==0) {
                while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
                    if(strcmp(oldFilePath, subPath0)==0) {
                        exists_instage=1;
                        break;
                    }
                    if(exists_instage==0) {
                        printf("%s -\n", oldFilePath);
                        break;
                    }
                    else {
                        while(fscanf(begStagedFilesptr, "%s%s%s", subPath3, subType3, subModified3)==3) {
                            if(strcmp(oldFilePath, subPath3)==0) {
                                if(strcmp(subModified0, subModified3)==0)
                                    printf("%s -\n", oldFilePath);
                                else 
                                    printf("%s +\n", oldFilePath);
                                exists_inBegstage=1;
                                break;
                            }
                        }
                        if(exists_inBegstage==0) {
                            printf("%s +\n", oldFilePath);
                        }
                        break;
                    }
                }
            }
        }
        fclose(newFilesptr); fclose(oldFilesptr); fclose(newAllptr); fclose(oldAllptr); fclose(stagedFilesptr); fclose(begStagedFilesptr);
    }
}