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

void execute (char* command) {
  char * line = strdup (command);
  char ** args = parse_args (line);
  printf ("%s...\n", command);
  execvp (args[0], args);
  printf ("Didn't work"); 
}
