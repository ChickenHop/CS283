#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"
/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist) {
   //printf("Function entered \n");
    if (cmd_line == NULL || strlen(cmd_line)==0) {
     //printf("Error: cmd_line is NULL\n");
        return WARN_NO_CMDS;
    }

    if (strlen(cmd_line) >= SH_CMD_MAX) {
        //printf("Error: cmd_line exceeds maximum allowed length\n");
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    memset(clist, 0, sizeof(command_list_t));
    //printf("Initialized command list\n");
    char *ptr1, *ptr2;  
    char *cmd = strtok_r(cmd_line, PIPE_STRING, &ptr1);
    int cmd_cnt = 0;

    while (cmd != NULL) {
        //printf("Processing command %d: %s\n", cmd_cnt + 1, cmd);
        if (cmd_cnt == CMD_MAX) {
            //printf("Error: Too many commands\n");
            return ERR_TOO_MANY_COMMANDS;
        }

        while (*cmd == SPACE_CHAR) {
            cmd++;  
        }

        char *end = cmd + strlen(cmd) - 1;
        while (end > cmd && *end == SPACE_CHAR) {
            end--;
        }
        end[1] = '\0';

       //printf("Trimmed command: '%s'\n", cmd);

        char *token = strtok_r(cmd, SPACE_STRING, &ptr2);
       //printf("Executable: %s\n", token);

        if (strlen(token) > EXE_MAX){
            //printf("Error: Command or arguments too big\n");
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strcpy(clist->commands[cmd_cnt].exe, token);
        //strcpy(clist->commands[cmd_cnt].args, "GARBAGE");
        while ((token = strtok_r(NULL, SPACE_STRING, &ptr2)) != NULL) {
            if (strlen(clist->commands[cmd_cnt].args) + strlen(token) + 1 > ARG_MAX) {
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            //printf("Argument: %s\n", token);
            strcat(clist->commands[cmd_cnt].args, token);
            strcat(clist->commands[cmd_cnt].args, " ");
        }
        
        cmd_cnt++;
        cmd = strtok_r(NULL, PIPE_STRING, &ptr1);
        
    }

    clist->num = cmd_cnt;
    //printf("Number of commands parsed: %d\n", cmd_cnt);
    return OK;
}
