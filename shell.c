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

void executeOne (char* command) {
  char * line = strdup (command);
  char ** args = parse_args (line);
  //printf ("Running: %s...\n", command);
  int id = fork(); //creates child process
  if (id == 0){ //child
    execvp(args[0], args);
    exit(0);
  }
  else{ //parent
    wait(0); //waits for child to finish
  }
}

void execmultiple (char* command) {
	char * line = strdup (command);
	char ** commands = malloc(256);
  int k = 0;
  while (line){
    commands[k] = strsep (&line, ";");
    k++;
  }

	int i = 0;
	while (commands[i] != NULL) {
		executeOne (commands[i]);
		i ++;
	}
}


int main(int argc, char *argv[]){
  char input[100] = "";
  int status = 0; //0 is true

  while(status == 0){ //if true then it continues to run
    printf("%s", "something: ");
    fgets(input, 100, stdin);
    input[strlen(input)-1] = '\0';
    execmultiple(input);
  }

  return 0;
}
