#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "syntaxErrorRecognizer.h"
#include "ngitHiddenDir.h"
#include "logicErrorRecognizer.h"
#include "userinfoEditor.h"
#include "listofcontents.h"
#include "addtoStage.h"
#include "resetStage.h"
#include "directoryStatus.h"
#include "commitCreator.h"
#include "totalStatus.h"
#include "commitMesSet.h"
#include "logFunctions.h"
#include "createBranch.h"
#include "checkOut.h"
#include "revertCommit.h"
#include "createTag.h"
#include "grepWord.h"
#include"diffCheck.h"
#include "conflictThreat.h"
#include "mergeCommit.h"
#include "stash.h"

int detachedHead=1;
int declaredUser=0;
int repositoryFound=0;

typedef struct {
    char username[50];
    char useremail[100];
    char currentbranch[50];
    char currentRepo[MAX_PATH];
}startupinfo;

void startUp(startupinfo* baseIntel) {
    FILE* userInfoptr = fopen("d:\\ANGP\\ngit-project\\userInfo.txt", "r");
    fscanf(userInfoptr, "%s%s", baseIntel->username, baseIntel->useremail);
    fclose(userInfoptr);
    if(strcmp(baseIntel->username, "0")==0 || strcmp(baseIntel->useremail, "0")==0) declaredUser=0;
    else declaredUser=1;
    FILE* branchfile=fopen("d:\\ANGP\\ngit-project\\currentbranch.txt","r");
    fscanf(branchfile, "%s", baseIntel->currentbranch);
    fclose(branchfile);
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
        if(result!=NULL)  {
            repositoryFound=1;
            break;
        }
    }
    fclose(reposfile);

    char HeadHash[9]="\0"; char currenntHash[9]="\0";
    if(repositoryFound==1) {
        strcpy(baseIntel->currentRepo,repoPath);
        char allCommitsPath[MAX_PATH]; sprintf(allCommitsPath, "%s\\ngit\\info\\allCommits.txt", repoPath);
        FILE* allCommitsptr=fopen(allCommitsPath, "r");
        char commitData[7][40]; char line[100]; 
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
            if(strcmp(baseIntel->currentbranch, commitData[1])==0) {
                strcpy(HeadHash, commitData[0]);
                break;
            }
        }
        fclose(allCommitsptr);
        char curCommithashPath[MAX_PATH]; sprintf(curCommithashPath, "%s\\ngit\\info\\curCommitHash.txt", repoPath);
        FILE* curCommithashptr=fopen(curCommithashPath, "r"); fscanf(curCommithashptr, "%s", currenntHash); fclose(curCommithashptr);
    }
    if(strcmp(currenntHash, HeadHash)==0) detachedHead=0;
    else if(strcmp(currenntHash, "")==0 || strcmp(HeadHash, "")==0) detachedHead=0;
}

