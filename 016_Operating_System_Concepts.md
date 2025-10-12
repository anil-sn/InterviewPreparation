# Operating System Concepts for Routers and Switches

## Introduction: Why OS Concepts Matter in Network Devices

When you think of a router or switch, you might picture a box that forwards packets. But beneath that simple exterior lies a sophisticated embedded operating system managing memory, processes, and hardware resources with extreme efficiency. Understanding these OS fundamentals isn't optional—it's critical for anyone working with network infrastructure, debugging performance issues, or developing network software.

Network devices face unique challenges that standard computer operating systems don't encounter at the same scale. A core router might need to process millions of packets per second while maintaining sub-millisecond latency, all with limited memory and processing power. This document explores the OS concepts that make this possible, focusing on the brutal realities and clever solutions that keep networks running.

## Memory Architecture in Network Devices

### The Memory Constraint Reality

Network devices operate under severe memory constraints compared to general-purpose computers. A high-end server might have 128GB of RAM, but a router often operates with a few hundred megabytes to a few gigabytes. Every byte matters because memory directly impacts how many routes you can store, how many concurrent connections you can track, and how large your packet buffers can be.

This constraint isn't arbitrary—it's driven by cost, power consumption, and heat dissipation. Memory chips consume power and generate heat, both precious commodities in a rack-mounted device that might be crammed into a data center alongside hundreds of siblings.

### Memory Hierarchy: Why Speed Costs More Than Space

The memory hierarchy exists because we can't build infinite fast memory. In network devices, this hierarchy becomes even more pronounced. At the top, processor registers provide the fastest access (typically 1 clock cycle), followed by multiple cache levels, then main RAM, and finally persistent storage like flash memory.

Consider a simple packet forwarding operation. The device must look up the destination address in a routing table. If that routing table entry sits in L1 cache, lookup takes perhaps 3-4 nanoseconds. If it's in main RAM, that same lookup might take 100 nanoseconds. This 25x difference multiplied across millions of packets per second means the difference between wire-speed forwarding and congestion.

Modern network processors employ sophisticated cache hierarchies. L1 cache typically runs at processor speed (maybe 32-128KB split between instruction and data). L2 cache is larger but slower (256KB to several MB). L3 cache, when present, might be several megabytes but takes 40-50 cycles to access. Understanding this hierarchy helps you write code that performs well in the real world.

Here's what cache-friendly code looks like in practice:

```c
/* Cache-unfriendly approach - random access pattern */
for (int i = 0; i < num_packets; i++) {
    int dest = packets[i].dest_port;
    forwarding_table[dest].packets_sent++;
}

/* Cache-friendly approach - sequential access with batching */
struct packet_batch batch[BATCH_SIZE];
int batch_count = 0;

for (int i = 0; i < num_packets; i++) {
    batch[batch_count++] = packets[i];
    
    if (batch_count == BATCH_SIZE) {
        /* Process entire batch, likely hitting same cache lines */
        process_batch(batch, batch_count);
        batch_count = 0;
    }
}
```

The second approach groups operations, increasing the likelihood that data stays in cache between accesses.

### RAM Types: SRAM vs DRAM Trade-offs

Network devices use both Static RAM (SRAM) and Dynamic RAM (DRAM), but for very different purposes. SRAM is fast, expensive, and doesn't need refreshing. Each SRAM bit requires 6 transistors, making it large and power-hungry. DRAM uses 1 transistor plus a capacitor per bit, making it dense and cheap, but it requires constant refreshing because capacitors leak charge.

In a typical router architecture, SRAM appears in caches and small high-speed buffers within the network processor. DRAM provides the bulk main memory. The routing table might live in DRAM, but the most frequently accessed entries get cached in SRAM. Hardware Content Addressable Memory (CAM) and Ternary CAM (TCAM), specialized forms of SRAM, enable parallel lookups for MAC addresses and ACL rules.

### ROM, Flash, and Persistent Storage

Read-Only Memory (ROM) in its pure form is rarely used in modern network devices. Instead, Flash memory and EEPROM serve as non-volatile storage. Flash stores the operating system image, configuration files, and logs. Unlike RAM, flash has limited write cycles (typically 10,000 to 100,000 writes per block), so the OS must manage it carefully.

Flash wears out, and network device operating systems employ wear-leveling algorithms to distribute writes evenly across blocks. This is why you'll see recommendations to limit syslog writes to flash and use remote logging instead. Every time you save a configuration change, you're consuming one write cycle from a finite budget.

EEPROM stores small amounts of critical data like MAC addresses and cryptographic keys. It's more expensive than flash but more durable and accessible at byte-level rather than block-level.

### Stack vs Heap: The Fundamental Divide

Every program divides its memory into regions with different characteristics. The stack grows and shrinks automatically as functions are called and return. The heap provides dynamic memory that persists until explicitly freed. In network devices with limited memory, understanding this distinction prevents crashes and memory exhaustion.

The stack is fast and efficient. When a function is called, the processor simply adjusts the stack pointer. Local variables live on the stack and disappear automatically when the function returns. But the stack has a fixed size, typically a few megabytes in embedded systems. Recursive functions or large local arrays can overflow the stack, causing crashes.

```c
/* Dangerous - large array on stack */
void process_packet(struct packet *pkt) {
    char buffer[65536];  /* 64KB on stack - risky */
    /* ... */
}

/* Better - allocate from heap if size is large */
void process_packet(struct packet *pkt) {
    char *buffer = malloc(65536);
    if (!buffer) {
        /* Handle allocation failure */
        return;
    }
    /* ... process ... */
    free(buffer);
}
```

The heap provides flexibility but requires manual management. In C, every malloc must be paired with a free, or memory leaks accumulate. In a router that runs for months or years without rebooting, even small leaks compound into operational problems.

### Static vs Dynamic Memory Allocation

Static allocation happens at compile time. Global variables and arrays declared with fixed sizes consume memory for the program's entire lifetime. Dynamic allocation happens at runtime using malloc, calloc, or similar functions.

Network device software often prefers static allocation for critical paths. A routing daemon might allocate a fixed pool of route entries at startup rather than dynamically allocating each route. This trades memory flexibility for predictability and speed. You know exactly how much memory you're using, and allocation never fails during packet processing.

Dynamic allocation offers flexibility but introduces failure modes. Malloc can fail if memory is exhausted. Allocation and deallocation take time—potentially hundreds of nanoseconds. In a fast packet processing path, these delays accumulate.

```c
/* Static allocation - predictable but inflexible */
#define MAX_ROUTES 100000
struct route routing_table[MAX_ROUTES];
int route_count = 0;

void add_route(struct route *r) {
    if (route_count < MAX_ROUTES) {
        routing_table[route_count++] = *r;
    } else {
        /* Table full - known limit */
    }
}

/* Dynamic allocation - flexible but can fail */
struct route *routing_table = NULL;
int route_count = 0;
int table_capacity = 0;

void add_route(struct route *r) {
    if (route_count >= table_capacity) {
        int new_capacity = table_capacity * 2 + 1;
        struct route *new_table = realloc(routing_table, 
                                         new_capacity * sizeof(struct route));
        if (!new_table) {
            /* Allocation failed - now what? */
            return;
        }
        routing_table = new_table;
        table_capacity = new_capacity;
    }
    routing_table[route_count++] = *r;
}
```

### Memory Leaks: The Silent Killer

A memory leak occurs when allocated memory is never freed. In a network device that runs continuously, leaks are catastrophic. A leak of 1KB per minute seems trivial, but after 24 hours you've lost 1.4MB. After a month, over 40MB. Eventually, the device exhausts memory and crashes.

Common leak patterns include forgetting to free memory in error paths, losing pointers to allocated memory, and reference counting bugs in complex data structures. Tools like Valgrind help detect leaks during development, but they can't run on production devices.

```c
/* Leak example - error path forgets to free */
int process_message(struct message *msg) {
    char *buffer = malloc(msg->size);
    if (!buffer) return -1;
    
    if (msg->type == INVALID_TYPE) {
        return -1;  /* LEAK - buffer not freed */
    }
    
    /* ... process message ... */
    free(buffer);
    return 0;
}

/* Fixed version */
int process_message(struct message *msg) {
    char *buffer = malloc(msg->size);
    if (!buffer) return -1;
    
    int result = 0;
    if (msg->type == INVALID_TYPE) {
        result = -1;
        goto cleanup;
    }
    
    /* ... process message ... */
    
cleanup:
    free(buffer);
    return result;
}
```

The goto cleanup pattern is common in C for ensuring resources are always freed, even in error cases.

### Memory Fragmentation: When Free Memory Isn't Usable

Fragmentation occurs when free memory exists but isn't available in contiguous blocks. External fragmentation happens when allocations and deallocations leave small gaps between allocated blocks. Internal fragmentation wastes space within allocated blocks when the allocation size exceeds the requested size.

In a router, fragmentation can prevent you from allocating a large buffer even though total free memory suggests you should have space. The free memory is scattered in small chunks. This is particularly problematic in embedded systems without virtual memory and defragmentation capabilities.

```c
/* Example showing fragmentation */
void *ptr1 = malloc(1000);  /* Allocate 1000 bytes */
void *ptr2 = malloc(1000);  /* Allocate 1000 bytes */
void *ptr3 = malloc(1000);  /* Allocate 1000 bytes */

free(ptr2);  /* Free middle block */

/* Now we have 1000 bytes free between ptr1 and ptr3,
   but can't allocate a 2000 byte buffer even if 
   total free memory > 2000 bytes */
void *big = malloc(2000);  /* Might fail due to fragmentation */
```

### Paging and Segmentation: Virtual Memory Basics

Paging divides memory into fixed-size pages (typically 4KB). The Memory Management Unit (MMU) translates virtual addresses to physical addresses using page tables. This allows the OS to present each process with its own address space, protect memory between processes, and even swap pages to disk when physical memory is scarce.

Segmentation divides memory into variable-sized segments based on logical divisions like code, data, and stack. Each segment has its own base address and limit. X86 processors historically used segmentation heavily, though modern operating systems primarily rely on paging.

Many embedded network processors lack full MMUs, using simpler Memory Protection Units (MPUs) instead. An MPU can't implement virtual memory or paging, but it can enforce memory protection boundaries between regions.

### Virtual Memory: Illusion and Reality

Virtual memory is less common in network devices than in general-purpose computers. The overhead of page table lookups and TLB misses can't be tolerated in fast packet processing paths. However, control plane software (routing protocols, management interfaces) often runs on separate processors with full virtual memory support.

Virtual memory provides isolation. Each process sees memory starting at address 0, unaware of other processes. The kernel maps these virtual addresses to physical addresses. If two processes access virtual address 0x1000, they're actually accessing different physical addresses. This prevents bugs in one process from corrupting another's memory.

### Cache Management and Hierarchies

Modern processors employ multiple cache levels because each level trades size for speed. L1 cache is smallest and fastest (32-128KB, 3-4 cycles). L2 is larger and slower (256KB-8MB, 10-20 cycles). L3, when present, is larger still but approaches main memory latency (several MB, 40-50 cycles).

Network processors often have specialized caches for packet data separate from general-purpose instruction and data caches. Some employ locked cache lines to guarantee critical data structures (like the most-used routing entries) always remain cached.

### Cache Coherence in Multi-Core Systems

Modern routers use multi-core processors to achieve high throughput. But multiple cores with separate caches create a problem: what happens when two cores cache the same memory location, and one modifies it?

Cache coherence protocols like MESI (Modified, Exclusive, Shared, Invalid) ensure all cores see consistent data. When one core writes to a cached location, hardware invalidates or updates that location in other caches. This works transparently but has performance implications. Excessive sharing between cores causes cache thrashing, where cores constantly invalidate each other's cache lines.

```c
/* Cache thrashing example */
int global_counter = 0;  /* Shared between all cores */

void packet_handler(struct packet *pkt) {
    /* Every core increments this, causing cache invalidation */
    global_counter++;
    
    /* Better approach - per-core counters */
}

/* Better design */
int per_core_counter[MAX_CORES];

void packet_handler(int core_id, struct packet *pkt) {
    per_core_counter[core_id]++;  /* No cache contention */
}
```

### Memory Protection Units

MPUs provide hardware-enforced memory protection without full virtual memory. An MPU defines regions with specific addresses, sizes, and access permissions. If code tries to access memory outside its permitted regions, the processor raises an exception.

This is critical in network devices where a bug in one software component shouldn't crash the entire system. The routing daemon runs in one protected region, the CLI in another, and packet processing in yet another. If the CLI has a buffer overflow, the MPU prevents it from corrupting routing tables.

### Memory Allocation Algorithms

When malloc is called, the allocator must find a free block of sufficient size. Different algorithms make different trade-offs between speed and fragmentation.

First Fit scans the free list and returns the first block large enough. It's fast but can leave many small fragments at the beginning of memory.

Best Fit searches the entire free list for the smallest block that fits. It minimizes wasted space but is slower and can create many tiny unusable fragments.

Worst Fit allocates from the largest available block, hoping to leave fragments large enough to be useful. It's generally the poorest performer.

Buddy Systems divide memory into powers of two. Allocating 5KB requires an 8KB block (next power of two). When freed, adjacent buddy blocks merge. This reduces fragmentation but wastes space to rounding.

```c
/* Simplified buddy allocator concept */
struct block {
    size_t size;  /* Must be power of 2 */
    int is_free;
    struct block *next;
};

void *buddy_alloc(size_t size) {
    /* Round up to next power of 2 */
    size_t rounded = next_power_of_2(size);
    
    /* Find free block of this size */
    struct block *b = find_free_block(rounded);
    if (b) {
        b->is_free = 0;
        return b + 1;  /* Return memory after header */
    }
    
    /* No exact match - split larger block */
    /* ... implementation ... */
}
```

### Memory Pools

Many network applications use memory pools to avoid allocation overhead and fragmentation. A pool pre-allocates a large block and divides it into fixed-size chunks. Allocating from the pool is just pointer manipulation—extremely fast and deterministic.

```c
struct packet_pool {
    struct packet buffers[POOL_SIZE];
    struct packet *free_list;
    int allocated;
};

void pool_init(struct packet_pool *pool) {
    /* Link all buffers into free list */
    for (int i = 0; i < POOL_SIZE - 1; i++) {
        pool->buffers[i].next = &pool->buffers[i + 1];
    }
    pool->buffers[POOL_SIZE - 1].next = NULL;
    pool->free_list = &pool->buffers[0];
    pool->allocated = 0;
}

struct packet *pool_alloc(struct packet_pool *pool) {
    if (!pool->free_list) return NULL;
    
    struct packet *p = pool->free_list;
    pool->free_list = p->next;
    pool->allocated++;
    return p;
}

void pool_free(struct packet_pool *pool, struct packet *p) {
    p->next = pool->free_list;
    pool->free_list = p;
    pool->allocated--;
}
```

Pools eliminate fragmentation within the pooled object type and provide O(1) allocation and deallocation. The downside is fixed sizing—if you need more packet buffers than the pool provides, you're out of luck.

### Memory-Mapped I/O

Network device hardware registers often appear at specific memory addresses. Writing to address 0xF0000000 might trigger a packet transmission, while reading from 0xF0000004 returns the number of packets received. This is memory-mapped I/O (MMIO).

From the CPU's perspective, MMIO looks like normal memory access. But the memory controller routes these addresses to device hardware instead of RAM. This provides a simple, efficient interface to hardware.

```c
/* Memory-mapped network interface */
#define NIC_BASE      0xF0000000
#define TX_BUFFER     (*(volatile uint32_t *)(NIC_BASE + 0x00))
#define RX_BUFFER     (*(volatile uint32_t *)(NIC_BASE + 0x04))
#define STATUS_REG    (*(volatile uint32_t *)(NIC_BASE + 0x08))
#define CONTROL_REG   (*(volatile uint32_t *)(NIC_BASE + 0x0C))

void send_packet(uint32_t *data, int length) {
    /* Wait for transmitter to be ready */
    while (STATUS_REG & TX_BUSY);
    
    /* Write packet data */
    for (int i = 0; i < length; i++) {
        TX_BUFFER = data[i];
    }
    
    /* Trigger transmission */
    CONTROL_REG = TX_START;
}
```

The volatile keyword is critical here—it tells the compiler that these memory locations can change unexpectedly (by hardware) and must not be optimized away or cached in registers.

### Registers and Memory Segments

Processor registers are the fastest storage, accessible in a single clock cycle. Network processors often have dozens or even hundreds of general-purpose registers plus specialized registers for packet descriptors and DMA operations. Keeping frequently used values in registers is the most important optimization.

Memory is typically divided into segments with different characteristics:

The code segment (text segment) contains executable instructions, usually read-only to prevent self-modifying code.

The data segment holds global and static variables with initial values.

The BSS segment holds uninitialized global and static variables. The OS zeroes this segment at program startup.

The heap grows upward from the end of BSS as memory is dynamically allocated.

The stack grows downward from high addresses. If stack and heap meet, you've exhausted memory.

## Process and Thread Management

### Understanding Processes, Threads, and Tasks

A process is an instance of a running program with its own memory space, file descriptors, and resources. Processes provide strong isolation—one process cannot directly access another's memory. Creating a process is expensive because the OS must allocate memory, copy page tables, and initialize numerous data structures.

A thread exists within a process and shares the process's memory space. Multiple threads can execute concurrently while accessing the same variables. Creating a thread is much cheaper than creating a process because no memory space duplication is required. However, sharing memory means threads must synchronize carefully to avoid race conditions.

In RTOS terminology, tasks resemble threads more than processes. An RTOS task typically has its own stack but shares memory with other tasks. The term "task" emphasizes real-time scheduling properties rather than isolation.

### Why Network Devices Use Multitasking

A router performs many activities simultaneously: receiving packets on multiple interfaces, processing routing protocol updates, responding to management commands, updating statistics, and more. Multitasking allows organizing this complexity into separate execution contexts that appear to run simultaneously.

Without multitasking, you'd need one giant loop polling all possible events:

```c
/* Single-threaded nightmare */
while (1) {
    if (packet_received()) {
        process_packet();
    }
    if (routing_update_pending()) {
        process_routing_update();
    }
    if (management_command_pending()) {
        process_management_command();
    }
    if (statistics_timer_expired()) {
        update_statistics();
    }
    /* Dozens more conditions... */
}
```

This approach is unmaintainable. Multitasking lets you write separate, focused functions for each activity while the OS schedules their execution.

### Concurrency vs Parallelism

Concurrency means multiple execution contexts making progress, possibly through time-slicing on a single CPU. Parallelism means multiple contexts executing simultaneously on multiple CPUs. A single-core router achieves concurrency through rapid context switching. A multi-core router achieves true parallelism.

The distinction matters for performance. Concurrent execution doesn't improve throughput on a single core—it just improves responsiveness by preventing one slow operation from blocking others. Parallel execution on multiple cores can multiply throughput proportionally to the number of cores (minus synchronization overhead).

### Process States and Transitions

A process exists in one of several states at any time:

Created/New: The process is being set up but isn't yet ready to run. The OS allocates memory, initializes the Process Control Block, and loads code.

Ready: The process is ready to run but waiting for CPU time. It's in the scheduler's ready queue.

Running: The process is currently executing on a CPU. On a single-core system, only one process runs at a time.

Blocked/Waiting: The process cannot continue until some event occurs (I/O completion, another process releasing a lock, timer expiration). Blocked processes don't consume CPU time.

Terminated/Finished: The process has completed execution. Its resources are being reclaimed.

Transitions occur due to scheduling decisions (ready to running), I/O operations (running to blocked), I/O completion (blocked to ready), and completion (running to terminated).

### RTOS Task States

Real-Time Operating Systems often use a slightly different state model:

Idle: The task exists but hasn't been started.

Ready: The task is ready to run and in the scheduler's queue.

Running: The task is executing.

Blocked: The task is waiting for an event (similar to the blocked state in general-purpose OSes).

Suspended: The task is explicitly suspended by another task or the system.

Deleted: The task has been removed and its resources freed.

### The Process Control Block

The Process Control Block (PCB) is the OS's data structure for tracking a process. When a process isn't running, the PCB stores everything needed to resume it:

Process ID (PID): Unique identifier.

Process state: Current state (ready, running, blocked, etc.).

Program counter: Address of the next instruction to execute.

CPU registers: Contents of all registers when the process was last interrupted.

Stack pointer: Location of the process's stack.

Memory management information: Page tables, memory limits.

Scheduling information: Priority, time slices used.

I/O status: Open files, pending I/O operations.

Accounting information: CPU time used, memory consumed.

```c
/* Simplified PCB structure */
struct pcb {
    int pid;
    int state;  /* READY, RUNNING, BLOCKED, etc. */
    void *program_counter;
    uint32_t registers[32];
    void *stack_pointer;
    int priority;
    uint64_t cpu_time_used;
    struct file *open_files[MAX_FILES];
    struct pcb *next;  /* For scheduler queues */
};
```

### Context Switching: The Hidden Cost

Context switching is the process of saving the current process's state and loading another's. It happens when the scheduler decides to run a different process. Context switches are pure overhead—no useful work is done during the switch.

A context switch involves:

1. Saving the current process's CPU registers to its PCB.
2. Saving the current process's stack pointer.
3. Updating the current process's state (e.g., running to ready).
4. Selecting the next process to run (scheduling decision).
5. Loading the next process's registers from its PCB.
6. Loading the next process's stack pointer.
7. Loading the next process's memory mappings (page tables).
8. Flushing TLBs and caches may be necessary.
9. Updating the next process's state (ready to running).

This takes hundreds to thousands of CPU cycles. On a general-purpose OS, context switches happen thousands of times per second. In an RTOS, context switch time is critical and must be minimized and bounded.

### Thread Safety and Shared Memory Problems

When multiple threads share memory, race conditions occur when the outcome depends on the timing of execution. Consider incrementing a counter:

```c
int packet_count = 0;

void packet_handler(struct packet *pkt) {
    packet_count++;  /* Looks atomic, but isn't */
}
```

The increment isn't atomic. It compiles to three operations: load the value from memory, add one, store the result back. If two threads execute this simultaneously:

Thread A loads packet_count (value 100).
Thread B loads packet_count (value 100).
Thread A increments to 101 and stores.
Thread B increments to 101 and stores.

The count is now 101, but it should be 102. One increment was lost. This is a race condition.

### Synchronization Primitives: Mutexes

A mutex (mutual exclusion lock) allows only one thread to access a shared resource at a time. Before accessing shared data, a thread locks the mutex. When finished, it unlocks the mutex. If another thread tries to lock an already-locked mutex, it blocks until the mutex is unlocked.

```c
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;
int packet_count = 0;

void packet_handler(struct packet *pkt) {
    pthread_mutex_lock(&counter_lock);
    packet_count++;
    pthread_mutex_unlock(&counter_lock);
}
```

