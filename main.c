#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

int main ()  {
  char line1[100] = "ls -1";
  char *p = "ls -1";//&line1;
  executeOne(p);
  return 0;
}
