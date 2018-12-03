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
  // set sighandler
  signal(SIGINT, sig_handler);

  printf("Hello! Welcome to the shell!\n\n");

  int max_command_size = 1;
  char * command = malloc(max_command_size * sizeof(char));
  char *** parsed_arr;

  while(1){

    new_line();

    // get latest command
    command = get_current_command(command, &max_command_size);

    // validate that input is valid


    // clear spaces and trim
    clear_multiple_spaces(command);
    trimstr(command);

    //printf("\n[%s]\n", command);

    char *** temp;

    temp = build_command_array(command);


    execute_command_array(temp);

    // build array of command strings
    // parsed_arr = parse_commands(command);
    //
    // // execute the commands
    // execute_commands(parsed_arr);
    //
    // // no leaks
    // free_command_array(&parsed_arr);

    //break;
    printf("\n");
    //return 0;
  }

  return 0; // done
}