Mutexes solve the race condition but introduce serialization. Only one thread can increment the counter at a time, potentially limiting parallelism. Mutexes also have overhead—locking and unlocking take dozens of CPU cycles.

### Semaphores: Counting and Signaling

A semaphore is a synchronization primitive with an integer value and two operations: wait (decrement) and post (increment). If wait would make the value negative, the thread blocks. When another thread calls post, one blocked thread is awakened.

Binary semaphores (value 0 or 1) function like mutexes. Counting semaphores (arbitrary positive values) limit the number of threads accessing a resource. For example, limiting concurrent database connections to 10:

```c
sem_t connection_semaphore;
sem_init(&connection_semaphore, 0, 10);  /* Max 10 connections */

void handle_request() {
    sem_wait(&connection_semaphore);  /* Acquire connection slot */
    
    /* Process request using database */
    query_database();
    
    sem_post(&connection_semaphore);  /* Release connection slot */
}
```

If 10 threads already have connections, the 11th thread blocks at sem_wait until another thread calls sem_post.

### Condition Variables

Condition variables allow threads to wait for specific conditions while releasing a mutex. This is more efficient than polling:

```c
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;
struct packet_queue queue;

void enqueue_packet(struct packet *pkt) {
    pthread_mutex_lock(&queue_lock);
    add_to_queue(&queue, pkt);
    pthread_cond_signal(&queue_not_empty);  /* Wake one waiting thread */
    pthread_mutex_unlock(&queue_lock);
}

struct packet *dequeue_packet() {
    pthread_mutex_lock(&queue_lock);
    
    while (queue_empty(&queue)) {
        /* Wait for packets, releasing lock while waiting */
        pthread_cond_wait(&queue_not_empty, &queue_lock);
    }
    
    struct packet *pkt = remove_from_queue(&queue);
    pthread_mutex_unlock(&queue_lock);
    return pkt;
}
```

The key is pthread_cond_wait atomically releases the mutex and puts the thread to sleep. When awakened, it re-acquires the mutex. This prevents the race condition where a packet is enqueued between checking the empty condition and going to sleep.

### Real-Time Scheduling

Real-time systems must meet timing deadlines. In hard real-time systems (like automotive brakes), missing a deadline causes catastrophic failure. In soft real-time systems (like video streaming), missing occasional deadlines is acceptable but degrades quality.

Network devices often have soft real-time requirements. Routing protocol updates should be processed promptly, but a few milliseconds delay rarely causes problems. Packet processing is more time-sensitive—buffering and forwarding delays directly impact application performance.

### Preemptive vs Cooperative Scheduling

In preemptive scheduling, the OS can interrupt a running task at any time to run a higher-priority task. This provides better responsiveness but requires tasks to be thread-safe since they can be interrupted at any point.

In cooperative scheduling, tasks run until they voluntarily yield the CPU. This simplifies synchronization (no need for mutexes if tasks never interrupt each other) but one misbehaving task can starve others.

Most network device operating systems use preemptive scheduling for better real-time response, but may use cooperative scheduling within specific subsystems.

### Priority-Based Scheduling

Tasks have priorities, and the scheduler runs the highest-priority ready task. This is simple and provides good real-time behavior, but suffers from priority inversion: if a low-priority task holds a lock that a high-priority task needs, the high-priority task is blocked. If a medium-priority task is running, it prevents the low-priority task from releasing the lock, and the high-priority task is indirectly delayed by the medium-priority task.

Priority inheritance protocols solve this by temporarily elevating the low-priority task's priority while it holds the lock.

```c
/* Priority inversion scenario */
Task Low (priority 1) locks mutex.
Task High (priority 10) tries to lock mutex, blocks.
Task Medium (priority 5) starts running.

/* Task High is blocked by Task Low, but Task Medium
   prevents Task Low from running and releasing the lock.
   High-priority task is stuck! */

/* With priority inheritance */
Task Low locks mutex, priority temporarily raised to 10.
Task High tries to lock mutex, blocks.
Task Low completes quickly (running at priority 10), releases mutex.
Task High acquires mutex and runs.
Task Low priority returns to 1.
```

### Round Robin Scheduling

Round robin gives each task a fixed time slice (quantum). When the quantum expires, the task is preempted and moved to the back of the ready queue. This provides fairness—all tasks get CPU time regardless of priority.

Quantum size matters. Too small, and context switch overhead dominates. Too large, and responsiveness suffers. Typical values range from 10 to 100 milliseconds in general-purpose systems, but RTOSes often use microsecond-scale quanta.

### Deadlock: The Four Conditions

Deadlock occurs when tasks wait indefinitely for each other. Four conditions must all be true for deadlock:

1. Mutual exclusion: Resources cannot be shared (e.g., only one task can hold a mutex at once).

2. Hold and wait: Tasks hold resources while waiting for others.

3. No preemption: Resources cannot be forcibly taken from tasks.

4. Circular wait: A cycle exists in the resource wait graph (Task A waits for resource held by Task B, which waits for resource held by Task C, which waits for resource held by Task A).

Classic deadlock example:

```c
pthread_mutex_t mutex1, mutex2;

void task_a() {
    pthread_mutex_lock(&mutex1);
    sleep(1);  /* Simulate work */
    pthread_mutex_lock(&mutex2);  /* May deadlock */
    /* ... */
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}

void task_b() {
    pthread_mutex_lock(&mutex2);
    sleep(1);  /* Simulate work */
    pthread_mutex_lock(&mutex1);  /* May deadlock */
    /* ... */
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
}
```

If task_a acquires mutex1 and task_b acquires mutex2 simultaneously, each waits forever for the other's mutex.

### Preventing Deadlock

The simplest prevention: impose a total ordering on resources. Always acquire mutexes in the same order:

```c
void task_a() {
    pthread_mutex_lock(&mutex1);
    pthread_mutex_lock(&mutex2);
    /* ... */
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}

void task_b() {
    pthread_mutex_lock(&mutex1);  /* Same order as task_a */
    pthread_mutex_lock(&mutex2);
    /* ... */
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
}
```

Now deadlock is impossible because the circular wait condition cannot occur.

Other prevention strategies include try-lock patterns (attempt to acquire a lock without blocking, backing off if unsuccessful) and lock-free data structures.

### Critical Sections

A critical section is a code region accessing shared resources that must not be executed by multiple threads simultaneously. Protecting critical sections requires synchronization:

```c
/* Critical section - must be protected */
void update_routing_table(struct route *new_route) {
    pthread_mutex_lock(&routing_lock);
    
    /* Critical section begins */
    struct route *existing = lookup_route(new_route->dest);
    if (existing) {
        update_route(existing, new_route);
    } else {
        insert_route(new_route);
    }
    /* Critical section ends */
    
    pthread_mutex_unlock(&routing_lock);
}
```

Critical sections should be as short as possible. Long critical sections reduce parallelism and increase lock contention. Move non-shared work outside the critical section:

```c
void update_routing_table(struct route *new_route) {
    /* Prepare data outside critical section */
    calculate_metrics(new_route);
    validate_route(new_route);
    
    pthread_mutex_lock(&routing_lock);
    /* Minimal critical section */
    struct route *existing = lookup_route(new_route->dest);
    if (existing) {
        swap_route_data(existing, new_route);
    } else {
        insert_route(new_route);
    }
    pthread_mutex_unlock(&routing_lock);
    
    /* Cleanup outside critical section */
    log_routing_change(new_route);
}
```

## Inter-Process Communication

### Why Processes Need to Communicate

Network device software is complex, and isolating functionality into separate processes improves reliability and maintainability. The routing daemon, management agent, packet forwarding engine, and monitoring tools are separate processes that must exchange information. If one crashes, others can continue operating.

IPC mechanisms allow these processes to coordinate without violating memory isolation. They range from simple signals to sophisticated message passing systems.

### Shared Memory

Shared memory is the fastest IPC mechanism. Multiple processes map the same physical memory region into their address spaces. Changes made by one process are immediately visible to others because they're all accessing the same memory.

```c
/* Process A - creates shared memory */
int shm_fd = shm_open("/routing_table", O_CREAT | O_RDWR, 0666);
ftruncate(shm_fd, sizeof(struct routing_table));
struct routing_table *table = mmap(NULL, sizeof(struct routing_table),
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED, shm_fd, 0);

/* Initialize table */
table->num_routes = 0;

/* Process B - opens existing shared memory */
int shm_fd = shm_open("/routing_table", O_RDWR, 0);
struct routing_table *table = mmap(NULL, sizeof(struct routing_table),
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED, shm_fd, 0);

/* Access table */
printf("Routes: %d\n", table->num_routes);
```

The danger: shared memory requires explicit synchronization. If Process A updates a route while Process B is reading it, Process B sees inconsistent data. Semaphores or mutexes (placed in shared memory themselves) must protect access.

```c
struct shared_data {
    pthread_mutex_t lock;
    struct routing_table table;
};

/* Both processes must lock before accessing */
pthread_mutex_lock(&data->lock);
/* Access data->table */
pthread_mutex_unlock(&data->lock);
```

### Message Passing and Message Queues

Message passing provides structured communication. One process sends a message, and another receives it. The OS handles buffering and synchronization. Processes never share memory, eliminating race conditions.

POSIX message queues provide this abstraction:

```c
/* Process A - sender */
mqd_t mq = mq_open("/routing_updates", O_WRONLY | O_CREAT, 0666, NULL);

struct route_update msg;
msg.dest = 0x0A000000;  /* 10.0.0.0 */
msg.mask = 0xFF000000;  /* /8 */
msg.next_hop = 0x0A000001;  /* 10.0.0.1 */

mq_send(mq, (char *)&msg, sizeof(msg), 0);

/* Process B - receiver */
mqd_t mq = mq_open("/routing_updates", O_RDONLY);

struct route_update msg;
mq_receive(mq, (char *)&msg, sizeof(msg), NULL);

/* Process update */
process_route_update(&msg);
```

Message queues have capacity limits. If the queue fills, senders block until space is available. This provides backpressure—fast senders automatically slow down when receivers can't keep up.

### Mailboxes

Mailboxes are similar to message queues but often refer to RTOS-specific implementations optimized for embedded systems. A mailbox typically holds one message at a time. Sending a message to a full mailbox blocks until the message is retrieved.

### Pipes

Pipes provide unidirectional byte streams between processes. Unnamed pipes connect processes with parent-child relationships. Named pipes (FIFOs) allow unrelated processes to communicate.

```c
/* Unnamed pipe */
int pipe_fd[2];
pipe(pipe_fd);  /* pipe_fd[0] = read end, pipe_fd[1] = write end */

if (fork() == 0) {
    /* Child process */
    close(pipe_fd[1]);  /* Close write end */
    
    char buffer[100];
    read(pipe_fd[0], buffer, sizeof(buffer));
    printf("Child received: %s\n", buffer);
    
    close(pipe_fd[0]);
} else {
    /* Parent process */
    close(pipe_fd[0]);  /* Close read end */
    
    write(pipe_fd[1], "Hello from parent", 18);
    
    close(pipe_fd[1]);
    wait(NULL);  /* Wait for child */
}
```

Pipes are simple and efficient but limited to related processes (unnamed pipes) or one-way communication. Full-duplex communication requires two pipes.

### Sockets

Sockets provide network-style IPC even between processes on the same machine. Unix domain sockets are optimized for local communication, avoiding the network stack overhead while maintaining the socket API.

```c
/* Server */
int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
struct sockaddr_un addr;
addr.sun_family = AF_UNIX;
strcpy(addr.sun_path, "/tmp/router.sock");

bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
listen(server_fd, 5);

int client_fd = accept(server_fd, NULL, NULL);
char buffer[100];
read(client_fd, buffer, sizeof(buffer));

/* Client */
int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
struct sockaddr_un addr;
addr.sun_family = AF_UNIX;
strcpy(addr.sun_path, "/tmp/router.sock");

connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
write(sock_fd, "GET /routes", 11);
```

Sockets provide bidirectional communication, work between unrelated processes, and can easily extend to network communication by changing address families (AF_UNIX to AF_INET).

### Remote Procedure Call

RPC abstracts network communication as function calls. A client calls a function that appears local but actually executes on a remote server. The RPC framework handles marshaling arguments, network transmission, and returning results.

In network devices, RPC often coordinates between control plane processors and forwarding plane processors. The control plane computes routing decisions and "calls" the forwarding plane to install routes.

```c
/* Client code - looks like local function call */
int result = install_route(dest_addr, next_hop);

/* Behind the scenes, RPC framework:
   1. Serializes arguments (dest_addr, next_hop)
   2. Sends message to server process
   3. Server deserializes arguments
   4. Server calls actual install_route function
   5. Server serializes result
   6. Server sends response
   7. Client deserializes result
   8. Returns to caller
*/
```

RPC simplifies distributed programming but adds latency and complexity. Network or process failures must be handled gracefully—what happens if the server crashes mid-call?

### Client/Server Model

The client/server pattern structures communication. Servers provide services and wait for requests. Clients initiate connections and send requests. This asymmetry simplifies design—servers don't need to know about all possible clients.

In routers, the forwarding engine is a server, and control plane processes are clients requesting route installations. The configuration database is a server, and various management tools are clients querying and updating configuration.

### Synchronization Across Processes

IPC mechanisms require synchronization just like shared memory within a process. Message queues provide implicit synchronization—the queue is the synchronization point. But shared memory between processes needs explicit locks.

POSIX provides process-shared mutexes and semaphores. When creating a mutex, set the process-shared attribute:

```c
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

/* Mutex must be in shared memory */
pthread_mutex_t *lock = /* address in shared memory */;
pthread_mutex_init(lock, &attr);
```

Now multiple processes can lock and unlock this mutex. It must reside in shared memory so all processes access the same lock object.

## System Calls and Services

### The System Call Interface

User programs cannot directly access hardware or kernel data structures. Doing so would violate security and stability—a buggy application could crash the entire system. Instead, applications request services through system calls, special functions that transition from user mode to kernel mode.

When a program calls a system function like write(), the library code executes a trap instruction (often called a software interrupt). This causes the CPU to switch to kernel mode and jump to a kernel handler. The kernel validates arguments, performs the operation, and returns results to user space.

```c
/* User code */
int fd = open("/etc/config", O_RDONLY);
char buffer[1024];
ssize_t bytes = read(fd, buffer, sizeof(buffer));
close(fd);

/* Each of these calls triggers a system call:
   - open() -> syscall number 2 (on Linux x86_64)
   - read() -> syscall number 0
   - close() -> syscall number 3
   
   The kernel handles the actual operations.
*/
```

### Process Control System Calls

