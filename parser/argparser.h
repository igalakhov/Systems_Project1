#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *   get_current_command   (char * , int *);
void     clear_multiple_spaces (char *        );
void     trimstr               (char *        );
char **  split_line            (char * , char );
char *** build_command_array   (char *        );
void     free_command_array    (char ***      );
