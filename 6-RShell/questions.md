1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

It can be determinete by EOF, \n or \0 terminators. Timeouts can be used to handle partial reads and avoid looping. Checksums can be used to ensure complete message transmisson.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

It should use lenght prefixing to know how many bytes it needs to read. If not handled correctly there may be partial reads in data and resource leaks.

3. Describe the general differences between stateful and stateless protocols.

Stateful maintains session info while stateles does not. But stateless uses less resources then statful. It is also easier to handle failures with stateless then statefil.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UUP is simple and faster then other options. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The interface is sockets to allow for network comunication.