fork() creates a new process by duplicating the calling process. The child process is nearly identical to the parent but has a new process ID. fork() returns twice—once in the parent (returning child's PID) and once in the child (returning 0).

```c
pid_t pid = fork();

if (pid == 0) {
    /* Child process */
    printf("I am the child, PID %d\n", getpid());
    exit(0);
} else if (pid > 0) {
    /* Parent process */
    printf("I am the parent, child PID is %d\n", pid);
    wait(NULL);  /* Wait for child to finish */
} else {
    /* fork() failed */
    perror("fork");
}
```

exec() replaces the current process's memory with a new program. Combined with fork(), this is how Unix-like systems start new programs: fork to create a process, then exec to load the desired program.

wait() pauses the calling process until a child process exits. This prevents zombie processes—dead processes whose parent hasn't acknowledged their exit.

exit() terminates the calling process, releasing its resources and notifying its parent.

### File Management System Calls

open() opens a file and returns a file descriptor, a small integer representing the open file. File descriptors 0, 1, and 2 are standard input, output, and error.

```c
int fd = open("/var/log/router.log", O_WRONLY | O_APPEND);
if (fd == -1) {
    perror("open");
    return;
}
```

read() and write() transfer data. They may transfer fewer bytes than requested—the return value indicates how many bytes were actually transferred:

```c
char buffer[1024];
ssize_t n;

while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
    write(STDOUT_FILENO, buffer, n);
}

if (n == -1) {
    perror("read");
}
```

close() releases the file descriptor. Forgetting to close files exhausts the file descriptor table, preventing new files from being opened.

### Device Management System Calls

Many hardware devices appear as files in Unix-like systems. Reading from /dev/random returns random bytes. Writing to /dev/null discards data. Network interfaces often appear as /dev/eth0, /dev/eth1, etc.

ioctl() (input/output control) configures device-specific settings that don't fit the read/write model:

```c
int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

struct ifreq ifr;
strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);

/* Get MAC address */
if (ioctl(sock_fd, SIOCGIFHWADDR, &ifr) == 0) {
    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/* Set interface flags */
ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
ioctl(sock_fd, SIOCSIFFLAGS, &ifr);
```

This is how network configuration tools set IP addresses, enable promiscuous mode, and configure interface parameters.

### Information Maintenance System Calls

time() and gettimeofday() retrieve the current time. Accurate timekeeping is critical for network protocols that use timestamps (NTP, routing protocol convergence monitoring).

getpid() returns the process ID. getppid() returns the parent's PID. getuid() returns the user ID.

sysinfo() provides system statistics like uptime, memory usage, and load average.

### Communication System Calls

pipe() creates a unidirectional communication channel, as shown earlier.

socket() creates a communication endpoint. bind() associates a socket with an address. listen() marks a socket as accepting connections. accept() retrieves an incoming connection. connect() initiates a connection.

send() and recv() transfer data over sockets.

These system calls form the foundation of network programming. Even high-level libraries ultimately use these calls.

### Privilege Levels: User Space vs Kernel Space

Modern CPUs implement privilege levels (rings on x86). User programs run at the lowest privilege (ring 3 on x86), unable to execute privileged instructions or access kernel memory. The kernel runs at the highest privilege (ring 0), with complete hardware access.

This separation protects the system. A buggy application can't crash the kernel or interfere with other applications. It can only harm itself.

System calls bridge the gap. When a system call is invoked, the CPU switches from user mode to kernel mode, executes the requested operation with full privileges, and returns to user mode.

### Hardware Abstraction Layer

The HAL provides a consistent interface to hardware, isolating hardware-specific code from the rest of the system. Different network interface cards have different register layouts and command protocols. The HAL presents a uniform API for packet transmission and reception regardless of the underlying hardware.

```c
/* HAL provides uniform interface */
int hal_transmit_packet(struct hal_device *dev, 
                       struct packet *pkt);

/* Implementation for Device A */
int device_a_transmit(struct hal_device *dev, 
                     struct packet *pkt) {
    /* Device-A-specific register writes */
}

/* Implementation for Device B */
int device_b_transmit(struct hal_device *dev, 
                     struct packet *pkt) {
    /* Device-B-specific register writes */
}

/* Higher layers just call hal_transmit_packet,
   HAL routes to correct implementation */
```

This modularity simplifies porting to new hardware and testing on simulators.

### Device Drivers

Drivers are kernel modules managing specific hardware devices. A network interface driver handles packet transmission and reception, interrupt processing, and device configuration.

Drivers register with the kernel and provide function pointers for various operations:

```c
struct net_device_ops {
    int (*ndo_open)(struct net_device *dev);
    int (*ndo_stop)(struct net_device *dev);
    netdev_tx_t (*ndo_start_xmit)(struct sk_buff *skb,
                                  struct net_device *dev);
    void (*ndo_set_rx_mode)(struct net_device *dev);
    int (*ndo_set_mac_address)(struct net_device *dev, void *addr);
};
```

When the system needs to transmit a packet, it calls the driver's ndo_start_xmit function. The driver translates this generic request into hardware-specific commands.

### Interrupt Handling

Hardware interrupts notify the CPU of events requiring attention—packet arrival, transmission completion, errors. When an interrupt occurs, the CPU saves its current state and jumps to an interrupt handler.

Interrupt Service Routines (ISRs) must execute quickly. They acknowledge the interrupt, perform minimal processing, and schedule deferred work if needed. Prolonged interrupt handling prevents other interrupts from being serviced, leading to packet loss.

```c
/* Interrupt handler - executes in interrupt context */
void network_isr(int irq, void *dev_id) {
    struct network_device *dev = dev_id;
    uint32_t status = read_interrupt_status(dev);
    
    if (status & RX_COMPLETE) {
        /* Packet received - schedule processing */
        schedule_rx_processing(dev);
        acknowledge_interrupt(dev, RX_COMPLETE);
    }
    
    if (status & TX_COMPLETE) {
        /* Transmission done - free buffers */
        acknowledge_interrupt(dev, TX_COMPLETE);
    }
    
    /* Total time: microseconds at most */
}

/* Deferred work - executes in process context later */
void rx_processing(struct network_device *dev) {
    struct packet *pkt;
    while ((pkt = dequeue_rx_packet(dev)) != NULL) {
        /* Can take longer - not in interrupt context */
        process_packet(pkt);
    }
}
```

Many systems use interrupt coalescing—delaying interrupt generation until multiple packets arrive or a timeout expires. This reduces interrupt overhead at the cost of slightly increased latency.

### Network Stack

The network stack implements protocol layers. In a typical TCP/IP stack:

The link layer handles physical transmission, MAC addressing, and framing (Ethernet, WiFi).

The network layer routes packets between networks (IP).

The transport layer provides end-to-end communication (TCP, UDP).

The application layer implements high-level protocols (HTTP, DNS, routing protocols).

Each layer adds headers to outgoing packets and strips headers from incoming packets:

```c
/* Simplified packet transmission through stack */
void application_send(void *data, int len, uint32_t dest_ip) {
    /* Application layer passes data to transport */
    transport_send(data, len, dest_ip, dest_port);
}

void transport_send(void *data, int len, uint32_t dest_ip, uint16_t port) {
    /* Add TCP/UDP header */
    struct tcp_header *tcp = prepend_tcp_header(data, len, port);
    
    /* Pass to network layer */
    network_send(tcp, len + sizeof(*tcp), dest_ip);
}

void network_send(void *data, int len, uint32_t dest_ip) {
    /* Add IP header */
    struct ip_header *ip = prepend_ip_header(data, len, dest_ip);
    
    /* Lookup next hop, pass to link layer */
    uint32_t next_hop = route_lookup(dest_ip);
    link_send(ip, len + sizeof(*ip), next_hop);
}

void link_send(void *data, int len, uint32_t next_hop) {
    /* Resolve MAC address via ARP */
    uint8_t mac[6];
    arp_resolve(next_hop, mac);
    
    /* Add Ethernet header */
    struct ethernet_header *eth = prepend_ethernet_header(data, len, mac);
    
    /* Transmit to hardware */
    device_transmit(eth, len + sizeof(*eth));
}
```

### Watchdog Timers

Watchdog timers detect software hangs. Hardware generates a reset if software doesn't periodically "pet the watchdog." If code crashes or enters an infinite loop, the watchdog timer expires and resets the system.

```c
void watchdog_task() {
    while (1) {
        /* Perform health checks */
        if (routing_daemon_healthy() &&
            forwarding_engine_healthy() &&
            management_agent_healthy()) {
            /* All systems operational - reset watchdog */
            watchdog_reset();
        }
        
        sleep(5);  /* Check every 5 seconds */
    }
}
```

Watchdog timers provide recovery from catastrophic software failures, ensuring the device eventually returns to operation even if software deadlocks.

## Embedded Operating Systems

### Real-Time Operating Systems

RTOSes prioritize determinism and low latency over throughput. When an event occurs, the system must respond within a guaranteed time bound. This predictability is critical for time-sensitive applications like packet forwarding, where delays accumulate across multiple hops.

RTOSes achieve this through:

Preemptive priority-based scheduling: High-priority tasks immediately preempt lower-priority tasks.

Bounded system call latency: Every system call completes within a known maximum time.

Priority inheritance: Prevents priority inversion from delaying high-priority tasks.

Minimal interrupt disable time: Interrupts are disabled only for microseconds at most.

### Hard vs Soft Real-Time

Hard real-time systems face absolute deadlines. Missing a deadline means system failure. Industrial control systems, automotive systems, and some telecommunications equipment are hard real-time.

Soft real-time systems have deadlines, but occasional violations are acceptable if rare. Video streaming, VoIP, and many networking applications are soft real-time. A dropped frame or delayed packet degrades quality but doesn't cause catastrophic failure.

Most routers and switches are soft real-time systems. A routing protocol update delayed by milliseconds rarely causes problems, but consistent delays would impact network convergence.

### Embedded Linux

Many modern network devices run embedded Linux (often distributions like OpenWrt or Yocto-based builds). Linux provides a full-featured OS with extensive driver support, networking stacks, and development tools.

However, standard Linux is not a true RTOS. While the kernel has been improved with preemption patches and real-time extensions (PREEMPT_RT), it doesn't provide the hard guarantees of dedicated RTOSes. For control plane functions (routing protocols, management), Linux works well. For data plane packet forwarding at wire speed, dedicated forwarding engines or specialized processors are often used.

### Monolithic vs Microkernel Architecture

Monolithic kernels run all OS services (drivers, filesystem, network stack) in kernel space. This is efficient—no context switches between services—but less robust. A bug in any kernel component can crash the entire system. Linux is monolithic.

Microkernels run minimal code in kernel space (scheduling, IPC, memory management). Everything else runs as user-space servers. This improves isolation and reliability—a driver crash doesn't bring down the kernel. But it increases overhead due to frequent context switches and message passing. QNX and L4 are microkernels.

Network devices increasingly adopt microkernel or hybrid approaches to improve reliability while maintaining performance.

### Power Management

Network devices must manage power consumption and heat dissipation. Techniques include:

Dynamic voltage and frequency scaling: Reducing processor speed during low load periods.

Power gating: Completely powering off unused components.

Sleep states: Putting interfaces into low-power modes when inactive.

Adaptive link rate: Reducing link speed on Ethernet interfaces during low traffic periods.

Power management trades performance for efficiency. The OS must balance power savings against responsiveness—waking from deep sleep might take milliseconds, unacceptable for packet forwarding.

### Bootloader

The bootloader initializes hardware and loads the operating system. When a router powers on:

1. CPU begins executing code from ROM or flash at a fixed address.
2. Bootloader initializes RAM, configures memory controllers.
3. Bootloader decompresses OS image from flash to RAM.
4. Bootloader verifies OS integrity (checksum, digital signature).
5. Bootloader jumps to OS entry point.

Bootloaders often provide recovery mechanisms. If the OS image is corrupted, the bootloader can load a backup image or enter a recovery mode allowing firmware updates over the network.

### File Systems

Embedded systems use specialized file systems optimized for flash storage. Flash wears out with repeated writes, and standard file systems like ext4 aren't flash-aware.

JFFS2 (Journaling Flash File System 2) and UBIFS (Unsorted Block Image File System) are designed for flash. They implement wear leveling and handle bad blocks gracefully.

Many network devices use read-only root file systems. The OS image is mounted read-only, preventing accidental corruption. Configuration files reside in a separate writable partition. This separation simplifies recovery—you can always revert to the factory OS image if the configuration becomes corrupted.

### Security Features

Network devices face constant attack. Security features include:

Secure boot: Verifying digital signatures on bootloader and OS images, preventing unauthorized firmware installation.

Encryption: Storing sensitive data (keys, passwords) encrypted.

Access control: Requiring authentication for management access.

Sandboxing: Isolating untrusted code in restricted environments.

Memory protection: Using MPUs or MMUs to prevent one component from corrupting another's memory.

Regular security updates: Patching vulnerabilities as they're discovered.

Modern devices implement defense in depth—multiple security layers so compromising one doesn't give complete system access.

## Conclusion

Understanding OS concepts is fundamental to working with routers and switches effectively. These devices aren't simple packet forwarders—they're sophisticated embedded systems managing limited resources under real-time constraints.

Memory management determines how many routes, connections, and packets your device can handle. Process and thread management enables concurrent operation of routing protocols, forwarding engines, and management interfaces. IPC mechanisms coordinate these components. System calls bridge user applications and kernel services. The operating system architecture—RTOS or embedded Linux, monolithic or microkernel—shapes performance and reliability characteristics.

This knowledge isn't academic. When debugging why a router drops packets under load, you're investigating memory exhaustion, CPU scheduling, or interrupt handling. When optimizing forwarding performance, you're thinking about cache hierarchies and context switch overhead. When designing reliable systems, you're leveraging process isolation and watchdog timers.

The concepts presented here provide a foundation, but real mastery comes from applying them to actual problems. Study your device's architecture. Measure its performance. Understand where bottlenecks occur. Only then will these OS concepts transform from abstract ideas into practical tools for building and maintaining robust networks.

# Memory Management: Developer's Guide for Network Applications

## The Reality Check

You're building network software that must handle millions of packets per second with sub-millisecond latency. Every memory allocation, every cache miss, every page fault directly impacts whether your system meets its performance targets or falls flat. This isn't theoretical—poor memory management is the difference between wire-speed forwarding and packet drops.

This guide focuses on what you, as a developer, need to know and do. Not theory for theory's sake, but practical decisions that determine whether your code scales or chokes under load.

## Memory Allocation: The Performance Killer You Control

### The malloc Trap

Every time you call malloc, you're introducing unpredictability. The allocator searches free lists, potentially fragmenting memory, and takes hundreds of nanoseconds. In a tight packet processing loop handling 10 million packets per second, each packet gets 100 nanoseconds total. You literally cannot afford malloc in the fast path.

**DON'T DO THIS:**

```c
void process_packet(uint8_t *pkt_data, int len) {
    /* WRONG - allocating in fast path */
    struct packet *pkt = malloc(sizeof(struct packet));
    if (!pkt) {
        /* Now you're dropping packets due to memory allocation */
        return;
    }
    
    memcpy(pkt->data, pkt_data, len);
    forward_packet(pkt);
    free(pkt);
}
```

Every packet allocation causes a malloc call. Under high load, this creates catastrophic performance degradation. You're also introducing failure modes—malloc can fail, and then what? Drop the packet silently?

**DO THIS INSTEAD:**

```c
/* Pre-allocated packet pool */
struct packet_pool {
    struct packet *buffers;
    struct packet **free_list;
    int pool_size;
    int free_count;
    pthread_spinlock_t lock;
};

void pool_init(struct packet_pool *pool, int size) {
    pool->pool_size = size;
    pool->free_count = size;
    
    /* Single large allocation - done at startup */
    pool->buffers = calloc(size, sizeof(struct packet));
    pool->free_list = calloc(size, sizeof(struct packet *));
    
    /* Build free list */
    for (int i = 0; i < size; i++) {
        pool->free_list[i] = &pool->buffers[i];
    }
    
    pthread_spin_init(&pool->lock, PTHREAD_PROCESS_PRIVATE);
}

struct packet *pool_alloc(struct packet_pool *pool) {
    struct packet *pkt = NULL;
    
    pthread_spin_lock(&pool->lock);
    if (pool->free_count > 0) {
        pkt = pool->free_list[--pool->free_count];
    }
    pthread_spin_unlock(&pool->lock);
    
    return pkt;
}

void pool_free(struct packet_pool *pool, struct packet *pkt) {
    pthread_spin_lock(&pool->lock);
    pool->free_list[pool->free_count++] = pkt;
    pthread_spin_unlock(&pool->lock);
}
```

Now allocation is O(1), deterministic, and never fails (assuming you sized the pool correctly). The lock adds overhead, but it's minimal compared to malloc's complexity.

### Best Practice: Pre-Allocate Everything Critical

At system initialization, allocate all buffers, data structures, and pools you'll need. Calculate maximum expected load and size pools accordingly. Add 20-30% headroom for bursts.

```c
/* Startup configuration */
void system_init(void) {
    /* Calculate based on line rate and latency budget */
    int max_packets_in_flight = (line_rate_gbps * 1000000000) / 
                                 (min_packet_size * 8) * 
                                 max_latency_seconds;
    
    /* Add headroom */
    int pool_size = max_packets_in_flight * 1.3;
    
    pool_init(&global_packet_pool, pool_size);
    
    /* Pre-allocate routing table */
    routing_table = calloc(MAX_ROUTES, sizeof(struct route));
    
    /* Pre-allocate connection tracking table */
    conn_table = calloc(MAX_CONNECTIONS, sizeof(struct connection));
}
```

**CRITICAL DON'T:** Never dynamically grow pools in production code paths. If you run out of buffers, you have a capacity planning problem, not a memory allocation problem. Log an error, drop packets if necessary, but don't call malloc.

## Stack vs Heap: Choosing the Right Tool

### Stack Allocation: Fast but Dangerous

The stack is blindingly fast—adjusting the stack pointer takes one instruction. But stacks are small (typically 1-8 MB) and overflowing the stack crashes your process with no recovery.

**Stack Allocation Rules:**

✅ **DO use stack for:** Small, fixed-size local variables (< 4KB)
❌ **DON'T use stack for:** Large buffers, variable-sized arrays, recursive algorithms with unbounded depth

```c
/* GOOD - small local variables on stack */
void parse_header(uint8_t *pkt) {
    struct ip_header hdr;  /* 20 bytes - fine */
    memcpy(&hdr, pkt, sizeof(hdr));
    
    if (hdr.version == 4) {
        process_ipv4(&hdr);
    }
}

/* DANGEROUS - large array on stack */
void parse_packet(uint8_t *pkt, int len) {
    char buffer[65536];  /* 64KB - WILL overflow stack eventually */
    memcpy(buffer, pkt, len);
}

/* CORRECT - use heap for large buffers */
void parse_packet(uint8_t *pkt, int len) {
    /* But don't malloc in fast path - use pre-allocated pool */
    char *buffer = pool_alloc(&large_buffer_pool);
    if (!buffer) {
        /* Handle allocation failure */
        return;
    }
    
    memcpy(buffer, pkt, len);
    /* ... process ... */
    pool_free(&large_buffer_pool, buffer);
}
```

### Variable Length Arrays: Just Don't

C99 introduced VLAs (Variable Length Arrays) allowing stack allocation based on runtime values. They seem convenient but are dangerous in network code where input sizes come from potentially malicious packets.

```c
/* EXTREMELY DANGEROUS */
void process_options(uint8_t *options, int option_len) {
    uint8_t local_copy[option_len];  /* VLA - attacker controls size! */
    memcpy(local_copy, options, option_len);
}
```

An attacker sends a packet with option_len = 1000000, your stack overflows, and your process crashes. Never use VLAs with untrusted input.

## Memory Leaks: The Silent Scale Killer

### Why Leaks Destroy Scale

In a server that runs for months, even tiny leaks compound. A 1KB leak per connection doesn't matter for 10 connections. For 100,000 connections, you've lost 100MB. For 1 million connections, 1GB is gone.

Leaks also fragment memory. As your process leaks and allocates, free memory becomes scattered, making large allocations fail even when total free memory suggests they should succeed.

### Common Leak Patterns

**Pattern 1: Early Return Without Cleanup**

```c
/* LEAKS - error path doesn't free */
int process_message(struct message *msg) {
    char *buffer = malloc(msg->size);
    struct parsed_msg *parsed = malloc(sizeof(*parsed));
    
    if (msg->type == INVALID) {
        return -1;  /* LEAK - buffer and parsed not freed */
    }
    
    if (!parse_message(buffer, parsed, msg)) {
        return -1;  /* LEAK */
    }
    
    handle_message(parsed);
    
    free(parsed);
    free(buffer);
    return 0;
}

/* FIXED - cleanup pattern */
int process_message(struct message *msg) {
    char *buffer = malloc(msg->size);
    struct parsed_msg *parsed = malloc(sizeof(*parsed));
    int result = -1;
    
    if (msg->type == INVALID) {
        goto cleanup;
    }
    
    if (!parse_message(buffer, parsed, msg)) {
        goto cleanup;
    }
    
    handle_message(parsed);
    result = 0;
    
cleanup:
    free(parsed);
    free(buffer);
    return result;
}
```

The goto cleanup pattern ensures resources are always freed regardless of exit path. Yes, goto is controversial, but this is one of the legitimate uses.

**Pattern 2: Lost Pointers**

```c
/* LEAKS - loses pointer to original allocation */
char *process_data(char *input) {
    char *data = malloc(1024);
    strcpy(data, input);
    
    /* Skip leading whitespace */
    while (*data == ' ') {
        data++;  /* Now data doesn't point to start! */
    }
    
    return data;  /* Caller can't free this correctly */
}

/* FIXED - preserve original pointer */
char *process_data(char *input) {
    char *data = malloc(1024);
    char *ptr = data;  /* Keep original pointer */
    strcpy(ptr, input);
    
    while (*ptr == ' ') {
        ptr++;
    }
    
    /* Move data down to start of buffer */
    memmove(data, ptr, strlen(ptr) + 1);
    return data;
}
```

**Pattern 3: Reference Counting Bugs**

```c
struct route {
    uint32_t dest;
    uint32_t mask;
    int refcount;
};

void route_ref(struct route *r) {
    __sync_fetch_and_add(&r->refcount, 1);
}

void route_unref(struct route *r) {
    if (__sync_sub_and_fetch(&r->refcount, 1) == 0) {
        free(r);
    }
}

/* SUBTLE LEAK */
void update_routing_table(struct route *old_route, struct route *new_route) {
    route_ref(new_route);
    install_route(new_route);
    
    /* Forget to unref old route! */
    /* Should have: route_unref(old_route); */
}
```

Reference counting is error-prone. Every ref must have a corresponding unref. Use static analysis tools (like Clang's analyzer) to catch these.

### Best Practice: Automated Leak Detection

**During Development:**

Use Valgrind to detect leaks:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./your_program
```

Use AddressSanitizer (ASan) during compilation:

```bash
gcc -fsanitize=address -g your_program.c -o your_program
```

ASan is faster than Valgrind and catches more errors (use-after-free, buffer overflows, etc.).

**In Production:**

Implement internal memory tracking:

```c
#ifdef MEMORY_TRACKING
struct allocation {
    void *ptr;
    size_t size;
    const char *file;
    int line;
};

static struct allocation *allocs = NULL;
static int alloc_count = 0;
static pthread_mutex_t alloc_lock = PTHREAD_MUTEX_INITIALIZER;

void *tracked_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    
    pthread_mutex_lock(&alloc_lock);
    /* Record allocation */
    allocs = realloc(allocs, (alloc_count + 1) * sizeof(struct allocation));
    allocs[alloc_count].ptr = ptr;
    allocs[alloc_count].size = size;
    allocs[alloc_count].file = file;
    allocs[alloc_count].line = line;
    alloc_count++;
    pthread_mutex_unlock(&alloc_lock);
    
    return ptr;
}

void tracked_free(void *ptr) {
    pthread_mutex_lock(&alloc_lock);
    /* Remove from tracking */
    for (int i = 0; i < alloc_count; i++) {
        if (allocs[i].ptr == ptr) {
            memmove(&allocs[i], &allocs[i+1], 
                    (alloc_count - i - 1) * sizeof(struct allocation));
            alloc_count--;
            break;
        }
    }
    pthread_mutex_unlock(&alloc_lock);
    
    free(ptr);
}

#define malloc(size) tracked_malloc(size, __FILE__, __LINE__)
#define free(ptr) tracked_free(ptr)
#endif
```

Monitor total allocations over time. Growing allocation counts indicate leaks.

## Memory Fragmentation: The Hidden Capacity Thief

### External Fragmentation Reality

You have 100MB free, but can't allocate a 10MB buffer because free memory is scattered in small chunks. This is external fragmentation, and it's deadly for long-running processes.

**Measuring Fragmentation:**

```c
void analyze_fragmentation(void) {
    /* Try allocating increasing sizes */
    size_t sizes[] = {1024, 4096, 16384, 65536, 262144, 1048576};
    
    for (int i = 0; i < sizeof(sizes)/sizeof(sizes[0]); i++) {
        void *ptr = malloc(sizes[i]);
        if (ptr) {
            printf("Can allocate %zu bytes\n", sizes[i]);
            free(ptr);
        } else {
            printf("Cannot allocate %zu bytes - fragmentation?\n", sizes[i]);
        }
    }
}
```

### Best Practice: Prevent Fragmentation

**Strategy 1: Use Fixed-Size Pools**

Pools eliminate external fragmentation for pooled objects. All objects are the same size, so any free slot can satisfy any allocation.

**Strategy 2: Slab Allocator**

Group allocations of similar sizes together. The kernel uses slab allocators extensively.

```c
struct slab {
    size_t object_size;
    int objects_per_slab;
    void *slab_memory;
    uint8_t *free_bitmap;
};

struct slab *slab_create(size_t obj_size, int count) {
    struct slab *s = malloc(sizeof(*s));
    s->object_size = obj_size;
    s->objects_per_slab = count;
    
    /* Single contiguous allocation */
    s->slab_memory = malloc(obj_size * count);
    s->free_bitmap = calloc((count + 7) / 8, 1);
    
    /* Mark all objects as free */
    memset(s->free_bitmap, 0xFF, (count + 7) / 8);
    
    return s;
}

void *slab_alloc(struct slab *s) {
    /* Find first free object */
    for (int i = 0; i < s->objects_per_slab; i++) {
        int byte = i / 8;
        int bit = i % 8;
        
        if (s->free_bitmap[byte] & (1 << bit)) {
            /* Mark as allocated */
            s->free_bitmap[byte] &= ~(1 << bit);
            return (uint8_t *)s->slab_memory + (i * s->object_size);
        }
    }
    
    return NULL;  /* Slab full */
}

void slab_free(struct slab *s, void *ptr) {
    /* Calculate object index */
    size_t offset = (uint8_t *)ptr - (uint8_t *)s->slab_memory;
    int index = offset / s->object_size;
    
    int byte = index / 8;
    int bit = index % 8;
    
    /* Mark as free */
    s->free_bitmap[byte] |= (1 << bit);
}
```

**Strategy 3: Arena Allocator for Related Objects**

If you're processing a transaction that allocates many temporary objects, use an arena. Allocate all objects from a large buffer, then free the entire arena at once.

```c
struct arena {
    uint8_t *buffer;
    size_t buffer_size;
    size_t used;
};

void arena_init(struct arena *a, size_t size) {
    a->buffer = malloc(size);
    a->buffer_size = size;
    a->used = 0;
}

void *arena_alloc(struct arena *a, size_t size) {
    /* Align to 8 bytes */
    size = (size + 7) & ~7;
    
    if (a->used + size > a->buffer_size) {
        return NULL;  /* Arena full */
    }
    
    void *ptr = a->buffer + a->used;
    a->used += size;
    return ptr;
}

void arena_reset(struct arena *a) {
    /* Free all allocations at once */
    a->used = 0;
}

void arena_destroy(struct arena *a) {
    free(a->buffer);
}

/* Usage example */
void process_transaction(struct transaction *txn) {
    struct arena arena;
    arena_init(&arena, 1024 * 1024);  /* 1MB arena */
    
    /* All allocations come from arena */
    struct temp_data *data = arena_alloc(&arena, sizeof(*data));
    char *buffer = arena_alloc(&arena, 4096);
    struct result *result = arena_alloc(&arena, sizeof(*result));
    
    /* Process transaction */
    /* ... */
    
    /* Free everything at once - no leaks possible */
    arena_destroy(&arena);
}
```

## Alignment and Padding: The Silent Performance Drain

### Why Alignment Matters

Modern CPUs access memory most efficiently when addresses are aligned to natural boundaries (4 bytes for 32-bit values, 8 bytes for 64-bit values). Misaligned accesses can be slower or even cause crashes on some architectures.

```c
/* Poor layout - lots of padding */
struct bad_packet {
    uint8_t type;        /* 1 byte */
    /* 3 bytes padding */
    uint32_t seq_num;    /* 4 bytes, requires 4-byte alignment */
    uint8_t flags;       /* 1 byte */
    /* 7 bytes padding */
    uint64_t timestamp;  /* 8 bytes, requires 8-byte alignment */
};  /* Total: 24 bytes (8 bytes wasted) */

/* Good layout - minimal padding */
struct good_packet {
    uint64_t timestamp;  /* 8 bytes */
    uint32_t seq_num;    /* 4 bytes */
    uint8_t type;        /* 1 byte */
    uint8_t flags;       /* 1 byte */
    uint8_t pad[2];      /* Explicit padding */
};  /* Total: 16 bytes (only 2 bytes wasted) */
```

**Best Practice:** Order struct members from largest to smallest to minimize padding.

### Cache Line Awareness

CPU caches load memory in cache lines (typically 64 bytes). If two threads frequently access different fields of the same struct, and those fields are in the same cache line, you get false sharing—each write by one thread invalidates the other's cache.

```c
/* BAD - false sharing */
struct counter {
    uint64_t thread1_count;
    uint64_t thread2_count;  /* Same cache line! */
};

/* GOOD - separate cache lines */
struct counter {
    uint64_t thread1_count;
    uint8_t pad1[64 - sizeof(uint64_t)];
    uint64_t thread2_count;
    uint8_t pad2[64 - sizeof(uint64_t)];
} __attribute__((aligned(64)));
```

Now each counter is in its own cache line, eliminating false sharing.

## Memory Barriers and Volatile: Correctness Under Concurrency

### When the Compiler and CPU Reorder Your Code

Modern compilers and CPUs reorder operations for performance. This breaks lockless algorithms if you're not careful.

```c
/* Lockless flag - WRONG */
int data_ready = 0;
int data = 0;

/* Thread 1 - producer */
data = 42;
data_ready = 1;  /* Compiler/CPU might reorder this before data = 42 */

/* Thread 2 - consumer */
if (data_ready) {
    use(data);  /* Might see old data value! */
}
```

**Solution: Memory Barriers**

```c
/* Thread 1 */
data = 42;
__sync_synchronize();  /* Full memory barrier */
data_ready = 1;

/* Thread 2 */
if (data_ready) {
    __sync_synchronize();
    use(data);  /* Now guaranteed to see data = 42 */
}
```

Or use atomic operations which include implicit barriers:

```c
int data_ready = 0;
int data = 0;

/* Thread 1 */
data = 42;
__atomic_store_n(&data_ready, 1, __ATOMIC_RELEASE);

/* Thread 2 */
if (__atomic_load_n(&data_ready, __ATOMIC_ACQUIRE)) {
    use(data);
}
```

### Volatile: Not What You Think

Many developers believe volatile prevents reordering. It doesn't. volatile only prevents the compiler from optimizing away reads/writes. It does NOT prevent CPU reordering and does NOT provide atomicity.

```c
volatile int flag = 0;

/* Thread 1 */
flag = 1;  /* Compiler won't optimize away, but CPU can reorder */

/* Thread 2 */
while (!flag);  /* Compiler won't optimize into infinite loop */
```

**Use volatile for:** Memory-mapped I/O registers, signal handlers, setjmp/longjmp.

**DON'T use volatile for:** Multithreading synchronization (use atomics or locks instead).

## Performance Best Practices Summary

### DO:
- ✅ Pre-allocate all critical buffers and pools at startup
- ✅ Use memory pools for fixed-size objects in fast paths
- ✅ Keep hot data structures cache-aligned and cache-line-aware
- ✅ Order struct members largest to smallest to minimize padding
- ✅ Use stack for small, fixed-size local variables
- ✅ Implement automated leak detection in development
- ✅ Profile your actual memory access patterns
- ✅ Use proper atomic operations or locks for synchronization
- ✅ Free resources on ALL code paths (use goto cleanup pattern)

### DON'T:
- ❌ Call malloc/free in packet processing hot paths
- ❌ Use VLAs with untrusted input
- ❌ Allocate large buffers (>4KB) on the stack
- ❌ Rely on volatile for thread synchronization
- ❌ Dynamically grow pools during normal operation
- ❌ Return early without freeing resources
- ❌ Assume malloc will always succeed
- ❌ Ignore alignment and padding
- ❌ Access memory-mapped registers without volatile
- ❌ Use reference counting without careful analysis

## Measurement and Validation

You can't optimize what you don't measure. Instrument your code:

```c
struct memory_stats {
    uint64_t allocs;
    uint64_t frees;
    uint64_t bytes_allocated;
    uint64_t bytes_freed;
    uint64_t peak_usage;
    uint64_t current_usage;
};

void update_stats(struct memory_stats *stats, size_t size, int is_alloc) {
    __atomic_add_fetch(&stats->allocs, 1, __ATOMIC_RELAXED);
    __atomic_add_fetch(&stats->bytes_allocated, size, __ATOMIC_RELAXED);
    
    uint64_t current = __atomic_add_fetch(&stats->current_usage, size, 
                                          __ATOMIC_RELAXED);
    
    /* Update peak */
    uint64_t peak;
    do {
        peak = __atomic_load_n(&stats->peak_usage, __ATOMIC_RELAXED);
        if (current <= peak) break;
    } while (!__atomic_compare_exchange_n(&stats->peak_usage, &peak, current,
                                         0, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
}
```

Monitor these metrics in production. Rising peak usage indicates leaks. High allocation rates indicate you're calling malloc too often.

## The Bottom Line

Memory management in high-performance network code is about control and predictability. You cannot tolerate allocation failures, unpredictable latency, or memory exhaustion. Pre-allocate, use pools, eliminate dynamic allocation from hot paths, and measure everything. The patterns in this guide are not theoretical—they're what separates code that scales from code that collapses under load.

# Thread Safety and Synchronization: High-Performance Patterns

## The Brutal Truth About Locks

Locks kill performance. Every mutex acquisition, every atomic operation, every memory barrier adds latency and limits parallelism. Your multi-core router with 16 CPUs will perform like a single-core machine if all threads are fighting over shared locks. This guide teaches you when to use locks, how to minimize their impact, and when to avoid them entirely.

## Understanding the Real Costs

### Lock Acquisition Isn't Free

A typical mutex lock/unlock pair costs 20-50 nanoseconds on modern hardware. Sounds trivial? Process 10 million packets per second, and you have 100 nanoseconds per packet total. Spend 50 nanoseconds on locking and you've consumed half your budget before doing any real work.

Contended locks are far worse. If another thread holds the lock, you don't just wait—you context switch, which costs thousands of nanoseconds. Under high contention, locks become serialization points that destroy parallelism.

```c
/* Measuring lock overhead */
#include <time.h>
#include <pthread.h>

void measure_lock_cost(void) {
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    struct timespec start, end;
    int iterations = 10000000;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < iterations; i++) {
        pthread_mutex_lock(&lock);
        pthread_mutex_unlock(&lock);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long ns = (end.tv_sec - start.tv_sec) * 1000000000L +
              (end.tv_nsec - start.tv_nsec);
    
    printf("Lock/unlock cost: %ld ns per iteration\n", ns / iterations);
}
```

Run this and understand your baseline cost. Then multiply by your packet rate.

## Lock-Free Design: The First Best Practice

### Design Principle: Avoid Sharing

The best lock is the one you never take. Design systems where threads don't share data.

**BAD: Shared Counter**

```c
pthread_mutex_t stats_lock;
uint64_t total_packets;

void process_packet(int thread_id, struct packet *pkt) {
    /* Process packet */
    
    /* Update shared counter - serialization point */
    pthread_mutex_lock(&stats_lock);
    total_packets++;
    pthread_mutex_unlock(&stats_lock);
}
```

Every thread contends for stats_lock. With 8 threads, you've serialized your entire packet processing pipeline.

**GOOD: Per-Thread Counters**

```c
#define MAX_THREADS 64
uint64_t per_thread_packets[MAX_THREADS] __attribute__((aligned(64)));

void process_packet(int thread_id, struct packet *pkt) {
    /* Process packet */
    
    /* No lock needed */
    per_thread_packets[thread_id]++;
}

uint64_t get_total_packets(void) {
    uint64_t total = 0;
    for (int i = 0; i < MAX_THREADS; i++) {
        total += per_thread_packets[i];
    }
    return total;
}
```

Now threads never contend. The cache line alignment prevents false sharing. You read all counters when generating statistics, which happens infrequently compared to packet processing.

### Per-CPU Data Structures

Take this further: allocate separate data structures for each CPU.

```c
struct per_cpu_data {
    struct packet_pool pool;
    struct flow_table flows;
    uint64_t packets_processed;
    uint64_t bytes_processed;
    /* Add more per-CPU state */
} __attribute__((aligned(64)));

struct per_cpu_data *cpu_data[MAX_CPUS];

void init_per_cpu_data(void) {
    for (int i = 0; i < num_cpus; i++) {
        cpu_data[i] = malloc(sizeof(struct per_cpu_data));
        packet_pool_init(&cpu_data[i]->pool, POOL_SIZE);
        flow_table_init(&cpu_data[i]->flows, MAX_FLOWS);
    }
}

void process_packet(struct packet *pkt) {
    int cpu = sched_getcpu();  /* Which CPU are we on? */
    struct per_cpu_data *data = cpu_data[cpu];
    
    /* No locks needed - only this CPU touches this data */
    data->packets_processed++;
    data->bytes_processed += pkt->len;
    
    /* Use CPU-local structures */
    struct flow *f = flow_lookup(&data->flows, pkt);
    /* ... */
}
```

**CRITICAL:** Pin threads to CPUs using CPU affinity. Otherwise the OS might migrate threads between CPUs, breaking the per-CPU assumption.

```c
void pin_thread_to_cpu(int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    
    pthread_t thread = pthread_self();
    pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
}
```

## When You Must Share: Choosing the Right Primitive

### Spinlocks vs Mutexes

**Spinlocks:** Loop continuously checking if lock is available. Fast if wait time is very short (microseconds). Wastes CPU if wait time is long.

**Mutexes:** Put thread to sleep if lock unavailable. Requires kernel involvement (context switch). Better for longer critical sections.

**Rule of thumb:** Use spinlocks for critical sections under ~100 nanoseconds. Use mutexes for longer sections.

```c
/* Spinlock - for very short critical sections */
pthread_spinlock_t spin;
pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);

pthread_spin_lock(&spin);
/* Critical section: < 100ns */
counter++;
pthread_spin_unlock(&spin);

/* Mutex - for longer critical sections */
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);

pthread_mutex_lock(&mutex);
/* Critical section: microseconds or more */
update_routing_table(route);
pthread_mutex_unlock(&mutex);
```

**DON'T:** Use spinlocks for I/O operations, memory allocation, or anything that might block. You'll spin for milliseconds wasting CPU.

### Read-Write Locks: Handle with Care

RW locks allow multiple readers or one writer. Sounds perfect for read-heavy workloads like routing tables. Reality is more complex.

```c
pthread_rwlock_t routing_lock;

/* Multiple readers can proceed simultaneously */
void lookup_route(uint32_t dest) {
    pthread_rwlock_rdlock(&routing_lock);
    struct route *r = find_route(dest);
    pthread_rwlock_unlock(&routing_lock);
}

/* Writer has exclusive access */
void update_route(struct route *r) {
    pthread_rwlock_wrlock(&routing_lock);
    install_route(r);
    pthread_rwlock_unlock(&routing_lock);
}
```

**The Problem:** RW locks have overhead. They're more complex than mutexes and slower to acquire. If your read critical section is very short, a simple mutex can outperform an RW lock despite blocking readers.

**When RW locks win:** Long read critical sections with infrequent writes. Reading a 100,000-entry routing table benefits from parallelism. Incrementing a counter doesn't.

**Better Alternative: RCU (Read-Copy-Update)**

Don't lock for reads at all. Writers create new versions of data structures and atomically switch pointers.

```c
struct route_table {
    struct route *routes;
    int count;
};

struct route_table *current_table;

/* Readers never lock */
struct route *lookup_route(uint32_t dest) {
    struct route_table *table = __atomic_load_n(&current_table, 
                                                __ATOMIC_ACQUIRE);
    
    /* Search table without holding any lock */
    for (int i = 0; i < table->count; i++) {
        if (table->routes[i].dest == dest) {
            return &table->routes[i];
        }
    }
    return NULL;
}

/* Writer creates new table */
void add_route(struct route *new_route) {
    struct route_table *old_table = current_table;
    
    /* Allocate new table */
    struct route_table *new_table = malloc(sizeof(*new_table));
    new_table->count = old_table->count + 1;
    new_table->routes = malloc(new_table->count * sizeof(struct route));
    
    /* Copy old routes */
    memcpy(new_table->routes, old_table->routes,
           old_table->count * sizeof(struct route));
    
    /* Add new route */
    new_table->routes[old_table->count] = *new_route;
    
    /* Atomic pointer swap */
    __atomic_store_n(&current_table, new_table, __ATOMIC_RELEASE);
    
    /* Wait for all readers to finish with old table */
    /* (RCU implementation detail - use RCU library in practice) */
    synchronize_rcu();
    
    /* Free old table */
    free(old_table->routes);
    free(old_table);
}
```

Readers proceed at full speed with zero synchronization overhead. Writers pay the cost of copying data structures, but routing updates are infrequent compared to lookups.

## Atomic Operations: Lock-Free Primitives

Modern CPUs provide atomic operations that execute without locks. Use them for simple operations on shared variables.

### Atomic Increment/Decrement

```c
/* Atomic counter - no lock needed */
uint64_t packet_count = 0;

void process_packet(struct packet *pkt) {
    /* Process packet */
    
    /* Atomic increment - no lock */
    __atomic_add_fetch(&packet_count, 1, __ATOMIC_RELAXED);
}
```

**Memory Ordering:** The second parameter controls memory ordering semantics:

- `__ATOMIC_RELAXED`: No ordering constraints. Fastest. Use for independent counters.
- `__ATOMIC_ACQUIRE`: Prevents reordering of subsequent reads. Use for lock acquisition.
- `__ATOMIC_RELEASE`: Prevents reordering of prior writes. Use for lock release.
- `__ATOMIC_SEQ_CST`: Full sequential consistency. Slowest but simplest semantics.

For simple counters, `__ATOMIC_RELAXED` is sufficient and fastest.

### Compare-and-Swap (CAS)

CAS atomically checks if a value equals an expected value, and if so, updates it. This is the foundation of lock-free algorithms.

```c
/* Lock-free stack push */
struct node {
    void *data;
    struct node *next;
};

struct node *stack_top = NULL;

void push(struct node *new_node) {
    struct node *old_top;
    
    do {
        old_top = __atomic_load_n(&stack_top, __ATOMIC_ACQUIRE);
        new_node->next = old_top;
    } while (!__atomic_compare_exchange_n(&stack_top, &old_top, new_node,
                                         0, __ATOMIC_RELEASE, __ATOMIC_ACQUIRE));
}

struct node *pop(void) {
    struct node *old_top, *new_top;
    
    do {
        old_top = __atomic_load_n(&stack_top, __ATOMIC_ACQUIRE);
        if (!old_top) return NULL;
        new_top = old_top->next;
    } while (!__atomic_compare_exchange_n(&stack_top, &old_top, new_top,
                                         0, __ATOMIC_RELEASE, __ATOMIC_ACQUIRE));
    
    return old_top;
}
```

Under low contention, this is faster than locking. Under high contention, many threads might retry the CAS loop, wasting CPU. Choose based on expected contention level.

**ABA Problem:** CAS has a subtle bug. If the stack top is A, another thread pops A and B, then pushes A back, your CAS succeeds even though the stack changed. Solutions include generation counters or pointer tagging.

## Minimizing Critical Sections

When you must lock, hold the lock for the shortest time possible.

### BAD: Large Critical Section

```c
void process_and_log(struct packet *pkt) {
    pthread_mutex_lock(&global_lock);
    
    /* Lots of work inside critical section */
    parse_packet(pkt);
    validate_checksum(pkt);
    lookup_route(pkt);
    format_log_message(pkt);
    write_log(pkt);
    update_statistics(pkt);
    
    pthread_mutex_unlock(&global_lock);
}
```

All threads serialize through this lock. If each operation takes 1 microsecond, the lock is held for 6 microseconds. At 1 million packets/second, you need 6 threads just to keep up, and they'll all contend for the lock.

### GOOD: Minimal Critical Section

```c
void process_and_log(struct packet *pkt) {
    /* Do all independent work outside critical section */
    parse_packet(pkt);
    validate_checksum(pkt);
    struct route *r = lookup_route_lockfree(pkt);
    char *log_msg = format_log_message(pkt);
    
    /* Only lock for the shared resource update */
    pthread_mutex_lock(&stats_lock);
    update_statistics(pkt);
    pthread_mutex_unlock(&stats_lock);
    
    /* Write log outside critical section if possible */
    write_log(log_msg);
    free(log_msg);
}
```

Now the lock is held for microseconds instead of milliseconds. Contention drops dramatically.

### Lock Ordering: Preventing Deadlock

If you must acquire multiple locks, always acquire them in the same order globally.

```c
pthread_mutex_t lock_a, lock_b;

/* BAD - can deadlock */
void thread_1(void) {
    pthread_mutex_lock(&lock_a);
    pthread_mutex_lock(&lock_b);
    /* ... */
    pthread_mutex_unlock(&lock_b);
    pthread_mutex_unlock(&lock_a);
}

void thread_2(void) {
    pthread_mutex_lock(&lock_b);  /* Opposite order! */
    pthread_mutex_lock(&lock_a);
    /* ... */
    pthread_mutex_unlock(&lock_a);
    pthread_mutex_unlock(&lock_b);
}

/* GOOD - consistent ordering */
void thread_1(void) {
    pthread_mutex_lock(&lock_a);  /* Always A then B */
    pthread_mutex_lock(&lock_b);
    /* ... */
    pthread_mutex_unlock(&lock_b);
    pthread_mutex_unlock(&lock_a);
}

void thread_2(void) {
    pthread_mutex_lock(&lock_a);  /* Same order */
    pthread_mutex_lock(&lock_b);
    /* ... */
    pthread_mutex_unlock(&lock_b);
    pthread_mutex_unlock(&lock_a);
}
```

**Dynamic Ordering:** If lock order isn't static (e.g., locking two flows based on packet content), impose an artificial order based on memory address:

```c
void lock_two_flows(struct flow *f1, struct flow *f2) {
    if (f1 < f2) {
        pthread_mutex_lock(&f1->lock);
        pthread_mutex_lock(&f2->lock);
    } else {
        pthread_mutex_lock(&f2->lock);
        pthread_mutex_lock(&f1->lock);
    }
}
```

## Lock-Free Data Structures

### Ring Buffers: Single-Producer Single-Consumer

For passing data between two threads, lock-free ring buffers are ideal.

```c
#define RING_SIZE 1024

struct ring_buffer {
    void *items[RING_SIZE];
    uint32_t write_pos;
    uint32_t read_pos;
};

/* Producer thread */
int ring_push(struct ring_buffer *ring, void *item) {
    uint32_t write_pos = __atomic_load_n(&ring->write_pos, __ATOMIC_RELAXED);
    uint32_t read_pos = __atomic_load_n(&ring->read_pos, __ATOMIC_ACQUIRE);
    uint32_t next_write = (write_pos + 1) % RING_SIZE;
    
    if (next_write == read_pos) {
        return -1;  /* Ring full */
    }
    
    ring->items[write_pos] = item;
    __atomic_store_n(&ring->write_pos, next_write, __ATOMIC_RELEASE);
    return 0;
}

/* Consumer thread */
void *ring_pop(struct ring_buffer *ring) {
    uint32_t read_pos = __atomic_load_n(&ring->read_pos, __ATOMIC_RELAXED);
    uint32_t write_pos = __atomic_load_n(&ring->write_pos, __ATOMIC_ACQUIRE);
    
    if (read_pos == write_pos) {
        return NULL;  /* Ring empty */
    }
    
    void *item = ring->items[read_pos];
    __atomic_store_n(&ring->read_pos, (read_pos + 1) % RING_SIZE, 
                     __ATOMIC_RELEASE);
    return item;
}
```

This works only for single producer/consumer. Multiple producers or consumers require CAS operations and are more complex.

### Hash Tables with Fine-Grained Locking

Instead of one lock for the entire hash table, use one lock per bucket (or group of buckets).

```c
#define HASH_BUCKETS 1024
#define LOCKS_PER_BUCKET 1  /* Or fewer locks covering multiple buckets */

struct hash_table {
    struct list_head buckets[HASH_BUCKETS];
    pthread_mutex_t locks[HASH_BUCKETS / LOCKS_PER_BUCKET];
};

void hash_insert(struct hash_table *table, uint32_t key, void *value) {
    int bucket = key % HASH_BUCKETS;
    int lock_idx = bucket / LOCKS_PER_BUCKET;
    
    pthread_mutex_lock(&table->locks[lock_idx]);
    list_add(&table->buckets[bucket], value);
    pthread_mutex_unlock(&table->locks[lock_idx]);
}
```

Now threads accessing different buckets don't contend. Trade-off: more locks mean more memory, and lock arrays can cause cache misses. Balance granularity vs overhead.

## Producer-Consumer Patterns

### Condition Variables: Efficient Waiting

Don't spin-wait for events. Use condition variables to sleep until data is available.

```c
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t data_available = PTHREAD_COND_INITIALIZER;
struct packet_queue queue;

/* Producer */
void enqueue(struct packet *pkt) {
    pthread_mutex_lock(&queue_lock);
    
    queue_add(&queue, pkt);
    
    pthread_cond_signal(&data_available);  /* Wake one consumer */
    pthread_mutex_unlock(&queue_lock);
}

/* Consumer */
struct packet *dequeue(void) {
    pthread_mutex_lock(&queue_lock);
    
    while (queue_empty(&queue)) {
        /* Atomically release lock and sleep */
        pthread_cond_wait(&data_available, &queue_lock);
    }
    
    struct packet *pkt = queue_remove(&queue);
    pthread_mutex_unlock(&queue_lock);
    return pkt;
}
```

**Critical:** Always check the condition in a loop, not just once. Spurious wakeups can occur.

### Batch Processing: Amortizing Lock Cost

Instead of locking for each item, process items in batches.

```c
#define BATCH_SIZE 32

void process_queue_batched(void) {
    struct packet *batch[BATCH_SIZE];
    int count;
    
    while (1) {
        /* Lock once, dequeue multiple items */
        pthread_mutex_lock(&queue_lock);
        count = 0;
        while (count < BATCH_SIZE && !queue_empty(&queue)) {
            batch[count++] = queue_remove(&queue);
        }
        pthread_mutex_unlock(&queue_lock);
        
        /* Process entire batch without holding lock */
        for (int i = 0; i < count; i++) {
            process_packet(batch[i]);
        }
        
        if (count == 0) {
            usleep(100);  /* Or use condition variable */
        }
    }
}
```

You've reduced lock acquisitions by 32x. At high packet rates, this is the difference between keeping up and falling behind.

## Thread Safety Anti-Patterns

### Don't: Trust "Probably Atomic" Operations

```c
/* NOT ATOMIC despite appearances */
shared_counter++;

/* Compiles to:
   load shared_counter into register
   add 1 to register
   store register to shared_counter
   
   Two threads can interleave these operations.
*/
```

Use explicit atomics or locks for shared updates.

### Don't: Assume Volatile is Enough

```c
volatile int flag = 0;

/* Thread 1 */
data = 42;
flag = 1;  /* volatile doesn't prevent CPU reordering! */

/* Thread 2 */
if (flag) {
    use(data);  /* Might see stale data */
}
```

Use memory barriers or atomic operations.

### Don't: Hold Locks While Blocking

```c
/* WRONG - holding lock during I/O */
pthread_mutex_lock(&lock);
read(fd, buffer, size);  /* Blocks other threads during I/O! */
pthread_mutex_unlock(&lock);

/* CORRECT - release lock before blocking */
pthread_mutex_lock(&lock);
struct data *local_copy = copy_data(shared_data);
pthread_mutex_unlock(&lock);

read(fd, buffer, size);  /* Other threads can proceed */
```

### Don't: Lock Inside Signal Handlers

Signal handlers can interrupt code at any point. If you lock in a signal handler, and the signal interrupts code already holding that lock, you deadlock with yourself.

```c
pthread_mutex_t lock;

void signal_handler(int sig) {
    pthread_mutex_lock(&lock);  /* DEADLOCK if main code holds lock */
    /* ... */
    pthread_mutex_unlock(&lock);
}
```

Use signal-safe functions only, or use signalfd/eventfd to convert signals to normal I/O.

## Performance Best Practices Summary

### DO:
- ✅ Design for minimal sharing (per-thread/per-CPU data)
- ✅ Use atomic operations for simple counters
- ✅ Keep critical sections as short as possible
- ✅ Pin threads to CPUs for per-CPU data structures
- ✅ Use cache line alignment to prevent false sharing
- ✅ Batch operations to amortize lock overhead
- ✅ Impose consistent lock ordering to prevent deadlock
- ✅ Use RCU for read-heavy workloads
- ✅ Profile lock contention with tools (perf, valgrind --tool=helgrind)
- ✅ Use condition variables instead of spin-waiting

### DON'T:
- ❌ Share data between threads unless absolutely necessary
- ❌ Use locks in fast packet processing paths if avoidable
- ❌ Hold locks during I/O or blocking operations
- ❌ Assume operations are atomic without verification
- ❌ Use volatile for thread synchronization
- ❌ Use RW locks for short read critical sections
- ❌ Use spinlocks for long critical sections
- ❌ Forget memory ordering when using atomics
- ❌ Lock in signal handlers
- ❌ Acquire locks in inconsistent orders

## Measuring Lock Contention

You can't optimize what you don't measure. Instrument your code:

```c
struct lock_stats {
    uint64_t acquisitions;
    uint64_t contentions;
    uint64_t total_wait_ns;
};

struct instrumented_lock {
    pthread_mutex_t lock;
    struct lock_stats stats;
};

void lock_acquire(struct instrumented_lock *l) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    int ret = pthread_mutex_trylock(&l->lock);
    if (ret == EBUSY) {
        /* Contention detected */
        __atomic_add_fetch(&l->stats.contentions, 1, __ATOMIC_RELAXED);
        pthread_mutex_lock(&l->lock);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    uint64_t wait_ns = (end.tv_sec - start.tv_sec) * 1000000000 +
                       (end.tv_nsec - start.tv_nsec);
    
    __atomic_add_fetch(&l->stats.acquisitions, 1, __ATOMIC_RELAXED);
    __atomic_add_fetch(&l->stats.total_wait_ns, wait_ns, __ATOMIC_RELAXED);
}

void print_lock_stats(struct instrumented_lock *l) {
    printf("Acquisitions: %lu\n", l->stats.acquisitions);
    printf("Contentions: %lu (%.2f%%)\n", 
           l->stats.contentions,
           100.0 * l->stats.contentions / l->stats.acquisitions);
    printf("Avg wait: %lu ns\n", 
           l->stats.total_wait_ns / l->stats.acquisitions);
}
```

High contention percentages (>5%) indicate you need to redesign to reduce sharing or use finer-grained locking.

## The Bottom Line

Thread synchronization in high-performance systems is about eliminating contention. Design to avoid sharing data. When you must share, use the lightest synchronization that provides necessary guarantees. Profile relentlessly to find unexpected contention. Remember: the fastest lock is the one you never take.

# Cache Optimization: Making Your Code CPU-Friendly

## Why Cache Matters More Than You Think

Your CPU can execute billions of instructions per second, but RAM responds in hundreds of nanoseconds. Without caches, your processor would spend 99% of its time idle waiting for memory. Cache optimization is not optional for high-performance network code—it's the difference between processing 10 million packets per second and 100,000.

## The Cache Hierarchy Reality

Modern processors have multiple cache levels, each faster and smaller than the last:

- **L1 Cache:** 32-128 KB per core, 3-4 cycle access (~1 ns)
- **L2 Cache:** 256 KB-2 MB per core, 10-20 cycles (~3-7 ns)
- **L3 Cache:** 8-64 MB shared across cores, 40-75 cycles (~15-30 ns)
- **RAM:** Gigabytes, 200-300 cycles (~100 ns)

Notice the pattern: each level is 3-10x slower than the previous. A cache miss to RAM costs 100x more than an L1 hit. Process 10 million packets per second and you have 100 nanoseconds per packet total. A single RAM access consumes your entire budget.

### Measuring Your Cache Behavior

Before optimizing, measure. Use performance counters to see cache hit rates.

```c
#include <stdio.h>
#include <time.h>

void measure_access_pattern(int *array, int size, int stride) {
    struct timespec start, end;
    long accesses = 10000000;
    volatile int sum = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (long i = 0; i < accesses; i++) {
        int idx = (i * stride) % size;
        sum += array[idx];
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long ns = (end.tv_sec - start.tv_sec) * 1000000000L +
              (end.tv_nsec - start.tv_nsec);
    
    printf("Size: %d, Stride: %d, Avg: %ld ns per access\n",
           size, stride, ns / accesses);
}

int main() {
    /* Test different sizes to see cache boundaries */
    int sizes[] = {1024, 4096, 32768, 262144, 2097152};
    
    for (int i = 0; i < 5; i++) {
        int *array = malloc(sizes[i] * sizeof(int));
        
        /* Sequential access */
        measure_access_pattern(array, sizes[i], 1);
        
        /* Random access */
        measure_access_pattern(array, sizes[i], 997);  /* Prime number */
        
        free(array);
    }
}
```

Run this and watch access time jump when arrays exceed cache sizes. Sequential access shows predictable prefetching benefits. Random access shows raw cache miss penalties.

## Cache Line Basics: The 64-Byte Reality

CPUs don't fetch individual bytes—they fetch cache lines, typically 64 bytes. When you access a single byte, the CPU loads the entire 64-byte line containing it.

**Implication 1: Spatial Locality Matters**

If your data is arranged so frequently accessed items are near each other, multiple accesses hit the same cache line. This is spatial locality.

```c
/* BAD - poor spatial locality */
struct packet_stats {
    uint64_t rx_packets;
    uint64_t rx_bytes;
    uint64_t tx_packets;
    uint64_t tx_bytes;
    uint64_t errors;
} __attribute__((packed));

struct packet_stats per_cpu_stats[64];

void update_rx_stats(int cpu, int bytes) {
    /* These two fields are in same cache line */
    per_cpu_stats[cpu].rx_packets++;
    per_cpu_stats[cpu].rx_bytes += bytes;
}

void update_tx_stats(int cpu, int bytes) {
    /* But different CPU updating TX stats loads same cache line!
       False sharing if another CPU updated RX recently */
    per_cpu_stats[cpu].tx_packets++;
    per_cpu_stats[cpu].tx_bytes += bytes;
}
```

**Implication 2: False Sharing Kills Multicore Performance**

False sharing occurs when two threads access different variables that reside in the same cache line. Every write by one thread invalidates the cache line in other CPU's caches, causing expensive cache coherency traffic.

```c
/* GOOD - eliminate false sharing */
struct packet_stats {
    uint64_t rx_packets;
    uint64_t rx_bytes;
    uint64_t tx_packets;
    uint64_t tx_bytes;
    uint64_t errors;
    uint8_t pad[64 - 40];  /* Pad to 64 bytes */
} __attribute__((aligned(64)));
```

Now each CPU's stats structure occupies its own cache line. No false sharing.

## Optimization Pattern 1: Sequential Access

The single most important cache optimization is accessing memory sequentially. Hardware prefetchers detect sequential patterns and load future cache lines before you request them.

**BAD: Random Access**

```c
/* Hash table with separate chaining */
struct hash_entry {
    uint32_t key;
    uint32_t value;
    struct hash_entry *next;
};

struct hash_entry *table[1024];

uint32_t lookup(uint32_t key) {
    int bucket = key % 1024;
    struct hash_entry *entry = table[bucket];
    
    /* Chase pointers - random memory access */
    while (entry) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = entry->next;  /* Next could be anywhere in memory */
    }
    
    return 0;
}
```

Each pointer dereference is a potential cache miss. For long chains, you're guaranteed multiple cache misses per lookup.

**GOOD: Linear Probing**

```c
#define TABLE_SIZE 1024

struct hash_entry {
    uint32_t key;
    uint32_t value;
    uint8_t occupied;
};

struct hash_entry table[TABLE_SIZE];

uint32_t lookup(uint32_t key) {
    int idx = key % TABLE_SIZE;
    
    /* Linear scan - sequential memory access */
    for (int i = 0; i < TABLE_SIZE; i++) {
        int probe = (idx + i) % TABLE_SIZE;
        
        if (!table[probe].occupied) {
            return 0;  /* Not found */
        }
        
        if (table[probe].key == key) {
            return table[probe].value;
        }
    }
    
    return 0;
}
```

Now entries are contiguous in memory. Scanning consecutive entries means high cache hit rates. Prefetchers load future entries before you access them.

**Trade-off:** Linear probing has worse worst-case performance (O(n) vs O(1) for ideal hashing). But with good hash functions and reasonable load factors (<0.7), average performance is better due to cache efficiency.

## Optimization Pattern 2: Structure of Arrays vs Array of Structures

How you organize data profoundly impacts cache utilization.

**Array of Structures (AoS):**

```c
struct particle {
    float x, y, z;
    float vx, vy, vz;
    float mass;
};

struct particle particles[10000];

void update_positions(float dt) {
    for (int i = 0; i < 10000; i++) {
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
        particles[i].z += particles[i].vz * dt;
    }
}
```

Each iteration loads 28 bytes (7 floats) but only uses 12 bytes (x, y, z, vx, vy, vz). You're wasting cache bandwidth loading mass which isn't used.

**Structure of Arrays (SoA):**

```c
struct particles {
    float x[10000];
    float y[10000];
    float z[10000];
    float vx[10000];
    float vy[10000];
    float vz[10000];
    float mass[10000];
};

struct particles particles;

void update_positions(float dt) {
    for (int i = 0; i < 10000; i++) {
        particles.x[i] += particles.vx[i] * dt;
        particles.y[i] += particles.vy[i] * dt;
        particles.z[i] += particles.vz[i] * dt;
    }
}
```

Now positions and velocities are contiguous. Each cache line contains 16 x-coordinates (64 bytes / 4 bytes). You're using all loaded data. Prefetching is more effective because access pattern is completely linear.

**When to use SoA:**
- Processing many objects of the same type
- Operations touch only subset of fields
- Performance critical loops

**When to use AoS:**
- Operations naturally work on complete objects
- Random access patterns (list traversal)
- Code clarity matters more than performance

## Optimization Pattern 3: Data Structure Packing

Smaller data structures mean more fit in cache. Be ruthless about size.

**BAD: Wasteful Structure**

```c
struct flow {
    uint32_t src_ip;        /* 4 bytes */
    uint16_t src_port;      /* 2 bytes */
    uint32_t dst_ip;        /* 4 bytes */
    uint16_t dst_port;      /* 2 bytes */
    uint64_t packet_count;  /* 8 bytes */
    uint64_t byte_count;    /* 8 bytes */
    time_t last_seen;       /* 8 bytes */
    uint8_t protocol;       /* 1 byte */
};  /* Total: 37 bytes, but likely 40 due to padding */
```

**GOOD: Packed Structure**

```c
struct flow {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint64_t packet_count;
    uint64_t byte_count;
    uint32_t last_seen;     /* Use uint32_t for timestamps, offset from epoch */
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
    uint8_t flags;          /* Use remaining byte for flags */
    uint16_t pad;           /* Explicit padding for alignment */
};  /* Total: 32 bytes - fits exactly in half a cache line */
```

You've reduced size from 40 to 32 bytes—20% reduction. In a flow table with 1 million entries, you've saved 8 MB. That's potentially L3 cache size.

**Bit fields for rare flags:**

```c
struct tcp_flags {
    uint8_t fin : 1;
    uint8_t syn : 1;
    uint8_t rst : 1;
    uint8_t psh : 1;
    uint8_t ack : 1;
    uint8_t urg : 1;
    uint8_t ece : 1;
    uint8_t cwr : 1;
};  /* 1 byte instead of 8 separate bytes */
```

**Warning:** Bit fields have performance cost—reading requires masking and shifting. Use only when space savings justify the cost.

## Optimization Pattern 4: Hot/Cold Data Splitting

Not all structure fields are accessed equally. Separate frequently accessed (hot) data from rarely accessed (cold) data.

```c
/* BAD - mixing hot and cold data */
struct connection {
    /* Hot - accessed every packet */
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint64_t packet_count;
    
    /* Cold - accessed rarely */
    time_t created_at;
    time_t last_accessed;
    char username[64];
    char process_name[256];
};

/* GOOD - separate hot and cold */
struct connection_hot {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint64_t packet_count;
    struct connection_cold *cold;  /* Pointer to cold data */
} __attribute__((aligned(64)));

struct connection_cold {
    time_t created_at;
    time_t last_accessed;
    char username[64];
    char process_name[256];
};
```

Fast path only loads hot structure (one cache line). Cold data is loaded only when needed (displaying connection info to user).

## Optimization Pattern 5: Prefetching

Manually instruct CPU to load data before you need it.

```c
void process_packets(struct packet **packets, int count) {
    for (int i = 0; i < count; i++) {
        /* Prefetch next packet while processing current */
        if (i + 1 < count) {
            __builtin_prefetch(packets[i + 1], 0, 3);
            /* Second param: 0=read, 1=write
               Third param: 0=no temporal locality, 3=high temporal locality */
        }
        
        process_packet(packets[i]);
    }
}
```

The prefetch hides memory latency by loading the next packet while you're still processing the current one.

**Prefetch timing:** Start prefetch ~100-200 cycles before you need data. Too early wastes cache space. Too late doesn't hide latency.

```c
/* Prefetch multiple elements ahead */
#define PREFETCH_DISTANCE 4

void process_flow_table(struct flow *flows, int count) {
    for (int i = 0; i < count; i++) {
        if (i + PREFETCH_DISTANCE < count) {
            __builtin_prefetch(&flows[i + PREFETCH_DISTANCE], 0, 3);
        }
        
        update_flow(&flows[i]);
    }
}
```

Experiment with prefetch distance. Too far ahead evicts other useful data. Too close doesn't hide latency.

## Optimization Pattern 6: Cache-Aware Algorithms

Choose algorithms based on cache behavior, not just O() complexity.

**Binary Search vs Linear Search:**

Binary search is O(log n), linear search is O(n). For small arrays in cache, linear search can be faster.

```c
int linear_search(int *array, int n, int target) {
    for (int i = 0; i < n; i++) {
        if (array[i] == target) return i;
    }
    return -1;
}

int binary_search(int *array, int n, int target) {
    int left = 0, right = n - 1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        if (array[mid] == target) return mid;
        if (array[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
```

For n=64 (fits in one cache line), linear search: 64 comparisons, all in cache. Binary search: log2(64)=6 comparisons, but each access is to a different cache line—6 potential cache misses.

**Measurement:**

```c
/* Benchmark shows linear search wins for small n */
/* n=64:   linear ~50ns, binary ~150ns (cache misses dominate)
   n=1024: linear ~800ns, binary ~300ns (binary search wins) */
```

**Rule:** For arrays under 128 elements that fit in 1-2 cache lines, linear search often wins despite worse asymptotic complexity.

## Real-World Example: Routing Table Lookup

Routing requires finding longest prefix match. Naive implementation uses tree structures with terrible cache behavior.

**BAD: Trie with Pointers**

```c
struct trie_node {
    struct trie_node *left;   /* Next bit is 0 */
    struct trie_node *right;  /* Next bit is 1 */
    struct route *route;      /* NULL if internal node */
};

struct route *lookup(struct trie_node *root, uint32_t dest) {
    struct route *best = NULL;
    struct trie_node *node = root;
    
    for (int i = 31; i >= 0 && node; i--) {
        if (node->route) best = node->route;
        
        int bit = (dest >> i) & 1;
        node = bit ? node->right : node->left;  /* Cache miss every step */
    }
    
    return best;
}
```

Each level of the trie is a separate allocation. Worst case: 32 cache misses for IPv4 lookup.

**GOOD: Compressed Array**

```c
/* Level-compressed trie in array */
#define STRIDE_BITS 8

struct lookup_level {
    struct route *routes[256];  /* 2^STRIDE_BITS entries */
    uint16_t next_level[256];   /* Index to next level, or 0 */
};

struct routing_table {
    struct lookup_level *levels;
    int num_levels;
};

struct route *lookup(struct routing_table *table, uint32_t dest) {
    struct route *best = NULL;
    int level_idx = 0;
    
    for (int i = 24; i >= 0 && level_idx; i -= 8) {
        uint8_t chunk = (dest >> i) & 0xFF;
        struct lookup_level *level = &table->levels[level_idx];
        
        if (level->routes[chunk]) {
            best = level->routes[chunk];
        }
        
        level_idx = level->next_level[chunk];
    }
    
    return best;
}
```

Each level is contiguous array. One level fits in 2-4 cache lines. Worst case: 4 levels × 4 cache lines = 16 cache lines loaded vs 32 in pointer-based trie. Typically better because most routes resolve in 1-2 levels.

## Measuring and Profiling

### Using perf for Cache Analysis

Linux perf tool can measure cache behavior:

```bash
# Count cache events
perf stat -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./your_program

# Profile cache misses
perf record -e cache-misses ./your_program
perf report
```

Look for:
- High cache miss rates (>3-5% is concerning)
- Hot functions with many cache misses
- False sharing (cache-line contention)

### Cache Simulator

For detailed analysis, use cache simulators like Cachegrind:

```bash
valgrind --tool=cachegrind ./your_program
```

This shows instruction cache misses, data cache misses, and last-level cache misses per function.

## Best Practices Summary

### DO:
- ✅ Access memory sequentially whenever possible
- ✅ Pack data structures to fit more in cache
- ✅ Align hot data to cache line boundaries (64 bytes)
- ✅ Separate hot and cold fields into different structures
- ✅ Use SoA for bulk processing operations
- ✅ Prefetch data before you need it
- ✅ Choose cache-friendly algorithms (linear search for small arrays)
- ✅ Profile with perf to find actual cache bottlenecks
- ✅ Keep working set under L3 cache size if possible
- ✅ Group related data together (spatial locality)

### DON'T:
- ❌ Chase pointers in tight loops (linked lists, trees)
- ❌ Access data in random order
- ❌ Mix hot and cold data in same structure
- ❌ Allow false sharing between cores (pad to cache line)
- ❌ Use structures larger than necessary
- ❌ Assume O() complexity predicts performance
- ❌ Ignore alignment (use __attribute__((aligned(64))))
- ❌ Prefetch too early or too late
- ❌ Skip profiling—intuition is often wrong about cache behavior
- ❌ Optimize before measuring—profile first

## The Hierarchy of Cache Optimization

1. **Design for sequential access** - Biggest impact, fundamental to all else
2. **Reduce working set size** - Fit hot data in cache
3. **Eliminate false sharing** - Critical for multicore
4. **Pack data structures** - More data per cache line
5. **Split hot/cold data** - Don't pollute cache with cold data
6. **Add prefetching** - Hide remaining latency

Work top-down. Sequential access alone can give 10-100x improvements. Prefetching might give another 20-30%. Start with fundamentals.

## The Bottom Line

Cache optimization is not premature optimization—it's fundamental to writing code that executes at hardware speed rather than memory speed. Modern CPUs are fast enough to process wire-speed network traffic, but only if you feed them from cache. Master cache-aware programming and your code will scale; ignore it and you'll spend millions on CPU cores that sit idle waiting for RAM.

# Inter-Process Communication: Performance and Scalability

## The IPC Performance Hierarchy

Not all IPC mechanisms are created equal. The performance gap between fastest and slowest can be 100x. Choose wrong and your multiprocess architecture becomes a bottleneck. Choose right and you achieve near-thread-level performance with process-level isolation.

**Performance Ranking (fastest to slowest):**
1. Shared memory (50-100 ns per message)
2. Unix domain sockets (1-3 µs per message)
3. Pipes (1-3 µs per message)
4. TCP loopback (5-10 µs per message)
5. Network RPC (milliseconds)

These numbers matter. At 1 million messages per second, shared memory uses 5-10% CPU. TCP loopback uses 50%. Choose based on requirements for isolation, network transparency, and throughput.

## Shared Memory: Maximum Performance

Shared memory provides the fastest IPC because no data copying occurs. Both processes map the same physical memory. One process writes, another reads—no kernel involvement after initial setup.

### Basic Shared Memory

```c
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

/* Process A - create and write */
int create_shared_memory(void) {
    int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return -1;
    }
    
    /* Set size */
    ftruncate(fd, 4096);
    
    /* Map into address space */
    void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    
    /* Write data */
    strcpy(ptr, "Hello from Process A");
    
    /* Don't close fd yet - keep mapping alive */
    return fd;
}

/* Process B - open and read */
void *open_shared_memory(void) {
    int fd = shm_open("/my_shm", O_RDWR, 0);
    if (fd == -1) {
        perror("shm_open");
        return NULL;
    }
    
    void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    
    /* Read data */
    printf("Process B reads: %s\n", (char *)ptr);
    
    return ptr;
}
```

**The Problem:** This is too simple for production. No synchronization. Process B might read while Process A is writing. You'll see corrupted data.

### Shared Memory with Synchronization

Add mutexes and condition variables in shared memory itself.

```c
struct shared_data {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int ready;
    char message[256];
};

/* Initialize shared mutex and condition variable */
void init_shared_sync(struct shared_data *data) {
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&data->lock, &mutex_attr);
    
    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&data->cond, &cond_attr);
    
    data->ready = 0;
}

/* Producer */
void send_message(struct shared_data *data, const char *msg) {
    pthread_mutex_lock(&data->lock);
    strcpy(data->message, msg);
    data->ready = 1;
    pthread_cond_signal(&data->cond);
    pthread_mutex_unlock(&data->lock);
}

/* Consumer */
void receive_message(struct shared_data *data, char *buf) {
    pthread_mutex_lock(&data->lock);
    while (!data->ready) {
        pthread_cond_wait(&data->cond, &data->lock);
    }
    strcpy(buf, data->message);
    data->ready = 0;
    pthread_mutex_unlock(&data->lock);
}
```

**Performance:** Lock overhead adds ~50ns per message. Still faster than any copy-based IPC.

### Lock-Free Shared Memory Ring Buffer

Eliminate locks for maximum throughput. Single-producer single-consumer ring buffers can be lock-free.

```c
#define RING_SIZE 1024

struct ring_buffer {
    uint32_t write_pos;
    uint8_t pad1[60];  /* Cache line padding */
    uint32_t read_pos;
    uint8_t pad2[60];
    void *items[RING_SIZE];
};

/* Producer writes */
int ring_push(struct ring_buffer *ring, void *item) {
    uint32_t write_pos = __atomic_load_n(&ring->write_pos, __ATOMIC_RELAXED);
    uint32_t read_pos = __atomic_load_n(&ring->read_pos, __ATOMIC_ACQUIRE);
    uint32_t next_write = (write_pos + 1) % RING_SIZE;
    
    if (next_write == read_pos) {
        return -1;  /* Full */
    }
    
    ring->items[write_pos] = item;
    __atomic_store_n(&ring->write_pos, next_write, __ATOMIC_RELEASE);
    return 0;
}

/* Consumer reads */
void *ring_pop(struct ring_buffer *ring) {
    uint32_t read_pos = __atomic_load_n(&ring->read_pos, __ATOMIC_RELAXED);
    uint32_t write_pos = __atomic_load_n(&ring->write_pos, __ATOMIC_ACQUIRE);
    
    if (read_pos == write_pos) {
        return NULL;  /* Empty */
    }
    
    void *item = ring->items[read_pos];
    __atomic_store_n(&ring->read_pos, (read_pos + 1) % RING_SIZE, 
                     __ATOMIC_RELEASE);
    return item;
}
```

**Performance:** No locks, no system calls. ~30-50ns per message. Scales to millions of messages per second.

**CRITICAL:** This only works for single producer and single consumer. Multiple producers or consumers require CAS operations and more complex protocols.

### Best Practice: Zero-Copy Message Passing

Don't copy message data—pass pointers to shared buffers.

```c
/* Shared memory layout */
struct shared_region {
    struct ring_buffer ring;
    uint8_t message_buffers[1024][4096];  /* Pool of message buffers */
    uint8_t buffer_free[1024];  /* Bitmap of free buffers */
};

/* Sender allocates buffer from pool */
int alloc_buffer(struct shared_region *region) {
    for (int i = 0; i < 1024; i++) {
        if (__atomic_exchange_n(&region->buffer_free[i], 0, 
                                __ATOMIC_ACQUIRE) == 1) {
            return i;
        }
    }
    return -1;  /* No free buffers */
}

/* Sender writes message and passes buffer index */
void send_message(struct shared_region *region, const char *msg) {
    int buf_idx = alloc_buffer(region);
    if (buf_idx < 0) {
        /* Handle allocation failure */
        return;
    }
    
    strcpy((char *)region->message_buffers[buf_idx], msg);
    ring_push(&region->ring, (void *)(long)buf_idx);
}

/* Receiver gets buffer index, reads message, frees buffer */
void receive_message(struct shared_region *region) {
    void *ptr = ring_pop(&region->ring);
    if (!ptr) return;
    
    int buf_idx = (int)(long)ptr;
    printf("Received: %s\n", (char *)region->message_buffers[buf_idx]);
    
    /* Return buffer to pool */
    __atomic_store_n(&region->buffer_free[buf_idx], 1, __ATOMIC_RELEASE);
}
```

You're passing 4-byte indices instead of 4096-byte messages. 1000x less data movement.

## Unix Domain Sockets: Balance of Performance and Flexibility

Unix domain sockets provide socket API with local-only optimization. Faster than TCP, more flexible than shared memory.

### Basic Unix Domain Socket

```c
/* Server */
int create_unix_socket_server(const char *path) {
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    
    unlink(path);  /* Remove old socket file */
    
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return -1;
    }
    
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        return -1;
    }
    
    return server_fd;
}

