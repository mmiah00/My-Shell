# myshell

## Bugs
    - Output redirection > was able to work (hopefully) and write to the file when running ls -l > file for 
      example. However, w > whofile leaves the file blank for some reason.
    - ls -l > file would create a file with the content of ls not ls -l.

## Our Features: 
    - char ** parse_args (char * line) 
      - parses through the line you are trying to execute
      - returns an array of strings with each command, flags, and parameter 
      - only parses on spaces 
 
    - char ** parse_argsSemi (char * line) 
      - same as parseargs but parses on ";" instead
      
    - void executeOne (char ** args) 
      - takes a parsed line and executes the command 
      
    - void redirectgreater (char * line) 
      - takes the command line as a parameter. Separate command line by the > into two parts. First part is the
      command while second is the filename. Removed leading and trailing spaces from both of them and then executed
      after backingup with dup and redirecting with dup2.
      - does not return anything
 
    - void redirectless (char * line) 
      - same as redirectgreater but is separated by "<"
      - redirects stdin from a file
  
    - void mypipe (char * line)
      - takes the command line as a parameter
      - does not return anything 
      - reads the input from the first paramter (before the |) and executes it in the second (after |)
     
     - void double1 (char * line)
      - this is for when the command line contains a < followed by a >.
      - first redirectgreater is ran but instead of execvping the commandline, redirectless is ran.
 
