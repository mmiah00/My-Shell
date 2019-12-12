# myshell

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
      - takes the command line as a parameter
      - does not return anything 
      - runs the first part of the line (before the >) and makes a new file (which is named from the second part)
        - redirects stdout of the first part to a file
 
    - void redirectless (char * line) 
      - same as redirectgreater but is separated by "<"
      - redirects stdin from a file
  
    - void mypipe (char * line)
      - takes the command line as a parameter
      - does not return anything 
      - reads the input from the first paramter (before the |) and executes it in the second (after |)
 