int accept_connection(int server_fd) {
    return accept(server_fd, NULL, NULL);
}

/* Client */
int connect_unix_socket(const char *path) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }
    
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }
    
    return fd;
}
```

### Performance Optimization: Batch Messages

Don't send one message at a time. Batch multiple messages into single send/recv calls.

```c
/* Inefficient - one syscall per message */
void send_messages_slow(int fd, struct message *msgs, int count) {
    for (int i = 0; i < count; i++) {
        send(fd, &msgs[i], sizeof(struct message), 0);
    }
}

/* Efficient - one syscall for multiple messages */
void send_messages_fast(int fd, struct message *msgs, int count) {
    send(fd, msgs, count * sizeof(struct message), 0);
}
```

System call overhead dominates for small messages. One syscall sending 100 messages beats 100 syscalls sending 1 message each.

### Using scatter-gather I/O

sendmsg/recvmsg with iovec allows sending multiple buffers in one syscall without copying to contiguous buffer.

```c
void send_multiple_buffers(int fd, void *buf1, size_t len1, 
                          void *buf2, size_t len2) {
    struct iovec iov[2];
    iov[0].iov_base = buf1;
    iov[0].iov_len = len1;
    iov[1].iov_base = buf2;
    iov[1].iov_len = len2;
    
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;
    
    sendmsg(fd, &msg, 0);
}
```

Kernel gathers data from both buffers in one syscall. No copying to intermediate buffer.

### Passing File Descriptors

Unix domain sockets have unique capability: passing file descriptors between processes.

```c
/* Send file descriptor over Unix socket */
void send_fd(int sock, int fd_to_send) {
    struct msghdr msg = {0};
    struct iovec iov[1];
    char buf[1] = {'X'};  /* Dummy data */
    
    iov[0].iov_base = buf;
    iov[0].iov_len = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    /* Control message containing fd */
    char cmsg_buf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = cmsg_buf;
    msg.msg_controllen = sizeof(cmsg_buf);
    
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(cmsg) = fd_to_send;
    
    sendmsg(sock, &msg, 0);
}

