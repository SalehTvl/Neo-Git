#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>
#include <sys/stat.h>
#include "generalSubFunctions.h"

int userInfoLER() {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentPath);
    char repoPath[100];
    int repoexists=0;
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt", "r");
    while(fgets(repoPath, sizeof(repoPath), reposfile)!=NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result=strstr(currentPath, repoPath);
        if(result!=NULL) {
            repoexists=1;
        }
    }
    if(repoexists==0) {
        printf("you are not inside any of your ngit repositories");
        return 0;
    }
    fclose(reposfile);
    return 1;
}

int initLER() {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentPath);
    char repoPath[100];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt", "r");
    char repoPathcopy[MAX_PATH]; char currentPathCopy[MAX_PATH];
    while(fgets(repoPath, sizeof(repoPath), reposfile)!=NULL) {
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        strcpy(repoPathcopy, repoPath); strcpy(currentPathCopy, currentPath);
        char* result=strstr(currentPath, repoPath);
        int idx1=0;
        char* piece1=strtok(currentPathCopy, "\\");
        while(piece1!=NULL) {
            piece1=strtok(NULL,"\\");
            idx1++;
        }
        int idx2=0;
        char* piece2=strtok(repoPathcopy, "\\");
        while(piece2!=NULL) {
            piece2=strtok(NULL,"\\");
            idx2++;
        }
        if(result!=NULL && idx1!=idx2) {
            printf("repository initialization failed, this directory is a ngit repositiory or a subdirectory");
            return 0;
        }
    }
    fclose(reposfile);
    return 1;
}

