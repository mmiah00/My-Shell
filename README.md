# myshell

## Bugs
    - Output redirection > was able to work (hopefully) and write to the file when running ls -l > file for
      example. However, w > whofile leaves the file blank for some reason.
    - two or more semicolons back does not work

## Our Features
    - Forks and execute commands. Commands with unnecessary spaces at the beginning and end will still be
      functional.
    - Is able to execute multiple commands on one line separated by semicolons.
    - Is able to execute single pipe like ls | wc.
    - Redirect is somewhat functional.

## Attempted
    - Tried redirecting w command.
    - Attempted multiple redirection in one command line.

## Headers
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
      (with executeOne) after backingup with dup and redirecting with dup2.
      - does not return anything

    - void redirectless (char * line)
      - same as redirectgreater but is separated by "<"
      - redirects stdin from a file

    - void mypipe (char * line)
      - takes the command line as a parameter
      - does not return anything
      - reads the input from the first paramter (before the |) and executes it in the second (after |)

     - void double1 (char * line)  -unsucessful-
      - this is for when the command line contains a < followed by a >.
      - first redirectgreater is runned but instead of execvping the commandline, redirectless is runned.
