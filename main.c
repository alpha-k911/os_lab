#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
// #include<conio.h>
#include<sys/wait.h>
// bold_red = [1;32m
// bold_blue = [1;34m
typedef enum bool_tag {false,true} bool;
static int status = 0;
void clrscr()
{
    system("clear");
}
void prompt_(){
    char* user = getenv("USER");
    char pwd[200];
    if(getcwd(pwd, sizeof(pwd)) != NULL) {
        printf("\033[1;31m%s@system\033[0m:\033[1;34m%s\033[0m# ", user, pwd);
    }else{
        printf("\033[1;31m%s@system\033[0m:\033[1;34m%s\033[0m# ", user, "ERROR in getcwd()");
    }
}

int cd(char *cmd){
    int p = chdir(cmd);
    return p;
}
void exec_(char* cmd_args[]){
    pid_t pid = fork();
    if(pid == -1){
        printf("Can't Fork...\n");
    }else if(pid == 0){
        if(strcmp(cmd_args[0],"cd") == 0){
            if(cd(cmd_args[1]) < 0){
                printf("Shell: Incorrect command\n");
            }
//            exit(0);
        }
        else if(execvp(cmd_args[0],cmd_args) < 0){
            status = 1;
            printf("Shell: Incorrect command\n");
        }
//        exit(0);
    }else{
        wait(NULL);
    }
    return;
}
void split_space(char *inp, char** cmd_args){
    int i = 0;
    cmd_args[i] = strtok(inp," ");
    while(cmd_args[i] != NULL){
        printf("word:%s\n", cmd_args[i]);
        i++;
        cmd_args[i] = strtok(NULL," ");
    }
}
void execute(char *inp, char** cmd_args){
    char *found;
    int i = 0;
    status = 0;
    char* new_inp;
    cmd_args[0] = NULL;
    if(status == 0 && (found = strsep(&inp,"&&")) != NULL){
//        printf("cmd_: %s\n",found);
        do {
            status = 0;
            new_inp = found;
            split_space(new_inp, cmd_args);
            exec_(cmd_args);
            cmd_args[0] = NULL;
//        if(status == 1)
        }while(status == 0 && (found = strsep(&inp,"&&")) != NULL);
    }else if(status == 0 && (found = strsep(&inp,"&&&")) != NULL){
        cmd_args[0] = found;

    }
}
void split_pipe(char *inp, char** cmd_args){
    int i = 0;
    cmd_args[i] = strtok(inp,"|");
    while(cmd_args[i] != NULL){
        printf("word:%s\n", cmd_args[i]);
        i++;
        cmd_args[i] = strtok(NULL," ");
    }
}
bool piped(char *inp){
    int l = strlen(inp);
    for(int i = 0; i < l; i++){
        if(inp[i] == '|'){
            return true;
        }
    }
    return false;
}

int main(int argc, char const *argv[]) {
    int flag = 1,size;
    // char inpt[1024];
    char* inp;
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    size_t buf = 48;
    clrscr();
    flag = 1;
    char* cmd_args[10];
    while(flag < 10 && flag != 0){
        // scanf("%[^\n]s",&inp);
        prompt_();
        size = getline(&inp,&buf,stdin);
        inp[size-1] = '\0';
        if(strcmp(inp,"exit") != 0){
//            split_space(inp,cmd_args);
//            exec_(cmd_args);
            execute(inp,cmd_args);
//             flag = 1;
        }else{
            printf("Exiting.....");
            break;
        }
        printf("%d lkkl ",flag);
        flag++;
    }
    return 0;
}