/* To compile: gcc -o usfsh.c
 * To execute: ./usfsh */

/* The purpose of this profram is to implement a mini UNIX shell
 *  and to use various system calls*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h>
#include <unistd.h>
#include <fcntl.h> // open

#define MAX_BUF_LEN 128
#define MAX_ARGS 128

/* This function output the prompt */
void print_prompt(void){
    int rv;
    rv = write(1, "$ ", 2);

    if (rv < 0) {
          printf("write() failed.\n");
          exit(-1);
    }
}

/* This function reads the commands into the huffer*/
void read_command_line(char *buf){
    int rv;
    
    rv = read(0, buf, (MAX_BUF_LEN-1));
    if (rv < 0) {
         printf("read() failed.\n");
         exit(-1);
    }
    buf[rv-1] = '\0';
}

/* This function parses the commands line by a white space 
 * and it returns the length of the commands  */
int parse_command_line(char *buf, char **args){
     int i = 0;
     args[i] = strtok(buf, " ");
     i = i + 1;
     while ((args[i] = strtok(NULL, " ")) != NULL) {
          i = i + 1;
    }
    for(int j=0;j<=i;j++)
        printf("%d,%s\n",j,args[j]);
    return i;
}

/* This function checks if there is a pipe redirection
 * by looking for the "|" sign */
bool is_pipe_redirection(char **args){
    int i;
    bool rv = false;
    
    for (i = 0; args[i] != NULL; i++) {
          if (strcmp(args[i], "|") == 0) {
               rv = true;
          }
    }

    return rv;
}

/* This finction makes the excution of pipe redirection
 * the parent creates two childs and they run simultaneously*/
void exec_pipe_redirection(char **args){
     int id, id2, i, j, k, pipefd[2];
     char *arr[100], *arr2[100];
     pipe(pipefd);

     id = fork(); // this child writes into the pipe
     if (id == 0){
          for (i = 0; args[i] != NULL; i++){
               if (strcmp(args[i], "|") == 0){
                    break;
               }
               arr[i] = args[i];
          }
          i++;
          arr[i] = NULL; // place a null where the pipe sign is located
          dup2(pipefd[1], 0);//replace stdout with pipefd write
          close(pipefd[0]);
          execvp(args[0], arr);// 1st arg (before the pipe)
          write(1, "1st exec pipe_redirection failed\n", 34);
          exit(-1);
          
     }
     id2 = fork();
     k = 0;
     int check = 1;
     if(id2 == 0){ // this child reads from the buffer
          for (i = 0; args[i] != NULL; i++){
               if( check != 1){
                    arr2[k] = args[i]; // add everything after the pipe to arr2[k]
                    k++;
               }
               if (strcmp(args[i], "|") == 0){
                    check = 0;
               }    
          }
          arr2[k] = NULL;// place a null where the after the last command

          dup2(pipefd[0], 0);//replace stdin with pipefd read
          close(pipefd[1]);
          execvp(arr2[0],arr2);// 1st arg (after the pipe) e.g. wc
          write(1, "exec pipe_redirection failed\n", 30);
          exit(-1);
          } 
               
     close(pipefd[0]);
     close(pipefd[1]);     
    
     id2 = wait(NULL);
     id = wait(NULL);
}

/* This function makes the excution of a simle program that 
 * not includes piping or file redirection */
void exec_simple_command(char **args){
     int id;
     id = fork();
     if (id == 0){
          execvp(args[0], args);
          printf("%s: command not found\n", args[0]);
          exit(-1);
     }
     id = wait(NULL);
}


int main(int argc, char **argv){
    char buf[MAX_BUF_LEN];
    char *args[MAX_ARGS];


    while(1){
          print_prompt();
          read_command_line(buf);
          argc = parse_command_line(buf, args);


           if (is_pipe_redirection(args)) {
                exec_pipe_redirection(args);
          
          }else{
               exec_simple_command(args);       
          }
    }
    return 0;
}
