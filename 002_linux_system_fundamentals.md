# Linux Process Management
## A Deep Dive into Process Creation, Management, and Communication

### Understanding Processes in Linux

A process in Linux is an instance of a running program. When you execute a program, the kernel creates a process by allocating memory, loading the program code, and setting up the execution environment. Each process has its own virtual address space, ensuring isolation from other processes.

The kernel maintains a process table containing process control blocks (PCB) for each active process. This PCB contains critical information including process ID (PID), parent process ID (PPID), user ID (UID), current state, memory pointers, open file descriptors, and scheduling information.

### Process States and Lifecycle

A Linux process transitions through several states during its lifetime. When first created, a process enters the **Running** state, meaning it's either executing on a CPU or ready to execute. When waiting for I/O operations or other resources, it moves to the **Waiting** (or Sleeping) state. The **Stopped** state occurs when the process receives signals like SIGSTOP or SIGTSTP, typically from job control. Finally, processes enter the **Zombie** state after termination but before the parent has collected their exit status.

Understanding these states is crucial for debugging. A zombie process indicates the parent hasn't called wait() to reap the child's exit status. An excessive number of processes in the Waiting state might indicate I/O bottlenecks. Stopped processes accumulate when applications don't properly handle job control signals.

### Process Creation with fork()

The fork() system call is fundamental to Unix process creation. It creates a new process by duplicating the calling process. The new process, called the child, is an almost exact copy of the parent process.

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    int shared_variable = 100;
    
    printf("Before fork: PID = %d\n", getpid());
    
    pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        return 1;
    } 
    else if (pid == 0) {
        // Child process
        shared_variable += 50;
        printf("Child: PID = %d, PPID = %d, var = %d\n", 
               getpid(), getppid(), shared_variable);
    } 
    else {
        // Parent process
        shared_variable += 25;
        printf("Parent: PID = %d, Child PID = %d, var = %d\n", 
               getpid(), pid, shared_variable);
    }
    
    return 0;
}
```

After fork(), both processes execute from the same point with separate memory spaces. The child receives a copy of the parent's memory, including the stack, heap, and data segments. This copy-on-write mechanism means pages are initially shared and only copied when modified, optimizing memory usage.

The return value distinguishes parent from child. The parent receives the child's PID, enabling it to track and control the child. The child receives zero, identifying itself. A negative return value indicates failure, typically due to resource exhaustion.

### The exec() Family

While fork() creates a process copy, exec() replaces the current process image with a new program. This combination of fork() followed by exec() is the standard Unix pattern for launching new programs.

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process executes 'ls -l'
        char *args[] = {"ls", "-l", "/tmp", NULL};
        execvp("ls", args);
        
        // If exec succeeds, we never reach here
        perror("execvp failed");
        return 1;
    } else {
        // Parent waits for child
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
    }
    
    return 0;
}
```

The exec family includes several variants: execl(), execle(), execlp(), execv(), execve(), and execvp(). The differences lie in how arguments are passed (list vs array), whether the PATH is searched, and whether environment variables are explicitly provided. The execve() system call is the actual system call; others are library wrappers providing convenient interfaces.

When exec() succeeds, the process ID remains unchanged, but the program code, data, heap, and stack are replaced. Open file descriptors typically remain open unless marked with the close-on-exec flag (FD_CLOEXEC). Signal dispositions are reset to default, but signal masks are preserved.

### Process Termination and Wait

Proper process termination involves cleanup and status communication to the parent. A process can terminate normally through exit() or return from main(), or abnormally through signals like SIGKILL or SIGSEGV.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void demonstrate_wait() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child does some work
        printf("Child working...\n");
        sleep(2);
        exit(42);  // Exit with status 42
    } else {
        // Parent waits for child
        int status;
        pid_t child_pid = wait(&status);
        
        printf("Child %d terminated\n", child_pid);
        
        if (WIFEXITED(status)) {
            printf("Normal exit, status = %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Killed by signal %d\n", WTERMSIG(status));
        }
    }
}

void demonstrate_waitpid() {
    pid_t pid1 = fork();
    if (pid1 == 0) {
        sleep(1);
        exit(1);
    }
    
    pid_t pid2 = fork();
    if (pid2 == 0) {
        sleep(2);
        exit(2);
    }
    
    // Wait for specific child
    int status;
    waitpid(pid2, &status, 0);  // Wait for second child specifically
    printf("Child %d finished first\n", pid2);
    
    // Wait for any remaining child with WNOHANG (non-blocking)
    while ((waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Another child finished\n");
    }
}
```

The wait() system call blocks until any child terminates, returning the child's PID and storing its exit status. The waitpid() function offers more control, allowing you to wait for a specific child, check status without blocking (WNOHANG), or wait for any child in a specific process group.

The status integer encodes both exit status and termination reason. The WIFEXITED macro checks for normal termination, while WEXITSTATUS extracts the exit code. WIFSIGNALED indicates signal termination, with WTERMSIG providing the signal number. WIFSTOPPED and WSTOPSIG handle stopped processes when using WUNTRACED.

Zombie processes occur when a child terminates but the parent hasn't called wait(). The kernel keeps minimal information (PID, termination status) to report to the parent. These zombies accumulate if the parent never calls wait(), eventually exhausting the process table. Proper cleanup requires either waiting for all children or setting up signal handlers for SIGCHLD.

### Signals and Signal Handling

Signals are software interrupts providing a way to handle asynchronous events. They can originate from hardware exceptions (SIGSEGV, SIGFPE), user actions (SIGINT from Ctrl+C), or software conditions (SIGPIPE, SIGCHLD).

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t keep_running = 1;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nReceived SIGINT, shutting down gracefully...\n");
        keep_running = 0;
    } else if (signum == SIGTERM) {
        printf("Received SIGTERM\n");
        keep_running = 0;
    }
}

void setup_signal_handling() {
    struct sigaction sa;
    
    // Initialize sigaction structure
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  // Restart interrupted system calls
    
    // Register signal handlers
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    
    // Block SIGUSR1 temporarily
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &block_mask, NULL);
}

int main() {
    setup_signal_handling();
    
    printf("Process running with PID %d\n", getpid());
    printf("Press Ctrl+C to stop gracefully\n");
    
    while (keep_running) {
        // Simulate work
        printf("Working...\n");
        sleep(1);
    }
    
    printf("Cleanup completed, exiting\n");
    return 0;
}
```

Signal handlers execute asynchronously, interrupting normal program flow. This asynchrony requires careful handling. Only async-signal-safe functions should be called within handlers. These include write(), _exit(), and signal manipulation functions, but exclude printf(), malloc(), and most library functions.

The SA_RESTART flag automatically restarts system calls interrupted by signals, crucial for network programming where read() or write() might be interrupted. Without this flag, such calls return EINTR, requiring explicit retry logic.

Signal masks control which signals are blocked. The sigprocmask() function modifies the process signal mask, while pthread_sigmask() modifies thread-specific masks in multithreaded programs. Blocked signals remain pending until unblocked, except for SIGKILL and SIGSTOP which cannot be blocked.

### Practical Considerations for Routing Daemons

Network routing daemons face unique process management challenges. They typically run as long-lived background processes (daemons), requiring robust signal handling for graceful shutdown and configuration reload.

A routing daemon should catch SIGTERM and SIGINT for graceful shutdown, closing protocol sessions properly and saving state. SIGHUP traditionally triggers configuration reload. SIGUSR1 and SIGUSR2 can trigger diagnostic dumps or logging level changes.

Fork safety is critical when spawning helper processes. After fork() but before exec(), only async-signal-safe functions are safe to call. This affects logging, error handling, and any cleanup operations in the child process before exec().

Process resource limits matter for routing daemons handling thousands of neighbors. The number of open file descriptors (RLIMIT_NOFILE) must accommodate all protocol sessions, timers, and IPC mechanisms. Address space limits (RLIMIT_AS) affect the routing table size and neighbor state.

Daemon processes typically double-fork to detach from the controlling terminal, change to the root directory to avoid holding mount points, close inherited file descriptors, and redirect stdin/stdout/stderr to /dev/null or log files. Session leadership and process groups affect signal delivery and job control, critical for proper daemon behavior.

### Debugging Process Issues

Common process-related problems include zombie accumulation, orphaned processes, fork bombs, and resource exhaustion. Understanding /proc filesystem entries helps diagnose these issues.

The /proc/[pid]/status file shows process state, memory usage, and resource limits. The /proc/[pid]/fd directory lists all open file descriptors, crucial for finding descriptor leaks. The /proc/[pid]/maps file displays memory mappings, helpful for analyzing memory usage patterns.

Tools like ps, top, and htop provide process overview information. The pstree command visualizes process relationships. For detailed analysis, strace traces system calls, revealing fork(), exec(), and wait() patterns. The lsof command lists open files and sockets, essential for debugging network daemons.

When debugging crashes, core dumps preserve process state at termination. Enable them with `ulimit -c unlimited`. The gdb debugger can load core dumps, showing the call stack and variable values at crash time. For routing daemons, this reveals protocol state during failures.

### Performance Considerations

Process creation overhead matters for applications frequently spawning processes. Fork() followed by exec() is expensive, requiring page table copying and program loading. Alternatives include vfork() (now obsolete), posix_spawn(), or using threads instead of processes.

Copy-on-write optimization reduces fork() cost by sharing pages until modification. However, the parent's memory size directly impacts fork() performance. Large routing daemons with gigabyte-sized routing tables experience significant fork() latency.

For protocol implementations, the question becomes whether to use multiple processes or threads. Multiple processes provide strong isolation, preventing one protocol's crash from affecting others. However, inter-process communication adds complexity and overhead. Threads share address space, enabling efficient communication through shared memory but requiring careful synchronization.

Modern designs often use a hybrid approach: separate processes for management and data planes, with threads within each process for parallelism. This balances isolation, performance, and complexity.

# Linux Inter-Process Communication (IPC)
## Mechanisms for Process Collaboration and Data Exchange

### Introduction to IPC Mechanisms

Inter-process communication enables separate processes to exchange data and coordinate actions. Linux provides multiple IPC mechanisms, each suited to different use cases. Understanding their trade-offs helps you choose the right mechanism for your networking applications.

Pipes and FIFOs provide simple, stream-oriented communication. Message queues offer structured message passing with priority support. Shared memory delivers the highest performance by allowing direct memory access. Semaphores coordinate access to shared resources. Sockets enable both local and network communication with a consistent API.

### Pipes: The Simplest IPC

Pipes create a unidirectional data channel between related processes. They work like physical pipes, with data written to one end flowing to the other end. Pipes exist only in kernel space, with no filesystem presence.

```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void demonstrate_pipe() {
    int pipefd[2];  // pipefd[0] for reading, pipefd[1] for writing
    pid_t pid;
    char write_msg[] = "Hello from parent process";
    char read_msg[100];
    
    // Create pipe before fork
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }
    
    pid = fork();
    
    if (pid == 0) {
        // Child process: reads from pipe
        close(pipefd[1]);  // Close unused write end
        
        ssize_t bytes_read = read(pipefd[0], read_msg, sizeof(read_msg));
        if (bytes_read > 0) {
            read_msg[bytes_read] = '\0';
            printf("Child received: %s\n", read_msg);
        }
        
        close(pipefd[0]);
    } else {
        // Parent process: writes to pipe
        close(pipefd[0]);  // Close unused read end
        
        write(pipefd[1], write_msg, strlen(write_msg));
        printf("Parent sent: %s\n", write_msg);
        
        close(pipefd[1]);
        wait(NULL);  // Wait for child to finish
    }
}
```

The pipe() system call creates two file descriptors. Data written to pipefd[1] can be read from pipefd[0]. Pipes are typically used after fork(), with the parent and child closing their unused ends to establish clear communication direction.

Pipes have a limited capacity, typically 65,536 bytes on modern Linux systems. Writing to a full pipe blocks until space becomes available. Reading from an empty pipe blocks until data arrives. When all write ends close, reads return zero (EOF). When all read ends close, writes generate SIGPIPE.

For bidirectional communication, create two pipes. One carries parent-to-child messages, the other child-to-parent messages. This pattern appears frequently in implementing command pipelines, where each process filters or transforms data.

### Named Pipes (FIFOs)

Named pipes extend pipe functionality to unrelated processes by existing as filesystem entries. They behave like pipes but can be opened by name, enabling communication between any processes with appropriate permissions.

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FIFO_NAME "/tmp/my_protocol_fifo"

void fifo_writer() {
    int fd;
    char *message = "Protocol update: neighbor 192.168.1.1 is up";
    
    // Create FIFO if it doesn't exist
    mkfifo(FIFO_NAME, 0666);
    
    // Open FIFO for writing (blocks until reader opens)
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    write(fd, message, strlen(message) + 1);
    printf("Writer: Sent message\n");
    
    close(fd);
}

void fifo_reader() {
    int fd;
    char buffer[256];
    
    // Open FIFO for reading (blocks until writer opens)
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        printf("Reader: Received: %s\n", buffer);
    }
    
    close(fd);
    unlink(FIFO_NAME);  // Clean up
}
```

FIFOs support multiple writers and readers, though data can become interleaved. Opening a FIFO for reading blocks until a writer opens it, and vice versa. The O_NONBLOCK flag prevents this blocking behavior.

Named pipes excel at connecting CLI tools with long-running daemons. A routing daemon could expose protocol statistics through a FIFO. Command-line tools write queries to one FIFO and read responses from another, creating a simple request-response pattern.

### Message Queues: Structured Communication

Message queues provide structured communication with message boundaries and priorities. Unlike pipes, messages are discrete units, and receivers can selectively retrieve messages by type.

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

// Message structure
struct protocol_msg {
    long msg_type;  // Must be positive
    struct {
        int neighbor_id;
        int state;
        char ip_addr[16];
    } data;
};

void demonstrate_message_queue() {
    key_t key;
    int msgid;
    struct protocol_msg message;
    
    // Generate unique key
    key = ftok("/tmp", 'R');
    
    // Create message queue
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        return;
    }
    
    // Send message
    message.msg_type = 1;  // Message type for neighbor updates
    message.data.neighbor_id = 42;
    message.data.state = 2;  // State: UP
    strncpy(message.data.ip_addr, "192.168.1.1", sizeof(message.data.ip_addr));
    
    if (msgsnd(msgid, &message, sizeof(message.data), 0) == -1) {
        perror("msgsnd");
    } else {
        printf("Sent neighbor update\n");
    }
    
    // Receive message of specific type
    struct protocol_msg received;
    if (msgrcv(msgid, &received, sizeof(received.data), 1, 0) != -1) {
        printf("Received: Neighbor %d, State %d, IP %s\n",
               received.data.neighbor_id,
               received.data.state,
               received.data.ip_addr);
    }
    
    // Cleanup
    msgctl(msgid, IPC_RMID, NULL);
}
```