int main(int argc, char *argv[]) {
    startupinfo baseIntel;
    startUp(&baseIntel);
    
    if(strcmp(argv[1], "config")==0 ) {
        int returnValue=0;
        returnValue = userInfoSER(argc, argv);
        switch(returnValue) {
            case 0:
                printf("Invalid command due to misspell or extra words\n");  
                break;
            case 1:
                userGlobalInfoE(argc, argv, 0);
                startUp(&baseIntel);
                printf("your global username was set successfully\n");
                break;
            case 2:
                userGlobalInfoE(argc, argv, 1);
                printf("your global email was set successfully\n");
                break;
            case 3:
                if(repositoryFound==0) {
                    printf("your are not inside any repository to set local userinfo\n"); return 0;
                }
                userLocalInfoE(argc, argv, 0);
                startUp(&baseIntel);
                printf("your local username was set successfully\n");
                break;
            case 4:
                if(repositoryFound==0) {
                    printf("your are not inside any repository to set local userinfo\n"); return 0;
                }
                userLocalInfoE(argc, argv, 1);
                printf("your local email was set successfully\n");
                break;
        }
        return 0;
        char alias_spell[6]="alais."; 
        for(int i=0; i<6; i++) {
            if(argv[3][i]!=alias_spell[i]) {
                break;
            }
            if(i==5)
                if(aliasSER(argc, argv)==0) return 0;
        }    
    }
    if(declaredUser==0) {
        printf("please edit your global personal info in order to furthur use\n");
        return 0;
    }
    else if(strcmp(argv[1], "init")==0 ) {
        if(initSER(argc, argv)==0) return 0;
        if(initLER()==0) return 0;
        makeHiddenNgitDir();
        startUp(&baseIntel);
        listDirectories(1);
        listDirectories(0);
        listFiles(1);
        listFiles(0);
        printf("initialized ngit repository at <%s>\n", baseIntel.currentRepo);
    }
    else if(strcmp(argv[1], "add")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to add files\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not add any files to staging area while on detached head\n");
            return 0;
        }
        int returnValue=0; int SERreturnValue=0;
        char *endptr; long result;
        SERreturnValue=addSER(argc, argv);
        listDirectories(0);
        listFiles(0);
        switch (SERreturnValue) {
            case 0:
                break;
            case 1:
                if(addLER(argv[2])==0) return 0;
                returnValue=addtoStage(argv[2]); 
                break;
            case 2:
                printf("sorry, redo is not supported\n");
                break;
            case 3:
                for(int i=3; i<argc; i++) {
                if(addLER(argv[i])==0) continue;
                returnValue=addtoStage(argv[i]);
                }
                break;
            case 4:
                result = strtol(argv[3], &endptr, 10);
                stageDepth(result);
                break;
            case 5:
                for(int i=2; i<argc; i++) {
                if(addLER(argv[i])==0) continue;
                returnValue=addtoStage(argv[i]);
                }
                break;
        }
        if(returnValue==1) {
            printf("there were no changes to stage\n");
        }
        else if(returnValue>1){
             printf("changes successfully added\n");
        }
        return 0;
    }
    else if(strcmp(argv[1], "reset")==0 ) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to add files\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not add any files to staging area while on detached head\n");
            return 0;
        }
        int returnValue=0;
        returnValue=resetSER(argc, argv);
        listDirectories(0);
        listFiles(0);
        switch (returnValue) {
            case 0:
                printf("Invalid command due to misspell or extra words\n");
                return 0;
            case 1:
                if(resetLER(argc, argv[2])==0) return 0;
                resetStage(argv[2]);
                break;
            case 2:
                for(int i=3; i<argc; i++) {
                    if(resetLER(argc, argv[i])==0) continue;
                    resetStage(argv[i]);
                }
                break;
            case 3:
                for(int i=2; i<argc; i++) {
                    if(resetLER(argc, argv[i])==0) continue;
                    resetStage(argv[i]);
                }
                break;
        }
        printf("changes were successfully discarded from staging area\n");
        return 0;
    }
    else if(strcmp(argv[1], "status")==0 ) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to check status\n");
            return 0;
        }
        if(statusSER(argc, argv)==0) return 0;
        listDirectories(0);
        listFiles(0);
        totalStatus(); return 0;
    }
    else if(strcmp(argv[1], "commit")==0 ) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to commit\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not commit while on detached head state\n");
            return 0;
        }
        if(commitSER(argc, argv)==0) return 0;
        if(commitLER()==0) return 0;
        listDirectories(0);
        listFiles(0);
        if(commitSER(argc, argv)==2) commitCreator(1, argv[3]);
        if(commitSER(argc, argv)==1) commitCreator(0, argv[3]);
        listDirectories(1);
        listFiles(1);
        listDirectories(0);
        listFiles(0);
        startUp(&baseIntel);
        return 0;
    }
    else if(strcmp(argv[1], "set")==0 ) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to set shortcuts\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not do commit related actions while on detached head state\n");
            return 0;
        }
        if(setSER(argc, argv)==0) return 0;
        if(commitSetLER(argv[5])==0) return 0;
        commitMesSet(argv[3], argv[5]); return 0;
    }
    else if(strcmp(argv[1], "replace")==0 ) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to replace shortcuts\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not do commit related actions while on detached head state\n");
            return 0;
        }
        if(replaceSER(argc, argv)==0) return 0;
        commitMesReplace(argv[3], argv[5]); return 0;
    }
    else if(strcmp(argv[1], "remove")==0 ) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to remove shortcuts\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not do commit related actions while on detached head state\n");
            return 0;
        }
        if(removeSER(argc, argv)==0) return 0;
        commitMesRemove(argv[3]); return 0;
    }
    else if(strcmp(argv[1], "log")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to review commit history\n");
            return 0;
        }
        if(logSER(argc, argv)==0) return 0;
        int returnValue=logLER(argc, argv);
        if(returnValue==0) return 0;
        if(returnValue==7) {
            normalLog();
            return 0;
        }
        if(returnValue==1) {
            numberLog(argv[3]);
            return 0;
        }
        if(returnValue==2) {
            branchLog(argv[3]);
            return 0;
        }
        if(returnValue==3) {
            authorLog(argv[3]);
            return 0;
        }
        if(returnValue==4 || returnValue==5) {
            dateLog(argv[3], returnValue-4);
            return 0;
        }
        if(returnValue==6) {
            wordLog(argv[3]);
            return 0;
        } 
    }
    else if(strcmp(argv[1], "branch")==0 ) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to create a new branch\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not create a new commit while on detached head state\n");
            return 0;
        }
        if(branchSER(argc, argv)==0) return 0;
        listFiles(0);
        listDirectories(0);
        if(argc==3) {
            createBranch(argv[2]);
            listFiles(1);
            listDirectories(1);
        }
        if(argc==2) listBranches();
        return 0;
    }
    else if(strcmp(argv[1], "checkout")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to checkout\n");
            return 0;
        }
        int returnedVale=checkoutSER(argc, argv);
        if(returnedVale==0) return 0;
        if(checkoutLER(argv[2])==0) return 0;
        listFiles(0);
        listDirectories(0);
        if(returnedVale==1) checkoutBranch(argv[2], 0);
        else if(returnedVale==2) checkoutHash(argv[2],0); 
        else if(returnedVale==3) checkoutHash(argv[2],0); 
        else if(returnedVale==4) {
            int digits=strlen(argv[2])-5;
            char number[digits];
            for(int i=0; i<digits; i++) {
                number[i]=argv[2][i+5];
            }
            checkoutHash(number,1); 
        }
        startUp(&baseIntel);
        if(detachedHead==0) {
            listFiles(1);
            listDirectories(1);
            checkoutHead();
        }
        //printf("checked out successfully\n");
        return 0;
    }
    else if(strcmp(argv[1], "revert")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to revert\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not create a new reverted commit while on detached head state\n");
            return 0;
        }
        int returnValue=revertSER(argc, argv);
        switch (returnValue) {
            case 0:
                printf("Invalid command due to misspell or extra words!");
                return 0;
            case 1:
                revertCommit(argv[2], "nuull", returnValue);
                break;
            case 2:
                checkoutHash(argv[3],0);
                commitCreator(0, argv[2]);
                break;
            case 3:
                revertCommit(argv[2], "nuull", returnValue);
                break;
            case 4:
                revertCommit(argv[3], "null", returnValue);
                break;
            case 5:
                revertCommit(argv[3], argv[2], returnValue);
                break;
        }
        listFiles(0);
        listFiles(1);
        listDirectories(0);
        listDirectories(1);
        return 0;
    }
    else if(strcmp(argv[1], "tag")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to tag commits\n");
            return 0;
        }
        int returnValue=tagSER(argc, argv);
        switch (returnValue) {
            case 0:
                printf("invalid command due to misspell or extra words\n");
                break;
            case 1:
                createTag(argv[3], "no message", "null", 1);
                break;
            case 2:
                createTag(argv[3], argv[5], "null", 2);
                break;
            case 3:
                createTag(argv[3], "no message", argv[5], 3);
                break;
            case 4:
                createTag(argv[3], argv[5], argv[7], 4);
                break;
            case 5:
                createTag(argv[3], argv[5], "null", 5);
                break;
            case 6:
                createTag(argv[3], "no message", argv[5], 6);
                break;
            case 7:
                createTag(argv[3], argv[5], argv[7], 7);
                break;
            case 8:
                tagsListShow();
                break;
            case 9:
                tagIDShow(argv[3]);
                break;
        }
        return 0;
    }
    else if(strcmp(argv[1], "stash")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to create stash\n");
            return 0;
        }
        int returnValue=stashSER(argc, argv);
        switch (returnValue) {
            case 0:
                printf("invalid command due to misspell or extra words\n");
                break;
            case 1:
                stashCreator("no message");
                break;
            case 2:
                stashCreator(argv[4]);
                break;
            case 3:
                stashList();
                break;
            case 4:
                stashPop("empty", 1);
                break;
            case 5:
                stashPop(argv[3], 0);
                break;
            case 6:
                stashClear();
                break;
            case 7:
                stashShow(argv[3]);
                break;
        }
        return 0;
    }
    else if(strcmp(argv[1], "grep")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to grep words\n");
            return 0;
        }
        int returnedVale=grepSER(argc, argv);
        switch (returnedVale) {
            case 0:
                printf("invalid command due to misspell or extra words\n");
                break;
            case 1:
                grepWord(argv[3], argv[5], "null", 1);
                break;
            case 2:
                grepWord(argv[3], argv[5], argv[7], 2);
                break;
            case 3:
                grepWord(argv[3], argv[5], "null", 3);
                break;
            case 4:
                grepWord(argv[3], argv[5], argv[7], 4);
                break;
        }
        return 0;
    }
    else if(strcmp(argv[1], "diff")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to diff files\n");
            return 0;
        }
        int returnValue=diffSER(argc, argv);
        if(returnValue==0) {
            printf("Invalid command due to misspell or extra words\n");
        }
        else if(returnValue==1) {
            conflictThreat(argv[3], argv[4], 0);
        } 
        else if(returnValue==2) {
            differenceCheck(argv[3], argv[4],1,1000000,1,1000000, 1);
        } 
        else if(returnValue==3) {
            int line1Beg1=atoi(argv[6]); int line1End1=atoi(argv[7]);
            differenceCheck(argv[3], argv[4],line1Beg1,line1End1,1,1000000, 1);
        } 
        else if(returnValue==4) {
            int line2Beg1=atoi(argv[6]); int line2End1=atoi(argv[7]);
            differenceCheck(argv[3], argv[4],1,1000000, line2Beg1,line2End1, 1);
        } 
        else if(returnValue==5) {
            int line1Beg2=atoi(argv[6]); int line1End2=atoi(argv[7]);
            int line2Beg2=atoi(argv[9]); int line2End2=atoi(argv[10]);
            differenceCheck(argv[3], argv[4],line1Beg2,line1End2,line2Beg2,line2End2, 1);
        }
        return 0;
    }
    else if(strcmp(argv[1], "merge")==0 && strcmp(argv[2], "-b")==0) {
        if(repositoryFound==0) {
            printf("you are not inside any of your repostories to merge branches\n");
            return 0;
        }
        if(detachedHead==1) {
            printf("you can not merge two branches while on detached head state\n");
            return 0;
        }
        mergeCommit(argv[3], argv[4]);
        return 0;
    }
    else {
        printf("Invalid command due to misspell or extra words\n");
    }
    return 0;
}