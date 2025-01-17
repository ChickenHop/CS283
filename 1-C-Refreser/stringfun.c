#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    int i = 0;  //number of charaters in buffer
    int j = 0;  //number of charater copied form the user_str
    int space=0; // 0 means that there was no prevoius space 1 means that there was a prevoius space
    char *temp = user_str; 
    int user_len = 0;
    //gets the lenght of user string
    while (*temp != '\0'){
        user_len++;
        temp++;
    }
    if (user_len > len){
         printf("String supplied is too long.");
        exit(-1);
    }
    while (*user_str != '\0') { 
        if (*user_str == ' ') {
            // checks to make sure that there was no space before.
            if (!space) { 
                *buff = ' ';
                space = 1;
                i++;
                j++;
                buff++;        
            }
        }
        // checks to see if there is a tab or new line and then convert it into a space
        else if (*user_str == '\t' || *user_str == '\n'){
            *buff = ' ';
            space = 1;
            i++;
            j++;
            buff++;
           
        }
        // adds every other charater to the buffer string
        else {
            *buff = *user_str;
            space = 0;
            i++;
            j++;
            buff++;
           
        }
        user_str++;
    }
    // adds to . to pad out buffer
    while (i < len){
        *buff= '.';
        buff++;
        i++;
    }
    return j;        
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count=0;

    for (int i = 0; i < len; i++){
        // when sees space add 1 to word count
        if(*buff == ' '){
            word_count++;
        }
        // when sees dot add 1 to word count then break loop
        if (*buff == '.'){
            word_count++;
            return word_count;
        }
        buff++;
    }
    return word_count;
}

void count_chars(char *buff, int len, int str_len) {
    int i;
    int char_count = 0;
    int word_count = 1;
    int new_word = 1; // if new word equal 1 if not equals 0

    for (i = 0; i < len; i++) {
        // make a new line for each word
        if (new_word) {
            printf("\n"); // put here to fix bug where after the frist word line all the others were offset by 1 space
            printf("%d. ", word_count);
            new_word = 0;
            char_count = 0;
        }
        // when sees space add 1 to word count then break loop
        if (*buff == ' ') {
            printf(" (%d)", char_count);
            word_count++;
            new_word = 1;
        // when sees dot add 1 to word count then break loop
        } else if (*buff == '.') { 
            printf(" (%d)", char_count);
            break; 
        }

        char_count++;
        putchar(*(buff));
        buff++;
    }
    printf("\n");
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      It is safe because we frist check if there is 2 strings when the commandline is used. The frist string is the actual command and the second the string so 2. 
    //      Since this is checked first if there is not 2 or higher for argc aka only the command then the checking of argv[1] doesn't happen.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      The purpose of this if statement is to check if there are 4 strings inputed. Since the command only work with 2 or 3 there should not be a 4th used. 
    //      So to make sure that only 2 or 3 are used we exit the command to avoid any uninted bugs if 4 strings are inputed.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Memory allocation failed.\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
//TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        case 'r':
            printf("Reversed string: ");
            // uses the user str lenght as the index and increments down
            for (int i= user_str_len-1; i >=0; i--){
                putchar(*(buff+i));
            }
            putchar('\n');
            break;
        case 'w':
            printf("Word Print\n");
            printf("----------");
            count_chars(buff,BUFFER_SZ,user_str_len);
            break;
            

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//          
//          It allows for flexibilty so if we want to change the buffer to a different number
//          it will not cause unrelated bugs and we won't need to change any other part of the code.
//          It is also more readable as a dev may not remeber why there is a random number being used in the helper function but will know if it is labled.