Message type enables selective message reception. A receiver can request messages of a specific type, the first message of any type, or messages within a type range. This flexibility supports implementing priority queues or routing different message types to different handlers.

Message queues persist until explicitly removed or system reboot, unlike pipes which disappear when the last process closes them. This persistence enables loose coupling, where senders and receivers need not run simultaneously.

The maximum message size (typically 8KB) and queue capacity (typically 16KB of messages) limit throughput. For high-volume communication, shared memory with careful synchronization outperforms message queues.

### Shared Memory: Maximum Performance

Shared memory allows multiple processes to access the same physical memory region, providing the fastest IPC mechanism. Without kernel intervention for data transfer, shared memory approaches memory bandwidth speeds.

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 1024

// Shared routing table structure
struct routing_table {
    int num_entries;
    struct {
        unsigned int prefix;
        unsigned int mask;
        unsigned int next_hop;
        int metric;
    } entries[100];
};

void demonstrate_shared_memory() {
    key_t key = ftok("/tmp", 'S');
    int shmid;
    struct routing_table *shared_rt;
    
    // Create shared memory segment
    shmid = shmget(key, sizeof(struct routing_table), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        return;
    }
    
    // Attach shared memory to process address space
    shared_rt = (struct routing_table *)shmat(shmid, NULL, 0);
    if (shared_rt == (void *)-1) {
        perror("shmat");
        return;
    }
    
    // Writer process
    pid_t pid = fork();
    if (pid == 0) {
        // Child writes to shared memory
        shared_rt->num_entries = 1;
        shared_rt->entries[0].prefix = 0xC0A80000;  // 192.168.0.0
        shared_rt->entries[0].mask = 0xFFFFFF00;     // /24
        shared_rt->entries[0].next_hop = 0xC0A80001; // 192.168.0.1
        shared_rt->entries[0].metric = 10;
        
        printf("Child: Written routing entry\n");
        shmdt(shared_rt);
        return;
    }
    
    // Parent reads from shared memory
    sleep(1);  // Ensure child writes first
    printf("Parent: Read %d routing entries\n", shared_rt->num_entries);
    printf("  Prefix: %x, NextHop: %x, Metric: %d\n",
           shared_rt->entries[0].prefix,
           shared_rt->entries[0].next_hop,
           shared_rt->entries[0].metric);
    
    // Cleanup
    shmdt(shared_rt);
    wait(NULL);
    shmctl(shmid, IPC_RMID, NULL);
}
```

Shared memory requires explicit synchronization. Multiple processes accessing shared memory concurrently without protection causes race conditions. Semaphores, mutexes, or atomic operations coordinate access.

Memory consistency matters in multiprocessor systems. Compiler optimizations and CPU reordering can make modifications invisible to other processors. Memory barriers ensure proper ordering. The volatile keyword prevents compiler optimization but doesn't guarantee memory ordering across processors.

For routing daemons, shared memory excels at maintaining large data structures like routing tables or neighbor databases accessed by multiple processes. A control plane process could update routes while a data plane process performs lookups, synchronized with read-write locks.

### Semaphores: Synchronization Primitives

Semaphores control access to shared resources, particularly shared memory. A semaphore maintains a counter, supporting atomic increment and decrement operations with blocking.

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

// Union required for semctl() on some systems
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Semaphore operations
void sem_wait(int semid) {
    struct sembuf op = {0, -1, 0};  // Decrement by 1
    semop(semid, &op, 1);
}

void sem_signal(int semid) {
    struct sembuf op = {0, 1, 0};   // Increment by 1
    semop(semid, &op, 1);
}

void demonstrate_semaphore_protection() {
    key_t key = ftok("/tmp", 'M');
    int semid, shmid;
    int *shared_counter;
    union semun arg;
    
    // Create semaphore
    semid = semget(key, 1, 0666 | IPC_CREAT);
    arg.val = 1;  // Initialize to 1 (binary semaphore/mutex)
    semctl(semid, 0, SETVAL, arg);
    
    // Create shared memory
    shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT);
    shared_counter = (int *)shmat(shmid, NULL, 0);
    *shared_counter = 0;
    
    // Create multiple processes that increment counter
    for (int i = 0; i < 5; i++) {
        if (fork() == 0) {
            // Child process
            for (int j = 0; j < 100; j++) {
                sem_wait(semid);  // Enter critical section
                (*shared_counter)++;
                sem_signal(semid);  // Exit critical section
            }
            shmdt(shared_counter);
            return;
        }
    }
    
    // Wait for all children
    for (int i = 0; i < 5; i++) {
        wait(NULL);
    }
    
    printf("Final counter value: %d (expected 500)\n", *shared_counter);
    
    // Cleanup
    shmdt(shared_counter);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
}
```

Binary semaphores (initialized to 1) act as mutexes, ensuring mutual exclusion. Counting semaphores (initialized to n) limit the number of processes simultaneously accessing a resource to n.

The SEM_UNDO flag automatically reverses semaphore operations when a process terminates abnormally, preventing deadlocks from crashed processes holding semaphores. This feature is crucial for robust daemon implementations.

POSIX semaphores (sem_init, sem_wait, sem_post) provide an alternative to System V semaphores with simpler API and better performance. Named POSIX semaphores work across unrelated processes, while unnamed semaphores require shared memory.

### Unix Domain Sockets: Flexible Local IPC

Unix domain sockets provide socket-based IPC for local communication. They offer the socket API's flexibility with better performance than inet sockets for local communication.

```c
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "/tmp/protocol_daemon.sock"

void unix_socket_server() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[256];
    
    // Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return;
    }
    
    // Setup address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // Remove existing socket file
    unlink(SOCKET_PATH);
    
    // Bind and listen
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        return;
    }
    
    listen(server_fd, 5);
    printf("Server listening on %s\n", SOCKET_PATH);
    
    // Accept connection
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd != -1) {
        ssize_t n = read(client_fd, buffer, sizeof(buffer));
        if (n > 0) {
            buffer[n] = '\0';
            printf("Server received: %s\n", buffer);
            
            // Send response
            write(client_fd, "ACK", 3);
        }
        close(client_fd);
    }
    
    close(server_fd);
    unlink(SOCKET_PATH);
}

void unix_socket_client() {
    int sock_fd;
    struct sockaddr_un addr;
    char *message = "GET NEIGHBOR STATUS";
    char buffer[256];
    
    sleep(1);  // Ensure server is ready
    
    // Create socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        return;
    }
    
    // Setup address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // Connect to server
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(sock_fd);
        return;
    }
    
    // Send request
    write(sock_fd, message, strlen(message));
    printf("Client sent: %s\n", message);
    
    // Receive response
    ssize_t n = read(sock_fd, buffer, sizeof(buffer));
    if (n > 0) {
        buffer[n] = '\0';
        printf("Client received: %s\n", buffer);
    }
    
    close(sock_fd);
}
```

Unix domain sockets support both stream (SOCK_STREAM) and datagram (SOCK_DGRAM) modes. Stream sockets provide reliable, ordered, bidirectional communication similar to TCP. Datagram sockets offer message boundaries without reliability guarantees.

Credential passing is a powerful Unix socket feature. Using ancillary data with sendmsg() and recvmsg(), processes can pass file descriptors, credentials (PID, UID, GID), and security labels between processes. This enables privilege separation architectures where privileged processes pass capabilities to unprivileged ones.

For routing daemons, Unix sockets excel at CLI tool integration. The daemon listens on a Unix socket, accepting connections from CLI tools that query protocol state or modify configuration. This pattern separates the daemon from user interfaces while maintaining security through filesystem permissions.

### Choosing the Right IPC Mechanism

The choice of IPC mechanism depends on several factors: relationship between processes, data volume, synchronization needs, and performance requirements.

Pipes suit parent-child relationships with simple, stream-oriented data flow. Command pipelines and process output capture exemplify pipe usage. Their simplicity and automatic cleanup make them ideal for temporary communication.

Message queues fit scenarios requiring structured messages with priorities. Protocol implementations often use message queues to route different message types to appropriate handlers. The type field enables efficient message classification without parsing.

Shared memory delivers maximum performance for high-volume data exchange. Large routing tables, neighbor databases, or packet buffers benefit from shared memory. However, the complexity of synchronization and memory management increases development and debugging effort.

Semaphores coordinate access to any shared resource, particularly shared memory. For complex synchronization, consider POSIX threads primitives (mutexes, condition variables) which offer better performance and semantics.

Unix domain sockets provide the most flexible IPC mechanism, supporting both stream and datagram modes with credential passing. They excel at daemon-client architectures and can scale from simple request-response to complex multiplexed protocols.

Network sockets (inet domain) enable IPC across machines. Despite higher overhead than local IPC, they provide transparent network distribution. Routing protocols inherently use network sockets, but management interfaces might prefer Unix sockets for local-only access.

### Performance Characteristics

Understanding IPC performance helps optimize communication patterns. Shared memory provides the highest throughput, limited only by memory bandwidth. A read-write operation on shared memory takes nanoseconds.

Unix domain sockets achieve throughput around 2-5 GB/s on modern systems, with latency in microseconds. This performance suffices for most control plane communication. Stream sockets add framing overhead compared to datagram sockets but provide reliability and ordering.

Message queues and System V IPC generally perform worse than Unix sockets due to kernel data copying and metadata management. They're suitable for low to moderate message rates (thousands per second) but struggle at high rates.

Pipes fall between Unix sockets and message queues in performance. Their simplicity and automatic buffering make them efficient for moderate data volumes. The kernel's pipe buffer eliminates explicit synchronization for producer-consumer scenarios.

### Security and Permissions

IPC security prevents unauthorized access to sensitive protocol data. Unix domain socket permissions derive from the filesystem, enabling standard permission checks. Restricting socket files to specific users or groups controls access.

Credential passing with SCM_CREDENTIALS ancillary data authenticates peer processes. The kernel provides the peer's PID, UID, and GID, preventing impersonation. This authentication enables privilege separation where only authenticated clients can modify routing state.

System V IPC objects (shared memory, semaphores, message queues) use separate permission schemes similar to file permissions but managed through IPC-specific calls. Setting appropriate permissions during creation prevents unauthorized access.

Namespaces provide IPC isolation in containerized environments. IPC namespaces separate System V IPC objects and POSIX message queues between containers. Network namespaces isolate sockets, enabling multiple network stacks on one system.

### Error Handling and Robustness

Robust IPC implementations handle failures gracefully. Connection-oriented sockets (SOCK_STREAM) detect peer termination through read() returning zero or EPIPE on write. Applications should monitor connections and reconnect after failures.

For shared memory, processes must handle partial writes from crashed peers. Versioning or sequence numbers help detect incomplete updates. A write lock held when a process crashes leaves shared resources locked unless using semaphore undo operations.

Message queue overflow occurs when senders outpace receivers. The IPC_NOWAIT flag enables non-blocking message send, returning EAGAIN when the queue is full. Monitoring queue depth prevents overflow and indicates capacity issues.

Timeouts prevent indefinite blocking. For blocking operations like accept() or read(), setting socket timeouts with setsockopt() or using select()/poll() with timeouts ensures responsiveness. This pattern allows checking for shutdown signals or other events periodically.

### Practical Patterns for Network Daemons

Modern routing daemons typically combine multiple IPC mechanisms. The control plane might use Unix sockets for CLI tools, shared memory for large data structures like routing tables, and message queues for event notification.

A common pattern separates control and data planes across processes. The control plane runs routing protocols and maintains the routing information base (RIB). The data plane performs packet forwarding using the forwarding information base (FIB). Shared memory enables efficient RIB-to-FIB updates while maintaining process isolation.

Event-driven architectures use select(), poll(), or epoll() to monitor multiple IPC channels simultaneously. A main event loop handles socket connections, pipe communications, and timers through a single multiplexing mechanism, avoiding thread complexity while maintaining responsiveness.

For high availability, consider IPC implications. Shared memory doesn't automatically synchronize to standby processes. Implementing state replication requires explicit messaging or database synchronization. Connection-oriented sockets require reconnection logic after failover.

### Debugging IPC Issues

IPC problems often manifest as hangs, crashes, or data corruption. Understanding available debugging tools accelerates troubleshooting.

The lsof command shows open file descriptors including pipes and sockets, revealing communication endpoints. The ipcs command lists System V IPC objects, showing shared memory segments, message queues, and semaphores with their owners and sizes.

For shared memory corruption, consider using mprotect() to make regions read-only temporarily, triggering segmentation faults on unexpected writes. Valgrind's memcheck tool detects uninitialized reads from shared memory.

Strace reveals IPC system calls, showing the sequence of operations and their results. Patterns of blocked calls or error returns often indicate synchronization issues or resource exhaustion. For socket debugging, tcpdump captures Unix socket traffic in some configurations, though typically you'll rely on application logging.

When diagnosing hangs, check for deadlocks in semaphore or lock acquisition. Tools like gdb can attach to hung processes, showing their call stacks. Multiple processes waiting on different resources in circular dependency indicate classic deadlock.

### Modern Alternatives

Recent Linux features provide additional IPC options. Eventfd creates file descriptors for event notification, enabling integration with select/poll/epoll. This lightweight mechanism suits signaling between threads or processes.