int addLER(char* argv) {
    char currentPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentPath);
    char repoPath[MAX_PATH];
    FILE* reposfile=fopen("d:\\ANGP\\ngit-project\\repositories.txt","r");
    int flag=0;
    while(fgets(repoPath, sizeof(repoPath), reposfile)!= NULL) {
        flag=0;
        size_t len = strlen(repoPath);
        if (len > 0 && repoPath[len - 1] == '\n') {
            repoPath[len - 1] = '\0';
        }
        char* result = strstr(currentPath, repoPath);
        if(result!=NULL) {
            char currentPath1[MAX_PATH];
            char repoPath1[MAX_PATH];
            strcpy(currentPath1, currentPath);
            strcpy(repoPath1, repoPath);
            char piecesofFilepath1[10][20];
            char piecesofFilepath2[10][20];
            int idx1=0;
            char* piece1=strtok(currentPath1, "\\");
            while(piece1!=NULL) {
                strcpy(piecesofFilepath1[idx1], piece1);
                piece1=strtok(NULL,"\\");
                idx1++;
            }
            int idx2=0;
            char* piece2=strtok(repoPath1, "\\");
            while(piece2!=NULL) {
                strcpy(piecesofFilepath2[idx2], piece2);
                piece2=strtok(NULL,"\\");
                idx2++;
            }
            for(int i=0; i<idx2; i++) {
                if(strcmp(piecesofFilepath1[i], piecesofFilepath2[i])!=0) flag=1;
            } 
            if(flag==0) break;
        }
    }
    fclose(reposfile);
    if(flag==1) {
        printf("you are not inside any of your repositories\n");
        return 0;
    }
    char newAlladdress[MAX_PATH];
    char stagedFilesaddress[MAX_PATH];
    char oldAlladdress[MAX_PATH];
    strcpy(newAlladdress, repoPath);
    strcpy(oldAlladdress, repoPath);
    strcpy(stagedFilesaddress, repoPath);
    strcat(newAlladdress, "\\ngit\\info\\contents\\newAll.txt");
    strcat(oldAlladdress, "\\ngit\\info\\contents\\oldAll.txt");
    strcat(stagedFilesaddress, "\\ngit\\info\\stagedfiles.txt");
    strcat(repoPath, "\\ngit\\info\\stagedfiles.txt");
    char stagedfile[MAX_PATH];
    char currentPathcopy[MAX_PATH];
    strcpy(currentPathcopy, currentPath);
    strcat(currentPath, "\\");
    strcat(currentPath, argv);
    FILE* fileExisits=fopen(currentPath, "r");
    DIR* dirExsists=opendir(currentPath);
    FILE* stagedFilesptr=fopen(stagedFilesaddress, "r");
    char subPath0[MAX_PATH];
    char subType0[5];
    char subModified0[30];
    int deleteFlag=0;
    while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
        if(strcmp(subPath0, currentPath)==0) {
            deleteFlag=1;
        }
    }
    if(fileExisits==NULL && dirExsists==NULL && !deleteFlag) {
        printf("this file <%s> is not inside this directory or doesn't exist in your repository\n", argv);
        fclose(fileExisits); closedir(dirExsists);
        fclose(stagedFilesptr); fclose(fileExisits);
        return 0;
    }
    fclose(fileExisits);
    /*FILE* newAllptr=fopen(newAlladdress, "r");
    FILE* stagedFilesptr=fopen(stagedFilesaddress, "r");
    char subPath0[MAX_PATH];
    char subPath1[MAX_PATH];
    char subType0[5];
    char subType1[5];
    char subModified0[30];
    char subModified1[30];
    int countStagedFiles=0;
    int countUnmodifiedFiles=0;
    FILE* fileExists=fopen(currentPath, "r");
    FILE* oldAllptr=fopen(oldAlladdress, "r");
    if(fileExisits!=NULL) {
        while(fscanf(newAllptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
            if(strcmp(subPath0, currentPath)==0) {
                while(fscanf(stagedFilesptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
                    if(strcmp(subPath0, subPath1)==0) {
                        if(strcmp(subModified0, subModified1)==0) {
                            printf("this file <%s> contains no recent changes to add", argv);
                            fclose(fileExisits);
                            fclose(newAllptr);
                            fclose(stagedFilesptr);
                            fclose(oldAllptr);
                            return 0;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    else {
        while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
            if(strstr(subPath0, currentPath)!=NULL && strcmp(subType0,"f")==0) {
                while(fscanf(oldAllptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
                    if(strcmp(subPath1, subPath0)==0) {
                        countStagedFiles++;
                        if(strcmp(subModified0, subModified1)==0) {
                            countUnmodifiedFiles++;
                        }
                    }
                }
                if(countStagedFiles==countUnmodifiedFiles) {
                    printf("this file <%s> is not in staging area\n", argv[2]);
                    fclose(fileExisits);
                    fclose(newAllptr);
                    fclose(stagedFilesptr);
                    fclose(oldAllptr);
                    return 0;
                }
                break;
            }
        }
    }*/
    fclose(fileExisits); closedir(dirExsists);
    fclose(stagedFilesptr); fclose(fileExisits);
    //fclose(newAllptr);
    //fclose(oldAllptr);
    return 1;
    /*if(argc==3) {
        strcat(currentPath, "\\");
        strcat(currentPath, argv[2]);
        FILE* fileExisits=fopen(currentPath, "r");
        DIR* dirExsists=opendir(currentPath);
        if(fileExisits==NULL && dirExsists==NULL) {
            printf("this file is not inside this directory or doesn't exist in your repository");
            return 0;
        }
        while(fgets(stagedfile, sizeof(stagedfile), stagedfiles)!=NULL) {
            size_t len = strlen(repoPath);
            if (len > 0 && repoPath[len - 1] == '\n') {
                repoPath[len - 1] = '\0';
            }
            char* result=strstr(currentPath, stagedfile);
            if(result!=NULL) {
                printf("this file is already staged via a parent directory or itself");
                return 0;
            }
        }
        return 1;
    }*/
}

