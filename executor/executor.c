#include "executor.h"
#include "../parser/argparser.h"

#define READ_END 0
#define WRITE_END 1

void execute_command_array(char *** command_array){
  int command_num = 0;
  int pipe_num, arg_num;
  int child_status;

  int current_pipe[2];
  int input_fd, file_fd;
  int stdout_temp, stdin_temp, stderr_temp;
  char ** args;

  // turn off signal handler
  signal(SIGINT, dummy_signal_handler);

  while(command_array[command_num]){ // go through each command
    pipe_num = 0;

    input_fd = STDIN_FILENO;
    while(command_array[command_num][pipe_num]){
      //printf("HUH?\n");
      pipe(current_pipe); // make pipe

      // make copies of everything
      stdout_temp = dup(STDOUT_FILENO);
      stdin_temp = dup(STDIN_FILENO);
      stderr_temp = dup(STDERR_FILENO);

      // file that we are writing to/from
      file_fd = 0;

      args = split_line(command_array[command_num][pipe_num], ' ');
      arg_num = 0;
      while(args[arg_num]){
        // first arg breaks everything
        if(arg_num){
          if(!strcmp(args[arg_num-1], ">")) setup_redirect(0, 0, args[arg_num], &file_fd);
          if(!strcmp(args[arg_num-1], "<")) setup_redirect(1, 2, args[arg_num], &file_fd);
          if(!strcmp(args[arg_num-1], ">>")) setup_redirect(0, 1, args[arg_num], &file_fd);
          if(!strcmp(args[arg_num-1], "2>")) setup_redirect(2, 0, args[arg_num], &file_fd);
          if(!strcmp(args[arg_num-1], "2>>")) setup_redirect(2, 1, args[arg_num], &file_fd);
        }

        // cut off command
        if(file_fd)
          args[arg_num-1] = NULL;


        //printf("\n[%s]\n", args[arg_num]);
        arg_num++;
      }

      if(!strcmp(args[0], "exit")){
        printf("\n\x1B[31m[shell terminated]\x1B[0m\n\n");
        exit(0);
      } else if(!strcmp(args[0], "cd")){
        if(chdir(args[1]) == -1){
          // error
          printf("\x1B[31m%s\x1B[31m\n", strerror(errno));
        }
      } else {
        // fork off
        if(!fork()){
          close(current_pipe[READ_END]);

          if(command_array[command_num][pipe_num + 1])
            dup2(current_pipe[WRITE_END], STDOUT_FILENO);

          if(!file_fd)
            dup2(input_fd, STDIN_FILENO);

          execvp(args[0], args);
          exit(errno);
        }
        close(current_pipe[WRITE_END]);

        wait(&child_status); // wait for child process to finish
        // print error
        if (WEXITSTATUS(child_status))
          printf("\x1B[31m%s\x1B[31m\n", strerror(WEXITSTATUS(child_status)));

        close(current_pipe[WRITE_END]);
        input_fd = current_pipe[READ_END];
      }
      dup2(stdout_temp, STDOUT_FILENO);
      dup2(stdin_temp, STDIN_FILENO);
      dup2(stderr_temp, STDERR_FILENO);

      pipe_num++;
    }

    // fix the file table and free up args
    int i;

    for(i = 0; args[i]; i++)
      free(args[i]);

    free(args);

    for(i = 3; i < getdtablesize(); i++)
      close(i);

    command_num++;
  }
}
void setup_redirect(int what, int how, char * name, int * where){
  int creation_possibilities[3] = {O_CREAT | O_WRONLY,
                            O_CREAT | O_WRONLY | O_APPEND,
                            O_CREAT | O_RDONLY};

  int redirection_possibilities[3] = {STDOUT_FILENO,
                               STDIN_FILENO,
                               STDERR_FILENO};
  *where = open(name, creation_possibilities[how], 0777);
  dup2(*where, redirection_possibilities[what]);
}

void dummy_signal_handler(int n){}