Signalfd converts signals into file descriptor events, allowing signal handling through the main event loop instead of signal handlers. This approach simplifies signal handling in event-driven applications.

Timerfd provides timer expiration through file descriptors, again enabling event loop integration. These fd-based mechanisms compose well, creating uniform event handling.

Memfd creates anonymous memory-backed file descriptors, providing shared memory without filesystem entries. Combined with file descriptor passing over Unix sockets, memfd enables dynamic shared memory setup between processes.

These modern interfaces integrate better with event-driven programming models common in high-performance network applications, though they require kernel version awareness for portability.

# Linux Memory Management
## Virtual Memory, Address Spaces, and Memory Optimization

### Virtual Memory Fundamentals

Linux provides each process with its own virtual address space, creating the illusion of exclusive memory access. This abstraction enables memory protection, simplifies programming, and allows efficient physical memory sharing. The Memory Management Unit (MMU) translates virtual addresses to physical addresses using page tables.

A process's virtual address space on x86-64 systems spans 48 bits (256 TB), though physical memory is much smaller. The kernel divides this space into user space (low addresses) and kernel space (high addresses). On 64-bit systems, the canonical address format requires bits 48-63 to match bit 47, creating two distinct regions separated by a large hole.

The virtual address space structure follows a standard layout. The text segment at low addresses contains executable code, typically starting at 0x400000. Above it sits the initialized data segment, followed by the BSS segment for uninitialized data. The heap grows upward from the BSS, while the stack grows downward from high addresses. Memory-mapped regions, including shared libraries and explicit mappings, occupy addresses between heap and stack.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void display_memory_layout() {
    char *heap_var = malloc(100);
    static int static_var = 42;
    int stack_var = 10;
    
    printf("Memory Layout for PID %d:\n", getpid());
    printf("  Text (main):        %p\n", (void*)main);
    printf("  Static data:        %p\n", (void*)&static_var);
    printf("  Heap:               %p\n", (void*)heap_var);
    printf("  Stack:              %p\n", (void*)&stack_var);
    printf("  Shared library:     %p\n", (void*)printf);
    
    // Check /proc/self/maps for complete picture
    printf("\nComplete memory map in /proc/%d/maps\n", getpid());
    
    free(heap_var);
}
```

Page tables map virtual pages to physical frames. On x86-64, a four-level hierarchy (PML4, PDP, PD, PT) translates addresses. Each level table contains 512 entries, and the TLB (Translation Lookaside Buffer) caches recent translations for performance. TLB misses trigger page table walks, significantly impacting performance with poor locality.

### Page Faults and Demand Paging

Linux uses demand paging, allocating physical memory only when accessed. When a process references an unmapped page, the CPU raises a page fault. The kernel's page fault handler determines whether the fault is valid (accessing allocated but not yet backed by physical memory) or invalid (accessing unallocated memory).

Three types of page faults exist: minor faults allocate physical pages without disk I/O, major faults load pages from disk (swap or file), and invalid faults trigger segmentation faults. Minor faults occur frequently during program startup as pages are brought into memory. Major faults indicate memory pressure forcing swap usage.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

void demonstrate_page_faults() {
    struct rusage usage_before, usage_after;
    size_t size = 100 * 1024 * 1024;  // 100 MB
    char *memory;
    
    getrusage(RUSAGE_SELF, &usage_before);
    
    // Allocate memory (no physical pages yet)
    memory = malloc(size);
    if (!memory) {
        perror("malloc");
        return;
    }
    
    // Touch each page to force allocation
    for (size_t i = 0; i < size; i += 4096) {
        memory[i] = 1;
    }
    
    getrusage(RUSAGE_SELF, &usage_after);
    
    long minor_faults = usage_after.ru_minflt - usage_before.ru_minflt;
    long major_faults = usage_after.ru_majflt - usage_before.ru_majflt;
    
    printf("Page faults incurred:\n");
    printf("  Minor: %ld\n", minor_faults);
    printf("  Major: %ld\n", major_faults);
    
    free(memory);
}
```

Copy-on-write (COW) optimization shares pages between processes until modification. After fork(), parent and child share physical pages marked read-only. When either process writes to a page, a page fault triggers copying the page, giving each process its own copy. This mechanism makes fork() efficient even for processes with large memory footprints.

Anonymous pages containing program data differ from file-backed pages containing memory-mapped files. Anonymous pages must go to swap when evicted, while file-backed pages can be discarded if unmodified or written back if dirty. This distinction affects performance under memory pressure.

### Memory Allocation with malloc

The malloc() family provides dynamic memory allocation. Understanding its implementation helps optimize memory usage and debug issues.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

void demonstrate_malloc_internals() {
    void *ptr1, *ptr2, *ptr3;
    
    // Small allocation (< 128KB typically uses brk)
    ptr1 = malloc(1024);
    printf("Allocated 1KB at %p\n", ptr1);
    
    // Large allocation (>= 128KB typically uses mmap)
    ptr2 = malloc(200 * 1024);
    printf("Allocated 200KB at %p\n", ptr2);
    
    // Realloc demonstration
    ptr3 = malloc(100);
    printf("Initial 100 bytes at %p\n", ptr3);
    
    ptr3 = realloc(ptr3, 1000);
    printf("After realloc to 1000 bytes: %p\n", ptr3);
    
    // Display malloc stats
    malloc_stats();
    
    // Cleanup
    free(ptr1);
    free(ptr2);
    free(ptr3);
}

// Custom allocation for routing table entries
struct route_entry {
    unsigned int prefix;
    unsigned int mask;
    unsigned int next_hop;
    int metric;
};

struct route_entry* allocate_route_pool(size_t count) {
    // Allocate aligned memory for better cache performance
    void *ptr;
    size_t size = count * sizeof(struct route_entry);
    
    // Align to cache line (typically 64 bytes)
    if (posix_memalign(&ptr, 64, size) != 0) {
        return NULL;
    }
    
    return (struct route_entry*)ptr;
}
```

For allocations below a threshold (typically 128KB), malloc uses the brk() system call to expand the heap. The allocator maintains free lists organized by size class, allocating from appropriate bins. Freed memory returns to bins but doesn't necessarily return to the OS immediately.

Large allocations use mmap() to create anonymous mappings. These allocations are separate from the heap and return to the OS immediately when freed. This behavior prevents heap fragmentation from large temporary allocations.

Memory alignment matters for performance. Misaligned accesses can be slower or even cause faults on some architectures. The posix_memalign() function allocates memory with specific alignment, useful for SIMD operations or cache line optimization.

### Memory-Mapped Files

The mmap() system call maps files or devices into memory, enabling file access through pointer operations. This approach offers performance benefits and simplified programming for certain use cases.

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void demonstrate_mmap() {
    int fd;
    char *mapped;
    struct stat sb;
    const char *filename = "/tmp/routing_config.txt";
    
    // Create and write to file first
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    const char *data = "route 192.168.1.0/24 via 10.0.0.1 metric 100\n";
    write(fd, data, strlen(data));
    
    // Get file size
    fstat(fd, &sb);
    
    // Map file into memory
    mapped = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE,
                  MAP_SHARED, fd, 0);
    
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }
    
    // Can now access file through pointer
    printf("File contents: %.*s", (int)sb.st_size, mapped);
    
    // Modify in memory (writes back to file with MAP_SHARED)
    mapped[0] = 'R';  // Change 'r' to 'R'
    
    // Sync changes to disk
    msync(mapped, sb.st_size, MS_SYNC);
    
    // Unmap and close
    munmap(mapped, sb.st_size);
    close(fd);
}

// Memory-mapped shared data structure
void create_shared_routing_table() {
    const char *shm_name = "/routing_table";
    size_t size = 1024 * 1024;  // 1MB
    int fd;
    void *ptr;
    
    // Create shared memory object
    fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return;
    }
    
    // Set size
    ftruncate(fd, size);
    
    // Map into memory
    ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
               MAP_SHARED, fd, 0);
    
    if (ptr != MAP_FAILED) {
        printf("Shared routing table mapped at %p\n", ptr);
        
        // Initialize routing table structure
        struct routing_table {
            int num_entries;
            char entries[1024 * 1024 - sizeof(int)];
        } *rt = ptr;
        
        rt->num_entries = 0;
        
        munmap(ptr, size);
    }
    
    close(fd);
}
```

MAP_SHARED creates a mapping shared with other processes, propagating modifications to the underlying file. MAP_PRIVATE creates a copy-on-write mapping where modifications remain private. For shared memory IPC, MAP_SHARED with MAP_ANONYMOUS creates shared memory without file backing.

Memory-mapped I/O benefits sequential access patterns where the kernel's readahead mechanism prefetches pages. Random access patterns may perform worse than traditional read/write due to page fault overhead. The madvise() system call provides hints about access patterns, helping the kernel optimize paging behavior.

For routing daemons, memory-mapped configuration files enable quick loading and easy parsing. Memory-mapped log files support efficient append operations. The mmap approach is particularly effective for large files accessed in patterns matching page size boundaries.

### Memory Protection and Security

Memory protection prevents corruption and unauthorized access. The mprotect() system call modifies protection on memory regions, enabling advanced memory management techniques.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>

void segfault_handler(int sig) {
    printf("Caught segmentation fault!\n");
    exit(1);
}

void demonstrate_memory_protection() {
    char *memory;
    size_t page_size = sysconf(_SC_PAGESIZE);
    
    // Set up segfault handler
    signal(SIGSEGV, segfault_handler);
    
    // Allocate memory
    memory = malloc(page_size);
    strcpy(memory, "Protected data");
    
    // Make read-only
    if (mprotect(memory, page_size, PROT_READ) == -1) {
        perror("mprotect");
        free(memory);
        return;
    }
    
    printf("Memory protected as read-only\n");
    printf("Reading: %s\n", memory);
    
    // This will cause segfault
    printf("Attempting to write to protected memory...\n");
    memory[0] = 'X';  // Triggers SIGSEGV
    
    free(memory);
}

// Implementing guard pages for stack overflow detection
void* allocate_with_guard_pages(size_t size) {
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t total_size = size + 2 * page_size;  // Add guard pages
    void *memory;
    
    // Allocate with mmap for protection control
    memory = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (memory == MAP_FAILED) {
        return NULL;
    }
    
    // Protect first and last pages
    mprotect(memory, page_size, PROT_NONE);
    mprotect((char*)memory + size + page_size, page_size, PROT_NONE);
    
    // Return pointer to usable region
    return (char*)memory + page_size;
}
```

Guard pages protect against buffer overflows. Placing PROT_NONE pages before and after allocations causes segmentation faults on overflow attempts, aiding debugging. Thread stacks typically have guard pages automatically.

Address Space Layout Randomization (ASLR) randomizes memory region locations, making exploit development harder. Stack, heap, and shared libraries load at random addresses on each execution. Some performance-critical applications disable ASLR, but security-conscious network daemons should keep it enabled.

### Memory Leaks and Debugging

Memory leaks occur when allocated memory is not freed, gradually exhausting available memory. Detecting and fixing leaks is crucial for long-running daemons.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Leak detection wrappers
static size_t total_allocated = 0;
static size_t allocation_count = 0;

void* debug_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size + sizeof(size_t));
    if (ptr) {
        *(size_t*)ptr = size;
        total_allocated += size;
        allocation_count++;
        printf("ALLOC: %zu bytes at %p (%s:%d)\n", 
               size, (char*)ptr + sizeof(size_t), file, line);
        return (char*)ptr + sizeof(size_t);
    }
    return NULL;
}

void debug_free(void *ptr, const char *file, int line) {
    if (ptr) {
        void *real_ptr = (char*)ptr - sizeof(size_t);
        size_t size = *(size_t*)real_ptr;
        total_allocated -= size;
        allocation_count--;
        printf("FREE:  %zu bytes at %p (%s:%d)\n", size, ptr, file, line);
        free(real_ptr);
    }
}

void print_memory_stats() {
    printf("\nMemory Statistics:\n");
    printf("  Total allocated: %zu bytes\n", total_allocated);
    printf("  Active allocations: %zu\n", allocation_count);
}

#define MALLOC(size) debug_malloc(size, __FILE__, __LINE__)
#define FREE(ptr) debug_free(ptr, __FILE__, __LINE__)

// Example with potential leak
void routing_protocol_handler() {
    char *neighbor_state = MALLOC(100);
    strcpy(neighbor_state, "ESTABLISHED");
    
    // Process protocol messages...
    
    // BUG: forgot to FREE(neighbor_state)
    // This leaks 100 bytes per invocation
}
```

Valgrind's memcheck tool detects memory leaks, uninitialized reads, and invalid memory access. Running `valgrind --leak-check=full ./program` reports detailed leak information including allocation call stacks. For routing daemons, periodic leak checking during development prevents accumulation of hard-to-find leaks.

The /proc/[pid]/status file shows VmSize (virtual memory) and VmRSS (resident set size). Monitoring these values during runtime detects leaks. A steadily growing VmRSS despite stable workload indicates leakage.

Modern allocators like tcmalloc or jemalloc provide better performance and built-in leak detection. They're drop-in replacements for malloc, often improving allocation-heavy applications' performance.

### Optimizing Memory Usage

Memory optimization reduces overhead and improves cache utilization. Several techniques apply to network daemon development.

Memory pooling reuses allocations instead of frequent malloc/free calls. Pre-allocating fixed-size objects in pools eliminates allocation overhead and fragmentation.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POOL_SIZE 1000

struct object_pool {
    void *objects[POOL_SIZE];
    int free_list[POOL_SIZE];
    int next_free;
    size_t object_size;
};

struct object_pool* create_pool(size_t object_size) {
    struct object_pool *pool = malloc(sizeof(struct object_pool));
    if (!pool) return NULL;
    
    pool->object_size = object_size;
    pool->next_free = 0;
    
    // Pre-allocate all objects
    for (int i = 0; i < POOL_SIZE; i++) {
        pool->objects[i] = malloc(object_size);
        if (!pool->objects[i]) {
            // Cleanup on failure
            for (int j = 0; j < i; j++) {
                free(pool->objects[j]);
            }
            free(pool);
            return NULL;
        }
        pool->free_list[i] = i;
    }
    
