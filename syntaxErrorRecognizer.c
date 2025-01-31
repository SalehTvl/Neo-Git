#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int userInfoSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc!=4 && argc!=5) error_occured=1;
    if(argc==5) {
        if(strcmp(argv[2], "-global")!=0) error_occured=1;           
        if(strcmp(argv[3],"user.name")!=0  && strcmp(argv[3],"user.email")!=0) error_occured=1;
        if(strcmp(argv[3],"user.name")==0) return 1;  
        if(strcmp(argv[3],"user.email")==0) return 2;  
    }
    if(argc==4) {
        if(strcmp(argv[2],"user.name")!=0 && strcmp(argv[2],"user.email")!=0) error_occured=1;
        if(strcmp(argv[2],"user.name")==0) return 3;
        if(strcmp(argv[2],"user.email")==0) return 4;
    }
    if(error_occured==1) {     
        return 0;
    }
}

int aliasSER(int argc, char* argv[]) {
    int error_occured=0;
    char alias_spell[6]="alais.";
    if(argc!=4 && argc!=5) error_occured=1;
    if(argc==5) {
        if(strcmp(argv[2], "-global")!=0) error_occured=1;      
        for(int i=0; i<6; i++) {
            if(argv[3][i]!=alias_spell[i]) {
                error_occured=1;
                break;
            }
        }       
    }
    if(argc==4) {
        for(int i=0; i<6; i++) {
            if(argv[2][i]!=alias_spell[i]) {
                error_occured=1;
                break;
            }
        } 
    }
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    return 1;
}

int initSER(int argc, char* argv[]) {
    if(argc!=2) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    return 1;
}

int addSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc<3) error_occured=1;
    if(argc==3 && strcmp(argv[2], "-redo")==0) return 2;
    if(strcmp(argv[2],"-n")==0 && argc!=4) error_occured=1;
    if(argc==3 && (strcmp(argv[2], "-f")==0 || strcmp(argv[2], "-n")==0)) error_occured=1;
    if(argc>3 && strcmp(argv[2], "-f")==0) return 3;
    if(argc==3) return 1;
    if(strcmp(argv[2],"-n")==0) {
        int length = strlen(argv[3]);
        for(int i=0; i<length; i++) {
            if(argv[3][i]<48 || argv[3][i]>57) {
                printf("Invalid command due to unacceptable depth number\n");
                return 0;
            }
        }
        return 4;
    }
    if(argc>3) return 5;
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words\n");
        return 0;
    }
}

int resetSER(int argc, char* argv[]) {
    if(argc<3) return 0;
    else if(argc==3 && strcmp(argv[2], "-f")==0) return 0;
    if(argc==3) return 1;
    if(argc>3 && strcmp(argv[2], "-f")==0) return 2;
    if(argc>3) return 3;
    return 0;
}

int statusSER(int argc, char* argv[]) {
    if(argc!=2) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    return 1;
}

int commitSER(int argc, char* argv[]) {
    int error_occured=0;
    if(strcmp(argv[2], "-m")!=0 && strcmp(argv[2], "-s")!=0) error_occured=1;
    if(strlen(argv[3])>75) {
        printf("commit message too long.");
        return 0;
    }
    if(strlen(argv[3])<1) {
        printf("There is no comment to your commit.");
        return 0;
    }
    if(argc==3) {
        printf("There is no comment to your commit.");
        return 0;
    }
    if(argc!=4) error_occured=1;
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    if(strcmp(argv[2], "-s")==0) return 2;
    return 1;
}

int setSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc!=6) error_occured=1;
    if(strcmp(argv[2], "-m")!=0) error_occured=1;
    if(strcmp(argv[4], "-s")!=0) error_occured=1;
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    return 1;
}

int replaceSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc!=6) error_occured=1;
    if(strcmp(argv[2], "-m")!=0) error_occured=1;
    if(strcmp(argv[4], "-s")!=0) error_occured=1;
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    return 1;
}

int removeSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc!=4) error_occured=1;
    if(strcmp(argv[2], "-s")!=0) error_occured=1;
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    return 1;
}

int logSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc==2) return 6;
    if(argc!=4 && argc!=2) error_occured=1;
    if(argc==4 && (strcmp(argv[2], "-n")!=0) && (strcmp(argv[2], "-branch")!=0) && (strcmp(argv[2], "-author")!=0) && (strcmp(argv[2], "-since")!=0) && (strcmp(argv[2], "-before")!=0) && (strcmp(argv[2], "-search")!=0))
        error_occured=1;
    
    if(strcmp(argv[2],"-n")==0) {
        int length = strlen(argv[3]);
        for(int i=0; i<length; i++) {
            if(argv[3][i]<48 || argv[3][i]>57) {
                printf("Invalid command due to unacceptable number.");
                return 0;
            }
        }
    }
    // code for invalid date of since and before
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    if(argc==2) return 6;
    if(strcmp(argv[2], "-n")==0) return 1;
    else if(strcmp(argv[2], "-branch")==0) return 2;
    else if(strcmp(argv[2], "-author")==0) return 3;
    else if(strcmp(argv[2], "-since")==0 || strcmp(argv[2], "-before")==0) return 4;
    else if(strcmp(argv[2], "-search")==0) return 5;
}

int branchSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc>3) error_occured=1;
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    if(argc==3) return 2; 
    return 1;
}

