#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void execute_command_array (char ***                     );
void setup_redirect        (int      , int, char *, int *);
void dummy_signal_handler  (int                          );
