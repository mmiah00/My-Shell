#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

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
    //printf("allCommands[0] is ->%s<-\n",allCommands[0]);
    //printf("allCommands[1] is ->%s<-\n", allCommands[1]);
    int i;
    for (i = 0; allCommands[i]; i++){
      char * line = strdup (allCommands[i]);
      //printf("line is ->%s<-",line);
      while (*line == ' ') {
        line++;
      }
      int u = strlen(line)-1;
      while( line[u] == ' ' || line[u] == '\n'){
        line[u] = 0;
        u--;
      }
      //printf("line is ->%s<-",line);
      char ** args = parse_args (line);
      //printf("->%s<-", line);

      if (strcmp(args[0], "cd") == 0){
        chdir(args[1]);
      }
      else if (strchr(allCommands[i],'>') != NULL && strchr(allCommands[i],'<') != NULL &&
              strchr(allCommands[i],'>') < strchr(allCommands[i],'<')){ //something > something < something
                printf("\n:(\n");
              }
      else if (strchr(allCommands[i],'>') != NULL && strchr(allCommands[i],'<') != NULL &&
              strchr(allCommands[i],'>') > strchr(allCommands[i],'<')){ //something < something > something
                printf("\n:(\n");
                double1(allCommands[i]);
              }
      else if (strchr(allCommands[i],'>') != NULL){

        redirectgreater(allCommands[i]);
      }
      else if (strchr (allCommands[i], '<') != NULL) {
	      redirectless (allCommands[i]);
      }
      else if (strcmp(args[0], "exit") == 0){
        exit(0);
      }
      else if (strchr (allCommands[i], '|') != NULL){
        mypipe(allCommands[i]);
      }
      else{
        printf("\n");
        executeOne(args);
      }

    }
  }

  return 0;
}
