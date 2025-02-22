1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  The fok allows us a way to make new process, manage resoaues, process isolation, and a find contrl over exection.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If a fork fails it returns -1 to the parent and makes no child. My emplimation prints an error message and error code.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It looks through dirtoites. Looking for the enviroment varible called PATH>

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: Wait block the paremnt process until it's child is executed. This is to avoid any possible errors that could occur if the process happen at the same time or if something in the parent reliea on something from the child.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It gets the exit code of the child process. This is immporant for error handling and letting the parent know to go if ecit 0.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  In my code the I get the frist " after the command and then copy that string until the closeing ". I check this frist to perseve any white space between the qoutes and so the items inside of it aren't treated as sepearte arguments.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  I had to add the qoute checking and remove the pipe parsing. It was changling having to change the way it was written due to the differnce in the structs.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signels are used for communiation in the operating system so it knows when certian events have happened. They are different as they are for controling processes while other IPC deal more with data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their t ypical use cases?

    > **Answer**:  SIGKILL terminates a process with no way of the process being albe to stop it. It is used when a process becomes unresponsive or is going infinite.
    SIGTERM ask the process the terminate. This is so that it can be able to clean up what it was doing before shutdown.
    SIGINT is when a user interrupts a process. This allows for the user to stop a process manually.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  The process is immediatlely frozen when it recieves SIGSSTOP. It can't be ignored or caught so the user is able to control the system better and as a way to stop malicious processes from being unstoppable.