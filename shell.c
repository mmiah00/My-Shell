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
  char * now = line;
  while (now) {
    ans [i] = strsep (&now, " ");
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
    execvp(args[0], args);
  }
  else{ //parent
    wait(0); //waits for child to finish
  }
}

void redirectgreater (char * line) {
  char ** command = malloc (256);
  command[0] = strsep(&line,">");
  printf("%s",command[0]);
  char * fileName = strsep(&line,">");
  while (*fileName == ' ') {
        fileName++;
  }
 //char ** command = parse_args (line);
  char * list = strdup(command[0]);
  int i = strlen(list)-1;
  for (; i > 1; i--){
    if (list[i] == ' ' || list[i] == '\n' ){
      list[i] = 0;
    }
  else{
    break;
  }
  }
  /*
  char * fileName = command[1];
  while (*fileName == ' ') {
  	fileName++;
  }*/
//printf("filename is %s command is %s", fileName, command[0]);
  int file = open(fileName, O_WRONLY | O_CREAT,0666);
  int backup = dup (1);
  dup2(file,1);
  close(file);
  executeOne(parse_args(command[0]));
  dup2 (backup, 1);
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
  // char ** command = malloc (256);
  // command[0] = strsep(&line,"<");
  // printf("%s",command[0]);
  // char * fileName = strsep(&line,"<");

  char ** command = parse_argsSpecial (line, "<");
  char * fileName = command[1];

  //printf("filename is %s command is %s", fileName, command[0]);
  int file = open(fileName, O_WRONLY | O_CREAT,0666);
  int backup = dup (0);
  dup2(file,0);
  close(file);
  executeOne(parse_args(command[0]));
  dup2 (backup, 0);
}

void mypipe (char * line) {
  // char ** command = malloc (256);
  // command[0] = strsep(&line,"|"); //string left of |
  // command[1] = strsep(&line,"|"); //string right of |

  char ** command = parse_argsSpecial (line, "|");

  int pd[2];
  int pid;
  int backup = dup(0);
  int backup2 = dup(1);
  pipe(pd);
  pid = fork();
  if (pid == 0){//child
      //close(pd[0]);
    //backup1 = dup(1); //
      //dup2(pd[1],1);
      //char ** args = parse_args (command[0]);
      //execvp (args[0], args);
    //dup2(backup1,1);
    //close(pd[1]);
    //close(backup1);
    //exit(0);
    backup = dup(0);
    dup2(pd[0],0);
    close(pd[1]);
    char ** args = parse_args (command[1]);
    executeOne (args); //execvp (args[0], args);

    dup2(backup,0);
    close(backup);
    close(pd[0]);

    // backup = dup(pd[1]);
    // dup2 (pd[1], 1);
    // close (pd[0]);
    // char ** args = parse_args (command[0]);
    // execvp (args[0], args);
    // dup2(backup,1);
  }
  else {//parent
    //close(pd[1]);
      //wait(0);
    //backup = dup(0);
      //dup2(pd[0],0);
      //close(pd[1]);
      //char ** args = parse_args (command[1]);
      //execvp (args[0], args);
      //dup2(backup,0);
      //close(backup);
      //close(backup1);
      //close(pd[1]);
      //close(pd[0]);
    backup2 = dup(1);
    dup2(pd[1],1);
    close(pd[0]);
    char ** args = parse_args (command[0]);
    executeOne (args); //execvp (args[0], args);

    dup2(backup2,1);
    close(backup2);
    close(pd[1]);

    // wait(0);
    // backup = dup(pd[1]);
    // dup2 (pd[0], 0);
    // close (pd[1]);
    // char ** args = parse_args (command[1]);
    // execvp (args[0], args);
    // dup2(backup,1);

  }
  //dup2(backup,0);
  //dup2(backup2,1);
}

int main(int argc, char *argv[]){
  char input[100] = "";
  int status = 0; //0 is true
  char * cwd= malloc(256 * sizeof(char));
  while(status == 0){ //if true then it continues to run
    printf("\n%s$ ", getcwd(cwd, 256));
    fgets(input, 100, stdin); //gets input
    input[strlen(input)-1] = '\0'; //removes the new line at the end
    char ** allCommands = malloc (256);
    allCommands = parse_argsSemi(input);
    int i;
    for (i = 0; allCommands[i]; i++){
      char * line = strdup (allCommands[i]);
      //printf("%s<-",line);
      char ** args = parse_args (line);
      //printf("->%s<-", line);

      if (strcmp(args[0], "cd") == 0){
        chdir(args[1]);
      }
      else if (strchr(allCommands[i],'>') != NULL){
        printf("detected");
        redirectgreater(line);
      }
      else if (strchr (allCommands[i], '<') != NULL) {
	      redirectless (line);
      }
      else if (strcmp(args[0], "exit") == 0){
        exit(0);
      }
      else if (strchr (allCommands[i], '|') != NULL){
        mypipe(line);
      }
      else{
        executeOne(args);
      }

    }
  }

  return 0;
}
