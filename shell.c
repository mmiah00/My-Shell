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
    ans [i] = strsep (&now, ";");
    i ++;
  }
  return ans;
}

void cd (char ** args) {
  if (strcmp(args[0], "cd") == 0){
    chdir(args[1]);
  }
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

void redirect (char * line) {
  if (strchr (line, '>')) {
    char ** args = parse_args (line);
    close (1);
    open (args[2], O_RDWR, 0666);
    if (fork () == 0) {
      execvp (args[0], args);
    }
  }
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


int main(int argc, char *argv[]){
  char input[100] = "";
  int status = 0; //0 is true

  while(status == 0){ //if true then it continues to run
    printf("%s", "something: ");
    fgets(input, 100, stdin); //gets input
    input[strlen(input)-1] = '\0'; //removes the new line at the end
    char ** allCommands = malloc (256);
    allCommands = parse_argsSemi(input);
    int i;
    for (i = 0; allCommands[i]; i++){
      char * line = strdup (allCommands[i]);
      char ** args = parse_args (line);
      if (strcmp(args[0], "cd") == 0){
        chdir(args[1]);
      }
      else if (strcmp(args[0], "exit") == 0){
        exit(0);
      }
      else{
        executeOne(args);
      }
    }
  }

  return 0;
}
