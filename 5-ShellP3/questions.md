1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

It calles waitpid() for the parent which will wait for all it's childern to finish running beofre it starts. If waitpid() is not called then there could be inncorect exacution in the shell and could lead to leaks.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is nessary to insure correct behvaior of the shell and pipes are not infinit so failing to close them will cause issues such as having a process fun infinalty or cauing a leak in resources.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

This is done so the parent shell that is running has the directory changed in it. If cd was external the change would not effecting the parent shell.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

The limit would have the be removed and memory be alloacted dynaimcally based on the number of pipes being used. This would make memory mamegmant harder as the we would need to make sure memory is freed as much as was alloacted. 