/* Receive file descriptor */
int recv_fd(int sock) {
    struct msghdr msg = {0};
    struct iovec iov[1];
    char buf[1];
    
    iov[0].iov_base = buf;
    iov[0].iov_len = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    char cmsg_buf[CMSG_SPACE(sizeof(int))];
    msg.msg_control = cmsg_buf;
    msg.msg_controllen = sizeof(cmsg_buf);
    
    recvmsg(sock, &msg, 0);
    
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg && cmsg->cmsg_type == SCM_RIGHTS) {
        return *(int *)CMSG_DATA(cmsg);
    }
    
    return -1;
}
```

Use this for privilege separation—privileged process opens file, passes fd to unprivileged process.

## Message Queues: POSIX vs System V

### POSIX Message Queues

```c
/* Create queue */
mqd_t mq = mq_open("/my_queue", O_CREAT | O_RDWR, 0666, NULL);

/* Send */
struct message {
    int type;
    char data[256];
};

struct message msg = {1, "Hello"};
mq_send(mq, (char *)&msg, sizeof(msg), 0);

/* Receive */
struct message received;
mq_receive(mq, (char *)&received, sizeof(received), NULL);

printf("Type: %d, Data: %s\n", received.type, received.data);

mq_close(mq);
mq_unlink("/my_queue");
```

**Performance:** ~1-2µs per message. Bounded queue size—sender blocks when full. Good backpressure mechanism.

**Best Practice:** Set appropriate queue size based on burst capacity.

```c
struct mq_attr attr;
attr.mq_flags = 0;
attr.mq_maxmsg = 100;  /* Hold up to 100 messages */
attr.mq_msgsize = 4096;  /* Max message size */
attr.mq_curmsgs = 0;