    return pool;
}

void* pool_alloc(struct object_pool *pool) {
    if (pool->next_free >= POOL_SIZE) {
        return NULL;  // Pool exhausted
    }
    
    int index = pool->free_list[pool->next_free++];
    return pool->objects[index];
}

void pool_free(struct object_pool *pool, void *ptr) {
    // Find object index
    for (int i = 0; i < POOL_SIZE; i++) {
        if (pool->objects[i] == ptr) {
            pool->free_list[--pool->next_free] = i;
            return;
        }
    }
}
```

Structure padding and alignment affect memory usage. Compilers insert padding to satisfy alignment requirements. Reordering structure members to place larger types first minimizes padding.

Cache line awareness improves performance. Aligning frequently accessed structures to cache line boundaries (64 bytes on x86-64) prevents false sharing in multi-threaded programs. The __attribute__((aligned(64))) directive forces alignment.

Huge pages reduce TLB pressure for large memory regions. The madvise() system call with MADV_HUGEPAGE hints that a region should use huge pages (typically 2MB). Transparent Huge Pages (THP) automatically use huge pages when beneficial, though explicitly requesting them via hugetlbfs provides more control.

For routing tables with millions of entries, memory layout significantly impacts performance. Array-of-structures might be less cache-efficient than structure-of-arrays for certain access patterns. Profiling guides optimization decisions.

# Linux Network Stack and Socket Programming
## From Hardware to Application: The Complete Journey

### Network Stack Architecture

The Linux network stack implements the TCP/IP protocol suite with a layered architecture. Understanding packet flow through these layers reveals optimization opportunities and debugging strategies critical for protocol implementation.

When a packet arrives at the network interface card (NIC), hardware generates an interrupt. The interrupt handler schedules a softirq, deferring most processing to reduce interrupt context time. The softirq polls packets from the NIC's ring buffer through NAPI (New API), processing batches for efficiency. This batching reduces per-packet overhead compared to processing each packet in interrupt context.

The network layer examines the IP header, performs route lookup in the Forwarding Information Base (FIB), and decides whether to forward, deliver locally, or drop the packet. Netfilter hooks at various points enable firewalling and NAT. For locally destined packets, the transport layer (TCP/UDP) demultiplexes to the correct socket based on port numbers.

```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

void demonstrate_packet_reception() {
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1500];  // MTU-sized buffer
    
    // Create raw socket to see IP layer
    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_fd < 0) {
        perror("socket (need root)");
        return;
    }
    
    // Receive a packet
    ssize_t packet_size = recvfrom(sock_fd, buffer, sizeof(buffer),
                                   0, (struct sockaddr*)&client_addr,
                                   &client_len);
    
    if (packet_size > 0) {
        struct iphdr *ip_header = (struct iphdr*)buffer;
        struct tcphdr *tcp_header = (struct tcphdr*)(buffer + (ip_header->ihl * 4));
        
        printf("Received packet:\n");
        printf("  IP: %s -> ", inet_ntoa(*(struct in_addr*)&ip_header->saddr));
        printf("%s\n", inet_ntoa(*(struct in_addr*)&ip_header->daddr));
        printf("  TCP: port %d -> %d\n", 
               ntohs(tcp_header->source),
               ntohs(tcp_header->dest));
        printf("  Size: %zd bytes\n", packet_size);
    }
    
    close(sock_fd);
}
```

The routing subsystem maintains two key tables: the Routing Information Base (RIB) contains all routes from routing protocols, while the Forwarding Information Base (FIB) contains a compressed subset optimized for lookup performance. The RIB-to-FIB synchronization happens as routes change, allowing the FIB to use radix tries or other efficient structures.

Neighbor discovery (ARP for IPv4, NDP for IPv6) resolves next-hop IP addresses to MAC addresses. The neighbor cache stores these mappings with state tracking (REACHABLE, STALE, DELAY, PROBE). Understanding neighbor state machines helps debug connectivity issues in protocol implementations.

### Socket Programming Fundamentals

Sockets provide the primary interface between applications and the network stack. Creating a socket allocates kernel data structures that track connection state, buffers, and protocol-specific information.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

int create_tcp_server(int port) {
    int sock_fd, opt = 1;
    struct sockaddr_in addr;
    
    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }
    
    // Set socket options
    // SO_REUSEADDR allows binding to TIME_WAIT ports
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Bind to address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock_fd);
        return -1;
    }
    
    // Listen for connections
    if (listen(sock_fd, 128) < 0) {  // 128 is backlog
        perror("listen");
        close(sock_fd);
        return -1;
    }
    
    printf("TCP server listening on port %d\n", port);
    return sock_fd;
}

int accept_connection(int server_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd;
    
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept");
        return -1;
    }
    
    printf("Accepted connection from %s:%d\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));
    
    return client_fd;
}

void handle_client(int client_fd) {
    char buffer[4096];
    ssize_t bytes_read;
    
    while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0) {
        // Echo back to client
        write(client_fd, buffer, bytes_read);
    }
    
    if (bytes_read < 0) {
        perror("read");
    }
    
    close(client_fd);
}
```

The socket() call creates an endpoint for communication. The domain parameter (AF_INET, AF_INET6) specifies the protocol family. The type parameter (SOCK_STREAM, SOCK_DGRAM, SOCK_RAW) determines connection semantics. The protocol parameter typically defaults to zero, selecting the standard protocol for the given type.

Binding associates a socket with a local address and port. The kernel allocates an ephemeral port if you bind to port zero. The INADDR_ANY address means the socket accepts connections on all interfaces. For multi-homed systems, binding to a specific interface IP restricts incoming connections.

The listen() backlog parameter limits queued connections awaiting accept(). Setting this too low causes connection refusals under load. Modern kernels automatically tune this value up to somaxconn (typically 128-4096), but protocol daemons with many simultaneous connections should increase it.

### Non-Blocking I/O and Multiplexing

Blocking I/O operations suspend the calling thread until completion. For servers handling many connections, this approach wastes resources. Non-blocking I/O with multiplexing enables handling thousands of connections in a single thread.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define MAX_CONNECTIONS 10000

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void epoll_server(int port) {
    int server_fd, epoll_fd;
    struct epoll_event ev, events[MAX_EVENTS];
    
    // Create and setup server socket
    server_fd = create_tcp_server(port);
    if (server_fd < 0) return;
    
    set_nonblocking(server_fd);
    
    // Create epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        close(server_fd);
        return;
    }
    
    // Add server socket to epoll
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
    
    printf("Epoll server running on port %d\n", port);
    
    // Event loop
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            break;
        }
        
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // New connection
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(server_fd, 
                                      (struct sockaddr*)&client_addr,
                                      &client_len);
                
                if (client_fd < 0) {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("accept");
                    }
                    continue;
                }
                
                set_nonblocking(client_fd);
                
                // Add client to epoll
                ev.events = EPOLLIN | EPOLLET;  // Edge-triggered
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
                
                printf("New connection: fd %d\n", client_fd);
            } else {
                // Data from client
                int client_fd = events[i].data.fd;
                char buffer[4096];
                ssize_t bytes_read;
                
                while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0) {
                    // Process data
                    write(client_fd, buffer, bytes_read);
                }
                
                if (bytes_read == 0) {
                    // Connection closed
                    printf("Connection closed: fd %d\n", client_fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read");
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                }
            }
        }
    }
    
    close(server_fd);
    close(epoll_fd);
}
```

The epoll interface is the most efficient multiplexing mechanism on Linux. Unlike select() and poll() which iterate through all file descriptors, epoll scales to tens of thousands of connections. The kernel maintains a data structure tracking which sockets have events, returning only ready descriptors.

Edge-triggered (EPOLLET) mode notifies applications only on state changes, reducing system call overhead. Applications must read until EAGAIN to avoid missing data. Level-triggered mode (default) notifies whenever data is available, simpler but potentially less efficient.

The EPOLLONESHOT flag ensures a file descriptor triggers only once, preventing race conditions in multi-threaded servers. After handling an event, the application must re-arm the descriptor with epoll_ctl().

### Socket Options and Tuning

Socket options control protocol behavior and performance characteristics. Understanding these options helps optimize protocol implementations.

```c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

void configure_tcp_socket(int sock_fd) {
    int opt = 1;
    
    // Disable Nagle's algorithm for low-latency protocols
    // Nagle buffers small packets for efficiency but adds latency
    setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    
    // Enable TCP keepalive
    setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    
    // Set keepalive parameters
    int keepidle = 60;   // Start probes after 60s idle
    int keepintvl = 10;  // Probe every 10s
    int keepcnt = 3;     // 3 failed probes = dead
    setsockopt(sock_fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
    setsockopt(sock_fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
    setsockopt(sock_fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
    
    // Set send/receive buffer sizes
    int sndbuf = 256 * 1024;  // 256KB send buffer
    int rcvbuf = 256 * 1024;  // 256KB receive buffer
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));
    
    // Set TCP user timeout (connection failure detection)
    unsigned int timeout = 30000;  // 30 seconds in milliseconds
    setsockopt(sock_fd, IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout, sizeof(timeout));
    
    // Enable TCP_QUICKACK to send ACKs immediately
    setsockopt(sock_fd, IPPROTO_TCP, TCP_QUICKACK, &opt, sizeof(opt));
    
    printf("TCP socket configured for protocol use\n");
}

void configure_udp_socket(int sock_fd) {
    int opt = 1;
    
    // Allow multiple sockets to bind to same port (for multicast)
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    
    // Set buffer sizes (UDP needs larger buffers for burst traffic)
    int sndbuf = 512 * 1024;
    int rcvbuf = 512 * 1024;
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));
    
    // Set TTL for multicast
    unsigned char ttl = 64;
    setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
    
    // Enable packet info (source address) reception
    setsockopt(sock_fd, IPPROTO_IP, IP_PKTINFO, &opt, sizeof(opt));
    
    printf("UDP socket configured for multicast protocols\n");
}
```

TCP_NODELAY disables Nagle's algorithm, which buffers small packets to reduce network overhead. For interactive protocols or those with their own message batching, disabling Nagle reduces latency without harming throughput.

SO_KEEPALIVE enables TCP keepalive probes detecting dead connections. The TCP_KEEPIDLE, TCP_KEEPINTVL, and TCP_KEEPCNT options control probe timing. Protocol implementations should tune these based on expected network conditions and convergence requirements.

Buffer sizes significantly impact throughput on high-bandwidth links. The kernel doubles requested buffer sizes, reserving half for overhead. For high-speed protocols, larger buffers prevent losses due to full buffers. However, excessive buffering increases latency (bufferbloat).

SO_REUSEPORT allows multiple sockets to bind to the same port, enabling load distribution across threads or processes. The kernel distributes incoming connections or packets among the bound sockets. This feature simplifies scaling protocol daemons across CPU cores.

### Raw Sockets and Packet Crafting

Raw sockets bypass transport layer processing, enabling custom protocol implementation and packet analysis. They require root privileges due to security implications.

```c
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

// Pseudo header for TCP checksum calculation
struct pseudo_header {
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;
};

unsigned short checksum(unsigned short *ptr, int nbytes) {
    long sum = 0;
    unsigned short oddbyte;
    
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    
    if (nbytes == 1) {
        oddbyte = 0;
        *((unsigned char*)&oddbyte) = *(unsigned char*)ptr;
        sum += oddbyte;
    }
    
    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    
    return (unsigned short)~sum;
}

