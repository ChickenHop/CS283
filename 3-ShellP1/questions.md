1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() allows us to set a maximum size for an input string avoiding over flows and it also reads space charaters.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Malloc allows for easier rezing based on the strong using realloc compared to allocating it with a fixed size. It is more dyanmic.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  We trim the trailing and leading spaces to avoid possible errors that may arise when parsing the string into arguments.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  there is > which redircts the stdout and adds it so a file. This would be challgning as we would have to open the file and empty it if it has contents then add it then close the file. >> is the same but it cats the output to the end of the file which would be esaier then > but still requires to open the file. < redircts a file as the sdtin. This would be a challange as we have to treat the file text as we have to open and close the file while also reading line by line to then feed that to the command. All of these also have to have error checks if the fail does not open.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection deals with files while piping is for commands. You pipe the output of a command as input to another. You redirect files to commands.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: It is immportant to keep them separate as STDERR is used to debug and woundn't be seen by regular users while STDOUT is what regular users will see and is what are supposed to be shown to users.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  We could have STDERR & STDOUT as a file that gets over written over after every functions and printed when called. This way commands can have seprate STDOUT AND STDERR. If we want to merge tehm have another file serve as the place where the merge is stored.