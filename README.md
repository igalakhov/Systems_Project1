# Shell
Written by Ivan Galakhov, pd 5

> "It's nice" - Sam Rosenstrauch

> "Unapologetically functional" - Gilvir Gill

## Features
* Forks child processes and runs commands
* Separates multiple commands through ";". Ignores empty commands as well.
* Fun colors!

## Attempted
* Nothing so far, but I feel like there will be more once I try doing pipes and redirects

## Bugs
* Sometimes, running a command causes a "pointer being freed was not allocated" error. I have only gotten this once or twice and I don't really know how to fix it or where it comes from.

## Files && Function headers
argparser.c:

    Handles parsing and reading user input


    /*======== char * get_current_command ==========*/

    Inputs: char * command_array,
            int * max_command_size

    Returns: char * to the latest user command

    Writes the current input of the user (without a newline) to command_array. If the size length of user input is greater than max_command_size, it will reallocate the memory, and update max_command_size.

    