int checkoutSER(int argc, char* argv[]) {
    int error_occured=0;
    if(argc!=3) error_occured=1;
    char head[4]="HEAD";
    int i;
    for(i=0; i<4; i++) {
        if(head[i]!=argv[2][i])
            break;
    }
    if(i==4) {
        if(strlen(argv[2])==4)
            return 3;
        else if(argv[2][4]=='-') {
            int length=strlen(argv[2]);
            for(int i=5; i<length; i++) {
                if(argv[2][i]<48 || argv[2][i]>57) {
                    printf("invalid number\n");
                    return 0;
                }
            }
            return 4;
        }
    }
    if(strlen(argv[2])==8) {
        return 2;
    }
    
    if(error_occured==1) {
        printf("Invalid command due to misspell or extra words.");
        return 0;
    }
    return 1;
}

int revertSER(int argc, char* argv[]) {
    if(argc==3) {
        if(strstr(argv[2], "HEAD-")!=NULL) {
            int len=strlen(argv[2]);
            for(int i=5; i<len; i++) {
                if(argv[2][i]<48 || argv[2][i]>57) return 0;
            }
        return 3;
        } 
        else return 1;
    }
    if(argc!=4) return 0;
    if(strcmp(argv[2], "-n")==0)  return 4;
    if(strstr(argv[3], "HEAD-")!=NULL) {
        int len=strlen(argv[3]);
        for(int i=5; i<len; i++) {
            if(argv[3][i]<48 || argv[3][i]>57) return 0;
        }
        return 5;
    }
    return 2;
}

int tagSER(int argc, char* argv[]) {
    if(argc==2) return 8;
    else if(argc==4 && strcmp(argv[2], "show")==0) return 9;
    else if(argc==4 && strcmp(argv[2], "-a")==0) return 1;
    else if(argc==5 || argc<4 || argc>9) return 0;
    else if(argc==6) {
        if(strcmp(argv[4], "-m")==0) return 2;
        if(strcmp(argv[4], "-c")==0) return 3;
    }
    else if(argc==7) {
        if(strcmp(argv[4], "-m")==0 && strcmp(argv[6], "-f")==0) return 5;
        if(strcmp(argv[4], "-c")==0 && strcmp(argv[6], "-f")==0) return 6;
    }
    else if(argc==8) {
        if(strcmp(argv[4], "-m")==0 && strcmp(argv[6], "-c")==0) return 4;
    }
    else if(strcmp(argv[4], "-m")==0 && strcmp(argv[6], "-c")==0 && strcmp(argv[8], "-f")==0) return 7;
    return 0;
}

int grepSER(int argc, char* argv[]) {
    if(argc<6 || argc>9) return 0;
    if(argc==6 && strcmp(argv[2], "-f")==0 && strcmp(argv[4], "-p")==0) return 1;
    if(argc==7 && strcmp(argv[2], "-f")==0 && strcmp(argv[4], "-p")==0 && strcmp(argv[6], "-n")==0) return 3;
    if(argc==8 && strcmp(argv[2], "-f")==0 && strcmp(argv[4], "-p")==0 && strcmp(argv[6], "-c")==0) return 2;
    if(argc==9 && strcmp(argv[2], "-f")==0 && strcmp(argv[4], "-p")==0 && strcmp(argv[6], "-c")==0 && strcmp(argv[8], "-n")==0) return 4;
}

int diffSER(int argc, char* argv[]) {
    if(argc<5) return 0;
    if(argc==5 && strcmp(argv[2], "-c")==0) return 1;
    if(strcmp(argv[2], "-f")==0) {
        if(argc==5) return 2;
        if(argc==8) {
            if(strcmp(argv[5], "-line1")==0) {
                int len1=strlen(argv[6]);
                for(int i=0; i<len1; i++) {
                    if(argv[6][i]<48 || argv[6][i]>57) return 0;
                }
                int len2=strlen(argv[7]);
                for(int i=0; i<len2; i++) {
                    if(argv[7][i]<48 || argv[7][i]>57) return 0;
                }
                return 3;
            }
            else if(strcmp(argv[5], "-line2")==0) {
                int len1=strlen(argv[6]);
                for(int i=0; i<len1; i++) {
                    if(argv[6][i]<48 || argv[6][i]>57) return 0;
                }
                int len2=strlen(argv[7]);
                for(int i=0; i<len2; i++) {
                    if(argv[7][i]<48 || argv[7][i]>57) return 0;
                }
                return 4;
            }
        }
        if(argc==11 && strcmp(argv[5], "-line1")==0 && strcmp(argv[8], "-line2")==0) {
            int len1=strlen(argv[6]);
            for(int i=0; i<len1; i++) {
                if(argv[6][i]<48 || argv[6][i]>57) return 0;
            }
            int len2=strlen(argv[7]);
            for(int i=0; i<len2; i++) {
                if(argv[7][i]<48 || argv[7][i]>57) return 0;
            }
            int len3=strlen(argv[9]);
            for(int i=0; i<len3; i++) {
                if(argv[9][i]<48 || argv[9][i]>57) return 0;
            }
            int len4=strlen(argv[7]);
            for(int i=0; i<len4; i++) {
                if(argv[10][i]<48 || argv[10][i]>57) return 0;
            }
            return 5;
        }
    }
    return 0;
}

int stashSER(int argc, char* argv[]) {
    if(argc==3) {
        if(strcmp(argv[2], "push")==0) return 1;
        else if(strcmp(argv[2], "list")==0) return 3;
        else if(strcmp(argv[2], "pop")==0) return 4;
        else if(strcmp(argv[2], "clear")==0) return 6;
    }
    else if(argc==4) {
        if(strcmp(argv[2], "pop")==0) return 5;
        if(strcmp(argv[2], "show")==0) return 7;
    }
    else if(argc==5) {
        if(strcmp(argv[2], "push")==0 && strcmp(argv[3], "-m")==0) return 2;
    }
    return 0;
}