int resetLER(int argc, char* argv) {
    if(strcmp(argv, "-undo")==0) return 1;
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
    char oldAlladdress[MAX_PATH];
    char stagedFilesaddress[MAX_PATH];
    strcpy(oldAlladdress, repoPath);
    strcpy(stagedFilesaddress, repoPath);
    strcat(oldAlladdress, "\\ngit\\info\\contents\\oldAll.txt");
    strcat(stagedFilesaddress, "\\ngit\\info\\stagedfiles.txt");
    strcat(currentPath, "\\");
    strcat(currentPath, argv);
    FILE* fileExisits=fopen(currentPath, "r");
    DIR* dirExsists=opendir(currentPath);
    if(fileExisits==NULL && dirExsists==NULL) {
        printf("this file <%s> is not inside this directory or doesn't exist in your repository", argv);
        fclose(fileExisits);
        closedir(dirExsists);
        return 0;
    }
    
    char repoPathcopy[MAX_PATH];
    strcpy(repoPathcopy, repoPath);
    strcat(repoPath, "\\ngit\\info\\stagedfiles.txt");
    char stagedfile[MAX_PATH];
    flag=0;
    FILE* oldAllptr=fopen(oldAlladdress, "r");
    FILE* stagedFilesptr=fopen(stagedFilesaddress, "r");
    char subPath0[MAX_PATH];
    char subPath1[MAX_PATH];
    char subType0[5];
    char subType1[5];
    char subModified0[30];
    char subModified1[30];
    while(fscanf(stagedFilesptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
        if(strcmp(subPath1, currentPath)==0) {
            flag=1;
        }
    }
    if(flag==0) {
        printf("this file <%s> is not in staging area\n", argv);
        fclose(oldAllptr);
        fclose(stagedFilesptr);
        return 0;
    }
    int countStagedFiles=0;
    int countUnmodifiedFiles=0;
    if(fileExisits!=NULL) {
        while(fscanf(oldAllptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
            if(strcmp(subPath0, currentPath)==0) {
                while(fscanf(stagedFilesptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
                    if(strcmp(subPath0, subPath1)==0) {
                        if(strcmp(subModified0, subModified1)==0) {
                            printf("this file <%s> is not in staging area\n", argv);
                            fclose(oldAllptr);
                            fclose(stagedFilesptr);
                            fclose(fileExisits);
                            closedir(dirExsists);
                            return 0;
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    else {
        while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
            if(strstr(subPath0, currentPath)!=NULL && strcmp(subType0,"f")==0) {
                while(fscanf(oldAllptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
                    if(strcmp(subPath1, subPath0)==0) {
                        countStagedFiles++;
                        if(strcmp(subModified0, subModified1)==0) {
                            countUnmodifiedFiles++;
                        }
                        break;
                    }
                }  
            }
        }
        if(countStagedFiles==countUnmodifiedFiles) {
            printf("this directory <%s> is not containing any recent staged changes to discard\n", argv);
            fclose(oldAllptr);
            fclose(stagedFilesptr);
            fclose(fileExisits);
            closedir(dirExsists);
            return 0;
        }
    }
    fclose(oldAllptr);
    fclose(stagedFilesptr);
    fclose(fileExisits);
    closedir(dirExsists);
    return 1;
}

int commitLER() {
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

    char stagedFielsaddress[MAX_PATH];
    char oldAllFilesaddress[MAX_PATH];
    char newAllFilesaddress[MAX_PATH];
    char begstagedFilesaddress[MAX_PATH];
    strcpy(stagedFielsaddress, repoPath);
    strcpy(oldAllFilesaddress, repoPath);
    strcpy(newAllFilesaddress, repoPath);
    strcpy(begstagedFilesaddress, repoPath);
    strcat(stagedFielsaddress, "\\ngit\\info\\stagedfiles.txt");
    strcat(oldAllFilesaddress, "\\ngit\\info\\contents\\oldAll.txt");
    strcat(newAllFilesaddress, "\\ngit\\info\\contents\\newAll.txt");
    strcat(begstagedFilesaddress, "\\ngit\\info\\begstagedfiles.txt");
    int countStagedFiles=0;
    int countUnmodifiedFiles=0;
    char subPath0[MAX_PATH];
    char subPath1[MAX_PATH];
    char subType0[5];
    char subType1[5];
    char subModified0[30];
    char subModified1[30];
    int flagOfAdded=0;
    FILE* stagedFilesptr=fopen(stagedFielsaddress, "r");
    FILE* begstagedFIlesptr=fopen(begstagedFilesaddress, "r");
    while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
        while(fscanf(begstagedFIlesptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
            if(strcmp(subPath0, subPath1)==0) {
                if(strcmp(subModified0, subModified1)==0) {
                   flagOfAdded=1;
                    break; 
                } 
            }
        }
        if(flagOfAdded==0) {
            return 1;
        }
        flagOfAdded=0;
        rewind(begstagedFIlesptr);
    }
    fclose(begstagedFIlesptr);
    FILE* oldallFilesptr=fopen(oldAllFilesaddress, "r");
    char stagedfile[MAX_PATH];
    int dirisOld=0;
    int dirFound=0;
    rewind(stagedFilesptr);
    while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
        if(strcmp(subType0, "d")==0) {
            dirFound=1;
            countStagedFiles--;
        }
        countStagedFiles++;
        while(fscanf(oldallFilesptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
            if(strcmp(subPath0, subPath1)==0) {
                if(strcmp(subModified0, subModified1)==0 && dirFound==0) {
                    countUnmodifiedFiles++;
                }
                if(dirFound==1) {
                    dirisOld=1;
                }
            }
        }
        if(dirisOld==0 && dirFound==1) {
            fclose(stagedFilesptr);
            fclose(oldallFilesptr);
            return 1;
        }
        dirisOld=0;
        dirFound=0;
        rewind(oldallFilesptr);
    }
    if(countStagedFiles==countUnmodifiedFiles) {
        printf("there isn't any modified or new file in staging area to commit\n");
        fclose(stagedFilesptr);
        fclose(oldallFilesptr);
        return 0;
    }   
    fclose(stagedFilesptr);
    fclose(oldallFilesptr);
    /*FILE* newAllFilesptr=fopen(newAllFilesaddress, "r");
    while(fscanf(stagedFilesptr, "%s%s%s", subPath0, subType0, subModified0)==3) {
        if(strcmp(subType0, "d")==0) continue;
        while(fscanf(newAllFilesptr, "%s%s%s", subPath1, subType1, subModified1)==3) {
            if(strcmp(subPath0, subPath1)==0) {
                if(strcmp(subModified0, subModified1)!=0) {
                    printf("not allowed to commit, you have recently modified files <%s> not staged since recent change\n", subPath0);
                    return 0;
                }
            }
        }
    }*/
    return 1;
}

int commitSetLER(char* shortcutName) {
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
    char tempShortcut[100];
    char tempMessage[100];
    int flag=0;
    while(fscanf(commitsetMessptr, "%99s", tempShortcut)==1) {
        fgets(tempMessage, sizeof(tempMessage), commitsetMessptr);
        size_t len1 = strlen(tempMessage);
        if (len1 > 0 && tempMessage[len1 - 1] == '\n') {
            tempMessage[len1 - 1] = '\0';
        }
        if(strcmp(tempShortcut,shortcutName)==0) {
            flag=1;
            break;
        }
    }
    if(flag==1) {
        if(tempMessage[0]==' ') {
            memmove(tempMessage, tempMessage + 1, strlen(tempMessage));
            tempMessage[strlen(tempMessage)] = '\0';
        }
        printf("shortcut <%s> is already assigned to <%s> message\n", shortcutName, tempMessage);
        return 0;
    }
    return 1;
}

int logLER(int argc, char* argv[]) {
    if(argc==2) return 7;
    if(strcmp(argv[2],"-branch")==0) {
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
        char branchesFilPath[MAX_PATH];
        strcpy(branchesFilPath, repoPath);
        strcat(branchesFilPath, "\\ngit\\info\\branches.txt");
        FILE* branchesFileptr=fopen(branchesFilPath, "r");
        char branch[20];
        int flag=0;
        while(fscanf(branchesFileptr, "%s", branch)==1) {
            if(strcmp(branch, argv[3])==0) {
                flag=1;
                return 2;
            }
        }
        if(flag==0) {
            printf("branch <%s> does not exist", argv[3]);
            return 0;
        }
    }
    else if(strcmp(argv[2],"-n")==0) return 1;
    else if(strcmp(argv[2],"-author")==0) return 3;
    else if(strcmp(argv[2],"-since")==0) return 4;
    else if(strcmp(argv[2],"-before")==0) return 5;
    else if(strcmp(argv[2],"-search")==0) return 6;
}

int checkoutLER(char* branchName) {
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

    char stagedFilesPath[MAX_PATH]; sprintf(stagedFilesPath, "%s\\ngit\\info\\stagedfiles.txt", repoPath);
    char oldAllFilesPath[MAX_PATH]; sprintf(oldAllFilesPath, "%s\\ngit\\info\\contents\\oldAll.txt", repoPath);
    FILE* stagedFilesptr=fopen(stagedFilesPath, "r"); FILE* oldAllFilesptr=fopen(oldAllFilesPath, "r");
    char filePath0[MAX_PATH]; char fileType0[5]; char fileModified0[20];
    char filePath1[MAX_PATH]; char fileType1[5]; char fileModified1[20];
    while(fscanf(stagedFilesptr, "%s%s%s", filePath0, fileType0, fileModified0)==3) {
        if(strcmp(fileType0, "d")==0) continue;
        while(fscanf(oldAllFilesptr, "%s%s%s", filePath1, fileType1, fileModified1)==3) {
            if(strcmp(filePath0, filePath1)==0) {
                if(strcmp(fileModified0, fileModified1)!=0) {
                    printf("you can not checkout while you have changes not commited\n");
                    fclose(oldAllFilesptr); fclose(stagedFilesptr);
                    return 0;
                }
            }
        }
    }
    fclose(oldAllFilesptr); fclose(stagedFilesptr);
    return 1;
}