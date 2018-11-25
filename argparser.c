#include "argparser.h"


// frees a command array
void free_command_array(char **** cmd_arr){
  char *** cmd = *cmd_arr;
  char *** next_cmd;

  // free commands
  while(*cmd){
    next_cmd = cmd + 1;
    free(*cmd);
    cmd = next_cmd;
  }

  // free the last one
  free(*cmd_arr);
}

// parses args out of a command and returns the array
char ** parse_args(char * line){
  int num_args = 1;
  char * i = line;
  while(*i){
    num_args++;
    i++;
  }

  char ** args = calloc(num_args + 1, sizeof(line)); // maximum number of args is 5, with maximum size of line
                                          // also one at the end for the NULL
  char * line_copy = malloc(sizeof(line));
  strcpy(line_copy, line);

  int c = 0;
  while((args[c] = strsep(&line_copy, " ")) && ++c);

  return args;
}

// parses commands out of a string and returns a pointer to a null terminated array with it
char *** parse_commands(char * cmd){
  //printf("[%s]\n", cmd);

  // count the number of ';'. The largest number of commands we are given is that number plus 1
  int num_commands = 1;
  char * p = cmd;
  while(*p){
    if(*p == ';')
      num_commands++;
    p++;
  }

  //printf("Num commands: %d\n", num_commands);

  // this is what we're returning
  char *** parsed_commands = calloc(sizeof(char ***), (num_commands + 1));

  // split the string by ';'
  char * cmd_cpy = malloc(strlen(cmd) + 1);
  strcpy(cmd_cpy, cmd);

  char * cur;
  char * curcmd;
  int i = 0;

  for(cur = strtok(cmd_cpy, ";"); cur != NULL; cur = strtok(NULL, ";")){
    // current command
    curcmd = malloc(sizeof(char) * (strlen(cur) + 1));
    strcpy(curcmd, cur);
    trimstr(curcmd);
    parsed_commands[i] = parse_args(curcmd);

    // only increment if we wrote something
    if(parsed_commands[i][0][0])
      i++;
  }

  //printf("%s\n", cmd_cpy);



  return parsed_commands;
}


// strips spaces from end and start of string (in place)
void trimstr(char * str){
  char *dst = str, *src = str;
  char *end;

  while (*src == ' ' && ++src);
  end = src + strlen(src) - 1;
  while (end > src && *end == ' '){
     *end-- = 0;
  }
  if (src != dst){
     while ((*dst++ = *src++));
  }
}
// reads the latest command from the user and writes it to command_array
// adds more space to it when neccesarry
char * get_current_command(char * command_array, int * max_command_size){
  int inc = 0;
  char cur;
  while((cur = getchar()) != '\n'){
    if(inc >= *max_command_size - 1){
      command_array = realloc(command_array, *max_command_size * 2);
      *max_command_size *= 2;
    }
    // set value
    *(command_array + inc) = cur;
    inc++;
  }
  // null terminate the string
  *(command_array + inc) = 0;

  return command_array;
}

// clears all multiple spaces from given string
// does all operations in that string, since the new space will always be smaller
void clear_multiple_spaces(char * command){
  char * temp = malloc(sizeof(char) * (strlen(command) + 1));
  strcpy(temp, command);
  memset(command, 0, strlen(command));

  int i = 0;
  int j = 0;
  char cur;
  int inspace = 0;
  while((cur = temp[i])){
    //printf("%c, %d, %d\n", cur, i, inspace);
    if(cur == ' ' && !inspace && i != 0){
      inspace = 1;
      command[j] = cur;
      j++;
    } else if (cur != ' '){
      inspace = 0;
      command[j] = cur;
      j++;
    } else {
    }
    i++;
  }

  // no memory leaks
  free(temp);
  //printf("%s\n", command);
}
