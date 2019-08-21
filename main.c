#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include<unistd.h>
// #include<conio.h>
#include<sys/wait.h>
// bold_red = [1;32m
// bold_blue = [1;34m
typedef enum bool_tag {false,true} bool;
void clrscr()
{
    system("clear");
}
void prompt(){
    char* user = getenv("USER");
    char* pwd = getenv("PWD");
    printf("\033[1;31m%s@system\033[0m:\033[1;34m%s\033[0m# ", user, pwd);
}
void exec(char* cmd_args[]){
    // char *args[]={"ls",NULL};
    execvp(cmd_args[0],cmd_args);
}
void split_space(char *inp, char** cmd_args){
    int i = 0;
    cmd_args[i] = strtok(inp," ");
    while(cmd_args[i] != NULL){
        printf("word:%s\n", cmd_args[i]);
        i++;
//        strcpy(cmd_args[i++],part);
        cmd_args[i] = strtok(NULL," ");
    }
}
void execute(){

}
void split_pipe(char *inp, char** cmd_args){
    int i = 0;
    cmd_args[i] = strtok(inp,"|");
    while(cmd_args[i] != NULL){
        printf("word:%s\n", cmd_args[i]);
        i++;
//        strcpy(cmd_args[i++],part);
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
    size_t buf = 48;
    clrscr();
    char* cmd_args[10];
    while(flag < 10 && flag){
        // scanf("%[^\n]s",&inp);
        prompt();
        size = getline(&inp,&buf,stdin);
        inp[size-1] = '\0';
        if(strcmp(inp,"^C") != 0 && strcmp(inp,"exit") != 0){
            if(piped(inp)){

            }else{
                split_space(inp,cmd_args);
                // printf("done...%s\n",inp);
                exec(cmd_args);
            }
            // flag = 1;
        }else{
            flag = 0;
        }
        flag++;
    }
    return 0;
}