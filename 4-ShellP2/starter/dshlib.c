#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"
#include "dragon.h"
/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX * sizeof(char));
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    cmd_buff->argc = 0;
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);
    strncpy(cmd_buff->_cmd_buffer, cmd_line, SH_CMD_MAX - 1);
    cmd_buff->_cmd_buffer[SH_CMD_MAX - 1] = '\0';

    char *token = cmd_buff->_cmd_buffer;
    int argc = 0;

    while (*token != '\0') {
        // Skip leading spaces
        while (isspace((unsigned char)*token)) {
            token++;
        }
        if (*token == '\0') {
            break;
        }

        char *arg_start;
        if (*token == '"') { 
            token++; 
            arg_start = token;
            while (*token != '"' && *token != '\0') {
                token++;
            }
            if (*token == '"') {
                *token = '\0'; 
                token++;      
            }
        } else {  
            arg_start = token;
            while (*token != '\0' && !isspace((unsigned char)*token)) {
                token++;
            }
            if (*token != '\0') {
                *token = '\0'; 
                token++;
            }
        }

        cmd_buff->argv[argc++] = arg_start;

        if (argc >= CMD_ARGV_MAX - 1) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }

    cmd_buff->argv[argc] = NULL;
    cmd_buff->argc = argc;

    return OK;
}


Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    } else if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    } else if (strcmp(input, "dragon") == 0) {
        return BI_CMD_DRAGON;
    }
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd, Built_In_Cmds command) {
    switch (command) {
        case BI_CMD_EXIT:
            exit(OK);
        case BI_CMD_CD:
            if (cmd->argc > 1) {
                if (chdir(cmd->argv[1]) != 0) {
                    fprintf(stderr, "cd: %s: No such file or directory\n", cmd->argv[1]);
                }
            }
            return BI_EXECUTED;
        case BI_CMD_DRAGON:
            print_dragon();
            return BI_CMD_DRAGON;
        default:
            return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp failed");
        exit(ERR_EXEC_CMD);
    } else if (pid > 0) {
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return ERR_EXEC_CMD;
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return ERR_EXEC_CMD;
    } else {
        perror("fork failed");
        return ERR_EXEC_CMD;
    }
}

int exec_local_cmd_loop() {
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    if (cmd_buff == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return ERR_MEMORY;
    }

    int rc;
    cmd_buff_t cmd;
    Built_In_Cmds bi;
    if (alloc_cmd_buff(&cmd) == ERR_MEMORY) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(cmd_buff);
        return ERR_MEMORY;
    }

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
        if (strlen(cmd_buff) == 0) {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        }
        if (strlen(cmd_buff) >= SH_CMD_MAX) {
            printf("%s", CMD_ERR_PIPE_LIMIT);
            continue;
        }

        rc = build_cmd_buff(cmd_buff, &cmd);
        switch (rc) {
            case ERR_CMD_OR_ARGS_TOO_BIG:
                fprintf(stderr, "Too many arguments\n");
                break;
            case OK:
                bi = match_command(cmd.argv[0]);
                if (bi != BI_NOT_BI) {
                    exec_built_in_cmd(&cmd, bi);
                } else {
                    exec_cmd(&cmd);
                }
                break;
            default:
                break;
        }
    }

    free_cmd_buff(&cmd);
    free(cmd_buff);
    return rc;
}