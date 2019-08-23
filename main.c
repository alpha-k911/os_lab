#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
// #include<conio.h>
#include<sys/wait.h>
#include <fcntl.h>

// bold_red = [1;32m
// bold_blue = [1;34m
void handler(int p){

}
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

//    printf("for cd in chdir \n");
    int p = chdir(cmd);
    return p;
}
void exec_(char* cmd_args[]){
//    printf("INexec\n");
    pid_t pid = fork();
    if(pid == -1){
        printf("Can't Fork...\n");
    }else if(pid == 0){
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        if(strcmp(cmd_args[0],"cd") == 0){
//            printf("for cd in exec_ \n");
            if(cd(cmd_args[1]) < 0){
                printf("Shell: Incorrect command\n");
            }
            exit(0);
        }
        else if(execvp(cmd_args[0],cmd_args) < 0){
            status = 1;
            printf("Shell: Incorrect command\n");
        }
        exit(0);
    }else{
        wait(NULL);
    }
    return;
}
void exec__(char* cmd_args[]){
    pid_t pid = fork();
    if(pid == -1){
        printf("Can't Fork...\n");
    }else if(pid == 0){
        signal(SIGINT,SIG_DFL);
        signal(SIGTSTP,SIG_DFL);
        if(strcmp(cmd_args[0],"cd") == 0){
            if(cd(cmd_args[1]) < 0){
                printf("Shell: Incorrect command\n");
            }
            exit(0);
        }
        else if(execvp(cmd_args[0],cmd_args) < 0){
            status = 1;
            printf("Shell: Incorrect command\n");
        }
        exit(0);
    }else{
//        wait(NULL);
    }
    return;
}
void split_space(char *inp, char** cmd_args){
    int i = 0;
    cmd_args[i] = strtok(inp," ");
    while(cmd_args[i] != NULL){
//        printf("word:%s\n", cmd_args[i]);
        i++;
        cmd_args[i] = strtok(NULL," ");
    }
}
char* split_less(char *inp, char** cmd_args){
    int i = 0;
    char *file;
    cmd_args[i] = strtok(inp,"<");
    cmd_args[1] = NULL;
    file = strtok(NULL," ");
    return file;
}
char*  split_great(char *inp, char** cmd_args){
    int i = 0;
    char *file;
    cmd_args[i] = strtok(inp,">");
    cmd_args[1] = NULL;
    file = strtok(NULL," ");
    return file;
}
int triple(char *inp){
    int j = 0,i = 0,c = 0;
    char *r = "^^";
    char *ret = NULL;
    ret = strstr(inp,r);
    return ret != NULL;
}
int doubl(char *inp){
    int j = 0,i = 0,c = 0;
    char *r = "&&";
    char *ret = NULL;
    ret = strstr(inp,r);
    return ret != NULL;
}
int less(char *inp){
    char *r = " < ";
    char *ret = NULL;
    ret = strstr(inp,r);
    return ret != NULL;
}
int great(char *inp){
    char *r = " > ";
    char *ret = NULL;
    ret = strstr(inp,r);
    return ret != NULL;
}
void execute(char *inp, char** cmd_args){
//    printf("En\n");
    char *found = NULL;
    int i = 0;
    status = 0;
    char* new_inp;
    cmd_args[0] = NULL;
    char** cmd_args_ptr[10];
    for(i = 0; i < 10; i++){
        cmd_args_ptr[i] = (char**)malloc(sizeof(char*));
    }
    i = 0;
    if((triple(inp)) == 1){
        found = strsep(&inp,"^^");
//        printf("cmd_: %s\n",found);
        cmd_args_ptr[i][0] = NULL;
        do {
            new_inp = found;
            split_space(new_inp,cmd_args_ptr[i]);
            exec__(cmd_args_ptr[i]);
            i++;
        }while((found = strsep(&inp,"^^")) != NULL);
        int status,id;
        while((id = waitpid(-1,&status,0)) != -1){
            printf("done with %d\n",id);
        }
    }else if(doubl(inp) == 1){
//    if((found = strsep(&inp," && ")) != NULL){
//        printf("in&&\n");
        found = strsep(&inp,"&&");
        do {
            status = 0;
            new_inp = found;
            split_space(new_inp, cmd_args);
            exec_(cmd_args);
            cmd_args[0] = NULL;
//        if(status == 1)
        }while((found = strsep(&inp,"&&")) != NULL);
    }else if(less(inp)){
//        printf("In Less\n");
        char* f;
        int std_copy = dup(0);
        f = split_less(inp,cmd_args);
//        int out_file = open(cmd_args[1],O_WRONLY | O_APPEND);
        int in_file = open(f,O_RDONLY );
//        printf("CMD: %s\n",cmd_args[0]);
        cmd_args[1] = NULL;
        dup2(in_file,0);
        split_space(cmd_args[0],cmd_args);
        exec_(cmd_args);
        close(in_file);
        dup2(std_copy,0);
    }else if(great(inp)) {
//        printf("In Great\n");
        char* f;
        int std_copy = dup(1);
        f = split_great(inp,cmd_args);
//        int out_file = open(cmd_args[1],O_WRONLY | O_APPEND);
        int out_file = open(f,O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
//        printf("CMD: %s\n",cmd_args[0]);
        cmd_args[1] = NULL;
        dup2(out_file,1);
        split_space(cmd_args[0],cmd_args);
        exec_(cmd_args);
        close(out_file);
        dup2(std_copy,1);
    }else{
        split_space(inp, cmd_args);
        if(strcmp(cmd_args[0],"cd") == 0){
            printf("for cd\n");
            if(cd(cmd_args[1]) < 0){
                printf("Shell: Incorrect command\n");
            }
        }else {
//        printf("entered...");
            exec_(cmd_args);
        }

    }
}
int main(int argc, char const *argv[]) {
    int flag = 1,size;
    // char inpt[1024];
    char* inp;
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
    size_t buf = 200;
    clrscr();
    flag = 1;
    char* cmd_args[10];
    while(flag < 10 && flag != 0){
        // scanf("%[^\n]s",&inp);
        prompt_();
        size = getline(&inp,&buf,stdin);
        inp[size-1] = '\0';
//        printf("entered...");
        if(strcmp(inp,"exit") != 0){
//            split_space(inp,cmd_args);
//            exec_(cmd_args);
            execute(inp,cmd_args);
//             flag = 1;
        }else{
            printf("Exiting.....");
            break;
        }
//        printf("%d-",flag);
//        flag++;
    }
    return 0;
}