void send_custom_tcp_packet(const char *dest_ip, int dest_port) {
    int sock_fd;
    char packet[4096];
    struct iphdr *ip_header = (struct iphdr*)packet;
    struct tcphdr *tcp_header = (struct tcphdr*)(packet + sizeof(struct iphdr));
    struct pseudo_header psh;
    char *data = packet + sizeof(struct iphdr) + sizeof(struct tcphdr);
    char *payload = "HELLO from custom TCP packet";
    
    // Create raw socket
    sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_fd < 0) {
        perror("socket (need root)");
        return;
    }
    
    // Enable IP_HDRINCL to specify custom IP header
    int one = 1;
    setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));
    
    // Clear packet buffer
    memset(packet, 0, sizeof(packet));
    
    // Copy payload
    strcpy(data, payload);
    
    // Fill IP header
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(payload);
    ip_header->id = htons(54321);
    ip_header->frag_off = 0;
    ip_header->ttl = 64;
    ip_header->protocol = IPPROTO_TCP;
    ip_header->check = 0;  // Kernel fills this
    ip_header->saddr = inet_addr("192.168.1.100");
    ip_header->daddr = inet_addr(dest_ip);
    
    // Fill TCP header
    tcp_header->source = htons(12345);
    tcp_header->dest = htons(dest_port);
    tcp_header->seq = htonl(1000);
    tcp_header->ack_seq = 0;
    tcp_header->doff = 5;  // TCP header size
    tcp_header->syn = 1;   // SYN flag
    tcp_header->window = htons(5840);
    tcp_header->check = 0;
    tcp_header->urg_ptr = 0;
    
    // Calculate TCP checksum
    psh.source_address = ip_header->saddr;
    psh.dest_address = ip_header->daddr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr) + strlen(payload));
    
    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr) + strlen(payload);
    char *pseudogram = malloc(psize);
    memcpy(pseudogram, &psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), tcp_header, 
           sizeof(struct tcphdr) + strlen(payload));
    
    tcp_header->check = checksum((unsigned short*)pseudogram, psize);
    free(pseudogram);
    
    // Send packet
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(dest_port);
    dest.sin_addr.s_addr = inet_addr(dest_ip);
    
    if (sendto(sock_fd, packet, ip_header->tot_len, 0,
               (struct sockaddr*)&dest, sizeof(dest)) < 0) {
        perror("sendto");
    } else {
        printf("Custom TCP packet sent\n");
    }
    
    close(sock_fd);
}
```

Raw sockets enable implementing custom protocols or testing protocol implementations. They provide access to the IP layer and below, allowing complete control over packet construction. However, this power requires careful handling of checksums, byte ordering, and protocol semantics.

For protocol development, raw sockets facilitate testing edge cases and malformed packets. Receiving raw packets enables protocol analyzers and monitoring tools. Many routing protocols use raw sockets or SOCK_DGRAM with IP_HDRINCL for custom packet formats.

### Network Performance Optimization

High-performance network applications require careful optimization. Understanding bottlenecks and measurement techniques guides optimization efforts.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <unistd.h>

void measure_throughput(int sock_fd, size_t total_bytes) {
    char *buffer = malloc(1024 * 1024);  // 1MB buffer
    size_t bytes_sent = 0;
    struct timeval start, end;
    
    gettimeofday(&start, NULL);
    
    while (bytes_sent < total_bytes) {
        size_t to_send = (total_bytes - bytes_sent < 1024 * 1024) ?
                         (total_bytes - bytes_sent) : 1024 * 1024;
        
        ssize_t sent = write(sock_fd, buffer, to_send);
        if (sent < 0) {
            perror("write");
            break;
        }
        
        bytes_sent += sent;
    }
    
    gettimeofday(&end, NULL);
    
    double elapsed = (end.tv_sec - start.tv_sec) + 
                    (end.tv_usec - start.tv_usec) / 1000000.0;
    double throughput = (bytes_sent / elapsed) / (1024.0 * 1024.0);
    
    printf("Sent %zu bytes in %.2f seconds\n", bytes_sent, elapsed);
    printf("Throughput: %.2f MB/s\n", throughput);
    
    free(buffer);
}

void demonstrate_zero_copy() {
    // Zero-copy with sendfile() - efficient file transmission
    int sock_fd, file_fd;
    off_t offset = 0;
    struct stat file_stat;
    
    // Setup socket and file...
    // (omitted for brevity)
    
    // Get file size
    fstat(file_fd, &file_stat);
    
    // Send file without copying to user space
    ssize_t sent = sendfile(sock_fd, file_fd, &offset, file_stat.st_size);
    printf("Zero-copy sent %zd bytes\n", sent);
}

void enable_tcp_fastopen(int sock_fd) {
    // TCP Fast Open reduces connection setup latency
    int qlen = 5;  // Queue length for pending connections
    setsockopt(sock_fd, SOL_TCP, TCP_FASTOPEN, &qlen, sizeof(qlen));
    
    printf("TCP Fast Open enabled\n");
}

void configure_for_high_throughput(int sock_fd) {
    // Disable Nagle for immediate transmission
    int nodelay = 1;
    setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
    
    // Large buffers for bandwidth-delay product
    int bufsize = 4 * 1024 * 1024;  // 4MB
    setsockopt(sock_fd, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
    setsockopt(sock_fd, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
    
    // Enable TCP window scaling
    // (automatically enabled if buffers > 64KB)
    
    // Set TCP congestion control algorithm
    char *algo = "cubic";  // or "bbr" for newer kernels
    setsockopt(sock_fd, IPPROTO_TCP, TCP_CONGESTION, algo, strlen(algo));
    
    printf("Socket configured for high throughput\n");
}
```

Zero-copy techniques eliminate data copying between kernel and user space. The sendfile() system call transfers data directly from a file descriptor to a socket. For protocols transmitting large data blocks, zero-copy significantly reduces CPU usage and improves throughput.

TCP Fast Open (TFO) reduces connection establishment latency by sending data in the SYN packet. This optimization benefits request-response protocols with many short-lived connections. However, security considerations limit its deployment.

Buffer sizing affects throughput on high-bandwidth, high-latency networks. The optimal buffer size equals the bandwidth-delay product. For a 1 Gbps link with 10ms RTT, this equals approximately 1.25 MB. Too-small buffers limit throughput; too-large buffers increase latency.

Congestion control algorithms significantly impact performance. CUBIC (default) optimizes for high-bandwidth networks. BBR (Bottleneck Bandwidth and RTT) achieves better throughput and fairness in some scenarios. The choice depends on network characteristics and kernel support.

### Routing Tables and FIB Management

Routing protocols interact with the kernel's routing table through netlink sockets. Understanding this interface enables efficient route updates and monitoring.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <unistd.h>
#include <arpa/inet.h>

