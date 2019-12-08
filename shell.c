#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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

void executeOne (char** args) {
  int id = fork(); //creates child process
  if (id == 0){ //child
    execvp(args[0], args);
  }
  else{ //parent
    wait(0); //waits for child to finish
  }
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
      else{
        executeOne(args);
      }
    }
  }

  return 0;
}