mqd_t mq = mq_open("/my_queue", O_CREAT | O_RDWR, 0666, &attr);
```

### System V Message Queues

Older API, similar performance. POSIX queues preferred for new code.

```c
/* Create queue */
key_t key = ftok("/tmp", 'M');
int msgid = msgget(key, IPC_CREAT | 0666);

/* Send */
struct msgbuf {
    long mtype;
    char mtext[256];
};

struct msgbuf msg;
msg.mtype = 1;
strcpy(msg.mtext, "Hello");
msgsnd(msgid, &msg, sizeof(msg.mtext), 0);

/* Receive */
struct msgbuf received;
msgrcv(msgid, &received, sizeof(received.mtext), 1, 0);

/* Cleanup */
msgctl(msgid, IPC_RMID, NULL);
```

## Pipes: Simple and Fast

Pipes provide unidirectional byte streams. Simple but limited.

### Anonymous Pipes

For parent-child communication.

```c
int pipe_fd[2];
pipe(pipe_fd);  /* pipe_fd[0] = read, pipe_fd[1] = write */

if (fork() == 0) {
    /* Child */
    close(pipe_fd[1]);  /* Close write end */
    
    char buf[256];
    read(pipe_fd[0], buf, sizeof(buf));
    printf("Child received: %s\n", buf);
    
    close(pipe_fd[0]);
    exit(0);
} else {
    /* Parent */
    close(pipe_fd[0]);  /* Close read end */
    
    write(pipe_fd[1], "Hello from parent", 18);
    
    close(pipe_fd[1]);
    wait(NULL);
}
```

### Named Pipes (FIFOs)

For unrelated processes.

```c
/* Create FIFO */
mkfifo("/tmp/myfifo", 0666);

/* Writer process */
int fd = open("/tmp/myfifo", O_WRONLY);
write(fd, "Hello", 5);
close(fd);

/* Reader process */
int fd = open("/tmp/myfifo", O_RDONLY);
char buf[10];
read(fd, buf, 5);
buf[5] = '\0';
printf("Read: %s\n", buf);
close(fd);

unlink("/tmp/myfifo");
```

**Performance:** ~1-2µs per message, similar to Unix sockets but unidirectional. Use socket pairs for bidirectional communication.

### Pipe Capacity and Batching

Pipes have limited capacity (typically 64KB). Writing more blocks.

```c
/* Check pipe capacity */
int capacity = fcntl(pipe_fd[1], F_GETPIPE_SZ);
printf("Pipe capacity: %d bytes\n", capacity);

/* Increase capacity (requires privileges) */
fcntl(pipe_fd[1], F_SETPIPE_SZ, 1024 * 1024);  /* 1MB */
```

**Best Practice:** Batch writes to reduce syscall overhead.

```c
/* Bad - many small writes */
for (int i = 0; i < 1000; i++) {
    write(pipe_fd[1], &data[i], sizeof(data[i]));  /* 1000 syscalls */
}

/* Good - one large write */
write(pipe_fd[1], data, 1000 * sizeof(data[0]));  /* 1 syscall */
```

## Event Notification: epoll for Scalability

When handling multiple IPC channels, use epoll (Linux) or kqueue (BSD) for efficient event notification.

```c
/* Create epoll instance */
int epoll_fd = epoll_create1(0);

/* Add socket to epoll */
struct epoll_event ev;
ev.events = EPOLLIN;
ev.data.fd = socket_fd;
epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &ev);

/* Event loop */
struct epoll_event events[100];
while (1) {
    int nfds = epoll_wait(epoll_fd, events, 100, -1);
    
    for (int i = 0; i < nfds; i++) {
        if (events[i].events & EPOLLIN) {
            int fd = events[i].data.fd;
            handle_readable(fd);
        }
    }
}
```

**Performance:** O(1) notification vs O(n) for select/poll. Critical for thousands of connections.

### Edge-Triggered vs Level-Triggered

```c
/* Level-triggered (default) - notified whenever data available */
ev.events = EPOLLIN;

/* Edge-triggered - notified once when data becomes available */
ev.events = EPOLLIN | EPOLLET;
```

**Edge-triggered:** More efficient but requires reading all available data.

```c
/* Edge-triggered read loop */
while (1) {
    ssize_t n = read(fd, buf, sizeof(buf));
    if (n <= 0) {
        if (errno == EAGAIN) break;  /* All data read */
        /* Handle error */
        break;
    }
    process_data(buf, n);
}
```

Set socket non-blocking for edge-triggered:

```c
int flags = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, flags | O_NONBLOCK);
```

## Choosing the Right IPC Mechanism

### Decision Matrix

| Mechanism | Latency | Throughput | Complexity | Best For |
|-----------|---------|------------|------------|----------|
| Shared Memory | 50ns | Millions/sec | High | Highest performance, same machine |
| Unix Sockets | 1µs | ~1M/sec | Medium | Flexible, connection-oriented |
| Pipes | 1µs | ~1M/sec | Low | Simple, unidirectional |
| Message Queues | 2µs | ~500K/sec | Low | Async messaging, bounded buffer |
| TCP Loopback | 10µs | ~100K/sec | Low | Network transparency |

### Use Shared Memory When:
- ✅ Maximum performance required (< 100ns latency)
- ✅ Both processes on same machine guaranteed
- ✅ High message rate (> 1M messages/sec)
- ✅ You can handle synchronization complexity

### Use Unix Domain Sockets When:
- ✅ Need socket API compatibility
- ✅ Passing file descriptors between processes
- ✅ Connection management important
- ✅ Good balance of performance and simplicity

### Use Pipes When:
- ✅ Simple producer-consumer pattern
- ✅ Parent-child relationship
- ✅ Unidirectional data flow
- ✅ Minimal setup overhead needed

### Use Message Queues When:
- ✅ Async communication with backpressure
- ✅ Message priority required
- ✅ Bounded buffering important
- ✅ Simple API preferred

## Performance Best Practices Summary

### DO:
- ✅ Use shared memory for highest performance
- ✅ Batch messages to reduce syscall overhead
- ✅ Pin threads to CPUs when using shared memory
- ✅ Use lock-free ring buffers when possible
- ✅ Leverage scatter-gather I/O (sendmsg/recvmsg)
- ✅ Use epoll/kqueue for many connections
- ✅ Profile actual IPC overhead in your system
- ✅ Pass pointers in shared memory, not data
- ✅ Align shared structures to cache lines
- ✅ Set appropriate buffer sizes

### DON'T:
- ❌ Use TCP loopback when Unix sockets suffice
- ❌ Send one message per syscall
- ❌ Forget synchronization in shared memory
- ❌ Mix multiple IPC mechanisms unnecessarily
- ❌ Block in critical sections
- ❌ Use System V IPC for new projects
- ❌ Ignore error handling on IPC calls
- ❌ Assume IPC is free—measure cost
- ❌ Use select/poll for many file descriptors
- ❌ Copy large messages—pass references

## Measuring IPC Performance

```c
void benchmark_ipc(int fd) {
    struct timespec start, end;
    int iterations = 100000;
    char msg[64];
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    for (int i = 0; i < iterations; i++) {
        write(fd, msg, sizeof(msg));
        read(fd, msg, sizeof(msg));
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long ns = (end.tv_sec - start.tv_sec) * 1000000000L +
              (end.tv_nsec - start.tv_nsec);
    
    printf("Round-trip: %ld ns\n", ns / iterations);
}
```

Run this for each IPC mechanism in your environment. Real-world performance varies by kernel version, CPU, and system load.

## The Bottom Line

IPC choice fundamentally impacts your architecture's scalability. Shared memory gives thread-like performance with process isolation. Unix sockets balance performance and flexibility. Message queues provide clean async semantics. Choose based on your latency budget, throughput requirements, and operational constraints. Profile ruthlessly—intuition about IPC performance is often wrong.

# Real-Time Scheduling: Meeting Deadlines at Scale

## The Real-Time Imperative

Real-time doesn't mean fast—it means predictable. A routing protocol update that completes in 5ms every time is better than one that usually takes 1ms but occasionally takes 100ms. Network devices face soft real-time requirements: packets must be forwarded within bounded time, routing convergence must complete predictably, management interfaces must remain responsive.

Understanding scheduling is critical because you can't optimize what the scheduler undermines. Write perfect zero-copy packet forwarding code, and it's useless if the scheduler gives your thread CPU time only after 50ms delays.

## Linux Scheduling Classes

Linux provides multiple scheduling policies with different guarantees and priorities.

### SCHED_FIFO: Hard Real-Time

First-In-First-Out real-time scheduling. Highest priority task runs until it blocks or yields. No time slicing—runs forever until voluntary yield.

```c
void set_realtime_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority;  /* 1-99, higher = higher priority */
    
    if (sched_setscheduler(0, SCHED_FIFO, &param) != 0) {
        perror("sched_setscheduler");
        /* Requires CAP_SYS_NICE capability or root */
    }
}

void packet_processing_thread(void) {
    /* Set to real-time priority */
    set_realtime_priority(80);
    
    /* Pin to specific CPU */
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
    
    while (1) {
        struct packet *pkt = receive_packet();
        process_packet(pkt);
        /* MUST yield or sleep occasionally, or this thread monopolizes CPU */
    }
}
```

**CRITICAL WARNING:** SCHED_FIFO thread that never yields can lock up the system. The kernel won't preempt it. Always have escape mechanisms—watchdog timers, periodic yields, or time limits.

**Best Practice: Watchdog Integration**

```c
void packet_thread_with_watchdog(void) {
    set_realtime_priority(80);
    
    struct timespec last_yield;
    clock_gettime(CLOCK_MONOTONIC, &last_yield);
    
    while (1) {
        struct packet *pkt = receive_packet_nonblocking();
        if (!pkt) {
            /* No packet - yield to other threads */
            sched_yield();
            continue;
        }
        
        process_packet(pkt);
        
        /* Periodically check if we've run too long */
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        long ms = (now.tv_sec - last_yield.tv_sec) * 1000 +
                  (now.tv_nsec - last_yield.tv_nsec) / 1000000;
        
        if (ms > 10) {  /* Ran for 10ms - yield */
            sched_yield();
            last_yield = now;
        }
    }
}
```

### SCHED_RR: Round-Robin Real-Time

Like SCHED_FIFO but with time slicing. Each thread gets a quantum (default 100ms on Linux), then rotates to back of queue at its priority level.

```c
void set_round_robin_priority(int priority) {
    struct sched_param param;
    param.sched_priority = priority;
    sched_setscheduler(0, SCHED_RR, &param);
}
```

**Use SCHED_RR when:** Multiple real-time threads at same priority need fair sharing. SCHED_FIFO when strict priority matters more than fairness.

### SCHED_DEADLINE: Deadline Scheduling

Most sophisticated real-time policy. Specify runtime, deadline, and period. Kernel uses EDF (Earliest Deadline First) to schedule.

```c
void set_deadline_scheduling(uint64_t runtime_ns, uint64_t deadline_ns, 
                            uint64_t period_ns) {
    struct sched_attr {
        uint32_t size;
        uint32_t sched_policy;
        uint64_t sched_flags;
        int32_t sched_nice;
        uint32_t sched_priority;
        uint64_t sched_runtime;
        uint64_t sched_deadline;
        uint64_t sched_period;
    } attr;
    
    memset(&attr, 0, sizeof(attr));
    attr.size = sizeof(attr);
    attr.sched_policy = SCHED_DEADLINE;
    attr.sched_runtime = runtime_ns;
    attr.sched_deadline = deadline_ns;
    attr.sched_period = period_ns;
    
    syscall(SYS_sched_setattr, 0, &attr, 0);
}

/* Example: Task needs 2ms every 10ms */
set_deadline_scheduling(2000000,   /* 2ms runtime */
                       10000000,   /* 10ms deadline */
                       10000000);  /* 10ms period */
```

**Advantage:** Kernel mathematically guarantees schedulability if sum of utilization < 1. With FIFO/RR, priority inversion can cause unpredictable delays.

**Disadvantage:** Complex to configure. Getting parameters wrong causes throttling or missed deadlines.

### SCHED_OTHER: Default Time-Sharing

For non-real-time tasks. Uses Completely Fair Scheduler (CFS)—gives each task fair CPU time based on nice value.

```c
/* Lower nice value = higher priority */
void set_nice(int nice_value) {
    /* -20 (highest) to +19 (lowest) */
    setpriority(PRIO_PROCESS, 0, nice_value);
}
```

**Use for:** Control plane tasks, management interfaces, logging—anything without strict timing requirements.

## Priority Inversion: The Silent Deadline Killer

High-priority task blocked by low-priority task holding a lock, while medium-priority task runs. High-priority task misses deadline despite having priority.

### Classic Priority Inversion Example

```c
pthread_mutex_t resource_lock = PTHREAD_MUTEX_INITIALIZER;

/* Low priority task */
void low_priority_task(void) {
    set_realtime_priority(10);
    
    pthread_mutex_lock(&resource_lock);
    /* Long operation - 10ms */
    slow_operation();
    pthread_mutex_unlock(&resource_lock);
}

/* Medium priority task */
void medium_priority_task(void) {
    set_realtime_priority(50);
    
    /* CPU-intensive work */
    while (1) {
        compute_intensive_operation();
    }
}

/* High priority task */
void high_priority_task(void) {
    set_realtime_priority(90);
    
    pthread_mutex_lock(&resource_lock);  /* BLOCKED by low-priority task */
    critical_operation();
    pthread_mutex_unlock(&resource_lock);
}
```

Timeline:
1. Low-priority task acquires lock
2. High-priority task tries to acquire lock, blocks
3. Medium-priority task runs (higher priority than low)
4. Low-priority task can't complete because medium task runs
5. High-priority task misses deadline

**Solution: Priority Inheritance**

```c
void setup_priority_inheritance(void) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, &attr);
}
```

When high-priority thread blocks on lock held by low-priority thread, low-priority thread temporarily inherits high priority. Now it can complete and release lock before medium-priority thread runs.

**ALWAYS use priority inheritance for real-time mutexes.**

## CPU Affinity: Control Your Destiny

Don't let the OS scheduler move your threads between CPUs. Pin critical threads to specific cores.

```c
void pin_to_cpu(int cpu_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);
    
    pthread_t thread = pthread_self();
    if (pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
    }
}

/* Multi-threaded packet processor */
void start_packet_processors(int num_threads) {
    for (int i = 0; i < num_threads; i++) {
        pthread_t thread;
        int *cpu_id = malloc(sizeof(int));
        *cpu_id = i;
        
        pthread_create(&thread, NULL, packet_processor, cpu_id);
    }
}

void *packet_processor(void *arg) {
    int cpu_id = *(int *)arg;
    free(arg);
    
    /* Pin this thread to its CPU */
    pin_to_cpu(cpu_id);
    
    /* Set real-time priority */
    set_realtime_priority(80);
    
    /* Process packets using per-CPU data structures */
    struct per_cpu_data *data = &cpu_data[cpu_id];
    
    while (1) {
        struct packet *pkt = receive_packet(data);
        process_packet(data, pkt);
    }
    
    return NULL;
}
```

**Benefits:**
- Cache stays warm (no cache flushing on migration)
- Per-CPU data structures work correctly
- Predictable performance (no migration overhead)

**CRITICAL:** Leave some CPUs free for OS work. Don't pin threads to ALL CPUs or system becomes unresponsive.

```c
/* Reserve CPU 0 for OS, use CPUs 1-N for packet processing */
void pin_threads_intelligently(void) {
    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    int packet_cpus = num_cpus - 1;  /* Leave CPU 0 for OS */
    
    for (int i = 0; i < packet_cpus; i++) {
        create_and_pin_thread(i + 1);  /* CPUs 1, 2, 3... */
    }
}
```

## Isolating CPUs: CPU Sets

Take CPU isolation further—tell kernel not to schedule normal processes on specific CPUs.

```bash
# At boot, reserve CPUs 1-7 for application
# Add to kernel command line:
isolcpus=1-7 nohz_full=1-7 rcu_nocbs=1-7
```

Now CPUs 1-7 are isolated:
- `isolcpus`: Normal tasks won't be scheduled there
- `nohz_full`: No scheduling clock tick (eliminates interrupt overhead)
- `rcu_nocbs`: RCU callbacks run on other CPUs

Your real-time threads get these CPUs nearly to themselves.

**In code, use cgroups to enforce:**

```c
/* Move process to isolated CPU set */
void move_to_isolated_cpus(void) {
    /* Create cgroup with isolated CPUs */
    mkdir("/sys/fs/cgroup/cpuset/isolated", 0755);
    
    FILE *f = fopen("/sys/fs/cgroup/cpuset/isolated/cpuset.cpus", "w");
    fprintf(f, "1-7");
    fclose(f);
    
    /* Add this process to the cgroup */
    f = fopen("/sys/fs/cgroup/cpuset/isolated/tasks", "w");
    fprintf(f, "%d", getpid());
    fclose(f);
}
```

## Measuring Scheduling Latency

You can't optimize what you don't measure. Instrument your real-time tasks.

```c
struct latency_stats {
    uint64_t samples;
    uint64_t sum_ns;
    uint64_t max_ns;
    uint64_t min_ns;
};

