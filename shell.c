#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"

/**
 * char ** parse_args (char * line):
    takes the command line and parses by spaces
    returns a char ** containing the command and each parameter
 */
char ** parse_args (char * line) {
  char ** ans = malloc (256);
  int i = 0;
  //char * now = line;
  char * temp;
  while (temp) {
    //ans [i] = strsep (&now, " ");
    temp = strsep(&line, " ");
    ans[i] = temp;
    i ++;
  }

  return ans;
}

/**
 * char ** parse_argsSemi (char * line):
    takes the command line and parses by semicolon
    returns a char ** containing the commands on each side of the semi colon
 */
char ** parse_argsSemi (char * line) {
  char ** ans = malloc (256);
  int i = 0;
  char * now = line;
  while (now) {
    char * currentCommand = strsep (&now, ";");
    ans[i] = currentCommand;
    i ++;
    int j;
    int length = strlen(currentCommand);
    if (currentCommand[length-1] == ' '){//removes trailing space
      currentCommand[length - 1] = 0;
    }
    if (currentCommand[0] == ' '){//removes beginning space by shifting everything over 1
      for (j = 0; j < length; j++){
        currentCommand[j] = currentCommand[j+1];
      }
    }
  }
  return ans;
}

/**
 * void executeOne (char ** args):
    takes the parsed command line and executes the commands
    prints "command does not work" if it didn't execute
    does not return anything
 */
void executeOne (char** args) {
  int id = fork(); //creates child process
  if (id == 0){ //child
    int num = execvp(args[0], args);
    if (num == -1){
      printf("command does not work\n");
    }
  }
  else{ //parent
    wait(0); //waits for child to finish
  }
}

/**
 * void redirectgreater (char * line):
    takes the command line
    parses by > to find the command and the filename
    creates the file with the given filename and writes the output of the command
    does not return anything
 */
void redirectgreater (char * line) {
  int f = fork();
  if (f){
    int status;
    wait(&status);
  }
  else{
    char * command = strsep(&line,">");
    char * fileName = malloc(256);
    fileName = strsep(&line,">");
    while (*fileName == ' ') {
    	fileName++;
    }
    int u = strlen(command)-1;
    while( command[u] == ' ' || command[u] == '\n'){
      command[u] = 0;
      u--;
    }
    int fd;
    fd = open(fileName, O_CREAT | O_WRONLY, 0644);
    if (fd < 0){
      printf("adfadsf\n");
    }
    char ** cA = parse_args(command);
    dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    execvp(cA[0], cA);
    close(fd);

  }
}

/**
 * void redirectless (char * line):
    takes the command line
    parses by < to find the command and the filename
    creates the file with the given filename and writes the input of the command
    does not return anything
 */
void redirectless (char * line) {
  fflush(stdout);
  char * command = strsep(&line,"<");
  char * fileName = strsep(&line,"<");

  while (*fileName == ' ') {
    fileName++;
  }
  int u = strlen(command)-1;
  while( command[u] == ' ' || command[u] == '\n'){
    command[u] = 0;
    u--;
  }

  int file = open(fileName, O_RDONLY,0644);
  if (file == -1){
    printf("file doesnt open\n");
  }
  int backup = dup (0);
  dup2(file,0);
  executeOne(parse_args(command));
  dup2 (backup, 0);
  close(file);
}

/**
 * void pipe (char * line):
    takes the command line
    parses by | to find both parameters
    uses the output of the first function to be the input of the second
    does not return anything
 */
void mypipe (char * line) {
  //char ** command = malloc (256);
  char * left = strsep(&line,"|"); //string left of |
  char * right = strsep(&line,"|"); //string right of |
  while (*right == ' ') {
    right++;
  }
  int i = strlen(left)-1;
  for (; i > 1; i--){
    if (left[i] == ' ' || left[i] == '\n' ){
      left[i] = 0;
    }
  }
  //char ** command = parse_argsSpecial (line, "|");

  int pd[2];
  int pid;
  int backup = dup(0);
  int backup2 = dup(1);
  pipe(pd);
  pid = fork();
  if (pid){
    close(pd[1]);
    //wait(0);
    backup = dup(0);
    dup2(pd[0],0);
    char ** args = parse_args (right);
    executeOne (args); //execvp (args[0], args);

    dup2(backup,0);
    close(backup);
    close(pd[0]);
  }
  else {
    close(pd[0]);
    backup2 = dup(1);
    dup2(pd[1],1);
    char ** args = parse_args (left);
    executeOne (args); //execvp (args[0], args);
    dup2(backup2,1);
    close(backup2);
    close(pd[1]);
    exit(0);
  }
  //dup2(backup,0);
  //dup2(backup2,1);
}

/**
 * void double1 (char * line):
    takes the command line
    runs a command line with both > and <
    uses similar function as redirectless and redirectgreater
    does not return anything
 */
void double1(char * line){//something < something > something
  char * command = strsep(&line,">");
  char * fileName = strsep(&line,">");
  while (*fileName == ' ') {
  	fileName++;
  }
  int i = strlen(command)-1;
  for (; i > 1; i--){
    if (command[i] == ' ' || command[i] == '\n' ){
      command[i] = 0;
    }
  }
  int file = open(fileName, O_WRONLY | O_CREAT,0666);
  int backup = dup (1);
  dup2(file,1);
  redirectless(command);
  //executeOne(parse_args(command));
  dup2 (backup, 1);
  close(file);
}
