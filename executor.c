#include "executor.h"

void execute_commands(char *** commands){
    //int errno;
    int status;
    // run through commands, forking off child processes
    char *** i = commands;
    char ** cur_command;

    while((cur_command = *i)){
      // special cases
      if(!strcmp(cur_command[0], "exit")){
        printf("\n\x1B[31m[shell terminated]\x1B[0m\n\n");
        exit(0);
      } else if(!strcmp(cur_command[0], "cd")){
        if(chdir(cur_command[1]) == -1){
          // error
          printf("\x1B[31m%s\x1B[31m\n", strerror(errno));
        }
      } else {
        // fork and do the command
        if(!fork()){
          execvp(cur_command[0], cur_command);
          exit(errno); // error if it exists
        }
        wait(&status); // wait for child process to finish
        // print error
        if (WEXITSTATUS(status))
          printf("\x1B[31m%s\x1B[31m\n", strerror(WEXITSTATUS(status)));
      }

      i++;
    }

}