void measure_scheduling_latency(struct latency_stats *stats) {
    struct timespec wake_time, run_time;
    
    /* Record when we should wake */
    clock_gettime(CLOCK_MONOTONIC, &wake_time);
    wake_time.tv_nsec += 1000000;  /* 1ms from now */
    if (wake_time.tv_nsec >= 1000000000) {
        wake_time.tv_sec++;
        wake_time.tv_nsec -= 1000000000;
    }
    
    /* Sleep until wake time */
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wake_time, NULL);
    
    /* Record actual wake time */
    clock_gettime(CLOCK_MONOTONIC, &run_time);
    
    /* Calculate latency */
    uint64_t latency_ns = (run_time.tv_sec - wake_time.tv_sec) * 1000000000UL +
                          (run_time.tv_nsec - wake_time.tv_nsec);
    
    /* Update statistics */
    stats->samples++;
    stats->sum_ns += latency_ns;
    if (latency_ns > stats->max_ns) stats->max_ns = latency_ns;
    if (latency_ns < stats->min_ns || stats->min_ns == 0) {
        stats->min_ns = latency_ns;
    }
}

void print_latency_stats(struct latency_stats *stats) {
    printf("Samples: %lu\n", stats->samples);
    printf("Avg latency: %lu us\n", stats->sum_ns / stats->samples / 1000);
    printf("Max latency: %lu us\n", stats->max_ns / 1000);
    printf("Min latency: %lu us\n", stats->min_ns / 1000);
}
```

**Target latencies:**
- Soft real-time: < 1ms worst case
- Hard real-time: < 100µs worst case
- Critical path: < 10µs worst case

If seeing larger latencies, check:
- Are you using SCHED_FIFO/SCHED_RR?
- Is CPU isolated from OS scheduling?
- Are there interrupt storms?
- Is power management causing CPU frequency changes?

## Interrupt Handling and IRQ Affinity

Hardware interrupts can preempt even real-time threads. Control which CPUs handle interrupts.

```bash
# View interrupt affinity
cat /proc/irq/*/smp_affinity_list

# Set NIC interrupt to CPU 0 only
echo 0 > /proc/irq/42/smp_affinity_list

# This leaves CPUs 1-7 free from interrupt handling
```

**In code, configure via sysfs:**

```c
void set_irq_affinity(int irq, int cpu) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/irq/%d/smp_affinity_list", irq);
    
    FILE *f = fopen(path, "w");
    if (f) {
        fprintf(f, "%d", cpu);
        fclose(f);
    }
}

/* Find NIC IRQ and pin to CPU 0 */
void configure_nic_interrupts(const char *interface) {
    /* Find IRQ for interface - simplified */
    int irq = find_irq_for_interface(interface);
    if (irq > 0) {
        set_irq_affinity(irq, 0);
    }
}
```

## Busy-Polling vs Blocking

Blocking I/O causes context switches. For ultra-low latency, use busy-polling.

### Blocking Approach (Higher Latency)

```c
void blocking_receive(void) {
    while (1) {
        struct packet *pkt = receive_packet();  /* Blocks in kernel */
        process_packet(pkt);
    }
}
```

Every receive() call:
1. Thread blocks
2. Context switch to kernel
3. Kernel checks for packet
4. Context switch back to userspace

Minimum latency: ~5-10µs per packet.

### Busy-Polling Approach (Lower Latency)

```c
void busy_poll_receive(void) {
    /* Set socket to non-blocking */
    int flags = fcntl(sock_fd, F_GETFL, 0);
    fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
    
    while (1) {
        struct packet *pkt = receive_packet_nonblocking();
        if (pkt) {
            process_packet(pkt);
        }
        /* No sleep - continuously poll */
    }
}
```

No blocking, no context switches. Latency: ~1-2µs.

**Cost:** 100% CPU usage even when idle. Only viable for dedicated CPUs.

### Hybrid: Adaptive Polling

```c
void adaptive_polling(void) {
    int consecutive_empty = 0;
    
    while (1) {
        struct packet *pkt = receive_packet_nonblocking();
        
        if (pkt) {
            process_packet(pkt);
            consecutive_empty = 0;
        } else {
            consecutive_empty++;
            
            if (consecutive_empty > 1000) {
                /* No packets for a while - sleep briefly */
                usleep(10);  /* 10µs sleep */
                consecutive_empty = 0;
            }
        }
    }
}
```

Burns CPU when busy, sleeps when idle. Balance between latency and CPU efficiency.

## DPDK: Kernel Bypass for Maximum Performance

Data Plane Development Kit bypasses kernel networking entirely. Packets go directly from NIC to userspace.

**Concept:**

```c
/* Traditional kernel path */
NIC -> Kernel driver -> Socket buffer -> recv() -> Userspace
/* Latency: 10-50µs per packet */

/* DPDK path */
NIC -> Userspace via memory-mapped DMA
/* Latency: 1-5µs per packet */
```

**Basic DPDK pattern:**

```c
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>

void dpdk_packet_processor(void) {
    /* Initialize DPDK */
    int ret = rte_eal_init(argc, argv);
    
    /* Configure port */
    uint16_t port_id = 0;
    struct rte_eth_conf port_conf = {0};
    rte_eth_dev_configure(port_id, 1, 1, &port_conf);
    
    /* Allocate memory pool for packets */
    struct rte_mempool *mbuf_pool = rte_pktmbuf_pool_create(
        "MBUF_POOL", 8192, 250, 0, RTE_MBUF_DEFAULT_BUF_SIZE, 
        rte_socket_id());
    
    /* Setup RX queue */
    rte_eth_rx_queue_setup(port_id, 0, 1024, rte_eth_dev_socket_id(port_id),
                          NULL, mbuf_pool);
    
    /* Start device */
    rte_eth_dev_start(port_id);
    
    /* Receive loop */
    struct rte_mbuf *bufs[BURST_SIZE];
    while (1) {
        uint16_t nb_rx = rte_eth_rx_burst(port_id, 0, bufs, BURST_SIZE);
        
        for (int i = 0; i < nb_rx; i++) {
            process_packet(bufs[i]);
            rte_pktmbuf_free(bufs[i]);
        }
    }
}
```

**Performance:** 10-40 million packets/sec per core. Orders of magnitude beyond kernel networking.

**Tradeoff:** Dedicated CPUs, complex setup, gives up kernel network stack features.

## Real-Time Best Practices Summary

### DO:
- ✅ Use SCHED_FIFO or SCHED_DEADLINE for time-critical threads
- ✅ Enable priority inheritance on all real-time mutexes
- ✅ Pin threads to specific CPUs with affinity
- ✅ Isolate CPUs from normal scheduling (isolcpus)
- ✅ Direct interrupts to non-isolated CPUs
- ✅ Measure actual scheduling latency continuously
- ✅ Pre-allocate all memory before real-time operation
- ✅ Use busy-polling for lowest latency (on dedicated CPUs)
- ✅ Implement watchdog timers for runaway SCHED_FIFO threads
- ✅ Test under maximum load to find worst-case latency

### DON'T:
- ❌ Use SCHED_FIFO without yield/sleep logic (locks up system)
- ❌ Allocate memory in real-time paths (causes latency spikes)
- ❌ Use standard mutexes with real-time threads (priority inversion)
- ❌ Let OS migrate threads between CPUs randomly
- ❌ Pin threads to ALL CPUs (leave some for OS)
- ❌ Block on I/O in real-time threads without timeout
- ❌ Run real-time threads without latency monitoring
- ❌ Assume SCHED_OTHER is "fast enough" without measuring
- ❌ Mix real-time and non-real-time threads accessing same locks
- ❌ Forget that page faults cause unbounded delays

## Latency Troubleshooting Checklist

**If experiencing high latency (>1ms):**

1. **Check scheduling policy:**
```c
int policy = sched_getscheduler(0);
printf("Policy: %s\n", 
       policy == SCHED_FIFO ? "FIFO" :
       policy == SCHED_RR ? "RR" : "OTHER");
```

2. **Check CPU affinity:**
```c
cpu_set_t cpuset;
pthread_getaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
for (int i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, &cpuset)) {
        printf("Can run on CPU %d\n", i);
    }
}
```

3. **Check for page faults:**
```bash
# Lock memory to prevent paging
mlockall(MCL_CURRENT | MCL_FUTURE);
```

4. **Disable power management:**
```bash
# Disable CPU frequency scaling
for cpu in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor; do
    echo performance > $cpu
done
```

5. **Check interrupt distribution:**
```bash
watch -n 1 'cat /proc/interrupts'
```

## The Bottom Line

Real-time scheduling is about controlling variability. Your average latency might be 10µs, but if 99.9th percentile is 50ms, your system isn't real-time. Use proper scheduling policies, pin threads, isolate CPUs, measure relentlessly, and design for worst-case behavior. The difference between working real-time system and one that occasionally fails is understanding these fundamentals and applying them rigorously.

# System Calls and Kernel Interaction: Performance Impact

## System Calls Are Expensive

Every system call transitions from user mode to kernel mode and back. This context switch costs 50-200 nanoseconds on modern hardware. Process 10 million packets per second, and you have 100 nanoseconds total per packet. A single unnecessary system call consumes your entire budget.

Understanding system call overhead and minimizing calls separates code that scales from code that chokes.

## The System Call Mechanism

### What Happens During a System Call

When you call read(), write(), or any system call:

1. **User code** executes special instruction (syscall on x86-64, svc on ARM)
2. **CPU switches** from user mode (ring 3) to kernel mode (ring 0)
3. **Kernel saves** user process state (registers, stack pointer, instruction pointer)
4. **Kernel validates** arguments (security checks, bounds checking)
5. **Kernel executes** the requested operation
6. **Kernel restores** user process state
7. **CPU switches** back to user mode
8. **User code** continues with return value

Steps 2-7 cost 50-200ns even if the operation itself is trivial.

### Measuring System Call Overhead

```c
#include <time.h>
#include <unistd.h>

void measure_syscall_overhead(void) {
    struct timespec start, end;
    int iterations = 1000000;
    
    /* Measure getpid() - trivial syscall */
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < iterations; i++) {
        getpid();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    long ns = (end.tv_sec - start.tv_sec) * 1000000000L +
              (end.tv_nsec - start.tv_nsec);
    
    printf("System call overhead: %ld ns\n", ns / iterations);
    
    /* Compare to function call */
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < iterations; i++) {
        getpid_cached();  /* User-space function */
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    ns = (end.tv_sec - start.tv_sec) * 1000000000L +
         (end.tv_nsec - start.tv_nsec);
    
    printf("Function call overhead: %ld ns\n", ns / iterations);
}

static pid_t cached_pid = 0;
pid_t getpid_cached(void) {
    if (cached_pid == 0) {
        cached_pid = getpid();
    }
    return cached_pid;
}
```

On modern systems:
- System call: 50-150ns
- Function call: 1-5ns

**30-150x difference.** This is why reducing system calls matters.

## Batch Operations: The First Optimization

Never make one system call per item. Batch multiple operations into single calls.

### File I/O: writev vs write

**BAD: Multiple write() calls**

```c
void write_log_entries_slow(int fd, struct log_entry *entries, int count) {
    for (int i = 0; i < count; i++) {
        write(fd, &entries[i], sizeof(struct log_entry));
    }
    /* 1000 entries = 1000 system calls */
}
```

**GOOD: Single writev() call**

```c
void write_log_entries_fast(int fd, struct log_entry *entries, int count) {
    struct iovec iov[count];
    
    for (int i = 0; i < count; i++) {
        iov[i].iov_base = &entries[i];
        iov[i].iov_len = sizeof(struct log_entry);
    }
    
    writev(fd, iov, count);
    /* 1000 entries = 1 system call */
}
```

1000x fewer system calls. 50-100µs saved per batch.

### Network I/O: sendmsg with Multiple Buffers

```c
/* Send header + payload without copying to contiguous buffer */
void send_packet_zerocopy(int sock, struct header *hdr, void *payload, 
                         size_t payload_len) {
    struct iovec iov[2];
    iov[0].iov_base = hdr;
    iov[0].iov_len = sizeof(*hdr);
    iov[1].iov_base = payload;
    iov[1].iov_len = payload_len;
    
    struct msghdr msg = {0};
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;
    
    sendmsg(sock, &msg, 0);
}
```

One syscall, no memcpy to combine buffers.

## Minimizing read/write Calls

### User-Space Buffering

Don't call write() for every small piece of data. Buffer in userspace.

```c
struct output_buffer {
    char data[65536];
    size_t used;
    int fd;
};

void buffer_init(struct output_buffer *buf, int fd) {
    buf->used = 0;
    buf->fd = fd;
}

void buffer_write(struct output_buffer *buf, const void *data, size_t len) {
    if (buf->used + len > sizeof(buf->data)) {
        /* Flush buffer */
        write(buf->fd, buf->data, buf->used);
        buf->used = 0;
    }
    
    memcpy(buf->data + buf->used, data, len);
    buf->used += len;
}

void buffer_flush(struct output_buffer *buf) {
    if (buf->used > 0) {
        write(buf->fd, buf->data, buf->used);
        buf->used = 0;
    }
}
```

Accumulate data in userspace, make large writes. Reduces syscalls by orders of magnitude.

**Standard Library Does This:** fprintf() and other stdio functions buffer internally. This is why printf() is often faster than repeated write() calls despite seeming heavier.

## Memory-Mapped Files: Eliminating read/write

For file operations, mmap() can eliminate read/write syscalls entirely.

### Traditional File Access

```c
/* Read file - requires read() syscall for each block */
void process_file_traditional(const char *path) {
    int fd = open(path, O_RDONLY);
    char buffer[4096];
    ssize_t n;
    
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        process_data(buffer, n);
    }
    
    close(fd);
}
```

For 100MB file: ~25,000 read() calls at 4KB per read.

### Memory-Mapped Access

```c
void process_file_mmap(const char *path) {
    int fd = open(path, O_RDONLY);
    struct stat st;
    fstat(fd, &st);
    
    void *mapped = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        return;
    }
    
    /* Access memory directly - no read() calls */
    process_data(mapped, st.st_size);
    
    munmap(mapped, st.st_size);
    close(fd);
}
```

After initial mmap(), all file access is just memory access. Zero system calls during processing. Kernel handles page faults transparently.

**When to use mmap:**
- ✅ Large files accessed sequentially or randomly
- ✅ Files accessed multiple times
- ✅ Shared memory between processes
- ❌ Very small files (mmap overhead > read overhead)
- ❌ Streaming data (network sockets)

### Shared Memory via mmap

```c
/* Create shared memory between processes */
void *create_shared_region(size_t size) {
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    
    return ptr;
}

/* Parent creates shared memory, forks child */
void shared_memory_example(void) {
    size_t size = 1024 * 1024;  /* 1MB */
    int *shared = create_shared_region(size);
    
    if (fork() == 0) {
        /* Child process */
        shared[0] = 42;
        exit(0);
    } else {
        /* Parent process */
        wait(NULL);
        printf("Child wrote: %d\n", shared[0]);
        munmap(shared, size);
    }
}
```

Both processes access same physical memory. Zero copy, zero syscalls after setup.

## Avoiding stat() and fstat()

Stat calls query file metadata. They're fast but not free.

### Bad: Stat Before Every Operation

```c
void process_files_slow(char **paths, int count) {
    for (int i = 0; i < count; i++) {
        struct stat st;
        if (stat(paths[i], &st) != 0) continue;
        
        if (S_ISREG(st.st_mode)) {
            process_regular_file(paths[i], st.st_size);
        }
    }
}
```

**Good: Get Metadata from open()**

```c
void process_files_fast(char **paths, int count) {
    for (int i = 0; i < count; i++) {
        int fd = open(paths[i], O_RDONLY);
        if (fd < 0) continue;
        
        struct stat st;
        fstat(fd, &st);  /* fstat on already-open fd is faster */
        
        if (S_ISREG(st.st_mode)) {
            /* Use fd directly, no need to open again */
            process_fd(fd, st.st_size);
        }
        
        close(fd);
    }
}
```

## Syscall Reduction via vDSO

Virtual Dynamic Shared Object provides certain syscalls in userspace with no mode switch.

**Supported by vDSO (no kernel entry):**
- `clock_gettime()` for CLOCK_REALTIME and CLOCK_MONOTONIC
- `gettimeofday()`
- `time()`
- `getcpu()`

```c
/* These are fast - implemented in userspace via vDSO */
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC, &ts);  /* ~20ns, no syscall */

/* This is slow - real syscall */
clock_gettime(CLOCK_MONOTONIC_RAW, &ts);  /* ~100ns, syscall required */
```

**Prefer vDSO-supported calls when possible.**

## epoll: Scalable Event Notification

select() and poll() require scanning all file descriptors. epoll() uses kernel-maintained interest list.

### select() Doesn't Scale

```c
void handle_connections_select(int *sockets, int count) {
    fd_set readfds;
    
    while (1) {
        FD_ZERO(&readfds);
        int max_fd = 0;
        
        /* O(n) setup */
        for (int i = 0; i < count; i++) {
            FD_SET(sockets[i], &readfds);
            if (sockets[i] > max_fd) max_fd = sockets[i];
        }
        
        /* Syscall passes entire fd_set to kernel */
        select(max_fd + 1, &readfds, NULL, NULL, NULL);
        
        /* O(n) to find which are ready */
        for (int i = 0; i < count; i++) {
            if (FD_ISSET(sockets[i], &readfds)) {
                handle_socket(sockets[i]);
            }
        }
    }
}
```

For 10,000 connections: O(10,000) work per select() call.

### epoll() Scales

```c
void handle_connections_epoll(int *sockets, int count) {
    int epoll_fd = epoll_create1(0);
    
    /* Register interest once */
    for (int i = 0; i < count; i++) {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = sockets[i];
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockets[i], &ev);
    }
    
    struct epoll_event events[100];
    
    while (1) {
        /* Kernel returns ONLY ready descriptors */
        int nfds = epoll_wait(epoll_fd, events, 100, -1);
        
        /* O(ready fds), not O(total fds) */
        for (int i = 0; i < nfds; i++) {
            handle_socket(events[i].data.fd);
        }
    }
}
```

For 10,000 connections with 10 ready: epoll processes 10, select processes 10,000.

## Reducing Timer System Calls

### Bad: sleep() or nanosleep() Every Iteration

```c
void periodic_task_slow(void) {
    while (1) {
        do_work();
        usleep(1000);  /* Syscall every iteration */
    }
}
```

**Good: timerfd with epoll**

```c
int create_periodic_timer(int interval_ms) {
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    
    struct itimerspec spec;
    spec.it_interval.tv_sec = interval_ms / 1000;
    spec.it_interval.tv_nsec = (interval_ms % 1000) * 1000000;
    spec.it_value = spec.it_interval;
    
    timerfd_settime(timer_fd, 0, &spec, NULL);
    return timer_fd;
}

void periodic_task_fast(void) {
    int timer_fd = create_periodic_timer(1);  /* 1ms interval */
    int epoll_fd = epoll_create1(0);
    
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = timer_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &ev);
    
    struct epoll_event events[1];
    
    while (1) {
        epoll_wait(epoll_fd, events, 1, -1);
        
        /* Read timer to reset */
        uint64_t expirations;
        read(timer_fd, &expirations, sizeof(expirations));
        
        do_work();
    }
}
```

One timer setup, then epoll_wait() handles everything. Fewer syscalls, integrates with other I/O events.

## System Call Alternatives

### gettimeofday() vs clock_gettime() vs RDTSC

```c
/* Slowest - real syscall (if no vDSO) */
struct timeval tv;
gettimeofday(&tv, NULL);  /* ~100ns */

/* Fast - vDSO optimized */
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC, &ts);  /* ~20ns */

/* Fastest - direct CPU instruction (no syscall) */
static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

uint64_t cycles = rdtsc();  /* ~5ns */
```

RDTSC reads CPU cycle counter directly. No system call at all. But:
- Not monotonic across cores unless synchronized
- Requires calibration to convert to real time
- Can be affected by CPU frequency scaling

**Use RDTSC for:** High-frequency relative timing on same core.
**Use clock_gettime() for:** Absolute timestamps, cross-core timing.

## Kernel Bypass Techniques

### User-Space Network Stacks

Bypass kernel networking entirely. Libraries like DPDK, Netmap, PF_RING provide user-space packet I/O.

**Traditional Network Stack:**
```
NIC -> Kernel Driver -> Kernel TCP/IP -> Socket -> User Space
```
Every packet: multiple syscalls, copies, context switches.

**Kernel Bypass:**
```
NIC -> Memory-mapped DMA -> User Space
```
Zero syscalls in fast path. Application handles everything.

```c
/* Pseudo-code for DPDK receive */
while (1) {
    /* No syscall - polls memory-mapped NIC registers */
    uint16_t n = rte_eth_rx_burst(port, queue, packets, BURST_SIZE);
    
    for (int i = 0; i < n; i++) {
        /* Process packet entirely in userspace */
        process_packet(packets[i]);
    }
}
```

**Performance:** 10-40 million packets/sec vs 1-2 million with kernel stack.

**Cost:** Dedicated CPUs, can't share NIC with other applications.

## Measuring System Call Usage

### strace: Trace All System Calls

```bash
# Trace program and count syscalls
strace -c ./your_program

# Shows output like:
# % time     seconds  usecs/call     calls    errors syscall
# 45.23    0.123456          10     12345           read
# 32.11    0.087654          15      5834           write
```

Identify which syscalls dominate. Optimize the most frequent ones.

### perf: Performance Analysis

```bash
# Count syscalls
perf stat -e 'syscalls:sys_enter_*' ./your_program