void add_route(const char *dest, const char *gateway, const char *interface) {
    int sock_fd;
    struct {
        struct nlmsghdr nlh;
        struct rtmsg rtm;
        char buf[1024];
    } req;
    
    struct sockaddr_nl sa;
    
    // Create netlink socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock_fd < 0) {
        perror("socket");
        return;
    }
    
    // Prepare netlink message
    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    req.nlh.nlmsg_type = RTM_NEWROUTE;
    req.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE | NLM_F_REPLACE;
    req.nlh.nlmsg_seq = 1;
    
    req.rtm.rtm_family = AF_INET;
    req.rtm.rtm_table = RT_TABLE_MAIN;
    req.rtm.rtm_protocol = RTPROT_STATIC;
    req.rtm.rtm_scope = RT_SCOPE_UNIVERSE;
    req.rtm.rtm_type = RTN_UNICAST;
    req.rtm.rtm_dst_len = 24;  // /24 prefix
    
    // Add destination attribute
    struct rtattr *rta = (struct rtattr*)(((char*)&req) + NLMSG_ALIGN(req.nlh.nlmsg_len));
    rta->rta_type = RTA_DST;
    rta->rta_len = RTA_LENGTH(4);
    inet_pton(AF_INET, dest, RTA_DATA(rta));
    req.nlh.nlmsg_len = NLMSG_ALIGN(req.nlh.nlmsg_len) + RTA_LENGTH(4);
    
    // Add gateway attribute
    rta = (struct rtattr*)(((char*)&req) + NLMSG_ALIGN(req.nlh.nlmsg_len));
    rta->rta_type = RTA_GATEWAY;
    rta->rta_len = RTA_LENGTH(4);
    inet_pton(AF_INET, gateway, RTA_DATA(rta));
    req.nlh.nlmsg_len = NLMSG_ALIGN(req.nlh.nlmsg_len) + RTA_LENGTH(4);
    
    // Send message
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    
    if (sendto(sock_fd, &req, req.nlh.nlmsg_len, 0,
               (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("sendto");
    } else {
        printf("Route added: %s via %s\n", dest, gateway);
    }
    
    close(sock_fd);
}

void monitor_route_changes() {
    int sock_fd;
    struct sockaddr_nl sa;
    char buffer[8192];
    
    // Create netlink socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock_fd < 0) {
        perror("socket");
        return;
    }
    
    // Bind to routing multicast group
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_IPV4_ROUTE | RTMGRP_LINK;
    
    if (bind(sock_fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(sock_fd);
        return;
    }
    
    printf("Monitoring routing changes...\n");
    
    // Receive routing events
    while (1) {
        ssize_t len = recv(sock_fd, buffer, sizeof(buffer), 0);
        if (len < 0) {
            perror("recv");
            break;
        }
        
        struct nlmsghdr *nlh = (struct nlmsghdr*)buffer;
        while (NLMSG_OK(nlh, len)) {
            if (nlh->nlmsg_type == RTM_NEWROUTE) {
                printf("New route added\n");
            } else if (nlh->nlmsg_type == RTM_DELROUTE) {
                printf("Route deleted\n");
            }
            
            nlh = NLMSG_NEXT(nlh, len);
        }
    }
    
    close(sock_fd);
}
```

Netlink sockets provide a bidirectional communication channel between user space and kernel space. Routing protocols use netlink to add, delete, and query routes. The RTM_NEWROUTE message adds or modifies routes, while RTM_DELROUTE removes them. RTM_GETROUTE queries existing routes.

Multicast groups enable monitoring routing changes. Subscribing to RTMGRP_IPV4_ROUTE and RTMGRP_IPV6_ROUTE notifies applications of route table modifications. This mechanism enables protocols to track routes added by other sources and maintain consistency.

For high-performance routing protocols, batching netlink operations reduces overhead. Multiple route updates can be sent in a single message, reducing context switches and improving throughput. The NLM_F_MULTI flag indicates multi-part messages.

Understanding the FIB structure helps optimize lookup performance. Linux uses a radix trie (PATRICIA tree) for IPv4 and a more complex structure for IPv6. Prefix aggregation reduces FIB size, improving lookup speed and memory usage.

# Linux Advanced Networking Topics
## Kernel Modules, eBPF, High-Performance Networking, and Deep Troubleshooting

### Kernel Modules and Device Drivers

Linux kernel modules extend kernel functionality without recompiling or rebooting. Network device drivers, protocol implementations, and custom packet processing logic often exist as loadable kernel modules. Understanding module development enables custom networking solutions and deep system integration.

A kernel module runs in kernel space with full hardware access and no memory protection. Unlike user-space programs, kernel code must handle all error conditions carefully. A null pointer dereference in kernel space crashes the entire system, not just the offending process.

Kernel modules use a different programming model than user space. They cannot use standard library functions like printf() or malloc(). Instead, they use kernel equivalents like printk() and kmalloc(). Module code must be reentrant and thread-safe, as multiple CPUs can execute module code simultaneously without explicit synchronization.

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Network Engineer");
MODULE_DESCRIPTION("Simple packet inspection module");

// Module initialization
static int __init packet_inspector_init(void) {
    printk(KERN_INFO "Packet Inspector: Module loaded\n");
    return 0;
}

// Module cleanup
static void __exit packet_inspector_exit(void) {
    printk(KERN_INFO "Packet Inspector: Module unloaded\n");
}

module_init(packet_inspector_init);
module_exit(packet_inspector_exit);

// Network device operations example
static netdev_tx_t custom_xmit(struct sk_buff *skb, struct net_device *dev) {
    struct iphdr *ip_header;
    
    // Extract IP header from socket buffer
    ip_header = ip_hdr(skb);
    
    if (ip_header) {
        printk(KERN_INFO "Packet: src=%pI4 dst=%pI4 protocol=%d\n",
               &ip_header->saddr, &ip_header->daddr, ip_header->protocol);
    }
    
    // Forward packet to actual hardware
    // Real implementation would call hardware-specific functions
    dev_kfree_skb(skb);
    
    return NETDEV_TX_OK;
}

static const struct net_device_ops custom_netdev_ops = {
    .ndo_start_xmit = custom_xmit,
    // Additional operations: open, stop, ioctl, etc.
};
```

Network device drivers interact with hardware through the net_device structure, which represents a network interface. The driver implements callbacks for packet transmission (ndo_start_xmit), device initialization (ndo_open), and shutdown (ndo_stop). The kernel calls these functions when applications send packets or configure interfaces.

The socket buffer (sk_buff) structure is central to Linux networking. It contains packet data, metadata like protocol headers, and reference counts for efficient memory management. Understanding sk_buff manipulation is essential for packet processing in kernel modules. Functions like skb_push(), skb_pull(), skb_put(), and skb_trim() adjust pointers to add or remove headers as packets traverse protocol layers.

Device driver development requires understanding DMA (Direct Memory Access), interrupt handling, and hardware register access. Network cards typically use ring buffers for packet transmission and reception. The driver allocates memory for these rings, programs DMA descriptors, and handles completion interrupts when hardware finishes processing packets.

Building and loading modules follows a standard process. The Makefile uses kernel build infrastructure to compile against running kernel headers. The insmod command loads modules, while rmmod removes them. The lsmod command lists loaded modules. For permanent loading, modules appear in /etc/modules or through systemd unit files.

Debugging kernel modules differs from user-space debugging. The printk() function logs messages to the kernel ring buffer, viewable with dmesg. The kernel provides debugging facilities like KASAN (Kernel Address Sanitizer) for memory errors and lockdep for deadlock detection. For serious development, tools like QEMU with kgdb enable interactive debugging without risking the main system.

### eBPF and XDP for Packet Processing

Extended Berkeley Packet Filter (eBPF) revolutionized Linux networking by enabling safe, efficient kernel-space programs loaded at runtime. Unlike kernel modules requiring compilation and privileged insertion, eBPF programs undergo verification ensuring safety before execution. This verification prevents crashes, infinite loops, and unauthorized memory access.

eBPF programs attach to various kernel hooks. For networking, XDP (eXpress Data Path) provides the earliest attachment point, processing packets immediately after the NIC driver receives them, before socket buffer allocation. This early processing enables line-rate packet filtering and forwarding with minimal CPU overhead.

```c
// eBPF XDP program for packet filtering
// Compile with clang -O2 -target bpf -c xdp_filter.c -o xdp_filter.o

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <bpf/bpf_helpers.h>

// BPF map for storing statistics
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 256);
    __type(key, __u32);
    __type(value, __u64);
} stats_map SEC(".maps");

// XDP program entry point
SEC("xdp")
int xdp_filter_prog(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    
    struct ethhdr *eth = data;
    
    // Bounds check required by verifier
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;
    
    // Check if IP packet
    if (eth->h_proto != __constant_htons(ETH_P_IP))
        return XDP_PASS;
    
    struct iphdr *ip = (void *)(eth + 1);
    
    // Another bounds check
    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;
    
    // Update statistics
    __u32 proto = ip->protocol;
    __u64 *count = bpf_map_lookup_elem(&stats_map, &proto);
    if (count)
        __sync_fetch_and_add(count, 1);
    
    // Filter packets to specific destination
    __u32 dest_ip = ip->daddr;
    if (dest_ip == __constant_htonl(0xC0A80001)) {  // 192.168.0.1
        return XDP_DROP;  // Drop packets to this IP
    }
    
    // For TCP SYN packets, could redirect to different queue
    if (ip->protocol == IPPROTO_TCP) {
        struct tcphdr *tcp = (void *)(ip + 1);
        if ((void *)(tcp + 1) > data_end)
            return XDP_PASS;
        
        if (tcp->syn && !tcp->ack) {
            // Could redirect to specific CPU/queue for SYN cookies
            return bpf_redirect_map(&cpu_map, 0, 0);
        }
    }
    
    return XDP_PASS;  // Allow packet to continue
}

char _license[] SEC("license") = "GPL";
```

XDP programs return action codes directing packet handling. XDP_DROP discards packets immediately, achieving multi-million packets-per-second filtering rates. XDP_PASS continues normal processing. XDP_TX bounces packets back out the receiving interface, enabling efficient load balancers. XDP_REDIRECT forwards packets to different interfaces or CPUs without kernel stack traversal.

BPF maps provide persistent storage and communication between eBPF programs and user space. Array maps store fixed-size data indexed by integers. Hash maps provide key-value storage. Per-CPU maps avoid synchronization overhead in multi-processor systems. Ring buffer maps efficiently transfer data to user space for monitoring and logging.

The eBPF verifier analyzes programs before loading, ensuring safety. It tracks register states, verifies all memory accesses stay within bounds, confirms the program terminates, and prevents unauthorized kernel memory access. Programs must be small (originally 4096 instructions, now larger with complexity limits) and avoid unbounded loops. The verifier rejects programs violating these constraints.

Loading eBPF programs uses the bpf() system call or higher-level tools like bpftool or libbpf. The iproute2 package enables attaching XDP programs to interfaces with `ip link set dev eth0 xdp obj xdp_filter.o`. The program remains attached until explicitly removed or the interface goes down.

eBPF applications extend beyond packet filtering. Network monitoring tools use eBPF to track connection statistics, latency distributions, and packet loss without performance overhead. Load balancers implement sophisticated algorithms in XDP for line-rate traffic distribution. DDoS mitigation systems drop attack traffic at the earliest possible point. Container networking uses eBPF for efficient policy enforcement and routing.

### Linux Network Namespaces and Containers

Network namespaces provide network stack isolation, enabling multiple independent network configurations on one system. Each namespace has its own network interfaces, routing tables, firewall rules, and socket ports. This isolation is fundamental to container networking, virtual routers, and network testing.

Creating a network namespace is straightforward using the ip command or the unshare() system call. Initially, a new namespace contains only a loopback interface. Virtual Ethernet (veth) pairs connect namespaces, with one end in each namespace. These behave like a virtual cable, packets transmitted on one end appear on the other.

```c
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>

// Create isolated network namespace
void create_network_namespace() {
    // Create new network namespace
    if (unshare(CLONE_NEWNET) != 0) {
        perror("unshare");
        exit(1);
    }
    
    printf("Created new network namespace\n");
    
    // In the new namespace, initially only loopback exists
    system("ip link show");
    
    // Bring up loopback
    system("ip link set lo up");
    
    // Namespace now has isolated network stack
    printf("Network namespace initialized\n");
}

// Demonstrate veth pair for namespace communication
void setup_veth_pair() {
    // These commands typically run from root namespace
    // Create veth pair
    system("ip link add veth0 type veth peer name veth1");
    
    // Move one end to namespace (assuming namespace named 'netns1')
    system("ip link set veth1 netns netns1");
    
    // Configure interfaces
    system("ip addr add 10.0.0.1/24 dev veth0");
    system("ip link set veth0 up");
    
    // In the namespace, configure other end
    system("ip netns exec netns1 ip addr add 10.0.0.2/24 dev veth1");
    system("ip netns exec netns1 ip link set veth1 up");
    
    printf("Veth pair configured between namespaces\n");
}
```

Containers like Docker use network namespaces extensively. When Docker starts a container, it creates a network namespace for isolation. A veth pair connects the container to the host, with one end in the container's namespace and the other in a bridge on the host. This topology allows containers to communicate while remaining isolated.

Container networking modes reflect different namespace strategies. Bridge mode creates a private network for containers connected via a software bridge. Host mode shares the host's network namespace, providing maximum performance but no isolation. Overlay networks span multiple hosts, tunneling traffic between containers on different machines.

The ip netns command manages namespaces. Creating a named namespace with `ip netns add myns` creates a persistent namespace. The `ip netns exec myns <command>` syntax executes commands within a namespace. Listing interfaces with `ip netns exec myns ip link show` displays only interfaces in that namespace.

Network namespace isolation affects routing and protocol implementations. Each namespace has independent routing tables manipulated normally with ip route commands while in that namespace. Routing protocols running in a namespace only see interfaces and routes within that namespace. Testing multiple routing protocol instances on one machine becomes simple with namespaces.

Virtual routing and forwarding (VRF) builds on network namespaces but maintains a shared kernel stack. VRF provides routing table isolation without full namespace separation. This approach suits multi-tenant routing where some kernel resources should be shared.

### DPDK: Data Plane Development Kit

DPDK provides libraries and drivers for fast packet processing, bypassing the kernel network stack entirely. By running in user space with direct hardware access, DPDK achieves rates exceeding 100 Gbps with minimal CPU utilization. This performance enables software routers, firewalls, and load balancers matching hardware equivalents.

DPDK fundamentals differ significantly from kernel networking. Applications run in user space but access NIC hardware directly through memory-mapped I/O. Poll mode drivers (PMDs) continuously poll NIC receive queues instead of using interrupts, eliminating interrupt overhead. Huge pages reduce TLB misses for packet buffers. CPU affinity pins threads to specific cores, avoiding scheduler overhead and ensuring cache locality.

```c
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>
#include <rte_lcore.h>

#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

// Initialize DPDK environment
int dpdk_init(int argc, char **argv) {
    int ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "EAL initialization failed\n");
    
    return ret;
}

// Configure ethernet port
void port_init(uint16_t port, struct rte_mempool *mbuf_pool) {
    struct rte_eth_conf port_conf = {0};
    const uint16_t rx_rings = 1, tx_rings = 1;
    uint16_t nb_rxd = 1024;
    uint16_t nb_txd = 1024;
    int ret;
    struct rte_eth_dev_info dev_info;
    
    // Get device info
    ret = rte_eth_dev_info_get(port, &dev_info);
    if (ret != 0)
        rte_exit(EXIT_FAILURE, "Cannot get device info: %s\n",
                 rte_strerror(-ret));
    
    // Configure device
    ret = rte_eth_dev_configure(port, rx_rings, tx_rings, &port_conf);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Cannot configure device\n");
    
    // Setup RX queue
    ret = rte_eth_rx_queue_setup(port, 0, nb_rxd,
                                  rte_eth_dev_socket_id(port),
                                  NULL, mbuf_pool);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Cannot setup RX queue\n");
    
    // Setup TX queue
    ret = rte_eth_tx_queue_setup(port, 0, nb_txd,
                                  rte_eth_dev_socket_id(port), NULL);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Cannot setup TX queue\n");
    
    // Start device
    ret = rte_eth_dev_start(port);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Cannot start device\n");
    
    // Enable promiscuous mode
    rte_eth_promiscuous_enable(port);
}

// Main packet processing loop
static int lcore_main(void *arg) {
    uint16_t port = 0;
    struct rte_mbuf *bufs[BURST_SIZE];
    
    printf("Core %u forwarding packets\n", rte_lcore_id());
    
    while (1) {
        // Receive burst of packets
        const uint16_t nb_rx = rte_eth_rx_burst(port, 0, bufs, BURST_SIZE);
        
        if (nb_rx == 0)
            continue;
        
        // Process each packet
        for (uint16_t i = 0; i < nb_rx; i++) {
            struct rte_mbuf *m = bufs[i];
            struct rte_ether_hdr *eth_hdr;
            
            eth_hdr = rte_pktmbuf_mtod(m, struct rte_ether_hdr *);
            
            // Simple L2 forwarding: swap MAC addresses
            struct rte_ether_addr tmp_mac;
            rte_ether_addr_copy(&eth_hdr->s_addr, &tmp_mac);
            rte_ether_addr_copy(&eth_hdr->d_addr, &eth_hdr->s_addr);
            rte_ether_addr_copy(&tmp_mac, &eth_hdr->d_addr);
        }
        
        // Send burst of packets back
        const uint16_t nb_tx = rte_eth_tx_burst(port, 0, bufs, nb_rx);
        
        // Free unsent packets
        if (nb_tx < nb_rx) {
            for (uint16_t i = nb_tx; i < nb_rx; i++)
                rte_pktmbuf_free(bufs[i]);
        }
    }
    
    return 0;
}

int main(int argc, char **argv) {
    struct rte_mempool *mbuf_pool;
    unsigned nb_ports;
    uint16_t portid;
    
    // Initialize DPDK
    int ret = dpdk_init(argc, argv);
    argc -= ret;
    argv += ret;
    
    // Check ports
    nb_ports = rte_eth_dev_count_avail();
    if (nb_ports == 0)
        rte_exit(EXIT_FAILURE, "No Ethernet ports\n");
    
    // Create packet buffer pool
    mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * nb_ports,
                                        MBUF_CACHE_SIZE, 0,
                                        RTE_MBUF_DEFAULT_BUF_SIZE,
                                        rte_socket_id());
    if (mbuf_pool == NULL)
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");
    
    // Initialize ports
    RTE_ETH_FOREACH_DEV(portid) {
        port_init(portid, mbuf_pool);
    }
    
    // Launch processing on each lcore
    rte_eal_mp_remote_launch(lcore_main, NULL, CALL_MAIN);
    
    return 0;
}
```

The mbuf (memory buffer) structure represents packets in DPDK. Mbuf pools pre-allocate packet buffers from huge pages. Applications receive mbufs on packet arrival and return them after transmission. This pooling eliminates per-packet allocation overhead.

DPDK's lockless ring buffers enable efficient inter-thread communication. Multiple producer/consumer rings move packets between stages without locks. This design supports pipeline architectures where different cores handle different processing stages.

Zero-copy semantics preserve performance. DPDK passes mbuf pointers between processing stages rather than copying packet data. Direct NIC-to-mbuf DMA eliminates kernel copying. Some NICs support packet splitting, placing headers and payloads in separate buffers for efficient processing.

Flow classification distributes packets across cores. RSS (Receive Side Scaling) hashes packet headers, directing flows to specific queues and cores. This distribution maintains per-flow ordering while enabling parallel processing across cores. Intel Flow Director and similar technologies provide more sophisticated classification.

DPDK applications typically use run-to-completion or pipeline models. Run-to-completion assigns each core full packet processing from receive to transmit. Pipeline models split processing into stages, passing packets between cores through rings. The choice depends on packet processing complexity and performance requirements.

### Zero-Copy Networking Techniques

Zero-copy networking eliminates data copying between kernel and user space, significantly improving throughput and reducing CPU utilization. Traditional socket I/O copies data from NIC to kernel buffers to user-space buffers. Each copy consumes CPU cycles and memory bandwidth. Zero-copy techniques avoid these copies.

The sendfile() system call implements zero-copy for file-to-socket transmission. It transfers data directly from the page cache to the socket buffer without user-space intervention. Web servers and file transfer applications benefit significantly from sendfile() when serving static content.

```c
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

// Zero-copy file transmission
ssize_t send_file_zerocopy(int socket_fd, const char *filename) {
    int file_fd;
    struct stat file_stat;
    off_t offset = 0;
    ssize_t sent;
    
    // Open file
    file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
        perror("open");
        return -1;
    }
    
    // Get file size
    if (fstat(file_fd, &file_stat) < 0) {
        perror("fstat");
        close(file_fd);
        return -1;
    }
    
    // Send entire file using zero-copy
    sent = sendfile(socket_fd, file_fd, &offset, file_stat.st_size);
    
    close(file_fd);
    return sent;
}
```

Memory-mapped I/O provides another zero-copy approach. Mapping files into process address space with mmap() allows direct memory access without read()/write() calls. Modifications appear in the file automatically with MAP_SHARED. Network applications can mmap() packet buffers shared with the kernel, eliminating copies.

Splice and tee system calls enable zero-copy pipe-based data movement. Splice() moves data between file descriptors through a pipe without user-space copying. This technique suits proxy applications moving data between sockets. Tee() duplicates data in pipes without consuming it, enabling packet inspection without copying.

```c
#include <fcntl.h>
#include <sys/types.h>

#define SPLICE_SIZE (64 * 1024)

// Zero-copy proxy between two sockets
void splice_proxy(int client_fd, int server_fd) {
    int pipe_fd[2];
    ssize_t bytes;
    
    // Create pipe for zero-copy transfer
    if (pipe(pipe_fd) < 0) {
        perror("pipe");
        return;
    }
    
    // Splice from client to pipe
    bytes = splice(client_fd, NULL, pipe_fd[1], NULL,
                   SPLICE_SIZE, SPLICE_F_MOVE | SPLICE_F_MORE);
    
    if (bytes > 0) {
        // Splice from pipe to server
        splice(pipe_fd[0], NULL, server_fd, NULL,
               bytes, SPLICE_F_MOVE | SPLICE_F_MORE);
    }
    
    close(pipe_fd[0]);
    close(pipe_fd[1]);
}
```

MSG_ZEROCOPY flag for send() enables zero-copy transmission with explicit completion notification. The kernel references user-space buffers directly, returning them through error queue messages after NIC completes transmission. This approach requires careful buffer management but achieves maximum throughput.

Receive zero-copy is harder than transmit zero-copy. Packet reception typically requires kernel buffering before applications know whether they want the data. Some approaches use memory mapping or dedicated hardware support. Technologies like DPDK bypass the kernel entirely, implementing zero-copy receive in user space.

TCP zero-copy faces challenges from protocol requirements. TCP segmentation, retransmission, and reordering complicate zero-copy implementations. TSO (TCP Segmentation Offload) and GSO (Generic Segmentation Offload) help by deferring segmentation to the NIC. These offloads reduce CPU usage and enable larger effective zero-copy transfers.

---

## Interview Question Deep Dives

### Question 1: How would you debug a memory leak in a production routing daemon?

Memory leaks in production routing daemons require systematic diagnosis and minimal disruption to service. The approach combines monitoring, analysis tools, and code review.

**Initial Detection and Monitoring**

Memory leaks manifest as steadily growing memory usage despite stable traffic and neighbor counts. The first step involves confirming the leak through monitoring. Examining /proc/[pid]/status provides current memory statistics. The VmRSS (Resident Set Size) field shows physical memory usage, while VmSize shows virtual memory allocation. Tracking these values over hours or days reveals growth patterns.

```bash
# Monitor memory growth
watch -n 60 'grep -E "VmSize|VmRSS|VmData|VmStk" /proc/$(pidof routing_daemon)/status'

# Check memory map for unexpected growth
cat /proc/$(pidof routing_daemon)/maps | awk '{print $1}' | \
  while read addr; do
    start=$(echo $addr | cut -d'-' -f1)
    end=$(echo $addr | cut -d'-' -f2)
    size=$(printf "%d\n" "0x$end - 0x$start" | bc)
    echo "$addr: $size bytes"
  done | sort -t: -k2 -n
```

The /proc/[pid]/smaps file provides detailed memory mapping information. Examining heap growth pinpoints whether leaks occur in dynamically allocated memory. Stack growth might indicate excessive recursion or large local variables.

**Using Valgrind for Leak Detection**

Valgrind's memcheck tool detects memory leaks, though it significantly slows execution. For production systems, run Valgrind in a test environment replicating production conditions. Load configuration, establish protocol sessions, and run traffic patterns matching production.

```bash
# Run daemon under Valgrind
valgrind --leak-check=full --show-leak-kinds=all \
         --track-origins=yes --log-file=valgrind.log \
         ./routing_daemon --config production.conf

# After running representative workload, examine report
less valgrind.log
```

Valgrind reports show allocation call stacks for leaked memory. Direct leaks are memory blocks with no remaining references. Indirect leaks are blocks referenced only by other leaked blocks. Possible leaks have ambiguous pointer references. Focusing on direct leaks first usually reveals root causes.

**Live Production Analysis**

For running production daemons, gdb can attach without restarting. Taking snapshots of heap state at different times reveals growing allocations.

```bash
# Attach gdb without stopping process
gdb -p $(pidof routing_daemon)

# In gdb, examine heap
(gdb) info proc mappings
(gdb) dump memory heap_snapshot1.bin <heap_start> <heap_end>

# Detach and reattach later
(gdb) detach

# Compare snapshots to find growth
```

Modern allocators like glibc's malloc can dump statistics. Calling malloc_stats() or analyzing /proc/[pid]/malloc shows allocation patterns. The mallinfo() function programmatically retrieves statistics.

**Code Review Focus Areas**

Memory leaks often occur in error paths. When functions allocate resources early but encounter errors before freeing them, leaks result. Reviewing error handling code for missing free() calls often reveals leaks.

```c
// Leak example in routing protocol
struct neighbor_entry* create_neighbor(uint32_t ip) {
    struct neighbor_entry *nbr = malloc(sizeof(*nbr));
    if (!nbr) return NULL;
    
    nbr->timers = malloc(sizeof(struct timer_list));
    if (!nbr->timers) {
        // BUG: Forgot to free nbr before returning
        return NULL;  // Leaks nbr allocation
    }
    
    return nbr;
}

// Fixed version
struct neighbor_entry* create_neighbor_fixed(uint32_t ip) {
    struct neighbor_entry *nbr = malloc(sizeof(*nbr));
    if (!nbr) return NULL;
    
    nbr->timers = malloc(sizeof(struct timer_list));
    if (!nbr->timers) {
        free(nbr);  // Properly clean up
        return NULL;
    }
    
    return nbr;
}
```

Protocol implementations often leak in state machine transitions. When neighbors go down or sessions reset, cleanup code must free all associated resources. Examining state transition functions for complete resource deallocation prevents leaks.

Reference counting errors cause leaks when objects maintain usage counts. If code increments references without corresponding decrements, objects never free. Searching for imbalanced reference operations reveals these leaks.

**Prevention Strategies**

Implementing custom allocation wrappers aids leak detection. Wrappers track all allocations and frees, enabling runtime leak detection.

```c
#ifdef DEBUG_MEMORY
static struct {
    void *ptr;
    size_t size;
    const char *file;
    int line;
} alloc_table[10000];
static int alloc_count = 0;

void* debug_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr && alloc_count < 10000) {
        alloc_table[alloc_count].ptr = ptr;
        alloc_table[alloc_count].size = size;
        alloc_table[alloc_count].file = file;
        alloc_table[alloc_count].line = line;
        alloc_count++;
    }
    return ptr;
}

#define malloc(size) debug_malloc(size, __FILE__, __LINE__)
#endif
```

Memory pools for fixed-size objects eliminate fragmentation and simplify leak detection. Pre-allocating neighbor structures, route entries, and packet buffers from pools makes tracking easier. Pools maintain usage statistics revealing leaks when free counts don't match allocations.

Using smart pointers or reference-counted structures in C (via libraries or custom implementation) prevents many leaks. Automatic cleanup when references reach zero eliminates manual free() calls.

### Question 2: Explain the journey of a packet from NIC to application

Understanding packet flow through the Linux network stack reveals optimization opportunities and debugging strategies essential for protocol implementation.

**Hardware Reception**

When a packet arrives at the NIC, the hardware performs initial processing. Modern NICs implement receive-side scaling (RSS), hashing packet headers to determine which receive queue handles the packet. This distribution spreads load across multiple CPU cores.

The NIC uses DMA to write the packet into pre-allocated memory buffers provided by the driver. These buffers typically exist in a ring structure where the driver provides descriptors containing buffer addresses, and the NIC writes packets and updates descriptors.

After writing the packet, the NIC generates an interrupt to notify the CPU. The interrupt handler runs in interrupt context with strict timing requirements. It cannot perform lengthy operations or sleep. The handler typically acknowledges the interrupt, schedules a softirq for deferred processing, and returns quickly.

**NAPI and SoftIRQ Processing**

New API (NAPI) reduces interrupt overhead by polling when traffic is high. When the interrupt handler schedules a softirq, it disables further interrupts for that queue. The softirq handler polls the receive queue, processing multiple packets before re-enabling interrupts.

```c
// Simplified NAPI polling (conceptual)
static int napi_poll(struct napi_struct *napi, int budget) {
    int work_done = 0;
    struct sk_buff *skb;
    
    while (work_done < budget) {
        skb = receive_packet_from_nic();
        if (!skb)
            break;  // No more packets
        
        // Process packet up the stack
        netif_receive_skb(skb);
        work_done++;
    }
    
    if (work_done < budget) {
        // All packets processed, re-enable interrupts
        napi_complete(napi);
        enable_nic_interrupts();
    }
    
    return work_done;
}
```

The softirq context (specifically NET_RX_SOFTIRQ) runs packet processing. Multiple softirqs can execute simultaneously on different CPUs, but only one softirq of each type runs per CPU. This design enables parallel packet processing while avoiding excessive synchronization.

**Socket Buffer Allocation**

The driver allocates a socket buffer (sk_buff) structure to represent the packet. This structure contains pointers to actual packet data plus extensive metadata including protocol headers, device information, timestamps, and checksum status.

```c
// sk_buff structure (simplified key fields)
struct sk_buff {
    struct sk_buff *next;
    struct sk_buff *prev;
    
    struct net_device *dev;      // Receiving device
    
    unsigned char *head;          // Buffer start
    unsigned char *data;          // Data start
    unsigned char *tail;          // Data end
    unsigned char *end;           // Buffer end
    
    __u32 priority;
    __u16 protocol;               // Ethernet protocol
    
    struct {
        struct tcphdr *th;        // Transport header
        struct iphdr *iph;        // Network header
        struct ethhdr *eth;       // Link header
    } headers;
    
    // Checksums, timestamps, routing info, etc.
};
```

The sk_buff design enables efficient header manipulation. As packets traverse protocol layers, code adjusts pointers rather than copying data. The skb_push() function adds headers by moving the data pointer backward. The skb_pull() function removes headers by advancing it forward.

**Network Layer Processing**

The network layer receives packets through netif_receive_skb(). This function examines the Ethernet header's protocol field to determine the next layer. For IP packets, it calls ip_rcv().

The IP layer first validates the packet: checking version, header length, checksum, and total length. Invalid packets are dropped with statistics updated. Valid packets proceed to routing lookup.

```c
// Conceptual IP receive processing
int ip_rcv(struct sk_buff *skb) {
    struct iphdr *iph = ip_hdr(skb);
    
    // Validate packet
    if (iph->version != 4)
        goto drop;
    
    if (iph->ihl < 5)
        goto drop;
    
    if (!pskb_may_pull(skb, iph->ihl * 4))
        goto drop;
    
    if (ip_fast_csum((u8 *)iph, iph->ihl))
        goto drop;
    
    // Netfilter hook: NF_INET_PRE_ROUTING
    NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING, skb, dev, NULL,
            ip_rcv_finish);
    
    return NET_RX_SUCCESS;
    
drop:
    kfree_skb(skb);
    return NET_RX_DROP;
}
```

Netfilter hooks enable packet filtering and modification at strategic points. The NF_INET_PRE_ROUTING hook fires before routing decisions. Firewall rules, NAT translations, and connection tracking operate at this hook. If filters accept the packet, processing continues to routing.

**Routing Decision**

The routing subsystem determines packet destination through FIB lookup. The lookup uses the destination IP address to find the matching route with the longest prefix. The FIB structure optimizes these lookups using compressed data structures like LC-trie.

```c
// Simplified routing lookup
static int ip_route_input(struct sk_buff *skb, __be32 daddr, __be32 saddr,
                          u8 tos, struct net_device *dev) {
    struct fib_result res;
    
    // Lookup route in FIB
    if (fib_lookup(&init_net, daddr, &res) == 0) {
        if (res.type == RTN_LOCAL) {
            // Packet for local delivery
            skb_dst_set(skb, &res.dst);
            return ip_local_deliver(skb);
        } else {
            // Packet for forwarding
            return ip_forward(skb);
        }
    }
    
    // No route found
    return -ENETUNREACH;
}
```

Three outcomes result from routing lookup. If the destination matches a local address, the packet undergoes local delivery. If the destination matches a forwarding route and forwarding is enabled, the packet forwards out another interface. If no route exists, the packet is dropped and an ICMP "Destination Unreachable" message may be sent.

**Local Delivery and Transport Layer**

For locally-destined packets, ip_local_deliver() passes them to the transport layer after another Netfilter hook (NF_INET_LOCAL_IN). The protocol field in the IP header determines which transport protocol handler receives the packet.

TCP and UDP handlers perform demultiplexing, finding the socket matching the packet's source/destination IP addresses and ports. The kernel maintains hash tables for efficient socket lookup.

```c
// Simplified TCP socket lookup
struct sock *tcp_v4_lookup(struct sk_buff *skb) {
    struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *th = tcp_hdr(skb);
    struct sock *sk;
    
    // Look up in established connection hash table
    sk = __inet_lookup_established(&tcp_hashinfo,
                                   iph->saddr, th->source,
                                   iph->daddr, th->dest,
                                   skb->dev->ifindex);
    
    if (sk)
        return sk;
    
    // Look up in listening socket hash table
    sk = __inet_lookup_listener(&tcp_hashinfo,
                                iph->daddr, th->dest,
                                skb->dev->ifindex);
    
    return sk;
}
```

For TCP, the packet enters the TCP state machine. Processing depends on connection state. For ESTABLISHED connections, data is queued in the socket's receive buffer. For SYN packets on listening sockets, the three-way handshake begins. TCP handles acknowledgments, retransmissions, congestion control, and flow control.

**Socket Buffers and Application Delivery**

Data accumulates in the socket's receive buffer until the application reads it. The buffer size limits how much data the kernel can queue. When the buffer fills, TCP's flow control mechanism advertises a zero window, stopping the sender.

When an application calls recv() or read(), the kernel copies data from the socket buffer to the user-space buffer provided by the application. This copy crosses the kernel-user space boundary. After copying, the kernel frees the socket buffer space, allowing more data to arrive.

```c
// Application receiving data
ssize_t receive_data(int sockfd, char *buffer, size_t length) {
    ssize_t bytes_read;
    
    // This call may block until data arrives
    bytes_read = recv(sockfd, buffer, length, 0);
    
    if (bytes_read > 0) {
        // Data successfully received
        // Socket buffer space freed, window updates sent
        printf("Received %zd bytes\n", bytes_read);
    } else if (bytes_read == 0) {
        // Connection closed
        printf("Connection closed by peer\n");
    } else {
        // Error occurred
        perror("recv");
    }
    
    return bytes_read;
}
```

For non-blocking sockets with epoll, the application polls for readability. When data arrives, epoll notifies the application through epoll_wait(). This approach allows handling many connections without blocking threads.

**Performance Optimizations**

Several optimizations accelerate this path. GRO (Generic Receive Offload) combines multiple packets of the same flow into larger packets before processing. This coalescing reduces per-packet overhead.

Receive Packet Steering (RPS) distributes packets to CPUs based on flow hash when the NIC doesn't support hardware RSS. Receive Flow Steering (RFS) extends RPS by directing packets to the CPU running the application consuming them, improving cache locality.

Hardware offloads reduce CPU load. Checksum offload makes the NIC compute or verify checksums. TCP Segmentation Offload (TSO) and Generic Segmentation Offload (GSO) defer packet segmentation to the NIC or late in the transmit path.

XDP (eXpress Data Path) provides the fastest path by processing packets before sk_buff allocation. eBPF programs can drop, redirect, or modify packets at the driver level, achieving millions of packets per second filtering rates.

### Question 3: How does Linux kernel routing table lookup work?

The Linux kernel routing subsystem uses sophisticated data structures and algorithms to achieve fast lookups supporting millions of routes while maintaining efficient updates.

**Routing Tables and FIB Structure**

Linux maintains two primary routing structures: the Routing Information Base (RIB) and the Forwarding Information Base (FIB). The RIB contains all routes from all sources, including static routes, directly connected networks, and routes from routing protocols. The FIB contains a compressed subset optimized for forwarding decisions.

Multiple routing tables can exist, selected by policy routing rules. The main table (ID 254) handles most traffic. The local table (ID 255) contains routes to local addresses. Custom tables enable advanced routing like VPN routing or multi-homing.

```c
// Conceptual routing table structure
struct fib_table {
    u32 tb_id;                     // Table ID
    struct hlist_node tb_hlist;    // Hash list linkage
    
    // LC-trie root for IPv4 lookups
    struct trie *trie;
    
    // Statistics
    atomic_t tb_entries;
    atomic_t tb_lookups;
};

// Route entry in FIB
struct fib_info {
    struct hlist_node fib_hash;
    int fib_dead;
    unsigned int fib_flags;
    unsigned char fib_scope;
    unsigned char fib_type;
    __be32 fib_prefsrc;            // Preferred source address
    u32 fib_priority;              // Route metric
    u32 fib_table;
    
    struct fib_nh fib_nh[0];       // Next-hop information
};
```

**LC-Trie: Level Compressed Trie**

The FIB uses an LC-trie data structure for IPv4 lookups. This compressed trie variant stores prefixes efficiently while enabling fast longest-prefix-match lookups. The trie compresses paths where nodes have only one child, reducing memory usage and cache misses.

Each trie node represents a bit position in the IP address. Branching occurs only where routes diverge. For example, routes 10.0.0.0/8 and 10.1.0.0/16 share the first 8 bits, so the trie shares the path until bit 9 where they diverge.

```c
// Simplified LC-trie node structure
struct tnode {
    t_key key;                     // Prefix
    unsigned char pos;             // Bit position
    unsigned char bits;            // Number of bits covered
    
    union {
        struct tnode *tnode[0];    // Internal node children
        struct leaf_info *leaf;    // Leaf node info
    };
};

// Leaf information for routes
struct leaf_info {
    struct hlist_node hlist;
    int plen;                      // Prefix length
    struct fib_alias *fib_alias;   // Route information
};
```

Lookup traverses the trie by extracting bits from the destination address at each node's position. The bits determine which child to follow. When reaching a leaf, the algorithm checks if the destination matches the prefix. Multiple prefixes can match, so the algorithm tracks the longest matching prefix encountered.

**Lookup Algorithm**

The lookup algorithm performs longest-prefix matching, finding the route with the most specific prefix covering the destination address. This operation must be fast, as every forwarded packet requires a lookup.

```c
// Conceptual FIB lookup algorithm
int fib_lookup(struct net *net, __be32 daddr, struct fib_result *res) {
    struct fib_table *tb;
    struct tnode *n;
    int pos, bits;
    t_key key = ntohl(daddr);
    struct fib_alias *fa = NULL;
    int best_plen = 0;
    
    // Get main routing table
    tb = fib_get_table(net, RT_TABLE_MAIN);
    if (!tb)
        return -ENETUNREACH;
    
    // Traverse trie
    n = tb->trie->root;
    
    while (n) {
        // Check if internal node or leaf
        if (IS_LEAF(n)) {
            // Found leaf, check prefixes
            struct leaf_info *li;
            
            hlist_for_each_entry(li, &n->list, hlist) {
                // Check if prefix matches and is longer than current best
                if ((key ^ n->key) >> (32 - li->plen) == 0) {
                    if (li->plen > best_plen) {
                        best_plen = li->plen;
                        fa = li->fib_alias;
                    }
                }
            }
            break;
        }
        
        // Internal node - extract bits and follow child
        pos = n->pos;
        bits = n->bits;
        int child_index = (key >> (32 - pos - bits)) & ((1 << bits) - 1);
        n = n->child[child_index];
    }
    
    if (fa) {
        // Found matching route
        res->type = fa->fa_type;
        res->nh = fa->fa_info->fib_nh;
        return 0;
    }
    
    return -ENETUNREACH;
}
```

The algorithm's time complexity is O(W) where W is the address width (32 for IPv4). In practice, lookups typically examine only a few nodes due to trie compression. Modern hardware caches accelerate memory accesses, making lookups very fast.

**Caching and Optimization**

Earlier Linux versions maintained a route cache storing recent lookup results. This cache provided O(1) lookups for cached routes. However, cache invalidation complexity and security concerns (cache poisoning attacks) led to its removal in kernel 3.6.

Modern kernels rely on CPU caches and optimized FIB structure. The LC-trie's memory layout improves cache locality. Frequently-accessed routes naturally remain in CPU cache. This approach scales better than an explicit route cache, especially for high route counts.

**Route Updates and Synchronization**

Adding or removing routes modifies the FIB structure. These updates must synchronize with concurrent lookups occurring on other CPUs. Read-Copy-Update (RCU) enables lockless lookups while allowing updates.

RCU allows readers to proceed without locks while writers create modified copies of data structures. After updating, writers wait for a grace period ensuring all readers holding old pointers finish. Then old structures are freed. This mechanism enables millions of lookups per second even during route updates.

```c
// Simplified route insertion with RCU
int fib_table_insert(struct fib_table *tb, struct fib_config *cfg) {
    struct tnode *new_node;
    struct tnode *old_node;
    
    // Allocate new node
    new_node = alloc_tnode();
    // ... initialize new_node ...
    
    // Insert into trie (RCU-protected)
    old_node = rcu_dereference_protected(parent->child[index],
                                         lockdep_is_held(&fib_lock));
    
    rcu_assign_pointer(parent->child[index], new_node);
    
    // Wait for grace period, then free old node
    if (old_node)
        call_rcu(&old_node->rcu, free_tnode);
    
    return 0;
}
```

**IPv6 Routing**

IPv6 routing uses different structures due to the 128-bit address space. The massive address space makes trie-based lookup less efficient. Instead, IPv6 uses a more complex approach combining radix trees with additional optimizations.

IPv6 prefix lengths vary widely, from /128 (host routes) to /48 (typical site prefixes) to /10 (regional allocations). The routing structure must handle this diversity efficiently. The implementation uses multiple techniques including prefix aggregation and specialized structures for common prefix lengths.

**Policy Routing**

Policy routing extends basic routing by selecting tables based on criteria beyond destination address. Source address, packet marks, incoming interface, or TOS fields can influence table selection. The ip rule command configures these policies.

Policy routing enables advanced scenarios like routing traffic from different source networks through different gateways, or routing marked traffic (from iptables) differently than normal traffic. Each rule specifies matching criteria and a table to consult.

### Question 4: What's the difference between soft and hard IRQs in network processing?

Understanding interrupt handling is crucial for optimizing network performance and diagnosing problems. Linux uses a two-stage interrupt handling model to balance responsiveness and throughput.

**Hard IRQs: Hardware Interrupts**

Hard IRQs (hardware interrupts) are signals from hardware devices demanding immediate CPU attention. When a network packet arrives, the NIC generates an interrupt. The CPU stops its current work, saves state, and executes the interrupt handler.

Hard IRQ handlers run in interrupt context with strict constraints. They cannot sleep, block, or perform lengthy operations. Other interrupts may be disabled during execution, so handlers must complete quickly to maintain system responsiveness. The handler's job is acknowledging the interrupt and scheduling deferred work.

```c
// Hard IRQ handler (conceptual NIC driver)
static irqreturn_t nic_irq_handler(int irq, void *dev_id) {
    struct net_device *dev = dev_id;
    struct nic_private *priv = netdev_priv(dev);
    u32 status;
    
    // Read interrupt status register
    status = readl(priv->base + NIC_INT_STATUS);
    
    if (status & NIC_INT_RX) {
        // Packet received - disable further RX interrupts
        writel(NIC_INT_RX_DISABLE, priv->base + NIC_INT_MASK);
        
        // Schedule NAPI polling (softirq)
        napi_schedule(&priv->napi);
    }
    
    if (status & NIC_INT_TX) {
        // Transmit complete - handle in softirq or tasklet
        // Free transmitted buffers, update statistics
    }
    
    // Acknowledge interrupt
    writel(status, priv->base + NIC_INT_CLEAR);
    
    return IRQ_HANDLED;
}
```

The handler disables further interrupts from the device to prevent interrupt storms. It schedules a softirq for packet processing, then returns quickly. This approach minimizes time spent in interrupt context, crucial for system stability and responsiveness.

Hard IRQ execution shows in /proc/interrupts. Each line represents an interrupt source, showing counts per CPU. Network interrupts typically appear as ethernet device names or MSI-X vectors. Monitoring these counts reveals interrupt distribution and potential issues.

**Soft IRQs: Deferred Processing**

Soft IRQs (software interrupts) provide a mechanism for deferred work with higher priority than normal processes but lower than hard IRQs. The kernel defines several softirq types including NET_TX_SOFTIRQ and NET_RX_SOFTIRQ for network processing.

Softirqs run with interrupts enabled, allowing hardware interrupts to preempt them. This design prevents network processing from blocking time-critical interrupts. However, softirqs have priority over normal processes, ensuring network traffic gets CPU time.

```c
// Network receive softirq (conceptual)
static void net_rx_action(struct softirq_action *h) {
    struct softnet_data *sd = this_cpu_ptr(&softnet_data);
    unsigned long time_limit = jiffies + 2;  // 2 jiffies time budget
    int budget = netdev_budget;               // Packet budget
    
    LIST_HEAD(list);
    
    // Get list of devices to poll
    list_splice_init(&sd->poll_list, &list);
    
    while (!list_empty(&list)) {
        struct napi_struct *napi;
        int work;
        
        napi = list_first_entry(&list, struct napi_struct, poll_list);
        list_del_init(&napi->poll_list);
        
        // Poll device for packets
        work = napi->poll(napi, budget);
        budget -= work;
        
        // Check if we've exceeded time or packet budget
        if (budget <= 0 || time_after(jiffies, time_limit)) {
            // Re-schedule and yield
            __raise_softirq_irqoff(NET_RX_SOFTIRQ);
            break;
        }
        
        // If device has more packets, re-queue it
        if (work >= budget) {
            list_add_tail(&napi->poll_list, &sd->poll_list);
        }
    }
}
```

The softirq respects time and packet budgets preventing network processing from monopolizing the CPU. If budgets expire with more work remaining, the softirq reschedules itself, yielding to other tasks. This fairness ensures interactive responsiveness even under network load.

**NAPI: Interrupt Mitigation**

New API (NAPI) bridges hard and soft IRQs, switching between interrupt-driven and polled modes based on load. Under light load, the device uses interrupts for low latency. Under heavy load, NAPI disables interrupts and polls, reducing overhead.

The transition occurs automatically. When an interrupt arrives and packets await processing, the hard IRQ handler disables interrupts and schedules NAPI polling. The softirq polls packets until the queue empties, then re-enables interrupts. This adaptive behavior optimizes for both latency and throughput.

```c
// NAPI poll function (driver-specific)
static int driver_napi_poll(struct napi_struct *napi, int budget) {
    struct nic_private *priv = container_of(napi, struct nic_private, napi);
    int work_done = 0;
    
    // Process up to budget packets
    while (work_done < budget) {
        struct sk_buff *skb;
        
        // Check if packet available
        if (!nic_has_packet(priv))
            break;
        
        // Allocate sk_buff and DMA packet data
        skb = nic_receive_packet(priv);
        if (!skb)
            break;
        
        // Pass to network stack
        napi_gro_receive(napi, skb);
        work_done++;
    }
    
    // If processed fewer than budget, no more packets
    if (work_done < budget) {
        napi_complete_done(napi, work_done);
        
        // Re-enable interrupts
        nic_enable_interrupts(priv);
    }
    
    return work_done;
}
```

GRO (Generic Receive Offload) operates within NAPI polling. It coalesces packets of the same flow into larger sk_buffs before delivering them up the stack. This aggregation reduces per-packet processing overhead, significantly improving throughput for bulk transfers.

**CPU Affinity and IRQ Balancing**

Interrupt affinity determines which CPUs handle specific interrupts. The /proc/irq/[number]/smp_affinity file controls affinity via bitmasks. Setting affinity pins interrupts to specific CPUs, useful for performance tuning.

Multi-queue NICs create multiple IRQ vectors, each with its own queue and interrupt. Distributing these across CPUs enables parallel packet processing. RSS (Receive Side Scaling) hashes packet headers to determine queue assignment, maintaining per-flow ordering while spreading load.

```bash
# Check interrupt distribution
cat /proc/interrupts | grep eth0

# Set IRQ affinity to CPU 2
echo 4 > /proc/irq/89/smp_affinity  # 4 = 0b0100 = CPU 2

# For multi-queue NIC, distribute interrupts across CPUs
# IRQ 89-92 are eth0's four queues
echo 1 > /proc/irq/89/smp_affinity  # CPU 0
echo 2 > /proc/irq/90/smp_affinity  # CPU 1
echo 4 > /proc/irq/91/smp_affinity  # CPU 2
echo 8 > /proc/irq/92/smp_affinity  # CPU 3
```

The irqbalance daemon automatically distributes interrupts across CPUs for optimal performance. However, manual tuning may outperform automatic distribution for specific workloads. Disabling irqbalance and setting affinity manually gives precise control.

**Monitoring and Debugging**

Several tools reveal interrupt behavior. The /proc/softirqs file shows softirq execution counts per CPU per type. Comparing NET_RX_SOFTIRQ counts across CPUs reveals load distribution. Imbalances indicate configuration issues or hardware problems.

```bash
# Monitor softirq activity
watch -n 1 'cat /proc/softirqs'

# Check if softirqs are consuming too much CPU
mpstat -P ALL 1

# %soft column shows softirq CPU usage
# High %soft indicates heavy network processing or I/O
```

High softirq CPU usage indicates network processing saturation. Solutions include adding NICs, enabling hardware offloads, or using kernel-bypass technologies like DPDK. Understanding the hard/soft IRQ split guides optimization decisions.

**Threaded IRQs**

Linux supports threaded IRQs where the hard IRQ handler just wakes a kernel thread performing actual processing. This approach suits devices without hard real-time requirements, allowing interrupt handling to sleep or block. Network drivers typically avoid threaded IRQs due to performance concerns, but they're useful for some device types.

The trade-off involves latency versus flexibility. Hard IRQ handlers have lowest latency but strictest constraints. Softirqs add slight latency but allow interrupts. Threaded IRQs add more latency but enable blocking operations. Network performance optimization typically uses hard IRQ + softirq + NAPI for best throughput and latency balance.
