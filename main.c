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
        //this code removes spaces at the beginning and end but idk what to do with it
        //redirect breaks for some reason
        /**
        printf("%s",line);
        char * command = strsep(&allCommands[i],">");
        char * fileName = strsep(&allCommands[i],">");
        while (fileName[0] == ' ') {
          fileName++;
        }
        int i = strlen(command)-1;
        for (; i > 1; i--){
          if (command[i] == ' ' || command[i] == '\n' ){
            command[i] = 0;
          }
        }
        printf("Your command is ->%s<- and your file is ->%s<-", command,fileName);
        **/
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
        executeOne(args);
      }

    }
  }

  return 0;
}

// int main(int argc, char *argv[]){
//   char input[100] = "";
//   int status = 0; //0 is true
//   char * cwd= malloc(256 * sizeof(char));
//   while(status == 0){ //if true then it continues to run
//     printf("%s$ ", getcwd(cwd, 256));
//     fgets(input, 100, stdin); //gets input
//     input[strlen(input)-1] = '\0'; //removes the new line at the end
//     char ** allCommands = malloc (256);
//     allCommands = parse_argsSemi(input);
//     int i;
//     for (i = 0; allCommands[i]; i++){
//       char * line = strdup (allCommands[i]);
//       //printf("%s<-",line);
//       char ** args = parse_args (line);
//       printf("->%s<-", line);
//
//       if (strcmp(args[0], "cd") == 0){
//         chdir(args[1]);
//       }
//       else if (strchr(line,'>') != NULL){
//         redirectgreater(line);
//       }
//       else if (strchr (line, '<') != NULL) {
// 	      redirectless (line);
//       }
//       else if (strcmp(args[0], "exit") == 0){
//         exit(0);
//       }
//       else if (strchr (line, '|') != NULL){
//         mypipe(line);
//       }
//       else{
//         executeOne(args);
//       }
//
//     }
//   }
//
//   return 0;
// }
