#include "argparser.h"

void free_command_array(char *** command_array){
  int i, j;

  for(i = 0; command_array[i]; i++){
    for(j = 0; command_array[i][j]; j++){
      free(command_array[i][j]);
    }
    free(command_array[i]);
  }
  free(command_array);
}

char *** build_command_array(char * command){

  int num_args;
  char *** to_return;
  char ** temp_commands = split_line(command, ';');

  // calculate the real number of arguments
  num_args = 0;
  int j = 0;
  while(temp_commands[j]){
    num_args++;
    j++;
  }

  to_return = calloc(num_args + 1, sizeof(char **));

  j = 0;
  // split pipes and write them
  while(j < num_args){
    to_return[j] = split_line(temp_commands[j], '|');
    j++;
  }
  // free temp_commands
  j = 0;
  while(temp_commands[j]){
    free(temp_commands[j]);
    j++;
  }

  free(temp_commands);

  return to_return;
}

// split line by a character
char ** split_line(char * line, char separate_on){
  // count the number of arguments (starting with 1)
  int num_args = 1;
  char * i = line;
  while(*i){
    if(*i == separate_on)
      num_args++;
    i++;
  }

  char ** ret = calloc(sizeof(char **), (num_args + 1));

  char * line_cpy = malloc(strlen(line) + 1);
  strcpy(line_cpy, line);

  char * cur;
  char * curpart;
  int j = 0;

  // turn character into string for strtok
  char splitter[2];
  splitter[0] = separate_on;
  splitter[1] = 0;

  for(cur = strtok(line_cpy, splitter); cur != NULL; cur = strtok(NULL, splitter)){
    // current command
    curpart = malloc(sizeof(char) * (strlen(cur) + 1));
    strcpy(curpart, cur);
    trimstr(curpart);
    ret[j] = curpart;

    // only increment if we wrote something
    if(ret[j][0])
      j++;
  }

  return ret;
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
