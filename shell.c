#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"

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

/*
char ** parse_argsSemi (char * line) {
  char ** ans = malloc (256);
  char * now = line;
  ans [0] = strsep (&now," ");
  //strsep (&now, ";");
  ans [1] = strsep (&now, ";");
  printf ("%s\n%s\n", ans [0], ans [1]);
  return ans;
}
*/

char ** parse_argsSpecial (char * line, char * character) {
  char ** ans = malloc (256);
  int i = 0;
  char * now = line;
  while (now) {
    char * currentCommand = strsep (&now, character);
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

void redirectgreater (char * line) {
  fflush(stdout);
  //printf("LINE IS ->%s<-\n", line);
  //char ** command = malloc (256);
  char * command = strsep(&line,">");
  //printf("%s",command[0]);
  char * fileName = strsep(&line,">");
 //char ** command = parse_args (line);
  /*
  char * fileName = command[1];
  while (*fileName == ' ') {
  	fileName++;
  }*/
  while (*fileName == ' ') {
  	fileName++;
  }
  int i = strlen(command)-1;
  for (; i > 1; i--){
    if (command[i] == ' ' || command[i] == '\n' ){
      command[i] = 0;
    }
  }
  //printf("filename is %s command is %s", fileName, command);
  int file = open(fileName, O_WRONLY | O_CREAT,0666);
  int backup = dup (1);
  dup2(file,1);
  executeOne(parse_args(command));
  dup2 (backup, 1);
  close(file);
  /*
  char ** args;
  args [0] = filefrom;
  args [1] = fileto;
  executeOne (args);
  int f = open (fileto, O_RDWR | O_CREAT, 0666);
  int backup = dup (1); //duplicating stdout
  dup2 (f, 1);
  close (f);
  */
}

void redirectless (char * line) {
  fflush(stdout);
  char * command = strsep(&line,"<");
  char * fileName = strsep(&line,"<");

  while (*fileName == ' ') {
    fileName++;
  }
  int i = strlen(command)-1;
  for (; i > 1; i--){
    if (command[i] == ' ' || command[i] == '\n' ){
      command[i] = 0;
    }
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
