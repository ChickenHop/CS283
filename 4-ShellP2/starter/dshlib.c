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
int parse(char *line, cmd_buff_t *buff) {
   // printf("enter parse\n");
    char temp[ARG_MAX];
    
    while (*line == SPACE_CHAR) {
        line++;  
    }
    
    char *end = line + strlen(line) - 1;
    while (end > line && *end == SPACE_CHAR) {
        end--;
    }
    end[1] = '\0';
    memset(buff, 0, sizeof(cmd_buff_t)); 

    char *ptr1;
    char *cmd = strtok_r(line, SPACE_STRING, &ptr1);
    

    if (strlen(cmd) > EXE_MAX) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    strcpy(buff->cmd, cmd);
   // printf("command:%s\n",buff->cmd);
   
    int i = 0;
    if (strcmp(cmd,"echo")==0){
        cmd = strtok_r(NULL, "\"", &ptr1);
        strcpy(buff->argv[i], cmd);
        return OK;
    }
    cmd = strtok_r(NULL, SPACE_STRING, &ptr1);
  //  printf("next:%s\n",cmd);
    while (cmd != NULL) {
        strcpy(buff->argv[i], cmd);
       //printf("arg:%s\n",buff->argv[i]);
        i++;
        cmd = strtok_r(NULL, SPACE_STRING, &ptr1);
    }
    buff->argc=i;
   // printf("%d\n",buff->argc);
   // printf("done\n");
    return OK;
}


int command_handler(cmd_buff_t *buff){
    if (strcmp(buff->cmd,"cd")==0){
        if (buff->argc==0){
            return -1; 
        }
        else if (buff->argc == 1) {
            chdir(buff->argv[0]);
            return 0;
        } 
        else {
            return -1; 
        }
    }
    else if (strcmp(buff->cmd,"pwd")==0){
        char cwd[ARG_MAX];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
    else if (strcmp(buff->cmd,"dragon")==0){
        print_dragon();
    }
    else if (strcmp(buff->cmd,"echo")==0){
        printf("%s\n",buff->argv[0]);
    }
}

int exec_local_cmd_loop(){
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    cmd_buff_t cmd;
    int exit;
    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            return OK;
            break;
        }
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';

        if (strcmp(cmd_buff,EXIT_CMD)==0){ 
            rc = OK;
            return rc;
        }
        if (cmd_buff == NULL || strlen(cmd_buff)==0) {
            printf("%s\n", CMD_WARN_NO_CMD);
            rc = WARN_NO_CMDS;
            return rc;
        }
        if (strlen(cmd_buff) >= SH_CMD_MAX) {
            rc=ERR_CMD_OR_ARGS_TOO_BIG;
        }
        int result = parse(cmd_buff,&cmd);
        switch (result) {
            case ERR_CMD_OR_ARGS_TOO_BIG:
                rc = ERR_CMD_OR_ARGS_TOO_BIG;
                return rc;
            case OK:
                exit = command_handler(&cmd);

        }
    }
    
}
    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

