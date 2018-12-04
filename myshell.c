#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include "parser/argparser.h"
#include "executor/executor.h"

void new_line(){
  char working_dir[PATH_MAX];
  getcwd(working_dir, sizeof(working_dir));
  printf("\x1B[35m%s\x1B[36m |||\x1B[0m ", working_dir);
  // prints everything out
  fflush(stdout);
}

static void sig_handler(int num){
  if(num == SIGINT){
    printf("\n");
    new_line();
    //printf("\n");
  }
}

int main(){

  printf("Hello! Welcome to the shell!\n\n");

  int max_command_size = 1;
  char * command = malloc(max_command_size * sizeof(char));
  char *** parsed_arr;

  while(1){
    // set sighandler because executing the command array disables it
    signal(SIGINT, sig_handler);

    new_line();

    // get latest command
    command = get_current_command(command, &max_command_size);

    // validate that input is valid

    // clear spaces and trim
    clear_multiple_spaces(command);
    trimstr(command);

    parsed_arr = build_command_array(command);

    execute_command_array(parsed_arr);

    // no leaks
    free_command_array(parsed_arr);
  }

  return 0; // never happens
}
