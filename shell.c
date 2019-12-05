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
  printf ("%s...\n", command);
  int id = fork();
  if (id == 0){
    execvp(args[0], args);
  }
  else{
    wait(0);
  }
  printf ("Didn't work");
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