# Profile syscall overhead
perf record -e raw_syscalls:sys_enter ./your_program
perf report
```

## Best Practices Summary

### DO:
- ✅ Batch operations into single syscalls (writev, sendmsg)
- ✅ Use memory-mapped I/O for files
- ✅ Use epoll for scalable I/O multiplexing
- ✅ Buffer in userspace to reduce write frequency
- ✅ Use vDSO-optimized calls (clock_gettime)
- ✅ Measure syscall frequency with strace/perf
- ✅ Use timerfd instead of repeated sleep calls
- ✅ Keep file descriptors open instead of open/close repeatedly
- ✅ Use fstat() instead of stat() when fd already open
- ✅ Consider kernel bypass (DPDK) for ultimate performance

### DON'T:
- ❌ Make syscalls in tight loops
- ❌ Call stat() before open() (fstat after open is faster)
- ❌ Use select/poll for thousands of file descriptors
- ❌ Sleep/usleep in high-frequency loops
- ❌ Make one write() call per log entry
- ❌ Use gettimeofday() if clock_gettime() suffices
- ❌ Forget that syscalls have 50-200ns overhead
- ❌ Call getpid() repeatedly (cache the result)
- ❌ Open/close files repeatedly (keep them open)
- ❌ Ignore system call profiling data

## Syscall Reduction Patterns

### Pattern 1: Cache Syscall Results

```c
/* Bad */
void log_message(const char *msg) {
    pid_t pid = getpid();  /* Syscall every time */
    fprintf(logfile, "[%d] %s\n", pid, msg);
}

/* Good */
static pid_t cached_pid = 0;
void log_message(const char *msg) {
    if (cached_pid == 0) {
        cached_pid = getpid();  /* Syscall once */
    }
    fprintf(logfile, "[%d] %s\n", cached_pid, msg);
}
```

### Pattern 2: Amortize Setup Cost

```c
/* Bad - setup overhead per operation */
void send_messages(struct message *msgs, int count) {
    for (int i = 0; i < count; i++) {
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        send(sock, &msgs[i], sizeof(msgs[i]), 0);
        close(sock);
    }
}

/* Good - reuse connection */
void send_messages(struct message *msgs, int count) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    for (int i = 0; i < count; i++) {
        send(sock, &msgs[i], sizeof(msgs[i]), 0);
    }
    
    close(sock);
}
```

### Pattern 3: Eliminate Polling

```c
/* Bad - polling with syscalls */
void wait_for_data(int fd) {
    char buf[1024];
    while (1) {
        int n = read(fd, buf, sizeof(buf));  /* Blocks or returns EAGAIN */
        if (n > 0) {
            process_data(buf, n);
            break;
        }
        usleep(1000);  /* Another syscall */
    }
}

/* Good - blocking I/O or epoll */
void wait_for_data(int fd) {
    char buf[1024];
    int n = read(fd, buf, sizeof(buf));  /* One syscall, blocks until ready */
    if (n > 0) {
        process_data(buf, n);
    }
}
```

## The Bottom Line

System calls are necessary but expensive. Every call costs 50-200ns. At high packet rates, unnecessary syscalls kill performance. Batch operations, use memory mapping, cache results, and measure your syscall frequency. The difference between 1,000 syscalls/second and 1,000,000 syscalls/second determines whether your application scales or collapses under load.

# RTOS and Embedded OS: Practical Development Guide

## Why RTOS Matters for Network Devices

Network hardware—routers, switches, firewalls—runs on embedded processors with constrained resources and strict timing requirements. A Real-Time Operating System provides deterministic scheduling, bounded interrupt latency, and predictable response times. These aren't luxuries; they're requirements for reliable packet forwarding and protocol operation.

This guide focuses on practical RTOS development for network devices, covering common RTOSes (FreeRTOS, VxWorks, QNX), and how to write code that meets real-time deadlines.

## RTOS vs General-Purpose OS

### Fundamental Differences

**General-Purpose OS (Linux, Windows):**
- Optimizes for throughput and fairness
- Unbounded worst-case latency
- Complex, feature-rich
- Memory protection via MMU
- Virtual memory and swapping
- Best-effort scheduling

**RTOS:**
- Optimizes for determinism and latency
- Bounded worst-case latency (guaranteed)
- Minimal, predictable codebase
- Often no MMU (Memory Protection Unit only)
- No swapping (all code in RAM)
- Priority-based preemptive scheduling

**Example:** Linux might have 1ms average latency, 100ms worst case. RTOS guarantees < 100µs worst case, every time.

## Task Model: RTOS Tasks vs OS Threads

### Task Characteristics

RTOS tasks are similar to threads but lighter weight with different semantics.

```c
/* FreeRTOS task example */
void packet_processor_task(void *params) {
    /* Task initialization */
    struct packet_queue *queue = (struct packet_queue *)params;
    
    /* Task infinite loop - typical RTOS pattern */
    while (1) {
        struct packet *pkt = queue_receive(queue, portMAX_DELAY);
        process_packet(pkt);
        packet_free(pkt);
        
        /* Yield or delay if needed */
        taskYIELD();
    }
    
    /* Tasks typically never return */
    /* If they do, must delete themselves */
    vTaskDelete(NULL);
}

/* Create task */
void create_packet_processor(void) {
    xTaskCreate(
        packet_processor_task,  /* Function pointer */
        "PacketProc",           /* Name for debugging */
        1024,                   /* Stack size (words) */
        &packet_queue,          /* Parameter */
        tskIDLE_PRIORITY + 2,   /* Priority */
        NULL                    /* Task handle */
    );
}
```

**Key Differences from POSIX Threads:**
- Fixed stack size allocated at creation
- Priority is mandatory and explicit
- Tasks typically never exit (infinite loop)
- No dynamic priority adjustment
- Context switch time is deterministic and bounded

## Priority-Based Scheduling

### Priority Levels

RTOS uses strict priority preemption. Highest priority ready task always runs.

```c
/* FreeRTOS priorities - typical network device */
#define PRIORITY_IDLE          0   /* Idle task */
#define PRIORITY_LOGGING       1   /* Background logging */
#define PRIORITY_MANAGEMENT    2   /* CLI, SNMP */
#define PRIORITY_ROUTING       3   /* Routing protocols */
#define PRIORITY_PACKET_RX     4   /* Packet reception */
#define PRIORITY_INTERRUPT     5   /* Deferred interrupt handling */

void init_tasks(void) {
    xTaskCreate(logging_task, "Log", 512, NULL, PRIORITY_LOGGING, NULL);
    xTaskCreate(cli_task, "CLI", 1024, NULL, PRIORITY_MANAGEMENT, NULL);
    xTaskCreate(ospf_task, "OSPF", 2048, NULL, PRIORITY_ROUTING, NULL);
    xTaskCreate(rx_task, "RX", 4096, NULL, PRIORITY_PACKET_RX, NULL);
}
```

**Rule:** Higher priority tasks preempt lower priority immediately. No time slicing unless tasks have equal priority.

### Priority Inversion Solution

```c
/* Mutex with priority inheritance */
SemaphoreHandle_t create_priority_safe_mutex(void) {
    return xSemaphoreCreateMutex();  /* Built-in priority inheritance */
}

void low_priority_task(void *param) {
    SemaphoreHandle_t mutex = (SemaphoreHandle_t)param;
    
    while (1) {
        xSemaphoreTake(mutex, portMAX_DELAY);
        
        /* While holding mutex, temporarily inherits priority
           of any higher-priority task blocking on this mutex */
        
        access_shared_resource();
        
        xSemaphoreGive(mutex);
    }
}
```

FreeRTOS mutexes automatically implement priority inheritance. Other RTOSes may require explicit configuration.

## Memory Management in RTOS

### Static Allocation: The RTOS Way

Dynamic allocation (malloc) is discouraged or disabled in hard real-time systems. Allocation can fail, is non-deterministic, and causes fragmentation.

```c
/* WRONG - dynamic allocation in RTOS */
void task_dynamic_alloc(void *param) {
    while (1) {
        struct packet *pkt = malloc(sizeof(*pkt));
        if (!pkt) {
            /* What now? Can't process packet */
            continue;
        }
        
        process_packet(pkt);
        free(pkt);  /* Fragmentation over time */
    }
}

/* CORRECT - static pool allocation */
#define PACKET_POOL_SIZE 100
struct packet packet_pool[PACKET_POOL_SIZE];
uint8_t pool_bitmap[PACKET_POOL_SIZE];
SemaphoreHandle_t pool_lock;

void init_packet_pool(void) {
    memset(pool_bitmap, 1, sizeof(pool_bitmap));  /* All free */
    pool_lock = xSemaphoreCreateMutex();
}

struct packet *packet_alloc(void) {
    struct packet *pkt = NULL;
    
    xSemaphoreTake(pool_lock, portMAX_DELAY);
    
    for (int i = 0; i < PACKET_POOL_SIZE; i++) {
        if (pool_bitmap[i]) {
            pool_bitmap[i] = 0;
            pkt = &packet_pool[i];
            break;
        }
    }
    
    xSemaphoreGive(pool_lock);
    return pkt;
}

void packet_free(struct packet *pkt) {
    int idx = pkt - packet_pool;
    
    xSemaphoreTake(pool_lock, portMAX_DELAY);
    pool_bitmap[idx] = 1;
    xSemaphoreGive(pool_lock);
}
```

### Stack Size Calculation

Each task has its own stack. Too small causes overflow (crash). Too large wastes RAM.

```c
/* Monitor stack usage */
void check_stack_usage(TaskHandle_t task) {
    UBaseType_t high_water = uxTaskGetStackHighWaterMark(task);
    
    /* High water mark in words (4 bytes each on 32-bit) */
    printf("Task has %u words unused\n", high_water);
    
    if (high_water < 100) {
        printf("WARNING: Stack nearly exhausted!\n");
    }
}

/* Call periodically in debug builds */
void monitor_all_stacks(void) {
    TaskHandle_t tasks[20];
    UBaseType_t task_count = uxTaskGetNumberOfTasks();
    
    task_count = uxTaskGetSystemState(tasks, task_count, NULL);
    
    for (int i = 0; i < task_count; i++) {
        check_stack_usage(tasks[i]);
    }
}
```

**Best Practice:** Start with generous stack sizes during development. Monitor high water marks. Reduce stacks to (high water mark + 50% margin) for production.

## Inter-Task Communication

### Queues: The Primary Mechanism

Queues are the standard RTOS IPC primitive. Thread-safe, blocking, priority-aware.

```c
/* Queue definition */
QueueHandle_t packet_queue;

void init_packet_queue(void) {
    /* Create queue holding 50 packet pointers */
    packet_queue = xQueueCreate(50, sizeof(struct packet *));
}

/* Producer task */
void rx_interrupt_task(void *param) {
    while (1) {
        struct packet *pkt = receive_from_hardware();
        
        /* Send to queue - blocks if full */
        if (xQueueSend(packet_queue, &pkt, pdMS_TO_TICKS(10)) != pdPASS) {
            /* Queue full - drop packet */
            packet_free(pkt);
            stats.rx_drops++;
        }
    }
}

/* Consumer task */
void packet_processor_task(void *param) {
    while (1) {
        struct packet *pkt;
        
        /* Block until packet available */
        xQueueReceive(packet_queue, &pkt, portMAX_DELAY);
        
        process_packet(pkt);
        packet_free(pkt);
    }
}
```

**Queue provides:**
- Automatic synchronization
- Blocking/timeout semantics
- Backpressure (full queue causes producer to block)
- Priority inheritance if configured

### Semaphores: Binary and Counting

```c
/* Binary semaphore for signaling */
SemaphoreHandle_t event_sem;

void init_event_semaphore(void) {
    event_sem = xSemaphoreCreateBinary();
}

/* ISR signals event */
void hardware_isr(void) {
    BaseType_t higher_priority_woken = pdFALSE;
    
    /* Give semaphore from ISR */
    xSemaphoreGiveFromISR(event_sem, &higher_priority_woken);
    
    /* Yield if higher priority task was woken */
    portYIELD_FROM_ISR(higher_priority_woken);
}

/* Task waits for event */
void event_handler_task(void *param) {
    while (1) {
        /* Block until signaled */
        xSemaphoreTake(event_sem, portMAX_DELAY);
        
        handle_hardware_event();
    }
}

/* Counting semaphore for resource pool */
SemaphoreHandle_t buffer_sem;

void init_buffer_semaphore(void) {
    buffer_sem = xSemaphoreCreateCounting(10, 10);  /* Max 10, initial 10 */
}

void allocate_buffer(void) {
    /* Wait for available buffer */
    xSemaphoreTake(buffer_sem, portMAX_DELAY);
    /* Use buffer */
}

void free_buffer(void) {
    xSemaphoreGive(buffer_sem);
}
```

## Interrupt Handling

### ISR Constraints

Interrupt Service Routines in RTOS have strict rules:

```c
/* WRONG - violates ISR constraints */
void bad_isr(void) {
    struct packet *pkt = malloc(sizeof(*pkt));  /* FORBIDDEN - blocking */
    printf("Interrupt!\n");  /* FORBIDDEN - too slow */
    xQueueSend(queue, &pkt, portMAX_DELAY);  /* FORBIDDEN - can block */
}

/* CORRECT - defer work to task */
TaskHandle_t deferred_task;

void good_isr(void) {
    BaseType_t higher_priority_woken = pdFALSE;
    
    /* Minimal work in ISR */
    acknowledge_interrupt();
    
    /* Wake deferred processing task */
    vTaskNotifyGiveFromISR(deferred_task, &higher_priority_woken);
    
    portYIELD_FROM_ISR(higher_priority_woken);
}

void deferred_interrupt_task(void *param) {
    while (1) {
        /* Block until ISR signals */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        /* Do expensive processing here */
        struct packet *pkt = packet_alloc();
        read_packet_from_hardware(pkt);
        process_packet(pkt);
        packet_free(pkt);
    }
}
```

**ISR Rules:**
- ❌ No blocking calls (can't wait for mutex, semaphore with timeout)
- ❌ No malloc/free
- ❌ No printf or other I/O
- ✅ Use FromISR variants of RTOS functions
- ✅ Keep ISR as short as possible
- ✅ Defer work to tasks

### Direct Task Notification

Faster than queues/semaphores for simple signaling.

```c
/* Task handle stored globally */
TaskHandle_t packet_task_handle;

/* Create task and save handle */
void create_packet_task(void) {
    xTaskCreate(packet_task, "Packet", 2048, NULL, 5, &packet_task_handle);
}

/* ISR notifies task directly */
void packet_rx_isr(void) {
    BaseType_t woken = pdFALSE;
    
    /* Increment task's notification value */
    vTaskNotifyGiveFromISR(packet_task_handle, &woken);
    
    portYIELD_FROM_ISR(woken);
}

/* Task waits for notification */
void packet_task(void *param) {
    while (1) {
        /* Wait for notification, returns count */
        uint32_t notifications = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        /* Process 'notifications' packets */
        for (int i = 0; i < notifications; i++) {
            process_next_packet();
        }
    }
}
```

**Performance:** Direct notification is fastest RTOS synchronization primitive. ~10x faster than queues.

## Timing and Delays

### Software Timers

```c
TimerHandle_t periodic_timer;

void timer_callback(TimerHandle_t timer) {
    /* Called every 100ms */
    update_statistics();
    check_link_status();
}

void init_periodic_timer(void) {
    /* Create timer: 100ms period, auto-reload */
    periodic_timer = xTimerCreate(
        "Stats",                    /* Name */
        pdMS_TO_TICKS(100),        /* Period */
        pdTRUE,                    /* Auto-reload */
        NULL,                      /* Timer ID */
        timer_callback             /* Callback */
    );
    
    xTimerStart(periodic_timer, 0);
}
```

**Note:** Timer callbacks run in timer task context, not interrupt context. But keep them short—they delay other timers.

### Task Delays

```c
void periodic_task(void *param) {
    TickType_t last_wake = xTaskGetTickCount();
    
    while (1) {
        perform_periodic_work();
        
        /* Delay until next period - drift-compensating */
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10));
    }
}
```

`vTaskDelayUntil()` compensates for drift. If work takes 2ms, delays 8ms to maintain 10ms period. `vTaskDelay()` would delay 10ms after completion, giving 12ms period.

## Deterministic Performance

### Measuring Worst-Case Execution Time

```c
void measure_wcet(void) {
    TickType_t start, end, worst = 0;
    
    for (int i = 0; i < 10000; i++) {
        start = xTaskGetTickCount();
        
        critical_operation();
        
        end = xTaskGetTickCount();
        TickType_t duration = end - start;
        
        if (duration > worst) {
            worst = duration;
        }
    }
    
    printf("Worst case: %u ticks (%u ms)\n", 
           worst, worst * portTICK_PERIOD_MS);
}
```

Run this test under maximum system load (all tasks active, queues full, interrupts firing).

### Disabling Interrupts Carefully

```c
/* Protect critical section from interrupts */
void update_shared_register(uint32_t value) {
    taskENTER_CRITICAL();
    
    /* Interrupts disabled - keep this SHORT */
    hardware_register = value;
    
    taskEXIT_CRITICAL();
}
```

**CRITICAL:** Time with interrupts disabled determines interrupt latency. Keep critical sections under 10µs. Long critical sections prevent timely interrupt handling.

## Common RTOS APIs Comparison

### Task Creation

```c
/* FreeRTOS */
xTaskCreate(task_func, "Name", stack_size, param, priority, &handle);

/* VxWorks */
taskSpawn("Name", priority, options, stack_size, task_func, 
          arg1, arg2, ...);

/* QNX */
pthread_create(&thread, &attr, task_func, param);
/* QNX uses POSIX threads with real-time extensions */

/* ThreadX */
tx_thread_create(&thread, "Name", task_func, param, 
                stack_ptr, stack_size, priority, priority, 
                TX_NO_TIME_SLICE, TX_AUTO_START);
```

### Queue Operations

```c
/* FreeRTOS */
xQueueSend(queue, &item, timeout);
xQueueReceive(queue, &item, timeout);

/* VxWorks */
msgQSend(queue, buffer, length, timeout, priority);
msgQReceive(queue, buffer, max_length, timeout);

/* QNX */
MsgSend(connection, send_buf, send_size, reply_buf, reply_size);
MsgReceive(channel, recv_buf, recv_size, &info);

/* ThreadX */
tx_queue_send(&queue, &item, timeout);
tx_queue_receive(&queue, &item, timeout);
```

## Embedded Linux vs RTOS

Many modern network devices use embedded Linux for control plane, dedicated RTOS or bare metal for data plane.

### Hybrid Architecture

```
┌─────────────────────────────────────────┐
│         Control Plane (Linux)           │
│   - Routing protocols (BGP, OSPF)      │
│   - Management (CLI, SNMP, REST API)   │
│   - Configuration                       │
└─────────────────┬───────────────────────┘
                  │ IPC (shared memory)
┌─────────────────┴───────────────────────┐
│       Data Plane (RTOS/Bare Metal)      │
│   - Packet forwarding                   │
│   - ACL processing                      │
│   - QoS                                 │
└─────────────────────────────────────────┘
```

**Communication:** Shared memory for route table updates, packet statistics. Minimal coupling for independence.

```c
/* Simplified hybrid interface */
struct shared_control {
    pthread_mutex_t lock;  /* Process-shared */
    struct route_table routes;
    struct statistics stats;
};

/* Linux control plane updates routes */
void control_plane_update_route(struct shared_control *ctl,
                                struct route *route) {
    pthread_mutex_lock(&ctl->lock);
    insert_route(&ctl->routes, route);
    pthread_mutex_unlock(&ctl->lock);
}

/* RTOS data plane reads routes (lockless possible with RCU) */
struct route *data_plane_lookup_route(struct shared_control *ctl,
                                      uint32_t dest) {
    /* Lockless read with memory barriers */
    __atomic_thread_fence(__ATOMIC_ACQUIRE);
    return find_route(&ctl->routes, dest);
}
```

## Best Practices Summary

### DO:
- ✅ Use static memory allocation (pools, fixed-size arrays)
- ✅ Keep ISRs short (< 10µs), defer work to tasks
- ✅ Use priority inheritance for all mutexes
- ✅ Monitor stack usage during development
- ✅ Measure worst-case execution time under load
- ✅ Use direct task notifications for fastest signaling
- ✅ Pin critical tasks to specific cores
- ✅ Use vTaskDelayUntil for periodic tasks (not vTaskDelay)
- ✅ Implement watchdog timers for fault recovery
- ✅ Profile interrupt latency and context switch time

### DON'T:
- ❌ Use malloc/free in production RTOS code
- ❌ Call blocking functions from ISRs
- ❌ Use printf or file I/O in time-critical paths
- ❌ Disable interrupts for long periods (> 10µs)
- ❌ Create tasks dynamically during operation
- ❌ Assume equal-priority tasks time-slice (verify RTOS behavior)
- ❌ Forget to enable priority inheritance on mutexes
- ❌ Use dynamic stack sizes (calculate and fix at compile time)
- ❌ Skip worst-case testing under maximum load
- ❌ Mix blocking and non-blocking I/O carelessly

## Debugging RTOS Issues

### Stack Overflow Detection

```c
/* FreeRTOS - enable stack checking */
#define configCHECK_FOR_STACK_OVERFLOW 2

/* Stack overflow hook */
void vApplicationStackOverflowHook(TaskHandle_t task, char *task_name) {
    /* Task 'task_name' overflowed stack */
    printf("STACK OVERFLOW: %s\n", task_name);
    
    /* Log to flash, reboot, etc. */
    for(;;);  /* Halt */
}
```

### CPU Usage Monitoring

```c
void print_task_stats(void) {
    TaskStatus_t *tasks;
    uint32_t total_runtime;
    
    UBaseType_t task_count = uxTaskGetNumberOfTasks();
    tasks = pvPortMalloc(task_count * sizeof(TaskStatus_t));
    
    task_count = uxTaskGetSystemState(tasks, task_count, &total_runtime);
    
    for (int i = 0; i < task_count; i++) {
        uint32_t percentage = (tasks[i].ulRunTimeCounter * 100) / total_runtime;
        printf("%s: %u%%\n", tasks[i].pcTaskName, percentage);
    }
    
    vPortFree(tasks);
}
```

### Trace Analysis

Most RTOSes support tracing for post-mortem analysis.

```c
/* Enable FreeRTOS trace */
#define configUSE_TRACE_FACILITY 1

/* Trace records all task switches, queue operations, etc.
   Analyze with tools like Tracealyzer */
```

## The Bottom Line

RTOS development for network devices requires discipline. Static allocation, bounded execution time, priority-based design, and minimal ISRs are not suggestions—they're requirements for meeting real-time deadlines. Hybrid architectures combining Linux control planes with RTOS data planes offer the best of both worlds: rich features and hard real-time performance. Measure everything, test worst cases, and never assume—verify with instrumentation.