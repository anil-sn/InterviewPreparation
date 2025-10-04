# Hypervisor: The Foundation of Virtualization

## Table of Contents
1. [Understanding the Problem](#understanding-the-problem)
2. [What is a Hypervisor?](#what-is-a-hypervisor)
3. [CPU Virtualization Basics](#cpu-virtualization-basics)
4. [Types of Hypervisors](#types-of-hypervisors)
5. [How Hypervisors Work](#how-hypervisors-work)
6. [Memory Virtualization](#memory-virtualization)
7. [Popular Hypervisors](#popular-hypervisors)
8. [Hypervisor vs Container](#hypervisor-vs-container)

---

## Understanding the Problem

Before hypervisors, organizations faced several challenges:

**Hardware Underutilization**: A physical server running one application might use only 10-20% of its CPU and memory capacity. The rest sits idle, wasting money and space.

**Resource Isolation**: Running multiple applications on one physical server was risky. If one application crashed or consumed too many resources, it could affect others.

**Server Sprawl**: Companies needed separate physical servers for different applications, leading to:
- High hardware costs
- Increased power consumption
- Large data center space requirements
- Complex cable management
- Difficult disaster recovery

**Inflexible Deployment**: Provisioning a new server could take weeks - order hardware, wait for delivery, install OS, configure network, etc.

---

## What is a Hypervisor?

A **hypervisor** (also called Virtual Machine Monitor or VMM) is a software layer that sits between physical hardware and virtual machines. It creates and manages virtual machines by:

1. **Abstracting physical hardware** into virtual resources
2. **Allocating resources** (CPU, memory, storage, network) to multiple VMs
3. **Isolating VMs** from each other for security and stability
4. **Scheduling** VM execution on physical CPUs

Think of it like an apartment building manager:
- The building (physical server) has limited resources (power, water, space)
- The manager (hypervisor) divides the building into apartments (VMs)
- Each tenant (guest OS) thinks they have their own building
- The manager ensures tenants don't interfere with each other

---

## CPU Virtualization Basics

To understand hypervisors, we first need to understand how CPUs work with privilege levels.

### CPU Privilege Rings

Modern CPUs (x86/x64) have protection rings:

```
Ring 0 (Most Privileged)  - Kernel mode
  ├── Direct hardware access
  ├── Execute privileged instructions
  └── Manage memory, I/O

Ring 1 & 2 - Rarely used

Ring 3 (Least Privileged) - User mode
  ├── Applications run here
  ├── Cannot access hardware directly
  └── Must make system calls to kernel
```

**The Virtualization Challenge**: Guest OS kernels expect to run in Ring 0, but with a hypervisor present, they can't all occupy Ring 0 simultaneously.

### Three Approaches to CPU Virtualization

**1. Full Virtualization (Binary Translation)**
- Hypervisor runs in Ring 0
- Guest OS runs in Ring 1 or Ring 3
- Hypervisor intercepts and translates privileged instructions on-the-fly
- Guest OS is unaware it's virtualized
- **Performance**: Good but has overhead from translation
- **Example**: Early VMware products

**2. Paravirtualization**
- Guest OS is modified to know it's running in a VM
- Guest OS makes "hypercalls" instead of privileged instructions
- Hypervisor runs in Ring 0, Guest OS cooperates from Ring 1
- **Performance**: Better than full virtualization (less overhead)
- **Drawback**: Requires modified guest OS
- **Example**: Xen with paravirtualized guests

**3. Hardware-Assisted Virtualization**
- CPU has special virtualization extensions (Intel VT-x, AMD-V)
- Introduces a new Ring -1 for hypervisor
- Guest OS runs unmodified in Ring 0 (in VM context)
- Hardware handles privilege transitions automatically
- **Performance**: Best - native speed
- **Example**: Modern KVM, VMware ESXi, Hyper-V

```
Hardware-Assisted Virtualization:

Ring -1: Hypervisor (VMX root mode)
Ring 0:  Guest OS Kernel (VMX non-root mode)
Ring 3:  Guest Applications
```

---

## Types of Hypervisors

### Type 1: Bare-Metal Hypervisor

Runs **directly on physical hardware** without a host OS.

```
┌─────────────────────────────────────┐
│  VM1    │   VM2    │    VM3         │
│  OS1    │   OS2    │    OS3         │
├─────────────────────────────────────┤
│       Hypervisor (Type 1)           │
├─────────────────────────────────────┤
│       Physical Hardware             │
└─────────────────────────────────────┘
```

**Characteristics**:
- Boots directly like an operating system
- Has its own device drivers for hardware
- Direct hardware control
- Better performance and security

**Use Cases**:
- Enterprise data centers
- Cloud infrastructure (AWS, Azure, GCP)
- Production environments
- Mission-critical applications

**Examples**:
- VMware ESXi
- Microsoft Hyper-V (when installed as standalone)
- Xen
- KVM (technically, but uses Linux as component)
- Proxmox VE

**Advantages**:
- Lower latency - no host OS overhead
- Better resource allocation
- Higher security - smaller attack surface
- Greater stability

**Disadvantages**:
- Requires dedicated hardware
- More complex management
- Limited hardware compatibility (needs specific drivers)

---

### Type 2: Hosted Hypervisor

Runs **on top of a host operating system** as an application.

```
┌─────────────────────────────────────┐
│  VM1    │   VM2    │    VM3         │
│  OS1    │   OS2    │    OS3         │
├─────────────────────────────────────┤
│   Hypervisor (Type 2 - Application) │
├─────────────────────────────────────┤
│       Host Operating System         │
├─────────────────────────────────────┤
│       Physical Hardware             │
└─────────────────────────────────────┘
```

**Characteristics**:
- Installed like normal software
- Relies on host OS for device drivers
- Must go through host OS for hardware access
- Easier to install and use

**Use Cases**:
- Development and testing
- Desktop virtualization
- Learning and experimentation
- Running alternate OS on personal computer

**Examples**:
- VMware Workstation / Fusion
- Oracle VirtualBox
- Parallels Desktop
- QEMU (without KVM)

**Advantages**:
- Easy to install and configure
- Works with existing OS
- Good for desktop/laptop use
- Can run alongside regular applications

**Disadvantages**:
- Performance overhead (two OS layers)
- Less efficient resource usage
- Host OS issues can affect VMs
- Not suitable for production servers

---

## How Hypervisors Work

### Resource Virtualization Components

A hypervisor virtualizes four main resource types:

**1. CPU Virtualization**
- Creates virtual CPUs (vCPUs)
- Schedules vCPUs on physical CPUs
- Handles CPU instruction emulation/translation
- Manages CPU affinity and NUMA

Example: A server with 16 physical CPU cores can run:
- VM1: 4 vCPUs
- VM2: 2 vCPUs  
- VM3: 8 vCPUs
- Total: 14 vCPUs allocated (can overcommit)

**2. Memory Virtualization**
- Creates virtual RAM for each VM
- Maps virtual memory to physical memory
- Implements memory overcommitment techniques:
  - **Ballooning**: Reclaims unused memory from VMs
  - **Swapping**: Pages out to disk when needed
  - **Deduplication**: Shares identical memory pages
  - **Compression**: Compresses less-used pages

**3. Storage Virtualization**
- Presents virtual disks to VMs
- Virtual disks are files on host filesystem
- Formats: VMDK, VHD, QCOW2, RAW
- Features: Snapshots, cloning, thin provisioning

**4. Network Virtualization**
- Creates virtual network adapters
- Implements virtual switches
- Connects VMs to physical networks
- Enables VM-to-VM communication

---

### The Hypervisor Stack

```
┌──────────────────────────────────────────┐
│           Guest Applications             │
├──────────────────────────────────────────┤
│           Guest OS Kernel                │
├──────────────────────────────────────────┤
│      Virtual Hardware Interface          │
│  (vCPU, vRAM, vDisk, vNIC)              │
├──────────────────────────────────────────┤
│         Hypervisor Core                  │
│  ┌────────────────────────────────────┐  │
│  │  VM Scheduler                      │  │
│  │  Memory Manager                    │  │
│  │  Virtual Device Emulator           │  │
│  │  Security & Isolation Engine       │  │
│  └────────────────────────────────────┘  │
├──────────────────────────────────────────┤
│       Hardware Abstraction Layer         │
├──────────────────────────────────────────┤
│         Physical Hardware                │
│  (CPU, RAM, Storage, Network)           │
└──────────────────────────────────────────┘
```

---

## Memory Virtualization

Memory virtualization involves multiple layers of address translation.

### Three Levels of Memory

**1. Guest Virtual Memory** (GVA)
- Memory addresses used by applications in the VM
- Managed by guest OS

**2. Guest Physical Memory** (GPA)
- Memory addresses the guest OS thinks are physical
- Actually virtual addresses created by hypervisor

**3. Host Physical Memory** (HPA)
- Actual physical RAM in the server
- Managed by hypervisor

### Address Translation Process

```
Application makes memory access
         ↓
Guest Virtual Address (GVA)
         ↓
[Guest OS Page Table] - Translates GVA to GPA
         ↓
Guest Physical Address (GPA)
         ↓
[Hypervisor Page Table] - Translates GPA to HPA
         ↓
Host Physical Address (HPA)
         ↓
Actual RAM access
```

**The Challenge**: Two-level translation adds overhead.

**Solution: Hardware-Assisted Memory Virtualization**

Modern CPUs provide hardware support:
- **Intel EPT** (Extended Page Tables)
- **AMD RVI** (Rapid Virtualization Indexing / NPT)

These allow CPU to handle both translations in hardware, dramatically improving performance.

### Memory Overcommitment Techniques

**Ballooning**:
```
1. Hypervisor needs memory for other VMs
2. Installs "balloon driver" in guest OS
3. Balloon driver allocates memory inside guest
4. Guest thinks balloon driver is using memory
5. Hypervisor reclaims that physical memory
6. Balloon deflates when memory available again
```

**Transparent Page Sharing (TPS)**:
- Identifies identical memory pages across VMs
- Multiple VMs running same OS have many identical pages
- Hypervisor maps them to single physical page (copy-on-write)
- Saves significant memory

**Memory Compression**:
- Compresses infrequently accessed pages
- Keeps more in RAM instead of swapping to disk
- Faster than disk access

---

## Popular Hypervisors

### VMware ESXi
- **Type**: Bare-metal (Type 1)
- **Best For**: Enterprise environments
- **Strengths**: 
  - Mature ecosystem
  - Advanced features (vMotion, DRS, HA)
  - Excellent management tools (vCenter)
  - Wide hardware support
- **Licensing**: Commercial (free basic version available)

### KVM (Kernel-based Virtual Machine)
- **Type**: Hybrid (Type 1 characteristics using Linux kernel)
- **Best For**: Linux environments, cloud infrastructure
- **Strengths**:
  - Integrated into Linux kernel
  - Open source
  - Excellent performance
  - Used by major cloud providers
- **Licensing**: Open source (GPL)

### Microsoft Hyper-V
- **Type**: Bare-metal or Hosted
- **Best For**: Windows environments
- **Strengths**:
  - Integrated with Windows Server
  - Good Windows guest performance
  - Azure integration
  - Free with Windows Server
- **Licensing**: Included with Windows Server

### Xen
- **Type**: Bare-metal (Type 1)
- **Best For**: Cloud infrastructure
- **Strengths**:
  - Pioneered paravirtualization
  - Strong isolation
  - Used by AWS (modified version)
  - Open source
- **Licensing**: Open source (GPL)

### Proxmox VE
- **Type**: Bare-metal (Type 1)
- **Best For**: Small to medium deployments, homelab
- **Strengths**:
  - Integrates KVM and LXC containers
  - Web-based management
  - Clustering and HA
  - Open source with commercial support
- **Licensing**: Open source (AGPL)

---

## Hypervisor vs Container

While both provide isolation, they work very differently:

### Hypervisor Approach

```
┌─────────┬─────────┬─────────┐
│ App A   │ App B   │ App C   │
│ Libs    │ Libs    │ Libs    │
│ Guest   │ Guest   │ Guest   │
│ OS 1    │ OS 2    │ OS 3    │
├─────────┴─────────┴─────────┤
│      Hypervisor             │
├─────────────────────────────┤
│   Physical Hardware         │
└─────────────────────────────┘
```

- Each VM has full OS
- Strong isolation (separate kernels)
- Higher resource overhead
- Boot time: seconds to minutes
- Use case: Running different OS types

### Container Approach

```
┌─────────┬─────────┬─────────┐
│ App A   │ App B   │ App C   │
│ Libs    │ Libs    │ Libs    │
├─────────┴─────────┴─────────┤
│   Container Runtime         │
├─────────────────────────────┤
│      Host OS Kernel         │
├─────────────────────────────┤
│   Physical Hardware         │
└─────────────────────────────┘
```

- Shares host kernel
- Process-level isolation
- Minimal overhead
- Boot time: milliseconds
- Use case: Microservices, scale-out apps

**Key Differences**:

| Aspect | Hypervisor | Container |
|--------|-----------|-----------|
| **Isolation** | Hardware-level | Process-level |
| **OS** | Each VM has full OS | Shares host kernel |
| **Boot Time** | Minutes | Milliseconds |
| **Size** | GBs | MBs |
| **Performance** | Near-native | Native |
| **Density** | 10s per host | 100s-1000s per host |
| **Security** | Stronger | Good (improving) |

---

## Summary

**Hypervisors solve the problem** of hardware underutilization and inflexibility by creating virtual machines that:
- Share physical hardware efficiently
- Run isolated from each other
- Can be created/destroyed quickly
- Enable server consolidation

**Type 1 hypervisors** run directly on hardware for maximum performance and are used in production environments.

**Type 2 hypervisors** run on host operating systems and are ideal for development and testing.

**Modern hypervisors** leverage hardware virtualization features (Intel VT-x, AMD-V, EPT/RVI) to achieve near-native performance.

**The foundation** they provide enables the entire cloud computing industry and modern infrastructure as we know it today.

---

# Virtual Machines

## Table of Contents
1. [What is a Virtual Machine?](#what-is-a-virtual-machine)
2. [VM Components](#vm-components)
3. [How VMs Work](#how-vms-work)
4. [Virtual Hardware](#virtual-hardware)
5. [VM Lifecycle](#vm-lifecycle)
6. [VM Storage](#vm-storage)
7. [VM Networking](#vm-networking)
8. [VM Performance](#vm-performance)
9. [Advanced VM Features](#advanced-vm-features)
10. [Best Practices](#best-practices)

---

## What is a Virtual Machine?

A **Virtual Machine (VM)** is a software-based emulation of a physical computer. It runs an operating system and applications just like a physical machine, but all its hardware is virtual.

### The Core Concept

Imagine you have a powerful physical server with:
- 64 CPU cores
- 512 GB RAM
- 10 TB storage
- Multiple network cards

Instead of using it for one purpose, you can divide it into multiple VMs:

```
Physical Server:
├── VM1: Web Server (4 cores, 8GB RAM, 100GB disk)
├── VM2: Database (16 cores, 64GB RAM, 500GB disk)
├── VM3: Application Server (8 cores, 16GB RAM, 200GB disk)
└── VM4: Testing Environment (2 cores, 4GB RAM, 50GB disk)
```

Each VM:
- Thinks it's running on dedicated hardware
- Has no knowledge of other VMs
- Can run a different operating system
- Is completely isolated from others

### Why Virtual Machines?

**1. Server Consolidation**
- Before: 1 application = 1 server (inefficient)
- After: Multiple applications on 1 server (efficient)

**2. Isolation**
- One VM crashes → others continue running
- Security breach in one VM → others remain safe

**3. Flexibility**
- Create VMs in minutes (vs. days for physical servers)
- Destroy when no longer needed
- Move between physical hosts

**4. Cost Savings**
- Fewer physical servers = less hardware cost
- Lower power consumption
- Reduced cooling requirements
- Smaller data center footprint

**5. Disaster Recovery**
- Snapshot VM state in seconds
- Back up entire VM as a file
- Restore quickly from backup
- Replicate to another location

---

## VM Components

Every VM consists of several components:

### 1. VM Configuration File

A text file (XML, VMX, etc.) containing VM metadata:

```
Example VM Configuration:
- VM Name: WebServer-01
- CPU: 4 virtual cores
- Memory: 8 GB
- Network: 2 virtual NICs
- Disk: 1 virtual disk (100 GB)
- OS Type: Linux (Ubuntu 22.04)
- Boot Order: Disk, Network
- UUID: 564d3c8a-7b9f-1a2c-d4e5-6f7890abcdef
```

### 2. Virtual Disk Files

Files that store the VM's hard drive contents:

**Common Formats**:
- **VMDK** (VMware Virtual Machine Disk)
- **VHD/VHDX** (Virtual Hard Disk - Microsoft)
- **QCOW2** (QEMU Copy-On-Write - KVM/QEMU)
- **VDI** (VirtualBox Disk Image)
- **RAW** (Raw disk image)

**Disk Types**:

**Thick Provisioned (Pre-allocated)**:
- Creates full-size file immediately
- Example: 100GB VM disk = 100GB file on host
- Performance: Better (no expansion overhead)
- Space: Uses more storage

**Thin Provisioned (Dynamically allocated)**:
- Grows as VM writes data
- Example: 100GB VM disk starts as 1GB file, grows to actual usage
- Performance: Slight overhead during growth
- Space: Uses less storage initially

### 3. Virtual Hardware Devices

The VM sees these as "physical" devices:

```
Virtual Machine "sees":
├── CPU: 4 cores @ 2.4 GHz
├── RAM: 8 GB
├── Disk Controller: SATA/SCSI/NVMe
├── Hard Disk: 100 GB
├── Network Card: Intel E1000 or VirtIO
├── Graphics: Virtual VGA/SVGA
├── USB Controller: USB 2.0/3.0
├── Sound Card: AC97 or HD Audio
└── Serial/Parallel Ports: COM1, LPT1
```

### 4. Guest Operating System

The OS running inside the VM:
- Can be Windows, Linux, BSD, etc.
- Unaware it's running in a VM (in most cases)
- Interacts with virtual hardware
- Installed just like on physical machine

### 5. Applications

Normal applications running on the guest OS:
- Web servers (Apache, Nginx)
- Databases (PostgreSQL, MySQL)
- Custom applications
- Anything that runs on the guest OS

---

## How VMs Work

### Boot Process

When you start a VM:

```
1. Hypervisor reads VM configuration file
   ↓
2. Allocates physical resources (CPU, RAM)
   ↓
3. Creates virtual hardware environment
   ↓
4. Loads virtual BIOS/UEFI
   ↓
5. Virtual BIOS reads boot order
   ↓
6. Boots from virtual disk (or ISO)
   ↓
7. Guest OS starts
   ↓
8. Guest OS loads drivers for virtual hardware
   ↓
9. Guest OS starts services
   ↓
10. System ready for use
```

### CPU Execution

**How VM processes run on physical CPUs**:

```
Time Slice 1: Physical CPU Core 1
├── VM1 vCPU1 executes for 10ms
├── [Context switch]
└── VM2 vCPU1 executes for 10ms

Time Slice 2: Physical CPU Core 1
├── VM3 vCPU1 executes for 10ms
├── [Context switch]
└── VM1 vCPU1 executes for 10ms
```

**Hypervisor Scheduler**:
- Manages when each vCPU gets physical CPU time
- Similar to OS process scheduling, but for entire VMs
- Considers priorities, reservations, limits
- Handles overcommitment (more vCPUs than physical cores)

### Memory Access

**Three-tier memory translation** (from previous hypervisor guide):

```
App in VM writes to address 0x1000
         ↓
Guest Virtual Address (GVA): 0x1000
         ↓
[Guest OS Page Table]
         ↓
Guest Physical Address (GPA): 0x5000
         ↓
[Hypervisor EPT/NPT]
         ↓
Host Physical Address (HPA): 0xA3F5000
         ↓
Actual RAM access
```

### I/O Operations

**Two approaches for I/O virtualization**:

**1. Device Emulation (Full virtualization)**:
```
VM application writes to disk
         ↓
Guest OS driver (e.g., SATA driver)
         ↓
Virtual SATA controller (emulated by hypervisor)
         ↓
Hypervisor intercepts operation
         ↓
Hypervisor performs actual I/O on real storage
         ↓
Returns result to VM
```

**2. Paravirtualized Drivers (Better performance)**:
```
VM application writes to disk
         ↓
Guest OS paravirtualized driver (VirtIO)
         ↓
Direct communication with hypervisor (hypercall)
         ↓
Hypervisor performs I/O
         ↓
Fast return to VM
```

---

## Virtual Hardware

### Virtual CPUs (vCPUs)

**vCPU Allocation**:
- Each vCPU is a thread scheduled on physical CPUs
- Can overcommit: 8 physical cores → 32 vCPUs across VMs
- Best practice: Don't massively overcommit (leads to CPU contention)

**vCPU Configurations**:
```
Single vCPU:
- Simple workloads
- Better for CPU cache utilization
- Lower overhead

Multiple vCPUs:
- Parallel processing
- Multi-threaded applications
- Database servers
- Higher overhead (vCPU synchronization)
```

**CPU Features Exposed to VMs**:
- Instruction sets (SSE, AVX)
- Hardware virtualization (nested virtualization)
- CPU flags and capabilities
- NUMA topology awareness

### Virtual Memory

**Memory Allocation**:
```
VM with 8 GB RAM:
├── Guest sees: 8 GB continuous memory
├── Actually: Mapped to various physical addresses
└── Hypervisor manages: Page tables, swapping, ballooning
```

**Memory Techniques**:

**Reservation**:
- Guaranteed minimum memory for VM
- Example: Reserve 4 GB for database VM
- Cannot be reclaimed by hypervisor

**Limit**:
- Maximum memory VM can use
- Prevents one VM from monopolizing memory

**Shares**:
- Relative priority during memory contention
- High shares = more memory during shortage

### Virtual Disks

**Disk Formats Compared**:

**VMDK (VMware)**:
```
Features:
- Split files or monolithic
- Supports snapshots
- Can be thin or thick
- Compatible with many tools
```

**QCOW2 (QEMU/KVM)**:
```
Features:
- Copy-on-write
- Built-in compression
- Encryption support
- Snapshot support
- Smaller file sizes
```

**VHD/VHDX (Hyper-V)**:
```
Features:
- VHDX supports up to 64 TB
- Better data corruption resilience
- Supports trim operations
```

**RAW**:
```
Features:
- Best performance (no format overhead)
- Simple (just raw bytes)
- Larger file size (no compression)
- No snapshot support in format itself
```

**Disk Controllers**:

VMs can use different virtual disk controllers:

**IDE/SATA**:
- Compatible with all OS
- Slower performance
- Good for CD/DVD

**SCSI**:
- Better performance
- Hot-plug support
- More drives per controller

**VirtIO (Paravirtualized)**:
- Best performance
- Requires guest drivers
- Modern Linux has built-in support

**NVMe**:
- Highest performance
- Modern storage interface
- Newer guest OS required

### Virtual Network Adapters

**Network Card Types**:

**Emulated (E1000, VMXNET)**:
- Emulates real network cards
- Good compatibility
- Lower performance

**Paravirtualized (VirtIO-net)**:
- Purpose-built for virtualization
- High performance
- Requires guest drivers

**SR-IOV Virtual Function**:
- Direct hardware access
- Near-native performance
- (Covered in SR-IOV section)

---

## VM Lifecycle

### Creation

```
1. Define VM specifications
   - Name, CPU, Memory, Disk size
   ↓
2. Create VM configuration file
   ↓
3. Create virtual disk file(s)
   ↓
4. Attach installation media (ISO)
   ↓
5. Start VM and install OS
   ↓
6. Install guest tools/drivers
   ↓
7. Configure and customize
   ↓
8. Create baseline snapshot (optional)
```

### States

VMs can be in different states:

**Powered Off**:
- VM is defined but not running
- No resource consumption
- Configuration exists, disks intact

**Powered On**:
- VM is running
- Consuming CPU, memory
- Guest OS and applications running

**Suspended (Saved State)**:
- VM state saved to disk
- Memory contents written to file
- Can resume exactly where left off
- No CPU usage, memory released

**Paused**:
- Temporarily frozen
- Memory retained in RAM
- No CPU execution
- Quick resume

### Cloning

**Full Clone**:
```
Original VM
    ↓
Copy entire VM (config + disks)
    ↓
Independent VM
```
- Complete copy
- No dependency on original
- Uses more storage

**Linked Clone**:
```
Original VM (Parent)
    ↓
Create differencing disk
    ↓
Linked Clone (Child)
```
- References parent disk
- Only stores differences
- Saves storage space
- Requires parent VM

### Snapshots

**How Snapshots Work**:

```
Before Snapshot:
VM uses: disk.vmdk (10 GB used)

Take Snapshot "Before Update":
VM now uses: disk-snapshot1.vmdk (writes go here)
Original disk: disk.vmdk (frozen, read-only)

Take another Snapshot "After Config":
VM now uses: disk-snapshot2.vmdk (writes go here)
Previous: disk-snapshot1.vmdk (frozen)
Original: disk.vmdk (frozen)

Snapshot Chain:
disk.vmdk → disk-snapshot1.vmdk → disk-snapshot2.vmdk
                                        ↑
                                   (active)
```

**Snapshot Includes**:
- Disk state (via delta disks)
- Memory state (optional)
- VM settings at that point
- Timestamp and description

**Snapshot Uses**:
- Before software updates
- Testing configurations
- Development/testing workflows
- Temporary safe points

**Snapshot Warnings**:
- Performance degrades with many snapshots
- Chain too long = slower I/O
- Not a backup solution (same storage)
- Should be temporary

### Migration

**Cold Migration**:
```
1. Power off VM
2. Copy VM files to new host
3. Register VM on new host
4. Power on VM
```
- Simple but causes downtime

**Live Migration (vMotion/Live Migration)**:
```
1. VM running on Host A
2. Copy memory pages to Host B (while running)
3. Track changed pages (dirty pages)
4. Brief pause (stun time)
5. Switch to Host B
6. VM continues running
```
- No downtime
- Requires shared storage or storage migration
- Allows host maintenance without VM downtime

---

## VM Storage

### Storage Types

**1. Local Storage**:
```
Hypervisor Host
├── Internal SSD/HDD
└── VM disks stored locally
```
- Fast (direct access)
- No network overhead
- Cannot live migrate VMs easily

**2. Shared Storage (SAN/NAS)**:
```
Multiple Hypervisor Hosts
        ↓
    Network
        ↓
Shared Storage (SAN/NAS)
└── VM disks accessible by all hosts
```
- Enables live migration
- High availability
- Centralized management
- Network dependency

**3. Distributed Storage**:
```
Each Hypervisor Host has local storage
        ↓
Software creates distributed storage pool
        ↓
VMs can run anywhere in cluster
```
- Combines local storage across hosts
- Redundancy through replication
- Examples: VMware vSAN, Ceph, Gluster

### Disk Operations

**Thin Provisioning**:
```
Create 100 GB disk:
Initial size: 1 GB
VM writes 10 GB: File grows to 11 GB
VM writes 30 GB more: File grows to 41 GB
```

**Disk Growth Management**:
- Monitor actual usage vs. allocated
- Reclaim unused space (trim/unmap)
- Convert thick to thin or vice versa

**Snapshots and Chains**:
```
Base Disk (base.vmdk): 50 GB
    ↓
Snapshot 1 (snap1.vmdk): +5 GB
    ↓
Snapshot 2 (snap2.vmdk): +3 GB
    ↓
Total: 58 GB across 3 files
```

**Consolidation**:
- Merges snapshot chain back to base disk
- Improves performance
- Reduces file count

---

## VM Networking

### Virtual Network Components

**Virtual Network Interface Card (vNIC)**:
- VM sees it as real NIC
- Has MAC address
- Connects to virtual switch

**Virtual Switch**:
```
┌─────────┬─────────┬─────────┐
│  VM1    │  VM2    │  VM3    │
│  vNIC   │  vNIC   │  vNIC   │
└────┬────┴────┬────┴────┬────┘
     └─────────┼─────────┘
          Virtual Switch
               │
        Physical NIC(s)
               │
          External Network
```

### Network Modes

**1. Bridged Mode**:
```
VM directly on physical network
- VM gets IP from network DHCP
- Appears as separate machine on network
- Can communicate with any device on LAN
```

**2. NAT Mode**:
```
VMs share host's IP address
- Private network for VMs
- VMs can access external network
- External network cannot directly access VMs
```

**3. Host-Only Mode**:
```
VMs isolated network with host
- VMs can talk to each other and host
- No external network access
- Good for testing
```

**4. Internal Mode**:
```
VMs communicate with each other only
- No host access
- No external access
- Completely isolated
```

### Network Performance

**Factors Affecting Performance**:
- Network adapter type (E1000 vs VirtIO)
- Virtual switch configuration
- Physical NIC bandwidth
- Network traffic patterns

**Optimization**:
- Use paravirtualized network drivers
- Enable jumbo frames
- Multiple vNICs for different purposes
- Proper VLAN configuration

---

## VM Performance

### CPU Performance

**Best Practices**:

**Right-sizing**:
```
Bad: 16 vCPUs for app needing 2
- Wasted resources
- Increased context switching
- Slower performance

Good: Match vCPUs to actual need
```

**CPU Ready Time**:
- Time vCPU waits for physical CPU
- High CPU ready = overcommitment
- Monitor and adjust

**NUMA Awareness**:
```
Proper NUMA alignment:
├── vCPUs from same physical CPU socket
└── Memory from same NUMA node

Result: Faster memory access
```

### Memory Performance

**Monitoring Metrics**:
- **Active Memory**: Actually used by VM
- **Consumed Memory**: All allocated to VM
- **Ballooned Memory**: Reclaimed by balloon driver
- **Swapped Memory**: Paged to disk (bad!)

**Performance Tips**:
- Don't over-allocate memory
- Monitor ballooning and swapping
- Use memory reservations for critical VMs
- Disable unnecessary services in guest

### Storage Performance

**IOPS (Input/Output Operations Per Second)**:
```
Spindle Disk: ~100 IOPS
SSD: ~10,000 IOPS
NVMe SSD: ~100,000+ IOPS
```

**Optimization**:
- Use SSD for demanding workloads
- Separate OS and data disks
- Use paravirtualized SCSI controllers
- Enable disk write caching (with caution)
- Align partitions properly

### Network Performance

**Throughput Considerations**:
- Physical NIC bandwidth
- Virtual switch overhead
- Multiple VMs sharing NIC
- Network protocol overhead

**Improvements**:
- SR-IOV for high-performance networking
- Multiple physical NICs (teaming/bonding)
- Traffic shaping and QoS
- Offload features (TSO, LRO, GSO)

---

## Advanced VM Features

### Guest Tools

Every hypervisor has guest tools/additions:

**VMware Tools / Hyper-V Integration Services / QEMU Guest Agent**:

**Features**:
- Better drivers (graphics, network, disk)
- Time synchronization with host
- Clipboard sharing
- Drag and drop files
- Graceful shutdown
- Heartbeat monitoring
- Memory ballooning
- Performance improvements

**Installation**:
```
Without tools: Limited functionality
With tools: Full features and better performance
```

### High Availability

**VM Monitoring**:
```
1. Hypervisor monitors VM heartbeat
2. If VM fails or hangs
3. Automatically restart on same or different host
```

**Host Failure**:
```
1. Host crashes
2. Cluster detects failure
3. VMs automatically restart on surviving hosts
```

Requires:
- Shared storage
- Cluster configuration
- Multiple hosts

### Resource Pools

**Organizing VMs by purpose**:
```
Production Resource Pool
├── Shares: 80%
├── Reservation: 100 GB RAM
└── Limit: 200 GB RAM
    ├── Web-VM-01
    ├── Web-VM-02
    └── DB-VM-01

Development Resource Pool
├── Shares: 20%
├── Reservation: 20 GB RAM
└── Limit: 50 GB RAM
    ├── Dev-VM-01
    └── Test-VM-01
```

### Templates

**Master Image Workflow**:
```
1. Create VM and configure perfectly
2. Install and configure OS
3. Install applications
4. Generalize (sysprep/virt-sysprep)
5. Convert to template
6. Deploy new VMs from template
7. Customize each deployment
```

**Benefits**:
- Consistent configurations
- Fast deployment
- Standardization
- Reduced errors

---

## Best Practices

### Design

**1. Right-Size VMs**:
- Don't over-provision (waste resources)
- Don't under-provision (poor performance)
- Monitor and adjust based on actual usage

**2. Resource Allocation**:
```
Total Physical: 64 cores, 512 GB RAM

Reserve some for hypervisor:
├── Available: 60 cores, 480 GB RAM

Don't overcommit excessively:
├── CPU: Can go 2:1 or 3:1
└── Memory: Minimal overcommit or none
```

**3. Naming Conventions**:
```
Environment-Purpose-OS-Number
├── PROD-WEB-UBUNTU-01
├── PROD-DB-RHEL-01
├── DEV-APP-WIN-01
└── TEST-API-UBUNTU-02
```

### Operations

**1. Regular Maintenance**:
- Update guest OS regularly
- Update guest tools
- Update hypervisor
- Monitor performance
- Clean up old snapshots

**2. Backup Strategy**:
- Regular backups (not just snapshots)
- Test restores periodically
- Off-site backup copies
- Document recovery procedures

**3. Monitoring**:
```
Monitor:
├── CPU usage and ready time
├── Memory usage and ballooning
├── Disk IOPS and latency
├── Network throughput
└── VM heartbeat
```

### Security

**1. Isolation**:
- Separate VMs by security level
- Different networks for different purposes
- Limit VM-to-VM communication

**2. Hardening**:
- Minimal OS installation
- Disable unnecessary services
- Keep software updated
- Use firewalls

**3. Access Control**:
- Limit who can manage VMs
- Audit VM changes
- Separate admin privileges

---

## Summary

**Virtual Machines** provide:
- Complete isolation from other VMs
- Ability to run different operating systems
- Full computer simulation in software
- Foundation for cloud computing

**VMs use virtual hardware** including:
- vCPUs scheduled on physical CPUs
- Virtual memory mapped to physical RAM
- Virtual disks stored as files
- Virtual network cards connected to virtual switches

**VM lifecycle** includes:
- Creation and configuration
- Cloning and templating
- Snapshots for point-in-time states
- Migration between hosts
- Backup and disaster recovery

**Performance** depends on:
- Proper resource allocation
- Hardware quality (SSD vs HDD)
- Hypervisor efficiency
- Guest tools installation
- Workload characteristics

VMs remain the foundation of modern data centers and cloud infrastructure, providing the isolation and flexibility needed for enterprise computing.

---

# Containers and Linux Namespaces

## Table of Contents
1. [Understanding the Evolution](#understanding-the-evolution)
2. [What Are Containers?](#what-are-containers)
3. [Linux Namespaces - The Foundation](#linux-namespaces---the-foundation)
4. [Control Groups (cgroups)](#control-groups-cgroups)
5. [Container Images](#container-images)
6. [Container Runtime](#container-runtime)
7. [Containers vs VMs](#containers-vs-vms)
8. [Container Networking](#container-networking)
9. [Container Storage](#container-storage)
10. [Container Orchestration](#container-orchestration)

---

## Understanding the Evolution

### The Problem VMs Solved (But Incompletely)

VMs revolutionized computing by allowing multiple isolated environments on one physical server. However:

```
Traditional VM Setup:
┌─────────────────────────────────────┐
│        VM 1 (Ubuntu)                │
│  ┌──────────────────────────────┐   │
│  │  App (100 MB)                │   │
│  │  Libraries (500 MB)          │   │
│  │  Full OS (2-5 GB)            │   │ Size: ~5 GB
│  └──────────────────────────────┘   │ Boot: 30-60s
├─────────────────────────────────────┤
│      Hypervisor                     │
├─────────────────────────────────────┤
│      Hardware                       │
└─────────────────────────────────────┘
```

**VM Limitations for Modern Apps**:

1. **Heavy Weight**: Each VM includes full OS (GBs of storage)
2. **Slow Start**: Boot time measured in seconds to minutes
3. **Resource Overhead**: Guest OS consumes CPU and memory
4. **Low Density**: Typically 10-50 VMs per physical host
5. **Image Size**: Difficult to distribute and update

**The Microservices Problem**:

Modern applications are decomposed into many small services:

```
E-commerce Application:
├── Authentication service
├── User profile service
├── Product catalog service
├── Shopping cart service
├── Payment service
├── Notification service
└── Analytics service

Running each in a VM:
- 7 VMs × 5 GB = 35 GB just for OS
- 7 VMs × 1 GB RAM overhead = 7 GB wasted
- Slow scaling (60s per VM)
```

**What Developers Really Needed**:
- Just the application and its dependencies
- Millisecond startup times
- Minimal resource overhead
- Easy to package and ship
- Consistent across environments

### Enter Containers

Containers provide **process-level isolation** without the overhead of full VMs:

```
Container Setup:
┌─────────────────────────────────────┐
│  Container 1  │ Container 2 │ C3    │
│  App + Libs   │ App + Libs  │ App   │ Size: 100-500 MB each
│  (200 MB)     │ (150 MB)    │(100MB)│ Start: milliseconds
├─────────────────────────────────────┤
│     Container Runtime (Docker)      │
├─────────────────────────────────────┤
│       Host OS (Linux Kernel)        │
├─────────────────────────────────────┤
│           Hardware                  │
└─────────────────────────────────────┘
```

**The Key Insight**: 
All containers share the same kernel but appear isolated from each other. No need for multiple OS copies!

---

## What Are Containers?

A **container** is a lightweight, standalone, executable package that includes:
- Application code
- Runtime (e.g., Node.js, Python interpreter)
- System tools and libraries
- Settings and configurations

**But NOT**:
- Full operating system
- Kernel
- Hardware drivers

### How Containers Achieve Isolation

Containers use two Linux kernel features:

**1. Namespaces** (Isolation):
- What a process can **see**
- Separate view of system resources
- Process thinks it's alone

**2. Cgroups** (Resource Limits):
- What a process can **use**
- Limit CPU, memory, I/O
- Prevent resource monopolization

Think of it like apartment buildings:

```
VM Approach (Each VM is a separate building):
Building 1: Foundation + Plumbing + Electrical + Apartment
Building 2: Foundation + Plumbing + Electrical + Apartment
Building 3: Foundation + Plumbing + Electrical + Apartment
└── Lots of duplication!

Container Approach (One building, multiple apartments):
Single Building: Shared Foundation, Plumbing, Electrical
├── Apartment 1 (isolated living space)
├── Apartment 2 (isolated living space)
└── Apartment 3 (isolated living space)
└── Shared infrastructure, isolated usage
```

---

## Linux Namespaces - The Foundation

**Namespaces** are a feature of the Linux kernel that partition kernel resources so that one set of processes sees one set of resources while another set of processes sees a different set.

### The Core Concept

Without namespaces:
```
All processes share:
- Same process ID space
- Same network interfaces
- Same filesystem mount points
- Same user IDs
- Same hostname
```

With namespaces:
```
Process Group 1 (Container 1):
- Sees PIDs 1-50
- Sees network interface eth0
- Sees /app mounted at /
- Sees users alice, bob
- Hostname: webapp-01

Process Group 2 (Container 2):
- Sees PIDs 1-30 (different processes!)
- Sees network interface eth0 (different interface!)
- Sees /db mounted at /
- Sees users postgres
- Hostname: database-01
```

Same system, but each container sees its own isolated view!

### Types of Namespaces

Linux provides several namespace types, each isolating different resource types:

---

#### 1. PID Namespace (Process Isolation)

**Purpose**: Isolates process IDs

**How It Works**:

```
Host System:
PID 1:    systemd
PID 100:  Container runtime
PID 101:  Container 1's init process
PID 102:  Container 1's app process
PID 103:  Container 1's worker process
PID 200:  Container 2's init process
PID 201:  Container 2's app process

Inside Container 1's view:
PID 1:    init process (actually PID 101 on host)
PID 2:    app process (actually PID 102 on host)
PID 3:    worker process (actually PID 103 on host)

Inside Container 2's view:
PID 1:    init process (actually PID 200 on host)
PID 2:    app process (actually PID 201 on host)
```

**Why This Matters**:
- Each container can have its own PID 1 (init process)
- Processes in one container cannot see/signal processes in another
- Container thinks it's the only thing running
- Traditional tools (ps, top) work as expected

**Practical Example**:
```bash
# On host
$ ps aux
PID 101: /bin/bash (container 1)
PID 102: python app.py (container 1)
PID 200: /bin/bash (container 2)

# Inside container 1
$ ps aux
PID 1: /bin/bash
PID 2: python app.py
# Cannot see container 2's processes!
```

---

#### 2. Network Namespace (Network Isolation)

**Purpose**: Isolates network interfaces, IP addresses, routing tables, firewall rules

**How It Works**:

```
Physical Host:
├── Physical NIC: eth0 (192.168.1.100)
├── Network Namespace 1 (Container 1):
│   ├── Virtual interface: eth0 (172.17.0.2)
│   ├── Loopback: lo
│   └── Routing table specific to this namespace
├── Network Namespace 2 (Container 2):
│   ├── Virtual interface: eth0 (172.17.0.3)
│   ├── Loopback: lo
│   └── Routing table specific to this namespace
└── Virtual bridge connecting namespaces
```

**Each Container Sees**:
- Its own network interfaces
- Its own IP addresses
- Its own routing table
- Its own firewall rules
- Its own port bindings (Container 1 and 2 can both use port 80!)

**Practical Example**:
```bash
# Container 1 sees:
$ ip addr
eth0: 172.17.0.2/16

$ netstat -ln
Port 80: LISTENING

# Container 2 sees:
$ ip addr
eth0: 172.17.0.3/16

$ netstat -ln
Port 80: LISTENING  # Same port, different namespace!
```

**Why This Matters**:
- Multiple containers can bind to same port number
- Network isolation between containers
- Custom network configuration per container
- Security: containers can't sniff each other's traffic by default

---

#### 3. Mount Namespace (Filesystem Isolation)

**Purpose**: Isolates filesystem mount points

**How It Works**:

```
Host Filesystem:
/
├── bin/
├── etc/
├── home/
└── var/
    └── lib/
        └── docker/
            └── overlay2/
                ├── container1-root/
                └── container2-root/

Container 1's View:
/
├── bin/ (from container1-root)
├── etc/ (from container1-root)
├── app/ (mounted volume)
└── tmp/

Container 2's View:
/
├── bin/ (from container2-root)
├── etc/ (from container2-root)
├── data/ (mounted volume)
└── tmp/
```

**Each Container Sees**:
- Its own root filesystem
- Its own mount points
- Cannot see other containers' filesystems

**Practical Example**:
```bash
# Container 1 creates file
$ touch /app/myfile.txt
$ ls /app
myfile.txt

# Container 2 cannot see it
$ ls /app
# Empty or different files

# On host
$ ls /var/lib/docker/overlay2/container1-root/app/
myfile.txt  # Actually stored here
```

**Why This Matters**:
- Each container has isolated filesystem
- Can use different OS file structures
- Secure: containers can't access each other's files
- Clean: container deletion removes its filesystem

---

#### 4. UTS Namespace (Hostname Isolation)

**Purpose**: Isolates hostname and domain name

**How It Works**:

```
Host:
$ hostname
server-01.example.com

Container 1:
$ hostname
webapp-container

Container 2:
$ hostname
database-container
```

**Why This Matters**:
- Each container can have its own hostname
- Applications see expected hostname
- Useful for configuration management
- Logging shows correct container identity

**Practical Example**:
```bash
# Start container with custom hostname
$ docker run --hostname myapp ubuntu bash
root@myapp:/# hostname
myapp

# From another terminal, different container
$ docker run --hostname mydb ubuntu bash
root@mydb:/# hostname
mydb
```

---

#### 5. IPC Namespace (Inter-Process Communication Isolation)

**Purpose**: Isolates System V IPC, POSIX message queues

**IPC Mechanisms Isolated**:
- **Shared Memory Segments**: Memory accessible by multiple processes
- **Semaphores**: Synchronization primitives
- **Message Queues**: Async message passing

**How It Works**:

```
Container 1:
- Shared Memory ID 1234 → Segment A
- Message Queue ID 5678 → Queue A
- Semaphore Set 9012 → Semaphore A

Container 2:
- Shared Memory ID 1234 → Segment B (Different from A!)
- Message Queue ID 5678 → Queue B (Different from A!)
- Semaphore Set 9012 → Semaphore B (Different from A!)
```

**Why This Matters**:
- Prevents IPC conflicts between containers
- Security: containers can't communicate via shared memory
- Each container has clean IPC namespace

**Practical Example**:
```bash
# Container 1
$ ipcmk -M 1000  # Create shared memory
Shared memory id: 0

$ ipcs -m
------ Shared Memory Segments --------
key        shmid   bytes
0x00000000 0       1000

# Container 2
$ ipcs -m
------ Shared Memory Segments --------
# Empty! Can't see Container 1's shared memory
```

---

#### 6. User Namespace (User ID Isolation)

**Purpose**: Isolates user and group IDs

**How It Works**:

```
Host System:
- User ID 1000: john
- User ID 2000: jane

Container:
- User ID 0 (root inside container) → Maps to 100000 on host
- User ID 1 (inside container) → Maps to 100001 on host
- User ID 2 (inside container) → Maps to 100002 on host
```

**Key Feature**: A process can be root (UID 0) inside container but unprivileged user outside!

**Why This Matters**:
- Security: "root" in container isn't real root
- Prevents container breakout escalation
- Different permission sets inside vs outside

**Practical Example**:
```bash
# Inside container
$ whoami
root
$ id
uid=0(root) gid=0(root)

# From host, looking at same process
$ ps aux | grep container_process
100000   12345  ... container_process
# Process runs as UID 100000 on host!
```

---

#### 7. Cgroup Namespace

**Purpose**: Isolates view of cgroup hierarchy

**How It Works**:
- Each container sees its own cgroup as root
- Prevents containers from seeing host's full cgroup tree
- Adds security layer

---

#### 8. Time Namespace

**Purpose**: Isolates system time (newer feature, Linux 5.6+)

**How It Works**:
- Containers can have different boot times
- Useful for testing time-dependent applications
- Can test different timezones

**Practical Example**:
```bash
# Container 1 sees one time
$ date
Mon Jan 1 12:00:00 UTC 2024

# Container 2 could see different time
$ date
Wed Jun 15 08:30:00 UTC 2025
```

---

### Namespace Nesting

Namespaces can be nested (parent-child relationships):

```
Host Namespace (parent)
├── Container 1 Namespace
│   └── Nested Container 1.1 Namespace
└── Container 2 Namespace
```

**User Namespace** specifically supports nesting:
- Every user namespace has a parent (except initial)
- Can have zero or more children
- Used for nested containers (Docker in Docker)

---

### How to Work with Namespaces

**Creating Namespaces**:

```bash
# Clone process into new namespace
unshare --pid --fork --mount-proc bash
# Now in new PID namespace with PID 1

# Create network namespace
ip netns add mynetns

# Execute in namespace
ip netns exec mynetns bash
```

**Viewing Namespaces**:

```bash
# List all namespaces
lsns

# View process's namespaces
ls -la /proc/$$/ns/
lrwxrwxrwx 1 root root 0 mnt -> 'mnt:[4026531840]'
lrwxrwxrwx 1 root root 0 net -> 'net:[4026531905]'
lrwxrwxrwx 1 root root 0 pid -> 'pid:[4026531836]'
lrwxrwxrwx 1 root root 0 user -> 'user:[4026531837]'
```

**Entering Namespaces**:

```bash
# Enter namespace of running process
nsenter --target $PID --mount --uts --ipc --net --pid
```

---

## Control Groups (cgroups)

While **namespaces** control what a process can **see**, **cgroups** control what resources a process can **use**.

### What Are Cgroups?

**Control Groups (cgroups)** are a Linux kernel feature that limits, accounts for, and isolates resource usage (CPU, memory, disk I/O, network) of process groups.

### Why Cgroups Are Needed

Without cgroups:
```
Container 1: CPU-intensive crypto mining
├── Uses 100% CPU
├── Uses all available memory
└── Starves other containers

Container 2: Web application
└── Cannot get resources, becomes unresponsive
```

With cgroups:
```
Container 1: Limited by cgroups
├── Maximum 25% CPU
├── Maximum 1 GB memory
└── Cannot exceed limits

Container 2: Guaranteed resources
├── Guaranteed 25% CPU
├── Guaranteed 512 MB memory
└── Runs smoothly
```

### Cgroup Subsystems (Controllers)

**1. CPU Controller**:
```
cpu.shares: Relative CPU weight
├── Container 1: 1024 shares (2× priority)
└── Container 2: 512 shares (1× priority)

cpu.cfs_period_us: Period length (default 100ms)
cpu.cfs_quota_us: Time allocation per period
├── Example: quota=50000, period=100000
└── Container gets 50% of one CPU core
```

**2. Memory Controller**:
```
memory.limit_in_bytes: Hard limit
├── Container 1: 2 GB
└── Container 2: 512 MB

memory.soft_limit_in_bytes: Soft limit (reclaimed under pressure)
memory.oom_control: Out-of-memory killer behavior
```

**3. Block I/O Controller**:
```
blkio.weight: I/O priority (100-1000)
blkio.throttle.read_bps_device: Read bytes/sec limit
blkio.throttle.write_bps_device: Write bytes/sec limit

Example:
Device 8:0 (sda)
├── Read: 10 MB/s max
└── Write: 5 MB/s max
```

**4. Network Controller** (tc + cgroups):
```
Bandwidth limits
├── Upload: 10 Mbps
└── Download: 50 Mbps
```

**5. Device Controller**:
```
Controls access to devices
├── Allow: /dev/null, /dev/zero
├── Deny: /dev/sda (raw disk)
└── Prevents container from accessing host devices
```

### Cgroup Hierarchies

**Cgroups v1** (Legacy):
```
/sys/fs/cgroup/
├── cpu/
│   ├── docker/
│   │   ├── container1/
│   │   └── container2/
├── memory/
│   ├── docker/
│   │   ├── container1/
│   │   └── container2/
└── blkio/
    ├── docker/
        ├── container1/
        └── container2/
```
Different hierarchies for each controller.

**Cgroups v2** (Modern):
```
/sys/fs/cgroup/
├── system.slice/
├── user.slice/
└── docker/
    ├── container1/
    │   ├── cpu.max
    │   ├── memory.max
    │   └── io.max
    └── container2/
        ├── cpu.max
        ├── memory.max
        └── io.max
```
Unified hierarchy for all controllers.

### Practical Example

**Creating a cgroup and limiting resources**:

```bash
# Create cgroup
mkdir /sys/fs/cgroup/mygroup

# Limit to 50% of 1 CPU
echo 50000 > /sys/fs/cgroup/mygroup/cpu.cfs_quota_us

# Limit to 512 MB memory
echo 536870912 > /sys/fs/cgroup/mygroup/memory.limit_in_bytes

# Add process to cgroup
echo $PID > /sys/fs/cgroup/mygroup/cgroup.procs

# Now process is limited!
```

**Docker example**:
```bash
# Run container with resource limits
docker run -d \
  --cpus=0.5 \          # 50% of one CPU
  --memory=512m \        # 512 MB RAM
  --memory-swap=1g \     # 1 GB swap
  --blkio-weight=500 \   # I/O priority
  nginx
```

---

## Container Images

### What Is a Container Image?

A **container image** is a lightweight, standalone, executable package that includes everything needed to run software:
- Application code
- Runtime
- Libraries
- Dependencies
- Configuration files

Think of it as a **template** or **blueprint** for creating containers.

```
Image (Read-Only Template)
├── Base OS layer (Ubuntu filesystem)
├── Runtime layer (Python 3.9)
├── Dependencies layer (pip packages)
├── Application layer (your code)
└── Configuration layer (settings)

Container (Running Instance)
├── Thin writable layer (changes)
└── Image layers (read-only)
```

### Layer-Based Architecture

Images are built in **layers**, like a stack of transparent sheets:

```
Layer 5: ENTRYPOINT ["python", "app.py"]     (1 KB)
Layer 4: COPY app.py /app/                    (10 KB)
Layer 3: RUN pip install -r requirements.txt  (50 MB)
Layer 2: COPY requirements.txt /app/          (1 KB)
Layer 1: FROM python:3.9                      (900 MB)

Total Image Size: ~960 MB
```

**Key Points**:
- Each layer is read-only
- Layers are cached and reused
- Multiple images can share layers
- Only changes create new layers

**Benefits**:
```
Image A:                  Image B:
├── Python 3.9 (900 MB)   ├── Python 3.9 (900 MB) ← Same layer!
├── Deps A (50 MB)        ├── Deps B (30 MB)
└── App A (10 MB)         └── App B (15 MB)

Disk usage: 900 + 50 + 10 + 30 + 15 = 1005 MB
(Not 1905 MB, because Python layer is shared!)
```

### Dockerfile - Building Images

**Dockerfile** is a text file with instructions for building an image:

```dockerfile
# Start from base image
FROM ubuntu:22.04

# Set working directory
WORKDIR /app

# Install dependencies
RUN apt-get update && \
    apt-get install -y python3 python3-pip

# Copy requirements
COPY requirements.txt .

# Install Python packages
RUN pip3 install -r requirements.txt

# Copy application code
COPY . .

# Expose port
EXPOSE 8000

# Define startup command
CMD ["python3", "app.py"]
```

**Build process**:
```bash
$ docker build -t myapp:1.0 .

Step 1/8 : FROM ubuntu:22.04
 ---> Pulling base image
Step 2/8 : WORKDIR /app
 ---> Creating layer 2
Step 3/8 : RUN apt-get update...
 ---> Creating layer 3
...
Successfully built myapp:1.0
```

### Image Registries

**Container Registry** is like GitHub for container images:

```
Docker Hub (Public Registry)
├── Official Images
│   ├── ubuntu:22.04
│   ├── nginx:latest
│   └── python:3.9
└── User Images
    └── myusername/myapp:1.0

Private Registry
└── company.registry.com/
    ├── production/webapp:2.3.4
    └── staging/api:latest
```

**Common Operations**:
```bash
# Pull image
docker pull nginx:latest

# Tag image
docker tag myapp:1.0 myregistry.com/myapp:1.0

# Push image
docker push myregistry.com/myapp:1.0
```

---

## Container Runtime

### What Is a Container Runtime?

The **container runtime** is the software responsible for running containers. It:
1. Pulls images from registry
2. Creates namespaces and cgroups
3. Mounts filesystem layers
4. Starts container process
5. Manages container lifecycle

### Container Runtime Layers

```
┌──────────────────────────────────────┐
│  High-Level Runtime (User Interface) │
│         Docker, Podman, etc.         │
├──────────────────────────────────────┤
│  Container Runtime Interface (CRI)   │
│         containerd, CRI-O            │
├──────────────────────────────────────┤
│   Low-Level Runtime (OCI Runtime)    │
│         runc, crun, kata             │
├──────────────────────────────────────┤
│        Linux Kernel                  │
│    (namespaces, cgroups, etc.)       │
└──────────────────────────────────────┘
```

**runc** (Low-level runtime):
- OCI (Open Container Initiative) compliant
- Creates and runs containers
- Manages namespaces and cgroups
- Written in Go

**containerd** (Mid-level runtime):
- Manages container lifecycle
- Image pulling and storage
- Network management
- Used by Docker, Kubernetes

**Docker** (High-level platform):
- User-friendly CLI
- Build images
- Manage networks and volumes
- Docker Compose for multi-container apps

### Container Lifecycle

```
1. docker run nginx
   ↓
2. Pull image (if not present)
   ↓
3. Create container
   ├── Create namespaces (PID, NET, MNT, etc.)
   ├── Create cgroups
   ├── Set resource limits
   └── Prepare filesystem
   ↓
4. Start container
   ├── Execute container command
   └── Container process runs
   ↓
5. Container Running
   ↓
6. docker stop (or container exits)
   ├── Send SIGTERM
   ├── Wait grace period (10s default)
   └── Send SIGKILL if needed
   ↓
7. Container Stopped
   ├── Namespaces removed
   ├── Cgroups removed
   └── Filesystem remains (can restart)
   ↓
8. docker rm (optional)
   └── Remove container and filesystem
```

---

## Containers vs VMs

### Side-by-Side Comparison

```
Virtual Machine:                    Container:
┌─────────────────────┐            ┌──────────────┐
│   App   │   App     │            │ App │  App   │
│  Bins   │  Bins     │            │Bins │ Bins   │
│  Libs   │  Libs     │            │Libs │ Libs   │
│  Guest  │  Guest    │            ├──────┴────────┤
│   OS    │   OS      │            │ Container Eng │
├─────────┴───────────┤            ├───────────────┤
│    Hypervisor       │            │   Host OS     │
├─────────────────────┤            ├───────────────┤
│  Host OS (Type 2)   │            │  Hardware     │
├─────────────────────┤            └───────────────┘
│     Hardware        │
└─────────────────────┘
```

### Detailed Comparison

| Aspect | Virtual Machines | Containers |
|--------|-----------------|------------|
| **Abstraction Level** | Hardware | OS (process) |
| **Isolation** | Strong (separate kernel) | Good (shared kernel) |
| **Startup Time** | Minutes | Milliseconds |
| **Size** | Gigabytes | Megabytes |
| **Performance** | Slower (virtualization overhead) | Near-native |
| **Resource Efficiency** | Lower (full OS per VM) | Higher (shared OS) |
| **Density** | 10-50 per host | 100-1000s per host |
| **OS Compatibility** | Can run different OS | Must match host kernel |
| **Portability** | Less portable | Highly portable |
| **Security** | Stronger isolation | Improving (still shared kernel) |
| **Use Case** | Different OS, legacy apps | Microservices, cloud-native |

### When to Use Each

**Use VMs when**:
- Need to run different operating systems (Windows + Linux)
- Require strong isolation (multi-tenant SaaS)
- Running legacy applications
- Need full OS control
- Security is paramount

**Use Containers when**:
- Building microservices
- CI/CD pipelines
- Cloud-native applications
- Need rapid scaling
- Want lightweight deployment

**Use Both** (Hybrid):
```
Physical Server
├── VM 1 (Ubuntu)
│   ├── Container 1 (Web App)
│   ├── Container 2 (API)
│   └── Container 3 (Cache)
├── VM 2 (Windows)
│   └── Legacy .NET App
└── VM 3 (Ubuntu)
    └── Kubernetes cluster (many containers)
```

---

## Container Networking

### Network Modes

**1. Bridge Network** (Default):
```
┌────────────────────────────────────┐
│  Container 1  │  Container 2       │
│  172.17.0.2   │  172.17.0.3        │
└───────┬───────┴────────┬───────────┘
        │                │
    ┌───┴────────────────┴───┐
    │   Docker Bridge        │
    │   docker0: 172.17.0.1  │
    └───────────┬────────────┘
                │
        ┌───────┴─────────┐
        │   Host: eth0    │
        │   192.168.1.100 │
        └─────────────────┘
```

- Containers get private IPs
- Communicate with each other via bridge
- NAT to external network
- Port mapping for external access

**2. Host Network**:
```
Container uses host's network stack directly
├── Same IP as host
├── Same ports as host
├── No network isolation
└── Best performance (no bridge overhead)
```

**3. None Network**:
```
Container has no network
└── Complete network isolation
```

**4. Custom Bridge Network**:
```
docker network create my-network

┌────────────────────────────────────┐
│  Container 1  │  Container 2       │
│  Can use name │  Can use name      │
│  "container2" │  "container1"      │
└───────┬───────┴────────┬───────────┘
        │                │
    ┌───┴────────────────┴───┐
    │   my-network bridge    │
    └────────────────────────┘
```

- Built-in DNS resolution
- Better isolation
- Custom IP ranges

### Port Mapping

```bash
# Map container port 80 to host port 8080
docker run -p 8080:80 nginx

Host:
└── Port 8080 → Container Port 80

Access:
http://host-ip:8080 → reaches nginx on port 80 inside container
```

---

## Container Storage

### Storage Types

**1. Container Layer (Ephemeral)**:
```
Container Running:
├── Writable Layer (changes)
└── Image Layers (read-only)

Container Deleted:
└── Writable Layer LOST!
```

**2. Volumes (Persistent)**:
```
Host:
/var/lib/docker/volumes/mydata/
└── _data/
    └── database files

Container:
/var/lib/mysql/ → Mounted from volume
└── Changes persist even if container deleted
```

**3. Bind Mounts**:
```
Host:
/home/user/project/

Container:
/app/ → Directly mounted from host path
└── Changes visible on both sides immediately
```

**4. tmpfs Mounts** (Memory only):
```
Container:
/tmp/ → Stored in RAM
└── Very fast, but lost on container stop
```

### Volume Examples

```bash
# Create volume
docker volume create mydata

# Run with volume
docker run -v mydata:/var/lib/mysql mysql

# Run with bind mount
docker run -v /host/path:/container/path nginx

# Run with tmpfs
docker run --tmpfs /tmp:rw,size=1g nginx
```

---

## Container Orchestration

### Why Orchestration?

Managing containers at scale is complex:

```
Single Container: Easy
docker run nginx

100 Containers: Manual nightmare
├── Which host has capacity?
├── How to load balance?
├── What if a container crashes?
├── How to update without downtime?
└── How to scale up/down?
```

### What Is Orchestration?

**Container Orchestration** automates:
- Container deployment
- Scaling (up/down)
- Load balancing
- Health monitoring
- Auto-healing (restart failed containers)
- Rolling updates
- Service discovery

### Popular Orchestrators

**Kubernetes** (Most popular):
```
Master Nodes:
├── API Server
├── Scheduler (decides where to run pods)
├── Controller Manager (maintains desired state)
└── etcd (cluster database)

Worker Nodes:
├── kubelet (node agent)
├── Container Runtime
└── kube-proxy (networking)

Pods (group of containers):
└── Run on worker nodes
```

**Docker Swarm**:
```
Simpler than Kubernetes
Built into Docker
Good for smaller deployments
```

**Nomad, Amazon ECS, etc.**

---

## Summary

**Containers** provide lightweight, fast, portable application packaging by:
- Using **namespaces** for isolation (what processes can see)
- Using **cgroups** for resource limits (what processes can use)
- Sharing the host kernel (no full OS duplication)
- Starting in milliseconds
- Using minimal resources

**Linux Namespaces** isolate:
- **PID**: Process IDs
- **Network**: Network interfaces and IPs
- **Mount**: Filesystem mounts
- **UTS**: Hostname
- **IPC**: Inter-process communication
- **User**: User and group IDs
- **Time**: System time

**Containers revolutionized** application deployment:
- From "works on my machine" to "works everywhere"
- From monoliths to microservices
- From slow deployments to rapid CI/CD
- From low density to high density

**The container ecosystem** includes:
- Container runtimes (Docker, Podman)
- Orchestrators (Kubernetes, Swarm)
- Registries (Docker Hub, Harbor)
- Build tools (Docker Build, Buildah)

Containers are now the foundation of modern cloud-native applications and DevOps practices.

---

# Virtual Networking

## Table of Contents
1. [Understanding Physical Networking First](#understanding-physical-networking-first)
2. [The Need for Virtual Networking](#the-need-for-virtual-networking)
3. [Virtual Network Interface Cards (vNICs)](#virtual-network-interface-cards-vnics)
4. [Virtual Switches](#virtual-switches)
5. [VLANs and Virtual Networks](#vlans-and-virtual-networks)
6. [Network Virtualization Overlays](#network-virtualization-overlays)
7. [Virtual Routers and Gateways](#virtual-routers-and-gateways)
8. [Software-Defined Networking (SDN)](#software-defined-networking-sdn)
9. [Container Networking](#container-networking)
10. [Network Performance](#network-performance)

---

## Understanding Physical Networking First

Before diving into virtual networking, let's establish the physical networking basics.

### Basic Physical Network

```
Computer A                    Computer B
├── Application               ├── Application
├── OS Network Stack          ├── OS Network Stack
└── NIC (192.168.1.10)       └── NIC (192.168.1.20)
         │                            │
         └────────┬───────────────────┘
                  │
           Physical Switch
           (Layer 2 Device)
                  │
              Router/Gateway
              (192.168.1.1)
                  │
              Internet
```

**Network Interface Card (NIC)**:
- Physical hardware in computer
- Has unique MAC address (e.g., 00:1A:2B:3C:4D:5E)
- Converts data to electrical/optical signals
- Connects via Ethernet cable or WiFi

**Switch**:
- Connects multiple devices in a network
- Forwards frames based on MAC addresses
- Operates at Layer 2 (Data Link)
- Learns which MAC is on which port

**Router**:
- Connects different networks
- Routes packets based on IP addresses
- Operates at Layer 3 (Network)
- Performs NAT, firewalling

### The OSI Model (Quick Reference)

```
Layer 7: Application (HTTP, FTP, DNS)
Layer 6: Presentation (Encryption, Compression)
Layer 5: Session (Session management)
Layer 4: Transport (TCP, UDP) - Ports
Layer 3: Network (IP) - IP addresses
Layer 2: Data Link (Ethernet, WiFi) - MAC addresses
Layer 1: Physical (Cables, Radio waves)
```

---

## The Need for Virtual Networking

### Problems with Physical Networking in Virtualized Environments

**Problem 1: Limited Physical Ports**
```
Physical Server with 2 NICs:
├── Running 20 VMs
└── Each VM needs network connectivity

Question: How do 20 VMs share 2 physical NICs?
```

**Problem 2: Isolation**
```
VM1 (Production):  Needs secure network
VM2 (Development): Needs separate network
VM3 (DMZ):         Needs public-facing network

Question: How to provide isolated networks without multiple physical NICs?
```

**Problem 3: Flexibility**
```
Physical Setup:
└── Recabling, switch configuration, physical changes

Virtual Setup (Desired):
└── Software configuration, instant changes, no physical work
```

**Problem 4: Mobility**
```
VM migrates from Host A to Host B
├── IP address should stay same
├── Network connectivity should continue
└── No physical network changes should be needed
```

### Virtual Networking Solution

Virtual networking creates software-based network infrastructure:

```
┌──────────────┬──────────────┬──────────────┐
│    VM1       │     VM2      │     VM3      │
│   (vNIC)     │   (vNIC)     │   (vNIC)     │
└──────┬───────┴──────┬───────┴──────┬───────┘
       └──────────────┼──────────────┘
               Virtual Switch
                      │
              ┌───────┴────────┐
              │  Physical NIC  │
              └────────────────┘
```

**Benefits**:
- Multiple virtual NICs share physical NIC
- Software-based switching and routing
- Network isolation via virtual networks
- Rapid reconfiguration
- VM mobility across hosts

---

## Virtual Network Interface Cards (vNICs)

### What Is a vNIC?

A **virtual NIC** is a software emulation of a physical network card.

**From VM's Perspective**:
```
$ ip link show
eth0: <BROADCAST,MULTICAST,UP,LOWER_UP>
    link/ether 52:54:00:12:34:56
    inet 192.168.1.100/24

$ ethtool eth0
Settings for eth0:
    Supported link modes: 1000baseT/Full
    Speed: 1000Mb/s
```

VM sees a "real" network card with:
- MAC address
- IP configuration
- Link speed
- Driver interface

**Reality Behind the Scenes**:
```
VM thinks: Real network hardware
Actually: Software emulation/paravirtualization
    ↓
Hypervisor intercepts network I/O
    ↓
Passes to virtual switch
    ↓
Eventually reaches physical NIC
```

### vNIC Types

**1. Emulated vNIC** (Full Virtualization):

```
Examples: Intel E1000, Realtek RTL8139

VM Guest:
├── Uses standard network driver (e.g., e1000 driver)
└── Thinks it's talking to real Intel NIC

Hypervisor:
├── Emulates hardware behavior
├── Intercepts I/O operations
├── Translates to actual network operations
└── Higher overhead (emulation cost)

Compatibility: Excellent (works with any OS)
Performance: Good (but not best)
```

**2. Paravirtualized vNIC**:

```
Examples: VirtIO-net (KVM), VMXNET3 (VMware)

VM Guest:
├── Uses special paravirtualized driver
└── Knows it's virtualized

Hypervisor:
├── Optimized communication interface
├── No hardware emulation overhead
├── Direct hypercall to hypervisor
└── Lower overhead

Compatibility: Requires guest support
Performance: Excellent
```

**3. SR-IOV Virtual Function** (Hardware-Assisted):

```
Physical NIC creates virtual functions
├── VF1 → Directly assigned to VM1
├── VF2 → Directly assigned to VM2
└── VF3 → Directly assigned to VM3

VM Guest:
└── Direct hardware access (no hypervisor intercept)

Performance: Near-native (best)
Compatibility: Requires SR-IOV support
(Covered in detail in SR-IOV section)
```

### vNIC Configuration

**Single vNIC** (Most common):
```
VM:
└── eth0 (vNIC) → Virtual Switch → Physical NIC
```

**Multiple vNICs** (Advanced scenarios):
```
VM:
├── eth0 (vNIC) → Virtual Switch 1 → Physical NIC 1 (Management)
├── eth1 (vNIC) → Virtual Switch 2 → Physical NIC 2 (Data)
└── eth2 (vNIC) → Virtual Switch 3 → Physical NIC 3 (Storage)

Use Cases:
- Network segmentation
- Traffic separation
- Performance isolation
- Security requirements
```

### MAC Address Assignment

**Static MAC**:
```
Manually assigned: 52:54:00:12:34:56
- Consistent across VM lifecycle
- Used when MAC must not change
```

**Dynamic MAC**:
```
Auto-generated by hypervisor
- Changes if VM recreated
- Usually fine for most scenarios
```

**Important**: 
- MAC must be unique in Layer 2 network
- Conflicts cause network issues
- Hypervisors handle this automatically

---

## Virtual Switches

### What Is a Virtual Switch?

A **virtual switch** (vSwitch) is a software program that operates like a physical network switch but runs in the hypervisor.

**Purpose**:
- Connect vNICs from VMs
- Forward frames based on MAC addresses
- Connect to physical NICs (uplinks)
- Provide network services (VLAN, QoS, etc.)

### Basic Virtual Switch Architecture

```
┌─────────┬─────────┬─────────┬─────────┐
│  VM1    │  VM2    │  VM3    │  VM4    │
│  vNIC   │  vNIC   │  vNIC   │  vNIC   │
│  MAC:01 │  MAC:02 │  MAC:03 │  MAC:04 │
└────┬────┴────┬────┴────┬────┴────┬────┘
     │         │         │         │
     └─────────┼─────────┼─────────┘
          Virtual Switch
          ┌─────────────────────┐
          │  Port 1 → VM1       │
          │  Port 2 → VM2       │
          │  Port 3 → VM3       │
          │  Port 4 → VM4       │
          │  Port 5 → Uplink    │
          └─────────┬───────────┘
                    │
            Physical NIC (Uplink)
                    │
            External Network
```

### How Virtual Switch Works

**Frame Forwarding**:

```
Example: VM1 sends packet to VM2

Step 1: VM1 sends frame
├── Source MAC: 00:11:22:33:44:01
├── Dest MAC: 00:11:22:33:44:02
└── Data: "Hello VM2"

Step 2: vSwitch receives frame on Port 1

Step 3: vSwitch checks MAC table
MAC Address         Port
00:11:22:33:44:01   Port 1 (VM1)
00:11:22:33:44:02   Port 2 (VM2)
00:11:22:33:44:03   Port 3 (VM3)

Step 4: Forward to Port 2 only (unicast)

Step 5: VM2 receives frame
```

**MAC Learning**:
```
vSwitch builds MAC address table automatically:

VM1 sends frame:
└── vSwitch learns: MAC 00:11:22:33:44:01 is on Port 1

VM2 sends frame:
└── vSwitch learns: MAC 00:11:22:33:44:02 is on Port 2

Unknown destination:
└── Flood to all ports (like physical switch)
```

### Virtual Switch Types

**1. Standard vSwitch** (VMware):
```
Hypervisor Local:
├── Configured per host
├── Simple management
└── No centralized control

Features:
├── VLAN support
├── Basic port groups
├── Traffic shaping
└── NIC teaming
```

**2. Distributed vSwitch** (VMware vDS):
```
Cluster-Wide:
├── Configured centrally
├── Consistent across hosts
└── Advanced features

Features:
├── All standard features
├── Network I/O control
├── Port mirroring
├── NetFlow
├── LACP
└── Private VLANs
```

**3. Open vSwitch** (OVS):
```
Open Source, Feature-Rich:
├── Widely used in cloud (OpenStack)
├── Supports advanced networking
└── Programmable

Features:
├── VLAN, VXLAN, GRE tunneling
├── OpenFlow support
├── QoS
├── sFlow/NetFlow
├── Port bonding
└── Active/active HA
```

**4. Linux Bridge**:
```
Simple, Built-in:
├── Part of Linux kernel
├── Basic switching
└── Used in KVM/QEMU

Features:
├── Basic frame forwarding
├── VLAN support
├── Port forwarding
└── Lightweight
```

### Virtual Switch Features

**VLAN Support**:
```
Single Physical Network:
├── VLAN 10 (Production)
├── VLAN 20 (Development)
└── VLAN 30 (DMZ)

Virtual Switch:
├── Port Group 1: VLAN 10 → Production VMs
├── Port Group 2: VLAN 20 → Dev VMs
└── Port Group 3: VLAN 30 → DMZ VMs

Result: Isolated networks without separate physical infrastructure
```

**Traffic Shaping**:
```
QoS Policies:
├── VM1: 1 Gbps guaranteed, 2 Gbps peak
├── VM2: 500 Mbps guaranteed, 1 Gbps peak
└── VM3: Best effort

vSwitch enforces limits:
└── Prevents one VM from saturating network
```

**NIC Teaming/Bonding**:
```
vSwitch → Multiple Physical NICs:
├── Active-Active: Load balancing
├── Active-Passive: Failover only
└── LACP: Link Aggregation

Benefits:
├── Redundancy (failover)
├── Increased bandwidth
└── Load distribution
```

**Security Features**:
```
Promiscuous Mode: Disabled by default
├── Prevents VMs from sniffing others' traffic

MAC Address Changes: Restricted
├── Prevents MAC spoofing

Forged Transmits: Blocked
├── Prevents source MAC spoofing
```

### Virtual Switch Uplinks

**Single Uplink**:
```
vSwitch → 1 Physical NIC
├── Single point of failure
└── Bandwidth limited to NIC speed
```

**Multiple Uplinks** (Recommended):
```
vSwitch → 2+ Physical NICs
├── Redundancy
├── Aggregated bandwidth
└── Load balancing

Teaming Modes:
├── Route based on originating port ID
├── Route based on source MAC hash
├── Route based on IP hash
├── LACP (Link Aggregation Control Protocol)
└── Explicit failover order
```

---

## VLANs and Virtual Networks

### VLAN Basics

**VLAN (Virtual LAN)** segments a physical network into multiple logical networks.

**Without VLANs**:
```
One physical switch:
└── All devices in same broadcast domain
    └── Security and performance issues
```

**With VLANs**:
```
One physical switch with VLANs:
├── VLAN 10: Engineering (isolated)
├── VLAN 20: HR (isolated)
└── VLAN 30: Guest (isolated)

Each VLAN = Separate broadcast domain
```

### VLAN Tagging (802.1Q)

**VLAN Tag** added to Ethernet frame:

```
Standard Ethernet Frame:
[Destination MAC][Source MAC][Type][Data][CRC]

802.1Q Tagged Frame:
[Dest MAC][Source MAC][802.1Q Tag][Type][Data][CRC]
                        └─ 4 bytes
                           ├─ TPID (0x8100)
                           └─ VLAN ID (12 bits, 1-4094)
```

**Trunk vs Access Ports**:

```
Access Port (VM sees untagged):
├── Configured for single VLAN
├── Tags added/removed by switch
└── VM unaware of VLANs

Trunk Port (Tags preserved):
├── Carries multiple VLANs
├── Tags remain in frame
└── Used between switches
```

### Virtual Networks in Hypervisors

**Port Groups** (VMware) / **Virtual Networks** (Hyper-V):

```
vSwitch:
├── Port Group "Production" (VLAN 10)
│   ├── VM1 → Gets VLAN 10 tag
│   └── VM2 → Gets VLAN 10 tag
├── Port Group "Development" (VLAN 20)
│   ├── VM3 → Gets VLAN 20 tag
│   └── VM4 → Gets VLAN 20 tag
└── Port Group "Guest" (No VLAN)
    └── VM5 → Untagged
```

**VM Assignment**:
```bash
# VM connects to port group
VM Network Adapter Settings:
├── Network: "Production Port Group"
└── VLAN: 10 (automatic)

Result:
└── All VM traffic tagged with VLAN 10
```

### Private VLANs

**Purpose**: Further segment a VLAN

```
VLAN 10 (Primary):
├── Promiscuous (10-0): Can talk to all
│   └── Gateway/Router
├── Isolated (10-1): Can only talk to promiscuous
│   ├── Web-VM-1 (cannot talk to Web-VM-2)
│   └── Web-VM-2 (cannot talk to Web-VM-1)
└── Community (10-2): Can talk within community + promiscuous
    ├── DB-VM-1 (can talk to DB-VM-2)
    └── DB-VM-2 (can talk to DB-VM-1)

Use Case:
├── Web servers isolated from each other
├── Database servers can cluster
└── All can reach gateway
```

---

## Network Virtualization Overlays

### The Problem with VLANs

**VLAN Limitations**:
- Only 4094 VLANs possible (12-bit VLAN ID)
- Requires physical network VLAN configuration
- Not designed for large-scale multi-tenant clouds

**Cloud Requirements**:
- Thousands of isolated networks (multi-tenant)
- Network mobility across data centers
- No physical network reconfiguration

### Overlay Networks Solution

**Overlay Network** creates virtual networks on top of physical networks.

```
Virtual Network 1 (Overlay):
└── VM1 (10.0.1.10) ↔ VM2 (10.0.1.20)

Virtual Network 2 (Overlay):
└── VM3 (10.0.1.10) ↔ VM4 (10.0.1.20)
    └── Same IP range as Network 1, but isolated!

Both run on:
Physical Network (Underlay):
└── Host1 (192.168.1.10) ↔ Host2 (192.168.1.20)
```

### VXLAN (Virtual Extensible LAN)

**Most popular overlay technology**.

**How VXLAN Works**:

```
VM1 (10.1.1.10) sends packet to VM2 (10.1.1.20)

Step 1: Original packet
├── Source IP: 10.1.1.10
├── Dest IP: 10.1.1.20
└── Data: "Hello"

Step 2: VXLAN encapsulation (by hypervisor)
├── Add VXLAN header (VNI = 5000)
├── Add UDP header (Dest Port 4789)
├── Add outer IP (Host1 → Host2)
└── Add outer Ethernet (Host1 MAC → Host2 MAC)

Step 3: Physical network transmission
[Outer Ethernet][Outer IP][UDP][VXLAN][Original Packet]
                                └─ VNI identifies virtual network

Step 4: Decapsulation (by destination hypervisor)
├── Remove outer headers
├── Check VNI (5000)
├── Deliver to correct VM
└── VM2 receives original packet
```

**VXLAN Benefits**:
- 16 million VNIs (24-bit identifier)
- Works over existing IP networks
- VM mobility across Layer 3 boundaries
- Multi-tenant isolation

**VXLAN Network Identifier (VNI)**:
```
VNI 5000: Tenant A Production
VNI 5001: Tenant A Development
VNI 6000: Tenant B Production
VNI 6001: Tenant B Development

Each VNI = Completely isolated network
```

### Other Overlay Technologies

**GRE (Generic Routing Encapsulation)**:
```
Simpler than VXLAN
├── IP-in-IP encapsulation
├── No tunnel key field (less isolation)
└── Used in some SDN implementations
```

**GENEVE (Generic Network Virtualization Encapsulation)**:
```
Newer, more flexible
├── Variable-length options
├── Protocol-agnostic
└── Designed to replace VXLAN long-term
```

**STT (Stateless Transport Tunneling)**:
```
TCP-like header structure
├── Better with existing network equipment
└── Less commonly used
```

---

## Virtual Routers and Gateways

### Need for Virtual Routers

**Routing Between Virtual Networks**:

```
VLAN 10 (10.1.0.0/24):
├── VM1: 10.1.0.10
└── VM2: 10.1.0.20

VLAN 20 (10.2.0.0/24):
├── VM3: 10.2.0.10
└── VM4: 10.2.0.20

Problem: VM1 cannot reach VM3 (different subnets)

Solution: Virtual Router
```

### Virtual Router Implementations

**1. VM-based Router**:
```
Router VM (pfSense, VyOS, etc.):
├── eth0: VLAN 10 (10.1.0.1)
├── eth1: VLAN 20 (10.2.0.1)
└── Routing software in VM

Pros: Full-featured, flexible
Cons: Uses VM resources, slower than hardware
```

**2. Distributed Virtual Router** (DVR):
```
Each Hypervisor Host runs router instance:
├── Local routing (east-west traffic)
├── No bottleneck through central router
└── Scales horizontally

Example: VMware NSX, OpenStack Neutron DVR
```

**3. Gateway Services**:
```
Edge Gateway:
├── NAT (network address translation)
├── Firewall rules
├── VPN termination
├── Load balancing
└── External network connectivity
```

### NAT in Virtual Networks

**SNAT (Source NAT)**:
```
VM (private IP) → Internet:

VM1 (10.1.0.10) sends packet to Internet (8.8.8.8)
    ↓
Gateway performs SNAT
    ↓
Source IP changed: 203.0.113.1 → 8.8.8.8
    ↓
Internet sees: 203.0.113.1 (public IP)

Return traffic:
    ↓
Gateway receives: 8.8.8.8 → 203.0.113.1
    ↓
Gateway translates back: → 10.1.0.10
    ↓
VM1 receives packet
```

**DNAT (Destination NAT)**:
```
Internet → VM (port forwarding):

External: 203.0.113.1:80 → Internal: 10.1.0.10:8080

Client connects to 203.0.113.1:80
    ↓
Gateway performs DNAT
    ↓
Forwards to 10.1.0.10:8080
    ↓
VM receives connection
```

---

## Software-Defined Networking (SDN)

### Traditional Networking Limitations

**Problem**:
```
Traditional Network:
├── Control Plane (routing decisions) }
└── Data Plane (forwarding packets)  } Tightly coupled in each device

Result:
├── Manual per-device configuration
├── Difficult to automate
├── Vendor lock-in
└── Slow to adapt
```

### SDN Approach

**Separation of Concerns**:

```
┌──────────────────────────────────┐
│   SDN Controller (Brain)         │
│   - Centralized control          │
│   - Network view                 │
│   - Policy decisions             │
└───────────┬──────────────────────┘
            │ Southbound API (OpenFlow)
┌───────────┴──────────────────────┐
│   Data Plane (Muscles)           │
│   - Switches (physical/virtual)  │
│   - Just forward packets         │
│   - Follow controller rules      │
└──────────────────────────────────┘
```

**Benefits**:
- Centralized management
- Programmable networks
- Automation-friendly
- Rapid reconfiguration
- Vendor independence

### SDN Components

**Controller**:
```
Examples: OpenDaylight, ONOS, VMware NSX

Responsibilities:
├── Maintain network topology
├── Compute optimal paths
├── Program flow rules
├── Respond to network events
└── Provide northbound API
```

**Southbound API** (Controller ↔ Devices):
```
OpenFlow (most common):
├── Flow table programming
├── Packet-in/packet-out
├── Statistics collection
└── Network events

Other protocols:
├── NETCONF
├── OVSDB
└── P4
```

**Northbound API** (Applications ↔ Controller):
```
REST API typically:
├── Create virtual network
├── Add security rule
├── Configure load balancer
└── Automation & orchestration
```

### Flow Tables (OpenFlow)

**Flow Table Entry**:
```
Match Fields              Action
─────────────────         ──────────
Ingress Port: 1           Forward to Port 2
Source MAC: XX:XX:XX      
Dest IP: 10.1.0.10       
TCP Port: 80             

When packet matches:
└── Execute action
```

**Example Flow**:
```
Flow 1:
Match: Dest IP = 10.1.0.10, TCP Port = 80
Action: Forward to Port 5

Flow 2:
Match: Source IP = 10.2.0.0/24
Action: Drop (firewall rule)

Flow 3:
Match: Any
Action: Send to controller (default)
```

---

## Container Networking

### Container Network Models

**1. Docker Bridge Network** (Default):
```
┌──────────────────────────────────┐
│  Container 1  │  Container 2     │
│  172.17.0.2   │  172.17.0.3      │
└───────┬───────┴────────┬─────────┘
        │                │
    ┌───┴────────────────┴───┐
    │   docker0 bridge       │
    │   172.17.0.1           │
    └──────────┬─────────────┘
               │
           iptables NAT
               │
        ┌──────┴──────┐
        │  Host eth0  │
        └─────────────┘
```

**2. Host Network**:
```
Container uses host network namespace:
└── Same IP, same ports as host
    └── Best performance, no isolation
```

**3. Macvlan**:
```
Containers get own MAC addresses:
├── Appear as separate devices on network
├── Direct Layer 2 connectivity
└── No NAT overhead

Container 1: 192.168.1.100 (MAC: AA:BB:CC:DD:EE:01)
Container 2: 192.168.1.101 (MAC: AA:BB:CC:DD:EE:02)
```

**4. Overlay Networks** (Multi-host):
```
Host 1                        Host 2
├── Container 1               ├── Container 3
│   (10.0.9.2)               │   (10.0.9.4)
└── Container 2               └── Container 4
    (10.0.9.3)                   (10.0.9.5)

VXLAN Overlay Network (10.0.9.0/24):
└── All containers can communicate
    └── Regardless of host location
```

### Kubernetes Networking

**Requirements** (CNI - Container Network Interface):
- All pods can communicate without NAT
- All nodes can communicate with all pods
- Pod sees its own IP (same as others see it)

**Common CNI Plugins**:

**Flannel**:
```
Simple overlay network:
├── VXLAN backend (most common)
├── Host-gateway backend
└── Easy to set up
```

**Calico**:
```
Layer 3 networking:
├── BGP routing (no overlay overhead)
├── Network policies
└── Better performance
```

**Weave**:
```
Mesh network:
├── Automatic VXLAN setup
├── Encryption support
└── Simple operation
```

**Cilium**:
```
eBPF-based:
├── High performance
├── Advanced network policies
└── Service mesh features
```

---

## Network Performance

### Performance Factors

**Latency**:
```
Physical NIC: ~0.01 ms
Virtual Switch: +0.1-1 ms
Overlay (VXLAN): +0.5-2 ms

Optimization:
├── SR-IOV (bypass vSwitch)
├── DPDK (kernel bypass)
└── Hardware offload
```

**Throughput**:
```
10 Gbps Physical NIC:
├── Emulated vNIC: ~5-7 Gbps
├── Paravirtualized: ~8-9 Gbps
├── SR-IOV: ~9.5 Gbps
└── DPDK: ~9.8 Gbps
```

**CPU Overhead**:
```
Traditional Path:
├── Interrupt per packet
├── Kernel network stack
├── Context switches
└── High CPU usage at 10 Gbps

DPDK/SR-IOV:
├── Polling (no interrupts)
├── Kernel bypass
├── Batching
└── Lower CPU usage
```

### Optimization Techniques

**Jumbo Frames**:
```
Standard: 1500 bytes MTU
Jumbo: 9000 bytes MTU

Benefits:
├── Fewer packets for same data
├── Less CPU overhead
├── Higher throughput
└── Lower latency

Requirement:
└── All devices in path must support
```

**TCP Offload**:
```
TSO (TCP Segmentation Offload):
├── NIC splits large packets
└── Reduces CPU load

LRO (Large Receive Offload):
├── NIC combines small packets
└── Reduces interrupt rate

Checksum Offload:
└── NIC calculates checksums
```

**Multi-Queue**:
```
NIC with multiple queues:
├── Queue 1 → CPU Core 1
├── Queue 2 → CPU Core 2
├── Queue 3 → CPU Core 3
└── Queue 4 → CPU Core 4

RSS (Receive Side Scaling):
└── Distributes incoming traffic across CPUs
```

---

## Summary

**Virtual Networking** provides:
- Multiple isolated networks on shared infrastructure
- Software-defined flexibility and automation
- Scalability beyond physical limitations
- Foundation for cloud computing

**Key Components**:
- **vNICs**: Virtual network cards for VMs/containers
- **Virtual Switches**: Software-based Layer 2 switching
- **VLANs**: Network segmentation
- **Overlays**: Scalable multi-tenant networking (VXLAN)
- **SDN**: Centralized programmable control

**Evolution**:
```
Physical → Virtual → Overlay → SDN

Each layer adds:
├── More flexibility
├── Better automation
├── Greater scale
└── Reduced hardware dependency
```

**Performance** considerations:
- Emulated vs paravirtualized drivers
- SR-IOV for highest performance
- DPDK for kernel bypass
- Proper NIC selection and configuration

Virtual networking is essential for modern data centers and cloud environments, enabling the flexibility and scale required for contemporary applications.

---

# Network Interface Cards (NICs):

## Table of Contents
1. [What is a NIC?](#what-is-a-nic)
2. [NIC Architecture](#nic-architecture)
3. [How NICs Work](#how-nics-work)
4. [NIC Types and Form Factors](#nic-types-and-form-factors)
5. [NIC Features and Offloads](#nic-features-and-offloads)
6. [NIC Performance](#nic-performance)
7. [Multi-Queue NICs](#multi-queue-nics)
8. [Smart NICs and DPUs](#smart-nics-and-dpus)
9. [NIC Selection for Virtualization](#nic-selection-for-virtualization)
10. [NIC Configuration](#nic-configuration)

---

## What is a NIC?

A **Network Interface Card (NIC)**, also called a network adapter or network interface controller, is the hardware component that connects a computer to a network.

### The Purpose

```
Computer Side:
├── CPU generates data
├── Memory stores data
└── NIC needed to send data over network

Network Side:
├── Physical medium (cable, fiber, wireless)
├── Electrical/optical signals
└── NIC converts between digital and physical
```

**Without NIC**:
```
Computer: Has data to send
Network: Cables waiting
Problem: No way to connect them!
```

**With NIC**:
```
Computer → NIC → Network Cable → Switch → Destination
         ↑
    Conversion happens here:
    - Digital data → Electrical signals
    - Framing, addressing, error checking
    - Physical transmission
```

### Physical vs Virtual NICs

**Physical NIC**:
```
Actual hardware card
├── Plugs into PCIe slot (or integrated on motherboard)
├── Has physical ports (RJ45, SFP, etc.)
├── Contains processors, memory, firmware
└── Handles actual signal transmission

Examples:
├── Intel X710 (10 Gbps)
├── Mellanox ConnectX-6 (100 Gbps)
└── Broadcom BCM57xxx (1 Gbps)
```

**Virtual NIC (vNIC)**:
```
Software emulation
├── No physical hardware
├── Presents interface to VM/container
├── Relies on physical NIC for actual transmission
└── Managed by hypervisor/host OS

We covered this in Virtual Networking section.
This guide focuses on physical NICs.
```

---

## NIC Architecture

### Basic NIC Components

```
┌─────────────────────────────────────────┐
│           NIC Card                      │
│                                         │
│  ┌─────────────────────────────────┐   │
│  │  NIC Processor/Controller       │   │
│  │  - Packet processing            │   │
│  │  - DMA engine                   │   │
│  │  - Hardware offloads            │   │
│  └──────────┬──────────────────────┘   │
│             │                           │
│  ┌──────────┴──────────┐               │
│  │   On-board Memory   │               │
│  │   (Buffer, Queues)  │               │
│  └─────────────────────┘               │
│             │                           │
│  ┌──────────┴──────────┐               │
│  │    PHY (Physical    │               │
│  │    Layer Chip)      │               │
│  └──────────┬──────────┘               │
│             │                           │
│          Port(s)                        │
│             ├─ RJ45                     │
│             └─ SFP/SFP+/QSFP           │
└─────────────┼───────────────────────────┘
              │
          Network Cable
```

### Component Details

**1. NIC Controller/Processor**:
```
Main chip that runs the NIC
├── Processes Ethernet frames
├── Implements network protocols
├── Performs hardware offloads
├── Manages DMA transfers
├── Handles interrupts
└── Executes firmware

Example: Intel 82599 controller
```

**2. On-board Memory**:
```
NIC's local RAM/buffers
├── Transmit queues (packets to send)
├── Receive queues (packets received)
├── Descriptor rings (metadata)
└── Statistics counters

Typical size: 32 KB - 1 MB
```

**3. PHY (Physical Layer)**:
```
Converts digital ↔ analog
├── Digital data from controller
├── Electrical signals on wire
├── Clock recovery
├── Signal conditioning
└── Auto-negotiation

Different PHYs for:
├── Copper (1000BASE-T)
├── Fiber (1000BASE-SX/LX)
└── DAC (Direct Attach Copper)
```

**4. PCIe Interface**:
```
Connects NIC to computer
├── PCIe x1, x4, x8, x16 slots
├── Bandwidth:
│   ├── PCIe 3.0 x8: ~64 Gbps
│   ├── PCIe 4.0 x8: ~128 Gbps
│   └── PCIe 5.0 x8: ~256 Gbps
└── DMA for direct memory access

Match PCIe bandwidth to network speed!
```

---

## How NICs Work

### Transmit Path (Sending Data)

```
Step-by-step packet transmission:

1. Application writes data
   └── Example: Web server sends HTTP response

2. Kernel network stack processes
   ├── TCP layer: Segments, sequence numbers
   ├── IP layer: IP header, routing
   ├── Ethernet layer: MAC addresses, frame
   └── Packet ready in kernel memory

3. Network driver prepares transmission
   ├── Allocates transmit descriptor
   ├── Maps packet memory (DMA)
   ├── Writes descriptor to TX queue
   └── Rings doorbell (MMIO write)

4. NIC fetches descriptor via DMA
   ├── Reads descriptor from memory
   ├── Gets packet memory addresses
   └── DMA transfers packet data to NIC

5. NIC processes packet
   ├── Applies hardware offloads:
   │   ├── Checksum calculation
   │   ├── TSO (segmentation)
   │   └── VLAN tagging
   └── Adds Ethernet FCS (frame check)

6. PHY transmits on wire
   ├── Converts to electrical signals
   ├── Adds preamble and SFD
   └── Sends bit by bit

7. NIC updates status
   ├── DMA writes completion
   ├── Optionally raises interrupt
   └── Driver reclaims resources
```

**Detailed Transmit Flow**:

```
CPU Memory:
┌──────────────────────────┐
│  Packet Data (Payload)   │ ◄─────┐
└──────────────────────────┘       │
                                   │ DMA Read
┌──────────────────────────┐       │
│  TX Descriptor Ring      │       │
│  [Desc 0] [Desc 1] ...   │ ──────┤
│  Head ─┐  Tail ──┐       │       │
└────────┼─────────┼───────┘       │
         │         │               │
         └─────────┼───────────────┘
                   │
                   ▼
            NIC Hardware:
         ┌──────────────────┐
         │  TX Queue         │
         │  Packet DMA'd     │
         │  Processing...    │
         └─────────┬─────────┘
                   │
                   ▼
              PHY → Wire
```

### Receive Path (Receiving Data)

```
Step-by-step packet reception:

1. PHY receives signal
   ├── Detects preamble
   ├── Synchronizes clock
   └── Converts to digital

2. NIC validates frame
   ├── Checks FCS (frame check sequence)
   ├── Filters by MAC address
   └── Validates length

3. NIC applies hardware offloads
   ├── Checksum verification
   ├── VLAN tag stripping
   ├── RSS (multi-queue distribution)
   └── Packet classification

4. NIC uses DMA to transfer
   ├── Writes packet to pre-allocated buffer
   ├── Updates receive descriptor
   └── DMA writes directly to memory

5. NIC signals CPU
   ├── Raises interrupt (traditional)
   │   OR
   └── Polling detects packet (NAPI/DPDK)

6. Driver processes packet
   ├── Reads descriptor
   ├── Allocates new receive buffer
   ├── Passes packet to network stack
   └── Refills RX queue

7. Network stack processes
   ├── Ethernet layer → IP layer
   ├── IP layer → TCP layer
   └── TCP layer → Application

8. Application receives data
   └── recv() call returns data
```

**Detailed Receive Flow**:

```
Wire → PHY:
         ┌──────────────────┐
         │  NIC Hardware     │
         │  RX Processing    │
         │  - Filtering      │
         │  - Checksumming   │
         │  - RSS            │
         └─────────┬─────────┘
                   │ DMA Write
                   ▼
CPU Memory:
┌──────────────────────────┐
│  Pre-allocated RX Buffer │ ◄─────┐
│  Packet written here     │       │
└──────────────────────────┘       │
                                   │
┌──────────────────────────┐       │
│  RX Descriptor Ring      │       │
│  [Desc 0] [Desc 1] ...   │ ──────┘
│  Status: Ready/Complete  │
└──────────────────────────┘
         │
         ├─ Interrupt or Poll
         ▼
     Network Stack
         ↓
    Application
```

### Interrupts vs Polling

**Interrupt-Driven (Traditional)**:
```
Packet arrives:
1. NIC raises interrupt
2. CPU stops current work
3. Context switch to interrupt handler
4. Driver processes packet
5. Return from interrupt

Pros:
├── Low latency for low traffic
└── CPU can do other work

Cons:
├── High interrupt rate at high traffic
├── Context switching overhead
└── CPU cache pollution
```

**Polling (NAPI/DPDK)**:
```
CPU continuously polls NIC:
1. Check if packets arrived
2. Process batch of packets
3. Repeat

Pros:
├── No interrupt overhead
├── Better for high traffic
├── Batch processing efficient
└── Predictable latency

Cons:
├── CPU dedicated to polling
└── Wastes CPU if no traffic
```

**Hybrid (NAPI - New API)**:
```
Best of both:
1. First packet → Interrupt
2. Switch to polling mode
3. Process packets in batches
4. If queue empty → Back to interrupts

Result: Efficient at all traffic levels
```

---

## NIC Types and Form Factors

### By Form Factor

**1. PCIe Add-in Card**:
```
┌─────────────────────────┐
│    NIC Card             │
│    ┌─────────────┐      │
│    │ Controller  │      │
│    └─────────────┘      │
│    Ports: ┌─┐ ┌─┐       │
│           │ │ │ │       │
└───────────┴─┴─┴─┴───────┘
            ↑
    PCIe Edge Connector

Slot Types:
├── PCIe x4 (up to 10 Gbps NIC)
├── PCIe x8 (up to 100 Gbps NIC)
└── PCIe x16 (high-end NICs)

Pros:
├── Upgradeable
├── High performance
└── Multiple ports possible

Cons:
├── Requires PCIe slot
└── More expensive
```

**2. Integrated/Onboard NIC**:
```
Built into motherboard
├── Soldered or integrated into chipset
├── Usually 1 Gbps
├── Consumer/workstation systems
└── Cannot be upgraded

Pros:
├── No slot required
├── Lower cost
└── Sufficient for basic use

Cons:
├── Fixed performance
├── Cannot upgrade
└── Usually basic features
```

**3. Mezzanine Card**:
```
Server-specific form factor:
├── LOM (LAN on Motherboard)
├── OCP (Open Compute Project)
├── Mezzanine slot
└── Space-saving in dense servers

Example: OCP NIC 3.0
```

### By Speed

**1 Gigabit Ethernet (1 GbE)**:
```
Common speeds: 10/100/1000 Mbps
Media: 
├── Copper: Cat5e/Cat6 cable, RJ45
└── Fiber: SFP, LC connector

Use cases:
├── Desktop computers
├── Basic servers
└── Office networks

Examples:
├── Intel I350
└── Realtek RTL8111
```

**10 Gigabit Ethernet (10 GbE)**:
```
Speed: 10,000 Mbps
Media:
├── Copper: Cat6a/Cat7, RJ45 (short distance)
├── SFP+: DAC (Direct Attach Copper)
└── SFP+: Fiber (longer distance)

Use cases:
├── Data center servers
├── Storage networks
└── Virtualization hosts

Examples:
├── Intel X710
├── Intel X550
└── Mellanox ConnectX-4
```

**25/40/50 Gigabit Ethernet**:
```
25 GbE: Newer, replacing 10 GbE
40 GbE: QSFP+
50 GbE: SFP56

Use cases:
├── High-performance servers
├── Cloud infrastructure
└── HPC (High Performance Computing)

Examples:
├── Mellanox ConnectX-5 (25/50 GbE)
└── Intel XXV710 (25 GbE)
```

**100 Gigabit Ethernet and Beyond**:
```
100 GbE: QSFP28
200 GbE: QSFP56
400 GbE: QSFP-DD / OSFP

Use cases:
├── Spine switches
├── High-frequency trading
├── AI/ML clusters
└── Large-scale data centers

Examples:
├── Mellanox ConnectX-6 (100/200 GbE)
└── Broadcom Thor (400 GbE)
```

### By Interface Type

**Copper (RJ45)**:
```
Uses twisted-pair cable
├── Cat5e: Up to 1 Gbps
├── Cat6: Up to 10 Gbps (55m)
├── Cat6a: Up to 10 Gbps (100m)
└── Cat7: Up to 10 Gbps (100m+)

Pros:
├── Inexpensive
├── Easy to install
└── Powered devices (PoE)

Cons:
├── Distance limited (~100m)
├── EMI susceptible
└── Higher latency than fiber
```

**Fiber Optic**:
```
Uses light through glass/plastic
├── Multimode (MMF): Shorter distance, LED
└── Singlemode (SMF): Long distance, Laser

Connector types:
├── SFP: 1 Gbps
├── SFP+: 10 Gbps
├── QSFP+: 40 Gbps
├── QSFP28: 100 Gbps
└── QSFP-DD: 400 Gbps

Pros:
├── Long distance (km to 100km+)
├── No EMI
├── Lower latency
└── Higher bandwidth

Cons:
├── More expensive
├── Fragile
└── Requires transceivers
```

**DAC (Direct Attach Copper)**:
```
Copper cable with integrated transceivers
├── Usually 1-7 meters
├── SFP+, QSFP+ formats
└── Lower cost than fiber for short runs

Pros:
├── Cheaper than fiber optics
├── Lower power than optics
└── No separate transceiver needed

Cons:
├── Short distance only
├── Less flexible cable
└── Heavier than fiber
```

---

## NIC Features and Offloads

Modern NICs offload work from the CPU to improve performance.

### Checksum Offload

**Purpose**: Verify data integrity

**Without Offload**:
```
CPU calculates checksums:
├── IP header checksum
├── TCP checksum
└── UDP checksum

Cost: CPU cycles per packet
```

**With Offload**:
```
TX (Transmit):
├── CPU marks packet "needs checksum"
├── NIC calculates and inserts checksum
└── CPU freed from calculation

RX (Receive):
├── NIC verifies checksum
├── Marks packet as good/bad
└── CPU just checks flag
```

**Benefit**: Saves CPU, especially at high packet rates

### TSO/GSO (TCP Segmentation Offload)

**Purpose**: Split large packets into MTU-sized segments

**Without TSO**:
```
Application sends 64 KB data:
├── TCP layer creates 64 KB segment
├── IP layer splits into 44 × 1500-byte packets
├── CPU processes 44 packets
└── Lots of CPU overhead
```

**With TSO**:
```
Application sends 64 KB data:
├── Kernel creates one large "super packet"
├── Passes to NIC with segmentation request
├── NIC splits into 44 packets
└── NIC transmits all 44

CPU work: 1 packet instead of 44!
```

**Benefit**: Massive CPU savings for large transfers

### LRO/GRO (Large Receive Offload)

**Purpose**: Combine small packets into larger ones

**Without LRO**:
```
Receive 100 small TCP packets:
├── 100 interrupts (or poll loops)
├── 100 network stack traversals
└── High CPU overhead
```

**With LRO**:
```
NIC/driver combines related packets:
├── Packets from same TCP stream
├── Combines into fewer larger packets
├── Passes to network stack
└── Fewer interrupts and stack traversals

Receive: 10 large packets instead of 100 small
```

**Benefit**: Reduced interrupt/processing overhead

### RSS (Receive Side Scaling)

**Purpose**: Distribute receive processing across multiple CPUs

**Single Queue**:
```
All packets → Queue 0 → CPU 0
├── CPU 0 overloaded
├── Other CPUs idle
└── Bottleneck at 1 CPU's capacity
```

**RSS with Multiple Queues**:
```
Hash on packet fields (IP, Port):
├── Flow 1 (hash=0) → Queue 0 → CPU 0
├── Flow 2 (hash=1) → Queue 1 → CPU 1
├── Flow 3 (hash=2) → Queue 2 → CPU 2
└── Flow 4 (hash=3) → Queue 3 → CPU 3

Result: Parallel processing, better throughput
```

**How RSS Works**:
```
1. Packet arrives
2. NIC computes hash on:
   ├── Source IP
   ├── Dest IP
   ├── Source Port
   └── Dest Port
3. Hash determines queue: hash % num_queues
4. Packet placed in selected queue
5. CPU assigned to that queue processes it
```

**Benefit**: Scales with CPU cores

### VLAN Offload

**TX VLAN Insertion**:
```
Kernel marks packet with VLAN ID:
├── NIC inserts 802.1Q tag
└── CPU doesn't modify packet
```

**RX VLAN Stripping**:
```
NIC removes VLAN tag:
├── Stores VLAN ID in metadata
└── Passes clean packet to kernel
```

### Flow Director / Flow Steering

**Purpose**: Precise packet-to-queue mapping

**Standard RSS**: Hash-based (random distribution)

**Flow Director**: Rule-based (precise control)

```
Rules:
├── Rule 1: TCP port 80 → Queue 5
├── Rule 2: Source IP 10.1.1.0/24 → Queue 3
├── Rule 3: UDP → Queue 7
└── Default: RSS hash

Use Cases:
├── Pin specific application to specific CPU
├── Isolate traffic types
└── Performance tuning
```

### SR-IOV Support

Allows NIC to present multiple virtual functions:
```
Physical NIC:
├── PF (Physical Function): Management
└── VFs (Virtual Functions): Passthrough to VMs
    ├── VF 0 → VM 1
    ├── VF 1 → VM 2
    └── VF 2 → VM 3

(Detailed in SR-IOV section)
```

### RDMA (Remote Direct Memory Access)

**Purpose**: Zero-copy, kernel-bypass networking

```
Traditional Network:
App → Kernel → NIC → Network → NIC → Kernel → App
    └── Multiple copies, CPU overhead

RDMA:
App → NIC → Network → NIC → App
    └── Direct memory access, no kernel
```

**Protocols**:
- **RoCE** (RDMA over Converged Ethernet)
- **iWARP** (Internet Wide Area RDMA Protocol)
- **InfiniBand**

**Use Cases**:
- High-performance computing
- Low-latency trading
- Storage networks (NVMe over Fabrics)

---

## NIC Performance

### Throughput

**Theoretical vs Actual**:
```
10 Gbps NIC:
├── Line rate: 10,000,000,000 bps
├── Ethernet overhead:
│   ├── Preamble: 8 bytes
│   ├── IFG (Inter-frame gap): 12 bytes
│   ├── Header: 14 bytes
│   └── FCS: 4 bytes
├── For 1500-byte packet:
│   └── Efficiency: 1500 / (1500+38) = 97.5%
└── Actual throughput: ~9.75 Gbps

Small packets worse:
├── 64-byte packet efficiency: 62.7%
└── Actual: ~6.27 Gbps
```

**Packets Per Second (PPS)**:
```
Maximum PPS at 10 Gbps:
├── Min Ethernet frame: 64 bytes
├── With overhead: 84 bytes
├── Time per packet: 84 × 8 / 10,000,000,000
└── Max PPS: ~14.88 million pps

1500-byte packets:
└── Max PPS: ~812,743 pps

NIC must handle both scenarios!
```

### Latency

**Components of Latency**:
```
Total Latency = 
  Transmission delay +
  Propagation delay +
  Processing delay +
  Queuing delay

NIC Processing:
├── Hardware: 1-5 microseconds
├── Driver: 1-10 microseconds
├── Kernel: 5-50 microseconds
└── Total: ~7-65 microseconds

Optimizations:
├── Kernel bypass (DPDK): <1 microsecond
├── SR-IOV: Reduced overhead
└── Hardware timestamping: Precise measurement
```

### CPU Efficiency

**CPU Usage Factors**:
```
High CPU usage causes:
├── Interrupt rate (one per packet)
├── Context switches
├── Software checksumming
├── Segmentation/reassembly
└── Memory copies

Reductions via:
├── Interrupt coalescing
├── NAPI polling
├── Hardware offloads
├── Multi-queue (RSS)
└── Kernel bypass (DPDK)
```

**Interrupt Coalescing**:
```
Without coalescing:
├── 1 interrupt per packet
├── 1 million pps = 1 million interrupts/sec
└── CPU overwhelmed

With coalescing:
├── Wait for multiple packets OR timeout
├── Example: Max 128 packets or 50 microseconds
├── 1 interrupt per batch
└── Interrupts reduced 10-100×
```

---

## Multi-Queue NICs

### Why Multiple Queues?

**Single Queue Limitation**:
```
Single RX Queue:
├── All packets → Queue 0
├── Processed by 1 CPU core
├── Other cores idle
└── Bottleneck: ~3-5 Gbps on 1 core

With 8 cores idle: Wasteful!
```

**Multi-Queue Solution**:
```
8 RX Queues:
├── Queue 0 → CPU 0
├── Queue 1 → CPU 1
├── Queue 2 → CPU 2
├── ...
└── Queue 7 → CPU 7

Result: 8× throughput (if traffic balanced)
```

### Queue Architecture

```
NIC with 8 RX Queues:

┌────────────────────────────────┐
│         NIC Hardware           │
│                                │
│  RX Queue 0  ──→ CPU 0         │
│  RX Queue 1  ──→ CPU 1         │
│  RX Queue 2  ──→ CPU 2         │
│  RX Queue 3  ──→ CPU 3         │
│  RX Queue 4  ──→ CPU 4         │
│  RX Queue 5  ──→ CPU 5         │
│  RX Queue 6  ──→ CPU 6         │
│  RX Queue 7  ──→ CPU 7         │
│                                │
│  TX Queue 0  ←── CPU 0         │
│  TX Queue 1  ←── CPU 1         │
│  ...                           │
└────────────────────────────────┘
```

### Queue Assignment (RSS)

**Hash Function**:
```python
# Simplified RSS hash
def rss_hash(packet):
    hash_input = (
        packet.src_ip +
        packet.dst_ip +
        packet.src_port +
        packet.dst_port
    )
    hash_value = toeplitz_hash(hash_input, secret_key)
    queue_id = hash_value % num_queues
    return queue_id

# Same flow always goes to same queue
# Different flows distributed across queues
```

**Indirection Table**:
```
Hash result (8 bits) → Indirection table (256 entries) → Queue ID

Example:
Hash  Indirection Table  Queue
0-31  →    0     →       Queue 0
32-63 →    1     →       Queue 1
64-95 →    2     →       Queue 2
...

Allows flexible queue mapping
```

### Queue Configuration

```bash
# Check queue count
ethtool -l eth0
Combined:  8
RX:        0
TX:        0

# Set queue count
ethtool -L eth0 combined 16

# Check RSS configuration
ethtool -x eth0

# Set RSS hash fields
ethtool -N eth0 rx-flow-hash tcp4 sdfn
# s=src ip, d=dst ip, f=src port, n=dst port
```

---

## Smart NICs and DPUs

### Evolution of NICs

```
Stage 1: Basic NIC
└── Just sends/receives packets

Stage 2: Offload NIC
└── Checksum, TSO, RSS offloads

Stage 3: Smart NIC
└── Programmable, can run applications

Stage 4: DPU (Data Processing Unit)
└── Full computer on NIC
```

### Smart NIC Architecture

```
┌─────────────────────────────────────┐
│         Smart NIC Card              │
│                                     │
│  ┌─────────────────────────────┐   │
│  │   ARM/x86 CPU Cores         │   │
│  │   (Run Linux, applications)  │   │
│  └──────────────┬──────────────┘   │
│                 │                   │
│  ┌──────────────┴──────────────┐   │
│  │   FPGA / ASIC               │   │
│  │   (Programmable logic)      │   │
│  └──────────────┬──────────────┘   │
│                 │                   │
│  ┌──────────────┴──────────────┐   │
│  │   Network Ports             │   │
│  │   (100 Gbps)                │   │
│  └─────────────────────────────┘   │
│                 │                   │
│         PCIe to Host                │
└─────────────────┼───────────────────┘
                  │
              Host Server
```

### Smart NIC Capabilities

**Offload Functions**:
```
Traditional: CPU does the work
Smart NIC: NIC does the work

Offloadable:
├── Virtual switching (OVS)
├── Firewalling
├── Load balancing
├── Encryption/Decryption
├── Compression
├── DPI (Deep Packet Inspection)
├── Tunnel encap/decap (VXLAN)
└── Storage protocols (NVMe-oF)

Benefit: Free host CPU for applications
```

**Use Cases**:

**1. Virtual Networking**:
```
Without Smart NIC:
├── Host CPU runs virtual switch
├── CPU cycles consumed
└── Lower VM performance

With Smart NIC:
├── Smart NIC runs virtual switch
├── Host CPU free
└── Better VM performance
```

**2. Security**:
```
On Smart NIC:
├── Firewall rules
├── IDS/IPS
├── DDoS protection
└── Traffic inspection

Benefit: Security without host CPU overhead
```

**3. Storage**:
```
NVMe over Fabrics (NVMe-oF):
├── Smart NIC handles storage protocol
├── Direct to storage network
└── Ultra-low latency
```

### DPU (Data Processing Unit)

**What is a DPU?**

```
DPU = Smart NIC + Full Computer

Components:
├── ARM cores (8-16 cores typical)
├── Network ports (100-400 Gbps)
├── Programmable accelerators
├── Security engines
├── Storage controllers
└── Runs full OS (Linux)

Examples:
├── NVIDIA BlueField DPU
├── Intel IPU (Infrastructure Processing Unit)
└── AMD Pensando DSC
```

**DPU Architecture**:

```
DPU as "Third Member" of Server:

CPU: General compute
├── VMs
├── Applications
└── User workloads

GPU: AI/Graphics compute
├── AI training
├── Inference
└── Graphics

DPU: Infrastructure compute
├── Networking
├── Security
├── Storage
└── Management

Result: Each specialized for its task
```

**DPU Benefits**:

```
Infrastructure Offload:
├── Networking: vSwitch, routing, tunnels
├── Security: Firewall, encryption
├── Storage: NVMe-oF, compression
└── Monitoring: Telemetry, logging

Isolation:
├── DPU runs infrastructure
├── CPU runs tenant workloads
└── Tenant cannot interfere with infrastructure

Performance:
├── Hardware acceleration
├── Dedicated resources
└── No CPU stealing
```

---

## NIC Selection for Virtualization

### Key Criteria

**1. Hardware Offloads**:
```
Essential:
├── Checksum offload
├── TSO/LSO
├── LRO/GRO
└── VLAN offload

Important:
├── RSS (multi-queue)
├── SR-IOV support
└── Jumbo frames

Nice to have:
├── Flow Director
├── RDMA
└── VXLAN offload
```

**2. SR-IOV Support**:
```
For VM direct assignment:
├── Number of VFs supported
│   ├── Intel X710: 128 VFs
│   └── Mellanox ConnectX-5: 127 VFs
└── VF features (which offloads available to VFs)
```

**3. Multi-Queue**:
```
Match to CPU count:
├── 16 CPU server → 16+ queue NIC
└── Allows scaling across all cores
```

**4. Performance Needs**:
```
Workload-dependent:
├── Web server: 10 Gbps sufficient
├── Storage network: 25-100 Gbps
├── HPC/AI: 100-200 Gbps
└── Database: Low latency critical
```

**5. Driver Support**:
```
Check compatibility:
├── Linux kernel version
├── Hypervisor (KVM, ESXi, Hyper-V)
├── VM guest OS support
└── Feature support in drivers
```

### Recommended NICs

**Budget / General Purpose**:
```
Intel I350 (1 GbE)
├── Quad port
├── Good offloads
├── Reliable
└── ~$300

Intel X550 (10 GbE)
├── Dual port
├── SR-IOV
├── Good driver support
└── ~$500
```

**Performance**:
```
Intel X710 (10 GbE)
├── Excellent offloads
├── 128 VFs
├── Flow Director
└── ~$700

Mellanox ConnectX-5 (25/100 GbE)
├── High performance
├── RDMA (RoCE)
├── SR-IOV
└── ~$1000-2000
```

**High-End / AI / HPC**:
```
Mellanox ConnectX-6 (100/200 GbE)
├── Extreme performance
├── Advanced RDMA
├── GPUDirect
└── ~$2000-4000

NVIDIA BlueField-2 DPU
├── 100/200 Gbps
├── ARM cores
├── Full infrastructure offload
└── ~$3000+
```

---

## NIC Configuration

### Driver Installation

```bash
# Check current driver
ethtool -i eth0
driver: i40e
version: 2.8.20
firmware-version: 8.00

# Update driver (example for Intel)
# Download from vendor
tar xzf i40e-<version>.tar.gz
cd i40e-<version>/src
make install
rmmod i40e
modprobe i40e

# Verify
ethtool -i eth0
```

### Performance Tuning

**Ring Buffer Size**:
```bash
# Check current
ethtool -g eth0
RX:  512
TX:  512

# Increase (reduce drops under load)
ethtool -G eth0 rx 4096 tx 4096

# Larger = fewer drops, more memory
```

**Interrupt Coalescing**:
```bash
# Check current
ethtool -c eth0

# Set coalescing
ethtool -C eth0 \
  rx-usecs 50 \
  tx-usecs 50 \
  rx-frames 32 \
  tx-frames 32

# Triggers interrupt after:
# - 50 microseconds, OR
# - 32 frames
# (whichever comes first)
```

**Multi-Queue Tuning**:
```bash
# Set queue count
ethtool -L eth0 combined 16

# Pin IRQs to CPUs
# Get IRQ numbers
grep eth0 /proc/interrupts

# Set CPU affinity
echo 0 > /proc/irq/<irq>/smp_affinity_list  # CPU 0
echo 1 > /proc/irq/<irq+1>/smp_affinity_list  # CPU 1
# etc.

# Or use irqbalance for automatic
systemctl start irqbalance
```

**Offload Settings**:
```bash
# Check current offloads
ethtool -k eth0

# Enable offloads
ethtool -K eth0 \
  tso on \
  gso on \
  gro on \
  lro on \
  rx on \
  tx on \
  sg on

# Disable for troubleshooting
ethtool -K eth0 tso off
```

**Jumbo Frames**:
```bash
# Set MTU to 9000
ip link set eth0 mtu 9000

# Verify
ip link show eth0
mtu 9000

# Note: All devices in path must support
```

### Monitoring

```bash
# Statistics
ethtool -S eth0
rx_packets: 1234567
tx_packets: 7654321
rx_errors: 0
rx_dropped: 0

# Continuous monitoring
watch -n1 'ethtool -S eth0 | grep -E "rx_|tx_"'

# Check for errors
ethtool -S eth0 | grep -i err
ethtool -S eth0 | grep -i drop
```

---

## Summary

**NICs are critical hardware** that:
- Connect systems to networks
- Convert digital data to/from physical signals
- Provide hardware offloads to reduce CPU load
- Enable high-performance networking

**Modern NICs provide**:
- Multiple speeds (1/10/25/100+ Gbps)
- Hardware offloads (checksum, TSO, LRO, RSS)
- Multi-queue support for multi-core scaling
- SR-IOV for VM direct assignment
- Advanced features (RDMA, flow steering)

**Smart NICs and DPUs** represent the future:
- Offload entire infrastructure stack
- Programmable networking
- Dedicated security processing
- Free host CPU for applications

**Selection considerations**:
- Match speed to workload
- Ensure driver support
- Consider SR-IOV for virtualization
- Plan for multi-queue with CPU count
- Budget for performance needs

NICs are the gateway between compute and network, and choosing the right NIC with proper configuration is essential for optimal system performance.

---
# PCI Passthrough:

## Table of Contents
1. [The Problem with Virtualized Devices](#the-problem-with-virtualized-devices)
2. [What is PCI Passthrough?](#what-is-pci-passthrough)
3. [How PCI Passthrough Works](#how-pci-passthrough-works)
4. [IOMMU - The Key Technology](#iommu---the-key-technology)
5. [Types of Passthrough](#types-of-passthrough)
6. [Setting Up PCI Passthrough](#setting-up-pci-passthrough)
7. [Performance Characteristics](#performance-characteristics)
8. [Limitations and Challenges](#limitations-and-challenges)
9. [Use Cases](#use-cases)
10. [Troubleshooting](#troubleshooting)

---

## The Problem with Virtualized Devices

### Virtual Device Performance Penalty

**Traditional VM I/O Path**:

```
Virtual Machine:
├── Application requests I/O
├── Guest OS processes request
├── VM sends I/O to virtual device
    ↓
Hypervisor:
├── Intercepts I/O (VM exit)
├── Emulates device behavior
├── Translates to real hardware
├── Performs actual I/O
├── Returns result
└── Re-enters VM (VM entry)
    ↓
Virtual Machine:
└── Receives result

Overhead at EVERY step:
├── VM exits (context switch)
├── Emulation/translation
├── VM entries (context switch)
└── Multiple memory copies
```

**Performance Impact**:

```
Physical Server → NIC directly:
├── Throughput: 10 Gbps line rate
├── Latency: ~1 microsecond
└── CPU overhead: Minimal

VM → Virtual NIC → Hypervisor → Physical NIC:
├── Throughput: 5-8 Gbps (emulated) or 8-9 Gbps (paravirtualized)
├── Latency: ~10-50 microseconds
└── CPU overhead: High (VM exits, emulation)

Loss: 20-50% throughput, 10-50× latency!
```

### Why the Performance Gap?

**1. VM Exits**:
```
Every I/O operation:
├── CPU switches from VM mode to hypervisor mode
├── Context switch overhead
├── CPU cache flush
└── Hundreds of CPU cycles wasted

High I/O rate = Many VM exits = Poor performance
```

**2. Emulation Overhead**:
```
Hypervisor must:
├── Interpret what VM wants
├── Translate virtual device I/O to physical device I/O
├── Emulate device registers
└── Handle interrupts

All in software = Slow
```

**3. Memory Copies**:
```
Data path:
├── VM memory
├── Copy to hypervisor memory
├── Copy to NIC memory
└── Multiple copies = Slow + CPU overhead
```

**4. Shared Resources**:
```
Multiple VMs sharing one physical device:
├── Contention
├── Scheduling overhead
└── Quality-of-service challenges
```

---

## What is PCI Passthrough?

**PCI Passthrough** (also called device passthrough, VT-d passthrough, or direct device assignment) allows a VM to directly access a physical PCI device, bypassing the hypervisor.

### The Concept

**Instead of**:
```
VM → Virtual Device → Hypervisor → Physical Device
```

**PCI Passthrough gives**:
```
VM → Physical Device (Direct!)
```

**Visual Comparison**:

```
Traditional Virtualization:
┌─────────────┬─────────────┬─────────────┐
│    VM1      │    VM2      │    VM3      │
│ (vNIC)      │ (vNIC)      │ (vNIC)      │
└──────┬──────┴──────┬──────┴──────┬──────┘
       └─────────────┼─────────────┘
            Virtual Switch
                 ↓
            Hypervisor
                 ↓
           Physical NIC


PCI Passthrough:
┌─────────────┬─────────────┬─────────────┐
│    VM1      │    VM2      │    VM3      │
│             │             │             │
│             │ Physical    │             │
│             │ NIC         │             │
│             │ (Direct)    │             │
└─────────────┴──────┬──────┴─────────────┘
                     │
                (No hypervisor intercept!)
                     │
               Physical NIC
```

### Key Characteristics

**1. Direct Access**:
```
VM has:
├── Direct MMIO (Memory-Mapped I/O) access
├── Direct DMA capability
├── Direct interrupt delivery
└── No hypervisor intermediary
```

**2. Exclusive Ownership**:
```
Device assigned to one VM:
├── That VM owns it completely
├── Other VMs cannot use it
├── Hypervisor cannot use it
└── 1 device : 1 VM relationship
```

**3. Near-Native Performance**:
```
Performance almost identical to physical:
├── ~99% of bare-metal throughput
├── Native latency
├── Minimal CPU overhead
└── No virtualization tax
```

**4. Driver Requirements**:
```
VM must have:
└── Device driver for the actual physical device

Not virtual device driver!
Example: Real Intel X710 driver, not virtio-net
```

---

## How PCI Passthrough Works

### Prerequisites

**Hardware Requirements**:

```
CPU must support:
├── Intel VT-x (Virtualization Technology)
├── Intel VT-d (Virtualization Technology for Directed I/O)
    OR
├── AMD-V (AMD Virtualization)
└── AMD-Vi (AMD I/O Virtualization)

Motherboard/Chipset must support:
├── IOMMU (Input-Output Memory Management Unit)
└── Interrupt remapping

Device must:
├── Be on its own IOMMU group (usually)
└── Support reset (FLR - Function Level Reset)
```

**Software Requirements**:

```
Hypervisor:
├── KVM/QEMU
├── VMware ESXi (DirectPath I/O)
├── Xen
└── Hyper-V (Discrete Device Assignment)

IOMMU enabled in:
├── BIOS/UEFI
└── Kernel boot parameters
```

### The Passthrough Process

**Step-by-Step**:

```
1. System Boot:
   ├── BIOS/UEFI detects PCI devices
   ├── Host OS loads
   └── Host initializes devices

2. Identify Device for Passthrough:
   ├── Find PCI address (e.g., 0000:02:00.0)
   ├── Unbind from host driver
   └── Bind to VFIO driver (Virtual Function I/O)

3. Create VM with Device Assignment:
   ├── Configure VM to use device
   ├── VFIO framework prepares device
   └── IOMMU configured for VM

4. Start VM:
   ├── VM boots
   ├── VM sees physical device
   ├── VM loads its own driver
   └── VM uses device directly

5. VM Runs:
   ├── Direct MMIO access
   ├── Direct DMA
   ├── Direct interrupts
   └── No hypervisor involvement

6. Stop VM:
   ├── Device reset
   ├── Unbind from VFIO
   └── Can reassign to host or another VM
```

### Address Translation

**Without Passthrough (Virtual DMA)**:

```
VM thinks it writes to physical address 0x1000:
    ↓
Hypervisor translates:
    VM Physical 0x1000 → Host Physical 0xA3F5000
    ↓
Hypervisor performs DMA
    ↓
Data transferred

Problem: Requires hypervisor involvement
```

**With Passthrough (Direct DMA via IOMMU)**:

```
VM thinks it writes to physical address 0x1000:
    ↓
Device performs DMA to address 0x1000
    ↓
IOMMU intercepts:
    VM Physical 0x1000 → Host Physical 0xA3F5000
    ↓
DMA completes to correct memory
    ↓
No hypervisor involvement!

IOMMU does translation in hardware!
```

### Interrupt Delivery

**Without Passthrough**:

```
Physical device generates interrupt:
    ↓
Hypervisor receives interrupt
    ↓
Hypervisor determines which VM
    ↓
Hypervisor injects virtual interrupt to VM
    ↓
VM handles interrupt

Slow: Multiple steps
```

**With Passthrough (Interrupt Remapping)**:

```
Physical device generates interrupt:
    ↓
IOMMU remaps interrupt directly to VM
    ↓
VM receives interrupt directly
    ↓
VM handles interrupt

Fast: Direct delivery
```

---

## IOMMU - The Key Technology

### What is IOMMU?

**IOMMU (Input-Output Memory Management Unit)** is hardware that provides:

1. **DMA address translation** for devices
2. **Memory protection** (devices can only access allowed memory)
3. **Interrupt remapping** (direct interrupt delivery to VMs)

Think of it as an "MMU for devices":

```
CPU MMU:
├── Translates virtual addresses to physical
├── Protects memory (process isolation)
└── For CPU memory accesses

IOMMU:
├── Translates device DMA addresses
├── Protects memory (device isolation)
└── For device memory accesses
```

### IOMMU Groups

**IOMMU Group** = Set of devices that must be assigned together.

**Why Groups?**

```
PCIe Switch Example:

    Root Complex
         ├── PCIe Switch
             ├── Port 1 → NIC 1
             ├── Port 2 → NIC 2
             └── Port 3 → NIC 3

IOMMU sees:
└── One IOMMU Group containing all 3 NICs

Reason: Devices can talk peer-to-peer
Must assign entire group to prevent isolation breach
```

**Checking IOMMU Groups**:

```bash
#!/bin/bash
for d in /sys/kernel/iommu_groups/*/devices/*; do 
    n=${d#*/iommu_groups/*}; n=${n%%/*}
    printf 'IOMMU Group %s ' "$n"
    lspci -nns "${d##*/}"
done

Output:
IOMMU Group 1  00:01.0 PCI bridge [0604]: Intel...
IOMMU Group 2  00:02.0 VGA [0300]: Intel...
IOMMU Group 13 01:00.0 Ethernet [0200]: Intel X710 [8086:1572]
IOMMU Group 13 01:00.1 Ethernet [0200]: Intel X710 [8086:1572]
...
```

**Ideal Situation**:

```
Each device in its own group:
├── IOMMU Group 13: NIC 1 (0000:01:00.0) ← Can assign alone
├── IOMMU Group 14: NIC 2 (0000:02:00.0) ← Can assign alone
└── IOMMU Group 15: GPU (0000:03:00.0) ← Can assign alone
```

**Problematic Situation**:

```
Many devices in one group:
└── IOMMU Group 1:
    ├── PCI Bridge
    ├── USB Controller
    ├── SATA Controller
    ├── NIC 1
    └── NIC 2

Problem: To assign NIC 1, must assign entire group!
└── Lose USB, SATA, NIC 2 from host
```

**Solution**: ACS Override (Advanced, use carefully):

```bash
# Enable ACS override in kernel
# Treats each device as separate group
# WARNING: May reduce isolation!

Kernel parameter:
pcie_acs_override=downstream,multifunction
```

### IOMMU Page Tables

**IOMMU maintains page tables** for each VM:

```
VM 1 IOMMU Page Table:
VM Physical → Host Physical
0x0000      → 0x10000000
0x1000      → 0x10001000
0x2000      → 0x10002000
...

VM 2 IOMMU Page Table:
VM Physical → Host Physical
0x0000      → 0x20000000
0x1000      → 0x20001000
0x2000      → 0x20002000
...

Device assigned to VM 1:
└── Uses VM 1's IOMMU page table
    └── DMA to 0x1000 → Translated to 0x10001000

Security: Device cannot access VM 2's memory!
```

---

## Types of Passthrough

### 1. GPU Passthrough

**Use Case**: Gaming, CAD, AI/ML in VMs

```
VM Configuration:
├── Assign entire GPU to VM
├── VM loads GPU driver (NVIDIA, AMD)
├── VM uses GPU directly
└── Native GPU performance

Requirements:
├── GPU in own IOMMU group
├── GPU supports UEFI (modern GPUs)
├── GPU BIOS compatible
└── Host must have alternate GPU (for display)
```

**Typical Setup**:

```
Physical Server:
├── Integrated GPU → Host uses this
└── Discrete GPU → Passed to VM

VM sees:
└── Full GPU with all features
    ├── CUDA (NVIDIA)
    ├── OpenCL
    ├── Video encode/decode
    └── Display output
```

### 2. NIC Passthrough

**Use Case**: High-performance networking

```
VM Configuration:
├── Assign entire NIC to VM
├── VM loads NIC driver (e.g., Intel i40e)
├── Direct network access
└── Near-native network performance

Benefits:
├── 99% of physical NIC performance
├── Low latency (<1 microsecond overhead)
├── Full NIC features (RDMA, etc.)
└── No vSwitch overhead
```

**Limitations**:

```
1 NIC : 1 VM (exclusive):
├── Other VMs cannot use this NIC
├── Need multiple NICs for multiple VMs
└── Reduces flexibility

Migration challenges:
├── Cannot live migrate VM (device attached)
├── Must stop VM to migrate
└── Network interruption
```

**Solution for Multiple VMs**: SR-IOV (Next section!)

### 3. Storage Controller Passthrough

**Use Case**: High-performance storage

```
Pass NVMe controller or RAID card to VM:
├── VM gets direct disk access
├── Native storage performance
├── Full controller features
└── No virtualization overhead

Example:
└── NVMe SSD → VM runs database
    └── Lowest latency, highest IOPS
```

### 4. USB Controller Passthrough

**Use Case**: USB device access

```
Passthrough entire USB controller:
├── VM gets all ports on controller
├── Can use any USB device
├── Hot-plug works
└── Native USB performance

Alternative: USB device passthrough
├── Pass individual USB devices (slower)
└── More flexible but lower performance
```

---

## Setting Up PCI Passthrough

### Step 1: Enable IOMMU in BIOS

```
BIOS/UEFI Settings:
├── Intel: Enable "VT-d"
└── AMD: Enable "AMD-Vi" or "IOMMU"

Usually in:
├── Advanced → CPU Configuration
└── Chipset Configuration
```

### Step 2: Enable IOMMU in Kernel

**Debian/Ubuntu**:

```bash
# Edit GRUB configuration
sudo nano /etc/default/grub

# Add to GRUB_CMDLINE_LINUX_DEFAULT:
# Intel:
intel_iommu=on iommu=pt

# AMD:
amd_iommu=on iommu=pt

# Example:
GRUB_CMDLINE_LINUX_DEFAULT="quiet intel_iommu=on iommu=pt"

# Update GRUB
sudo update-grub

# Reboot
sudo reboot
```

**Verify IOMMU enabled**:

```bash
# Check if IOMMU is active
dmesg | grep -i iommu

Output should show:
DMAR: IOMMU enabled
# or
AMD-Vi: AMD IOMMUv2 loaded
```

### Step 3: Identify PCI Device

```bash
# List all PCI devices
lspci -nn

Output:
01:00.0 Ethernet controller [0200]: Intel Corporation Ethernet Controller X710 [8086:1572]
01:00.1 Ethernet controller [0200]: Intel Corporation Ethernet Controller X710 [8086:1572]

# Note:
# - Bus address: 01:00.0
# - Vendor ID: 8086 (Intel)
# - Device ID: 1572 (X710)
```

**Check IOMMU Group**:

```bash
# Find IOMMU group for device
ls -l /sys/bus/pci/devices/0000:01:00.0/iommu_group

Output:
lrwxrwxrwx ... -> ../../../../kernel/iommu_groups/13

# Device is in IOMMU Group 13

# See all devices in that group
ls /sys/kernel/iommu_groups/13/devices/
0000:01:00.0  0000:01:00.1

# Good: Only the 2 ports of the same NIC
```

### Step 4: Unbind from Host Driver

```bash
# Find current driver
lspci -k -s 01:00.0

Output:
01:00.0 Ethernet controller: Intel X710
    Kernel driver in use: i40e
    Kernel modules: i40e

# Unbind from i40e driver
echo "0000:01:00.0" > /sys/bus/pci/drivers/i40e/unbind

# Verify
lspci -k -s 01:00.0
# Should show no driver in use
```

### Step 5: Bind to VFIO Driver

**Load VFIO modules**:

```bash
# Load VFIO modules
modprobe vfio
modprobe vfio_pci
modprobe vfio_iommu_type1

# Make permanent
echo "vfio" >> /etc/modules
echo "vfio_pci" >> /etc/modules
echo "vfio_iommu_type1" >> /etc/modules
```

**Bind device to VFIO**:

```bash
# Get vendor and device ID
lspci -n -s 01:00.0
01:00.0 0200: 8086:1572

# Bind to VFIO
echo "8086 1572" > /sys/bus/pci/drivers/vfio-pci/new_id

# Verify
lspci -k -s 01:00.0
    Kernel driver in use: vfio-pci
```

**Automatic binding** (better method):

```bash
# Edit /etc/modprobe.d/vfio.conf
options vfio-pci ids=8086:1572

# Update initramfs
update-initramfs -u

# Reboot
# Device will automatically bind to vfio-pci
```

### Step 6: Assign to VM

**QEMU/KVM command line**:

```bash
qemu-system-x86_64 \
  -enable-kvm \
  -m 4G \
  -smp 4 \
  -drive file=vm.qcow2 \
  -device vfio-pci,host=01:00.0 \
  ...
```

**libvirt XML**:

```xml
<domain type='kvm'>
  <name>myvm</name>
  <memory unit='GiB'>4</memory>
  <vcpu>4</vcpu>
  
  <devices>
    <hostdev mode='subsystem' type='pci' managed='yes'>
      <source>
        <address domain='0x0000' bus='0x01' slot='0x00' function='0x0'/>
      </source>
    </hostdev>
  </devices>
</domain>
```

**Proxmox GUI**:

```
VM → Hardware → Add → PCI Device:
├── Select device (01:00.0)
├── Enable "All Functions" (if multi-function)
├── Enable "Primary GPU" (for GPU passthrough)
└── Add
```

### Step 7: Install Driver in VM

```
Boot VM:
├── VM sees physical device
├── Install appropriate driver
│   ├── Windows: Vendor driver package
│   └── Linux: Usually in kernel already
└── Device ready to use

Verify:
├── Linux: lspci, ip link show
└── Windows: Device Manager
```

---

## Performance Characteristics

### Benchmarks

**Network Performance** (10 Gbps NIC):

```
Bare Metal:
├── Throughput: 9.8 Gbps
├── Latency: ~1 microsecond
└── CPU: 20% at line rate

Virtual NIC (virtio):
├── Throughput: 8.5 Gbps
├── Latency: ~15 microseconds
└── CPU: 50% at line rate

PCI Passthrough:
├── Throughput: 9.7 Gbps
├── Latency: ~1.2 microseconds
└── CPU: 22% at line rate

Passthrough ≈ 99% of bare metal!
```

**GPU Performance**:

```
Bare Metal: 100 FPS
Virtual GPU: ~70 FPS (30% overhead)
GPU Passthrough: ~98 FPS (2% overhead)

Passthrough ≈ 98% of bare metal!
```

**Storage Performance** (NVMe):

```
Bare Metal:
├── IOPS: 500,000
└── Latency: 20 microseconds

Virtual Disk:
├── IOPS: 300,000 (40% overhead)
└── Latency: 50 microseconds

NVMe Passthrough:
├── IOPS: 495,000
└── Latency: 21 microseconds

Passthrough ≈ 99% of bare metal!
```

### Overhead Sources

**Minimal overhead from**:

```
1. IOMMU translation:
   └── Hardware-accelerated, <1% overhead

2. VFIO framework:
   └── Minimal, just setup/teardown

3. Interrupt remapping:
   └── Hardware feature, negligible overhead

Total overhead: ~1-2%
```

**Why so good?**

```
No VM exits:
└── Device doesn't trap to hypervisor

No emulation:
└── Real hardware, real drivers

Direct DMA:
└── No data copying

Direct interrupts:
└── No interrupt injection
```

---

## Limitations and Challenges

### 1. Device Exclusivity

```
Problem:
└── 1 device can only be used by 1 VM at a time

Impact:
├── Need multiple devices for multiple VMs
├── Cannot share expensive hardware (GPU)
├── Reduced consolidation ratio
└── Higher hardware costs

Solution:
└── SR-IOV (next section) for NICs
└── vGPU for GPUs (vendor-specific)
```

### 2. Migration Limitations

```
Live Migration:
└── NOT POSSIBLE with passthrough devices

Reason:
├── Device state is in hardware
├── Cannot capture/restore hardware state
├── Device attached to specific physical location

Workaround:
├── Stop VM
├── Detach device
├── Migrate VM
├── Attach device on destination
└── Downtime required
```

### 3. IOMMU Group Constraints

```
If IOMMU group contains multiple devices:
└── Must pass through entire group

Example Problem:
IOMMU Group 1:
├── USB Controller (need for host)
├── SATA Controller (need for host)
└── NIC (want to pass to VM)

Cannot pass NIC alone!

Solutions:
├── ACS override (reduces isolation)
├── Move device to different PCIe slot
└── Upgrade motherboard/CPU
```

### 4. Reset Issues

```
Some devices don't support proper reset:
├── FLR (Function Level Reset) missing
└── Cannot properly reset between VM uses

Impact:
├── Device in bad state after VM shutdown
├── Cannot restart VM without host reboot
└── Cannot reassign to different VM

Solution:
├── Use devices that support FLR
└── Vendor quirks/workarounds
```

### 5. Driver Compatibility

```
VM must have driver for real hardware:
├── Not virtual device driver
└── May not exist for all guest OS

Example:
├── New GPU, old Windows 7 guest
└── No driver available → Won't work
```

### 6. VFIO Interrupt Limits

```
VFIO has limit on interrupt vectors:
└── Problem for devices with many queues

Example:
├── 64-queue NIC wants 64 MSI-X vectors
├── VFIO may limit to fewer
└── Reduced performance

Solution:
└── Newer kernels have higher limits
```

---

## Use Cases

### 1. High-Performance Networking

```
Scenario: Network Function Virtualization (NFV)

Setup:
├── Multiple VMs running network functions
│   ├── Firewall VM → NIC 1 (passthrough)
│   ├── Router VM → NIC 2 (passthrough)
│   └── IDS VM → NIC 3 (passthrough)
└── Each VM has dedicated NIC

Benefits:
├── Native network performance
├── Full NIC features (DPDK compatible)
└── Low latency processing
```

### 2. GPU-Accelerated Workloads

```
Scenario: ML Training in VMs

Setup:
├── Physical server with 4 GPUs
├── 4 VMs, each with 1 GPU (passthrough)
└── Each researcher has dedicated GPU

Benefits:
├── Native GPU performance
├── Full CUDA support
├── Isolation between users
└── Flexibility of VMs
```

### 3. Gaming VMs

```
Scenario: Multiple gaming VMs on one PC

Setup:
├── Host: Linux
├── VM 1: Windows + GPU 1 (passthrough)
├── VM 2: Windows + GPU 2 (passthrough)
└── Each user gets dedicated gaming VM

Benefits:
├── Native gaming performance
├── Multiple users simultaneously
└── Flexible scheduling
```

### 4. Storage Performance

```
Scenario: Database server needs fast storage

Setup:
├── NVMe SSD passthrough to VM
└── VM runs database

Benefits:
├── Native NVMe performance
├── Lowest latency
├── Maximum IOPS
└── No virtualization overhead
```

### 5. Security Isolation

```
Scenario: High-security application

Setup:
├── Dedicated NIC for secure VM
├── Physically separate network path
└── IOMMU ensures DMA isolation

Benefits:
├── Hardware-enforced isolation
├── Cannot sniff other VMs' traffic
├── Separate network segment
└── Compliance requirements met
```

---

## Troubleshooting

### Device Not Visible in VM

```
Check:
1. IOMMU enabled in BIOS?
   └── dmesg | grep -i iommu

2. IOMMU enabled in kernel?
   └── Check boot parameters

3. Device bound to vfio-pci?
   └── lspci -k -s <device>

4. IOMMU group contains only wanted devices?
   └── ls /sys/kernel/iommu_groups/*/devices/

5. VM configuration correct?
   └── Check domain/bus/slot/function numbers
```

### Poor Performance

```
Check:
1. IOMMU in passthrough mode?
   └── iommu=pt kernel parameter

2. Device using MSI-X interrupts?
   └── cat /proc/interrupts

3. CPU pinning configured?
   └── Avoid CPU overcommit

4. NUMA alignment?
   └── Pin VM to same NUMA node as device

5. Interrupt affinity?
   └── Ensure interrupts go to VM's CPUs
```

### Cannot Reset Device

```
Symptoms:
└── Device doesn't work after VM restart

Solutions:
1. Check if device supports FLR:
   └── lspci -vv -s <device> | grep FLR

2. Try vendor-specific reset:
   └── Some devices need special handling

3. Use workarounds:
   └── echo 1 > /sys/bus/pci/devices/.../reset

4. Last resort:
   └── Reboot host
```

### IOMMU Group Too Large

```
Problem:
└── Too many devices in IOMMU group

Solutions:
1. Try different PCIe slot
2. Enable ACS override (carefully!):
   └── pcie_acs_override=downstream
3. Get better motherboard/CPU
4. Use SR-IOV (if NIC supports)
```

---

## Summary

**PCI Passthrough** enables:
- Near-native hardware performance in VMs
- Direct device access bypassing hypervisor
- Full hardware features available to VM
- ~99% of bare-metal performance

**Key Requirements**:
- IOMMU-capable hardware (VT-d/AMD-Vi)
- Device in appropriate IOMMU group
- VFIO driver framework
- Proper configuration

**Benefits**:
- Maximum performance
- Hardware feature access
- Low latency
- Minimal CPU overhead

**Limitations**:
- 1 device : 1 VM (exclusive)
- No live migration
- IOMMU group constraints
- Reset challenges

**Best For**:
- High-performance networking (NFV)
- GPU workloads (gaming, ML, CAD)
- Low-latency storage (NVMe)
- Security-sensitive applications

For NICs specifically, SR-IOV (next section) solves the exclusivity problem by creating multiple virtual functions that can be passed to different VMs!

---
# SR-IOV (Single Root I/O Virtualization): 

## Table of Contents
1. [The Problem SR-IOV Solves](#the-problem-sr-iov-solves)
2. [What is SR-IOV?](#what-is-sr-iov)
3. [SR-IOV Architecture](#sr-iov-architecture)
4. [How SR-IOV Works](#how-sr-iov-works)
5. [Physical Function vs Virtual Function](#physical-function-vs-virtual-function)
6. [Setting Up SR-IOV](#setting-up-sr-iov)
7. [SR-IOV in Different Environments](#sr-iov-in-different-environments)
8. [Performance](#performance)
9. [Limitations and Considerations](#limitations-and-considerations)
10. [SR-IOV vs Alternatives](#sr-iov-vs-alternatives)

---

## The Problem SR-IOV Solves

### PCI Passthrough Limitation

From the previous section, we learned about PCI passthrough:

```
Traditional PCI Passthrough:
├── 1 Physical NIC → 1 VM (Exclusive)
├── Other VMs cannot use this NIC
└── Need multiple NICs for multiple VMs

Example Scenario:
Physical Server:
├── 2 Physical NICs
├── 20 VMs needing network
└── Problem: Only 2 VMs can get passthrough!

Other 18 VMs must use:
└── Slow virtual NICs (performance penalty)
```

**The Dilemma**:

```
Option 1: Virtual NICs (vNICs)
├── Pros: All VMs get network, flexible
└── Cons: 20-50% performance loss, high CPU overhead

Option 2: PCI Passthrough
├── Pros: Native performance
└── Cons: Only 2 VMs out of 20 can use it

We want: Native performance for ALL VMs!
```

### Business Impact

```
Cloud Provider Problem:
├── 1000 VMs per server
├── Each needs high-performance network
├── Options:
    ├── 1000 NICs per server? → Impossible!
    ├── Virtual NICs? → Poor performance
    └── Passthrough? → Only few VMs benefit

Need: Way to share one NIC among many VMs
      WITH near-native performance!
```

---

## What is SR-IOV?

**SR-IOV (Single Root I/O Virtualization)** is a specification that allows a PCIe device to present itself as multiple separate devices.

### The Core Concept

**Magic**: One physical NIC becomes many virtual NICs

```
One Physical NIC with SR-IOV:
┌─────────────────────────────────┐
│     Physical NIC (Intel X710)   │
│                                 │
│  Physical Function (PF):        │
│  └── Management, configuration  │
│                                 │
│  Virtual Functions (VFs):       │
│  ├── VF 0 → VM 1 (passthrough)  │
│  ├── VF 1 → VM 2 (passthrough)  │
│  ├── VF 2 → VM 3 (passthrough)  │
│  ├── VF 3 → VM 4 (passthrough)  │
│  └── ... up to 128 VFs          │
└─────────────────────────────────┘

Result: 
├── 1 Physical NIC
├── Supports 128 VMs simultaneously
└── Each VM gets native performance!
```

**Analogy**:

```
Physical NIC = Apartment Building (SR-IOV enabled)

Physical Function (PF) = Building Manager
├── Manages the building
├── Configures infrastructure
└── Not for tenants to use

Virtual Functions (VFs) = Individual Apartments
├── VF 0 = Apartment 1 → Tenant/VM 1
├── VF 1 = Apartment 2 → Tenant/VM 2
├── ...
├── Each apartment is independent
├── Each has own entrance (direct access)
└── Each is isolated from others

Each tenant (VM) thinks they own the whole building (NIC)!
```

### Key Benefits

```
1. Performance:
   └── Near-native (99% of physical NIC)

2. Scalability:
   └── Many VMs per physical NIC (up to 128+)

3. Isolation:
   └── VMs cannot interfere with each other

4. Efficiency:
   └── Lower CPU overhead than vNICs

5. Hardware-Based:
   └── Not software emulation
```

---

## SR-IOV Architecture

### Components

```
┌────────────────────────────────────────────┐
│        SR-IOV Capable NIC                  │
│                                            │
│  ┌──────────────────────────────────┐     │
│  │  Physical Function (PF)          │     │
│  │  ├── Full NIC capabilities       │     │
│  │  ├── Creates/manages VFs         │     │
│  │  ├── Host driver controls this   │     │
│  │  └── Configuration interface     │     │
│  └──────────────────────────────────┘     │
│                                            │
│  ┌──────────────────────────────────┐     │
│  │  Virtual Functions (VFs)         │     │
│  │                                  │     │
│  │  VF 0: Lightweight NIC           │     │
│  │  ├── TX queue                    │     │
│  │  ├── RX queue                    │     │
│  │  ├── Limited config              │     │
│  │  └── Assigned to VM 1            │     │
│  │                                  │     │
│  │  VF 1: Lightweight NIC           │     │
│  │  └── Assigned to VM 2            │     │
│  │                                  │     │
│  │  ... (up to max VFs)             │     │
│  └──────────────────────────────────┘     │
│                                            │
│  Hardware Resources:                      │
│  ├── Shared: PHY, MAC, Memory             │
│  └── Per-VF: Queues, Registers            │
└────────────────────────────────────────────┘
```

### Layered View

```
VMs:
┌──────┬──────┬──────┬──────┐
│ VM1  │ VM2  │ VM3  │ VM4  │
│ VF0  │ VF1  │ VF2  │ VF3  │ ← Each VM sees a "full" NIC
└───┬──┴───┬──┴───┬──┴───┬──┘
    │      │      │      │
    └──────┴──────┴──────┘
           │
    ┌──────┴───────┐
    │   Hypervisor │
    │   (PF)       │ ← Host controls Physical Function
    └──────┬───────┘
           │
    ┌──────┴───────┐
    │  SR-IOV NIC  │
    │  Hardware    │ ← NIC implements VF separation
    └──────┬───────┘
           │
        Network
```

---

## How SR-IOV Works

### Creating Virtual Functions

**Step 1: Enable SR-IOV on NIC**:

```bash
# Check if NIC supports SR-IOV
lspci -vvv -s 01:00.0 | grep SR-IOV

Output:
    Capabilities: [160] Single Root I/O Virtualization (SR-IOV)
        IOVCap: Migration-, Interrupt Message Number: 000
        IOVCtl: Enable+ Migration- Interrupt- MSE+ ARIHierarchy+
        Initial VFs: 64, Total VFs: 64, ...

# Enable VFs (create 4 virtual functions)
echo 4 > /sys/class/net/eth0/device/sriov_numvfs

# Verify VFs created
lspci | grep Virtual

Output:
01:00.0 Ethernet controller: Intel X710 (Physical Function)
01:02.0 Ethernet controller: Intel X710 Virtual Function
01:02.1 Ethernet controller: Intel X710 Virtual Function
01:02.2 Ethernet controller: Intel X710 Virtual Function
01:02.3 Ethernet controller: Intel X710 Virtual Function
```

**What Just Happened?**:

```
Before (no VFs):
01:00.0 - Physical Function only

After (4 VFs enabled):
01:00.0 - Physical Function (PF)
01:02.0 - Virtual Function 0 (VF0)
01:02.1 - Virtual Function 1 (VF1)
01:02.2 - Virtual Function 2 (VF2)
01:02.3 - Virtual Function 3 (VF3)

Each VF appears as separate PCI device!
```

### VF Creation in Hardware

```
NIC Hardware Changes:

1. NIC allocates hardware resources:
   ├── Each VF gets:
   │   ├── TX queue(s)
   │   ├── RX queue(s)
   │   ├── Dedicated registers
   │   ├── Interrupt vectors
   │   └── MAC address
   └── Shared across all VFs:
       ├── Physical port
       ├── PHY
       └── Link

2. NIC configures IOMMU groups:
   ├── Each VF in own IOMMU group
   └── Enables safe passthrough

3. PCIe configuration space:
   ├── Each VF gets own PCIe config
   └── Appears as separate device to OS
```

### Data Path

**Transmit (VM sends packet)**:

```
1. VM writes packet to VF's TX queue
   └── Direct memory write (DMA)

2. VF hardware processes:
   ├── Reads packet from queue
   ├── Applies VF-specific settings
   │   ├── Source MAC (VF's MAC)
   │   ├── VLAN tag (if configured)
   │   └── QoS settings
   └── Schedules for transmission

3. Shared TX scheduler:
   ├── Arbitrates between all VFs
   ├── Enforces QoS/rate limits
   └── Sends to PHY

4. PHY transmits on wire
   └── No hypervisor involvement!

Performance: Native speed, minimal overhead
```

**Receive (Packet arrives for VM)**:

```
1. PHY receives packet

2. NIC hardware classifies:
   ├── Checks destination MAC
   ├── Matches to VF MAC table
   └── Determines target VF

3. VF receives packet:
   ├── DMA writes to VF's RX queue
   ├── Queue is in VM's memory
   └── Direct write, no copy!

4. VF generates interrupt (or VM polls)
   └── VM processes packet

5. VM sees packet:
   └── As if from physical NIC

Performance: Native speed, minimal overhead
```

### Traffic Isolation

**MAC-based Isolation**:

```
VF0: MAC 52:54:00:00:00:01
VF1: MAC 52:54:00:00:00:02
VF2: MAC 52:54:00:00:00:03

Incoming packet for 52:54:00:00:00:02:
├── NIC hardware checks MAC
├── Determines: Goes to VF1
├── DMA to VF1's RX queue only
└── VF0 and VF2 cannot see it

Security: Hardware-enforced isolation
```

**VLAN Isolation** (optional):

```
VF0: VLAN 10
VF1: VLAN 20
VF2: VLAN 30

NIC can:
├── Tag outgoing packets with VF's VLAN
├── Filter incoming packets by VLAN
└── Enforce VLAN isolation in hardware
```

### Memory Isolation (IOMMU)

```
VF0 assigned to VM1:
├── VM1 memory: 0x1000-0x5000 (guest physical)
├── IOMMU maps: VM1 addresses → Host physical
├── VF0 can DMA to: Only VM1's memory
└── Cannot DMA to VM2, VM3, or host memory

IOMMU enforces:
└── VF cannot access other VMs' memory
    └── Hardware-level protection
```

---

## Physical Function vs Virtual Function

### Physical Function (PF)

**Characteristics**:

```
Full-Featured NIC:
├── All hardware capabilities
├── Full configuration access
├── Can create/destroy VFs
├── Manages shared resources
├── Host OS uses this
└── Cannot be assigned to VM (usually)

Configuration capabilities:
├── Link speed/duplex
├── Flow control
├── Number of VFs
├── VF MAC addresses
├── VF VLAN settings
├── VF rate limiting
└── Global NIC settings
```

**Host Driver** (Example: i40e for Intel X710):

```bash
# PF driver operations
# Load driver
modprobe i40e

# Configure PF
ip link set eth0 up
ethtool -s eth0 speed 10000 duplex full

# Create VFs
echo 8 > /sys/class/net/eth0/device/sriov_numvfs

# Configure VF settings
ip link set eth0 vf 0 mac 52:54:00:00:00:01
ip link set eth0 vf 0 vlan 100
ip link set eth0 vf 0 rate 1000  # 1000 Mbps max
```

### Virtual Function (VF)

**Characteristics**:

```
Lightweight NIC:
├── Subset of PF features
├── Own TX/RX queues
├── Own MAC address
├── Independent operation
├── Assigned to VM (passthrough)
└── Limited self-configuration

Typical limitations:
├── Cannot change own MAC (PF controls)
├── Cannot change VLAN (PF controls)
├── Cannot change rate limit (PF controls)
├── Reduced statistics/counters
└── Fewer queues than PF
```

**VF Driver** (in VM):

```
VM sees VF as regular NIC:
├── Standard NIC driver (e.g., iavf for Intel)
├── Standard network operations
├── No knowledge of virtualization
└── Near-native performance

VM operations:
# Inside VM
ip link set eth0 up
ip addr add 192.168.1.10/24 dev eth0
# Works like physical NIC!
```

### Feature Comparison

| Feature | Physical Function (PF) | Virtual Function (VF) |
|---------|------------------------|------------------------|
| **Used By** | Host OS | VM (passthrough) |
| **Full Features** | Yes | Subset |
| **Create VFs** | Yes | No |
| **Set VF Config** | Yes | No |
| **MAC Address** | Self-configured | PF-assigned |
| **VLAN** | Full control | PF-assigned |
| **Rate Limiting** | Configure for VFs | Enforced, cannot change |
| **Link Control** | Yes | No (follows PF) |
| **Statistics** | Complete | Basic |
| **Queues** | Many (e.g., 16-64) | Fewer (e.g., 2-8) |
| **Performance** | Maximum | Near-maximum |

---

## Setting Up SR-IOV

### Prerequisites

**Hardware**:

```
1. SR-IOV capable NIC:
   ├── Intel: X520, X540, X710, E810
   ├── Mellanox: ConnectX-4, ConnectX-5, ConnectX-6
   ├── Broadcom: BCM57xxx series
   └── Others: Check vendor specs

2. CPU with VT-d/AMD-Vi:
   └── Same as PCI passthrough requirement

3. IOMMU support:
   └── Motherboard/chipset compatibility
```

**Software**:

```
1. IOMMU enabled:
   └── In BIOS and kernel (see PCI Passthrough section)

2. NIC drivers:
   ├── Host: PF driver (e.g., i40e)
   └── Guest: VF driver (e.g., iavf)

3. Hypervisor support:
   ├── KVM/QEMU: Full support
   ├── VMware ESXi: Full support
   ├── Hyper-V: Full support
   └── Xen: Full support
```

### Step-by-Step Setup (Linux/KVM)

**Step 1: Verify NIC Supports SR-IOV**:

```bash
# Check SR-IOV capability
lspci -vvv -s 01:00.0 | grep -A 10 SR-IOV

Output:
    Capabilities: [160] Single Root I/O Virtualization (SR-IOV)
        IOVCap: Migration-, Interrupt Message Number: 000
        Initial VFs: 64, Total VFs: 64
        Number of VFs: 0, Function Dependency Link: 00
        ...
```

**Step 2: Enable IOMMU** (if not already):

```bash
# Check if enabled
dmesg | grep -i iommu

# If not, add to /etc/default/grub:
GRUB_CMDLINE_LINUX_DEFAULT="intel_iommu=on iommu=pt"

# Update and reboot
update-grub
reboot
```

**Step 3: Create Virtual Functions**:

```bash
# Find network interface name
ip link show

# Enable VFs (example: create 8 VFs)
echo 8 > /sys/class/net/ens1f0/device/sriov_numvfs

# Make permanent (systemd service or rc.local)
cat > /etc/systemd/system/sriov-vfs.service << 'EOF'
[Unit]
Description=Enable SR-IOV Virtual Functions
After=network.target

[Service]
Type=oneshot
ExecStart=/bin/bash -c 'echo 8 > /sys/class/net/ens1f0/device/sriov_numvfs'
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
EOF

systemctl enable sriov-vfs.service
```

**Step 4: Configure VFs**:

```bash
# Assign MAC addresses to VFs
ip link set ens1f0 vf 0 mac 52:54:00:00:00:01
ip link set ens1f0 vf 1 mac 52:54:00:00:00:02
ip link set ens1f0 vf 2 mac 52:54:00:00:00:03
# ... for each VF

# Optional: Set VLAN for VF
ip link set ens1f0 vf 0 vlan 100

# Optional: Set rate limit (Mbps)
ip link set ens1f0 vf 0 max_tx_rate 1000  # 1 Gbps max

# Optional: Enable trust mode (allows VM to change some settings)
ip link set ens1f0 vf 0 trust on

# Optional: Spoofcheck (prevent MAC/VLAN spoofing)
ip link set ens1f0 vf 0 spoofchk on
```

**Step 5: List VFs**:

```bash
# Show VF information
ip link show ens1f0

Output:
4: ens1f0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500
    vf 0 MAC 52:54:00:00:00:01, vlan 100, spoof checking on, link-state auto
    vf 1 MAC 52:54:00:00:00:02, spoof checking on, link-state auto
    vf 2 MAC 52:54:00:00:00:03, spoof checking on, link-state auto
    ...

# Find VF PCI addresses
lspci | grep "Virtual Function"

Output:
01:02.0 Ethernet: Intel X710 Virtual Function
01:02.1 Ethernet: Intel X710 Virtual Function
01:02.2 Ethernet: Intel X710 Virtual Function
...
```

**Step 6: Bind VF to VFIO (for passthrough)**:

```bash
# Bind VF to vfio-pci driver
# Get vendor:device ID
lspci -n -s 01:02.0
01:02.0 0200: 8086:154c

# Bind to vfio-pci
modprobe vfio-pci
echo "8086 154c" > /sys/bus/pci/drivers/vfio-pci/new_id

# Or automatic in /etc/modprobe.d/vfio.conf:
options vfio-pci ids=8086:154c
```

**Step 7: Assign VF to VM**:

**libvirt XML**:

```xml
<domain type='kvm'>
  <name>vm1</name>
  ...
  <devices>
    <!-- Assign VF 0 -->
    <interface type='hostdev' managed='yes'>
      <source>
        <address type='pci' domain='0x0000' bus='0x01' 
                 slot='0x02' function='0x0'/>
      </source>
      <mac address='52:54:00:00:00:01'/>
    </interface>
  </devices>
</domain>
```

**QEMU Command Line**:

```bash
qemu-system-x86_64 \
  -enable-kvm \
  -m 4G \
  -smp 4 \
  -drive file=vm1.qcow2 \
  -device vfio-pci,host=01:02.0 \
  ...
```

**Step 8: Verify in VM**:

```bash
# Inside VM after boot
lspci | grep Ethernet
00:05.0 Ethernet controller: Intel X710 Virtual Function

ip link show
eth0: <BROADCAST,MULTICAST> mtu 1500
    link/ether 52:54:00:00:00:01

# Configure network
ip link set eth0 up
ip addr add 192.168.1.100/24 dev eth0

# Test performance
iperf3 -c <server>
```

---

## SR-IOV in Different Environments

### VMware ESXi

**Enable SR-IOV**:

```
1. ESXi Host Configuration:
   ├── Hosts → Configure → Hardware → PCI Devices
   ├── Find SR-IOV capable NIC
   └── Toggle SR-IOV: Enabled

2. Reboot host

3. Set number of VFs:
   ├── Advanced Settings → SR-IOV
   └── Set max VFs per device
```

**Assign VF to VM**:

```
VM Settings:
├── Add → Network Adapter
├── Adapter Type: SR-IOV passthrough
├── Select physical NIC
└── VM gets VF automatically
```

### Docker/Kubernetes

**SR-IOV CNI Plugin**:

```yaml
# ConfigMap for SR-IOV CNI
apiVersion: v1
kind: ConfigMap
metadata:
  name: sriovdp-config
data:
  config.json: |
    {
      "resourceList": [{
        "resourceName": "intel_sriov_netdevice",
        "selectors": {
          "vendors": ["8086"],
          "devices": ["154c"],
          "drivers": ["iavf"]
        }
      }]
    }
```

**Pod with SR-IOV**:

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: sriov-pod
spec:
  containers:
  - name: app
    image: nginx
    resources:
      requests:
        intel.com/intel_sriov_netdevice: '1'
      limits:
        intel.com/intel_sriov_netdevice: '1'
```

### OpenStack

**Configure SR-IOV**:

```ini
# /etc/nova/nova.conf
[pci]
passthrough_whitelist = {"devname": "ens1f0", "physical_network": "physnet2"}

# /etc/neutron/plugins/ml2/sriov_agent.ini
[sriov_nic]
physical_device_mappings = physnet2:ens1f0
exclude_devices =
```

**Create SR-IOV Port**:

```bash
# Create SR-IOV network
openstack network create --provider-network-type vlan \
  --provider-physical-network physnet2 sriov-net

# Create port
openstack port create --network sriov-net \
  --vnic-type direct sriov-port

# Boot instance with SR-IOV
openstack server create --flavor m1.large \
  --image ubuntu --nic port-id=<port-id> vm1
```

---

## Performance

### Benchmarks

**Network Throughput** (10 Gbps NIC):

```
Bare Metal:           9.8 Gbps
SR-IOV VF:            9.7 Gbps (99%)
Virtio (optimized):   8.5 Gbps (87%)
Emulated (e1000):     6.0 Gbps (61%)

SR-IOV: ~99% of native performance!
```

**Latency** (Round-trip time):

```
Bare Metal:           1.0 µs
SR-IOV VF:            1.2 µs (+20%)
Virtio (optimized):   15 µs (+1400%)
Emulated (e1000):     50 µs (+4900%)

SR-IOV: Minimal latency overhead!
```

**CPU Overhead** (at 10 Gbps):

```
Bare Metal:           20% CPU
SR-IOV VF:            22% CPU
Virtio (optimized):   50% CPU
Emulated (e1000):     90% CPU

SR-IOV: Minimal CPU overhead!
```

**Packet Rate** (Small packets, 64 bytes):

```
Bare Metal:           14.8 Mpps
SR-IOV VF:            14.5 Mpps (98%)
Virtio (optimized):   8.0 Mpps (54%)
Emulated (e1000):     3.0 Mpps (20%)

SR-IOV: Excellent small packet performance!
```

### Why SR-IOV is Fast

```
Direct Data Path:
├── No VM exits (no hypervisor involvement)
├── No emulation/translation
├── DMA directly to/from VM memory
└── Hardware-based switching

Hardware Acceleration:
├── Checksum offloading
├── TSO/LSO
├── RSS (multi-queue)
├── VLAN offloading
└── All at native speed

Dedicated Resources:
├── Own TX/RX queues
├── Own interrupt vectors
├── No contention with other VFs
└── Predictable performance
```

---

## Limitations and Considerations

### 1. VF Count Limitations

```
Per-NIC VF Limit:
├── Intel X710: 128 VFs max
├── Mellanox ConnectX-5: 127 VFs max
├── Intel X520: 63 VFs max
└── Varies by model

Practical limits:
├── CPU cores (affinity)
├── PCIe bandwidth
├── IOMMU entries
└── Memory for queues

Example:
├── 10 Gbps NIC with 64 VFs
├── Each VF gets ~156 Mbps average
└── May be fine or insufficient (depends on use case)
```

### 2. Live Migration Challenges

```
Standard SR-IOV:
└── Cannot live migrate (same as PCI passthrough)

Reason:
├── VF state in hardware
├── Cannot capture/restore
└── Network interruption required

Solutions:
├── Bond VF + virtio (failover during migration)
├── Migration with downtime
└── Advanced: PCI hot-unplug/replug
```

**Migration Bonding** (Linux):

```bash
# In VM: Bond VF (primary) + virtio (backup)
# During migration:
#   1. Traffic fails over to virtio
#   2. VF detached
#   3. VM migrates
#   4. New VF attached
#   5. Traffic returns to VF

# Result: Brief performance dip, no downtime
```

### 3. Limited VF Features

```
VFs typically lack:
├── Full statistics/counters
├── Some advanced features
├── Self-configuration ability
└── Debugging capabilities

Host must configure:
├── VF MAC address
├── VF VLAN
├── VF rate limits
└── VF permissions
```

### 4. Security Considerations

```
MAC Spoofing:
├── VF could change its source MAC
├── Confuse network
└── Solution: Enable spoofchk

VLAN Hopping:
├── VF could inject VLAN tags
├── Access other VLANs
└── Solution: VLAN filtering by PF

Trust Mode:
├── Allowing VM to configure VF
├── Security vs flexibility tradeoff
└── Use cautiously
```

### 5. Driver Compatibility

```
Guest OS needs VF driver:
├── Linux: Usually in kernel (iavf, mlx5_core)
├── Windows: Vendor driver required
├── Older OS: May lack driver
└── Custom OS: May need porting

Check compatibility before deployment!
```

### 6. Failover Handling

```
Physical link down:
└── All VFs affected simultaneously

Solution: NIC bonding/teaming
├── Multiple physical NICs
├── VFs from different NICs
└── Redundancy maintained
```

---

## SR-IOV vs Alternatives

### Comparison Matrix

| Aspect | SR-IOV | Virtio | PCI Passthrough | DPDK |
|--------|--------|--------|-----------------|------|
| **Performance** | 99% | 85-90% | 99% | 99%+ |
| **VMs per NIC** | Many (64-128) | Unlimited | 1 | N/A (userspace) |
| **Live Migration** | Difficult | Easy | No | N/A |
| **Setup Complexity** | Medium | Low | Medium | High |
| **Driver Required** | VF driver | Virtio driver | Full driver | DPDK PMD |
| **Use Case** | Cloud/NFV | General VMs | Single high-perf VM | NFV/DPDK apps |
| **Flexibility** | Medium | High | Low | High (userspace) |
| **Hardware Req** | SR-IOV NIC | Any | Any | DPDK-capable NIC |

### When to Use Each

**Use SR-IOV when**:
```
✓ Need high performance for many VMs
✓ Have SR-IOV capable NICs
✓ Can tolerate migration complexity
✓ Cloud/multi-tenant environments
✓ NFV workloads

Example: OpenStack cloud with 100 VMs
```

**Use Virtio when**:
```
✓ Performance acceptable (85-90%)
✓ Live migration required
✓ Maximum flexibility needed
✓ No SR-IOV hardware
✓ General-purpose VMs

Example: Enterprise VM infrastructure
```

**Use PCI Passthrough when**:
```
✓ Single VM needs maximum performance
✓ No migration needed
✓ Dedicated hardware acceptable
✓ Special device features required

Example: ML training VM with GPU
```

**Use DPDK when**:
```
✓ Application can be DPDK-aware
✓ Need absolute maximum performance
✓ Kernel bypass acceptable
✓ NFV/packet processing workloads

Example: Software router/firewall
(Covered in DPDK section)
```

---

## Summary

**SR-IOV** enables:
- Multiple VMs to share one physical NIC
- Near-native performance (99%)
- Hardware-based isolation
- Scalability (64-128 VMs per NIC)

**Key Concepts**:
- **PF (Physical Function)**: Management interface (host)
- **VF (Virtual Function)**: Lightweight NIC (VM passthrough)
- Hardware creates VFs as separate PCI devices
- Each VF is passed through to VM

**Benefits**:
- High performance (native speed)
- High density (many VMs)
- Hardware isolation
- Low CPU overhead

**Limitations**:
- Requires SR-IOV capable NIC
- Limited VF count
- Live migration challenges
- VFs have reduced features

**Perfect For**:
- Cloud computing (multi-tenant)
- NFV (Network Function Virtualization)
- High-performance VM networking
- Environments needing both scale and performance

SR-IOV is the sweet spot between virtio (flexible but slower) and full PCI passthrough (fast but exclusive), making it ideal for modern cloud and virtualization environments!

---
# DPDK (Data Plane Development Kit): Complete Guide

## Table of Contents
1. [The Problem DPDK Solves](#the-problem-dpdk-solves)
2. [What is DPDK?](#what-is-dpdk)
3. [DPDK Architecture](#dpdk-architecture)
4. [How DPDK Works](#how-dpdk-works)
5. [Core DPDK Concepts](#core-dpdk-concepts)
6. [Setting Up DPDK](#setting-up-dpdk)
7. [DPDK in Practice](#dpdk-in-practice)
8. [Performance](#performance)
9. [DPDK Use Cases](#dpdk-use-cases)
10. [DPDK vs Traditional Networking](#dpdk-vs-traditional-networking)

---

## The Problem DPDK Solves

### Traditional Linux Network Stack Limitations

**The Performance Ceiling**:

```
Traditional Linux Network Path:

1. Packet arrives at NIC
   ↓
2. NIC generates interrupt
   ↓ (~1000 CPU cycles)
3. CPU stops current work
   ↓
4. Context switch to interrupt handler
   ↓ (~1000 CPU cycles)
5. Kernel network stack processing:
   ├── sk_buff allocation
   ├── Protocol layers (Ethernet → IP → TCP)
   ├── Socket lookup
   ├── Copy to socket buffer
   └── Wake up application
   ↓ (~5000 CPU cycles)
6. System call overhead
   ↓ (~1000 CPU cycles)
7. Application receives packet
   ↓
Total: ~8000-10000 CPU cycles per packet
```

**At High Packet Rates**:

```
10 Gbps line rate with 64-byte packets:
├── Maximum: 14.88 Million packets/sec (Mpps)
├── Per packet cost: 10,000 cycles
├── CPU needed: 148 Billion cycles/sec
└── That's ~74 CPU cores at 2 GHz!

Problem: Cannot achieve line rate!

Actual Linux performance:
└── ~3-5 Mpps per CPU core (not 14.88 Mpps)
```

### The Overhead Sources

**1. Interrupts**:
```
One interrupt per packet = Millions of interrupts
├── Each interrupt: CPU state save/restore
├── Cache pollution
└── Cannot scale
```

**2. Context Switches**:
```
Kernel ↔ User space transitions
├── Each switch: 1000+ cycles
├── TLB flushes
└── Cache misses
```

**3. Memory Copies**:
```
Packet data copied multiple times:
├── NIC → Kernel buffer
├── Kernel buffer → Socket buffer
├── Socket buffer → Application buffer
└── Each copy costs CPU cycles + memory bandwidth
```

**4. Kernel Complexity**:
```
Generic network stack:
├── Supports many protocols
├── Security features
├── Compatibility layers
├── Error handling
└── All add overhead
```

**5. Shared Data Structures**:
```
Multiple CPUs accessing same structures:
├── Locks and synchronization
├── Cache line bouncing
└── Serialization bottlenecks
```

### What High-Performance Apps Need

```
Requirements:
├── 10-100 Gbps throughput
├── Millions of packets per second
├── Sub-microsecond latency
├── Predictable performance
└── Maximum CPU efficiency

Examples:
├── Software routers/switches
├── Firewalls/IDS
├── Load balancers
├── Network Function Virtualization (NFV)
├── High-frequency trading
└── 5G packet processing
```

**Traditional Linux**: Cannot meet these requirements

**Solution Needed**: Bypass the kernel entirely!

---

## What is DPDK?

**DPDK (Data Plane Development Kit)** is a set of libraries and drivers for fast packet processing that **bypasses the kernel**.

### The Core Idea

**Instead of**:
```
Application → System Call → Kernel → NIC Driver → NIC
          (slow)
```

**DPDK does**:
```
Application → DPDK Library → NIC (Direct!)
          (fast)
```

**Visual Comparison**:

```
Traditional Linux:
┌─────────────────────────┐
│      Application        │
└───────────┬─────────────┘
            │ System calls
┌───────────┴─────────────┐
│    Kernel Network Stack │
│    - Protocol layers    │
│    - Socket management  │
│    - Interrupts         │
└───────────┬─────────────┘
            │
┌───────────┴─────────────┐
│    Kernel NIC Driver    │
└───────────┬─────────────┘
            │
         [NIC]


DPDK:
┌─────────────────────────┐
│   DPDK Application      │
│   - Packet processing   │
│   - Protocol stack      │
│   (in userspace!)       │
└───────────┬─────────────┘
            │ Direct access
┌───────────┴─────────────┐
│   DPDK Poll Mode Driver │
│   (Userspace Driver)    │
└───────────┬─────────────┘
            │ Memory mapped
         [NIC]

No kernel involvement!
```

### Key Characteristics

**1. Userspace**:
```
Everything runs in userspace:
├── Packet processing
├── Protocol handling
├── NIC driver (Poll Mode Driver)
└── No kernel involvement
```

**2. Polling (Not Interrupts)**:
```
CPU continuously polls NIC:
├── Check if packets arrived
├── Process batch
├── Repeat
└── No interrupts = No context switches
```

**3. Zero-Copy**:
```
Packet stays in one place:
├── NIC DMA → Memory pool
├── Application accesses directly
├── No copies
└── Maximum efficiency
```

**4. Hugepages**:
```
Use large memory pages (2MB/1GB):
├── Fewer TLB misses
├── Better memory performance
└── Reduced page table overhead
```

**5. CPU Affinity**:
```
Pin threads to specific CPU cores:
├── No context switching
├── Better cache utilization
└── Predictable performance
```

---

## DPDK Architecture

### Components

```
┌─────────────────────────────────────────────┐
│         DPDK Application Layer              │
│  ┌───────────────────────────────────────┐  │
│  │  Your Application                     │  │
│  │  (Router, Firewall, Load Balancer)   │  │
│  └───────────────┬───────────────────────┘  │
└──────────────────┼──────────────────────────┘
                   │
┌──────────────────┼──────────────────────────┐
│         DPDK Core Libraries                 │
│  ┌────────────────────────────────────────┐ │
│  │  rte_eal   - Environment Abstraction   │ │
│  │  rte_ring  - Lock-free queues          │ │
│  │  rte_mempool - Memory management       │ │
│  │  rte_mbuf  - Packet buffer management  │ │
│  │  rte_timer - High-precision timers     │ │
│  │  rte_hash  - Hash tables               │ │
│  │  rte_lpm   - Routing tables            │ │
│  └────────────────┬───────────────────────┘ │
└────────────────────┼─────────────────────────┘
                     │
┌────────────────────┼─────────────────────────┐
│         Poll Mode Drivers (PMDs)            │
│  ┌──────────────────────────────────────┐   │
│  │  Intel PMD (i40e, ixgbe, ice)        │   │
│  │  Mellanox PMD (mlx4, mlx5)           │   │
│  │  Broadcom PMD                         │   │
│  │  Virtual PMD (virtio, vmxnet3)       │   │
│  │  Crypto PMD (QAT, AES-NI)            │   │
│  └──────────────────┬───────────────────┘   │
└─────────────────────┼───────────────────────┘
                      │
┌─────────────────────┼───────────────────────┐
│            Hardware Layer                   │
│  ┌──────────────────────────────────────┐   │
│  │  NIC (via UIO/VFIO)                  │   │
│  │  - Memory mapped I/O                 │   │
│  │  - Direct memory access              │   │
│  └──────────────────────────────────────┘   │
└─────────────────────────────────────────────┘
```

### Memory Architecture

```
System Memory:
┌──────────────────────────────────────┐
│  Hugepages (2MB or 1GB pages)       │
│                                      │
│  ┌────────────────────────────────┐ │
│  │  Mempool 1 (RX packets)        │ │
│  │  ├── mbuf 0 [Packet data]      │ │
│  │  ├── mbuf 1 [Packet data]      │ │
│  │  ├── mbuf 2 [Packet data]      │ │
│  │  └── ... (pre-allocated)       │ │
│  └────────────────────────────────┘ │
│                                      │
│  ┌────────────────────────────────┐ │
│  │  Mempool 2 (TX packets)        │ │
│  │  └── ... (pre-allocated)       │ │
│  └────────────────────────────────┘ │
│                                      │
│  ┌────────────────────────────────┐ │
│  │  Ring Buffers (queues)         │ │
│  │  ├── Producer ptr              │ │
│  │  ├── Consumer ptr              │ │
│  │  └── Lock-free!                │ │
│  └────────────────────────────────┘ │
└──────────────────────────────────────┘
         ↑
         │ Direct access (memory mapped)
         │
      [NIC]
```

### Thread Model

```
CPU Cores:
┌─────────────────────────────────────┐
│  Core 0: Master thread              │
│  ├── Initialization                 │
│  ├── Management                     │
│  └── Statistics                     │
├─────────────────────────────────────┤
│  Core 1: RX thread                  │
│  ├── Poll NIC queue 0               │
│  ├── Process packets                │
│  └── Forward to TX                  │
├─────────────────────────────────────┤
│  Core 2: RX thread                  │
│  ├── Poll NIC queue 1               │
│  └── ...                            │
├─────────────────────────────────────┤
│  Core 3: Processing thread          │
│  ├── Application logic              │
│  └── Packet modification            │
├─────────────────────────────────────┤
│  Core 4: TX thread                  │
│  ├── Get packets from ring          │
│  └── Transmit via NIC               │
└─────────────────────────────────────┘

Each thread pinned to dedicated core
No context switching
```

---

## How DPDK Works

### Initialization Flow

```
DPDK Application Startup:

1. EAL Initialization
   ├── Parse command-line args
   ├── Setup hugepages
   ├── Initialize memory
   ├── Detect and probe PCI devices
   └── Setup per-core state

2. NIC Configuration
   ├── Bind NIC to DPDK driver (UIO/VFIO)
   ├── Configure queues (RX/TX)
   ├── Allocate memory pools
   └── Setup descriptors

3. Thread Setup
   ├── Launch threads on cores
   ├── Set CPU affinity
   └── Start polling loops

4. Run
   └── Main processing loop
```

### Receive Path (RX)

**Traditional Linux**:
```
Packet arrives → Interrupt → Kernel → Application
(10,000 CPU cycles)
```

**DPDK**:
```
Packet Processing Loop (per core):

while (1) {  // Infinite loop
    // 1. Poll NIC for packets (no interrupt!)
    nb_rx = rte_eth_rx_burst(port_id, queue_id, 
                             pkts, BURST_SIZE);
    
    // 2. Process batch of packets
    for (i = 0; i < nb_rx; i++) {
        process_packet(pkts[i]);
    }
    
    // 3. Transmit if needed
    rte_eth_tx_burst(port_id, queue_id, 
                    pkts, nb_tx);
}

Packet arrives:
1. NIC DMAs packet to pre-allocated mbuf
   └── Direct to hugepage memory
   
2. NIC updates descriptor
   └── Status: Ready to read
   
3. Application polls descriptor
   └── Detects new packet
   
4. Application reads packet
   └── Direct memory access (zero-copy)
   
5. Process packet
   └── All in userspace

Total: ~500-1000 CPU cycles!
(10× faster than kernel!)
```

### Transmit Path (TX)

```
DPDK Transmit:

1. Application allocates mbuf
   mbuf = rte_pktmbuf_alloc(mempool);

2. Write packet data
   rte_memcpy(mbuf->data, packet, len);

3. Set packet metadata
   mbuf->pkt_len = len;
   mbuf->data_len = len;

4. Send packet (batch)
   rte_eth_tx_burst(port, queue, &mbuf, 1);

5. NIC reads packet via DMA
   └── Directly from mbuf memory
   
6. NIC transmits
   └── No kernel involvement

7. Mbuf freed after transmission
   └── Returned to mempool for reuse
```

### Zero-Copy Mechanism

```
Memory Layout:

┌─────────────────────────────────┐
│  Hugepage Memory                │
│                                 │
│  Packet Buffer (mbuf):          │
│  ┌───────────────────────────┐  │
│  │ Metadata (headroom)       │  │
│  ├───────────────────────────┤  │
│  │ Packet Data               │◄─┼─── NIC DMA writes here
│  │ [Ethernet][IP][TCP][...]  │  │
│  ├───────────────────────────┤  │
│  │ Tailroom                  │  │
│  └───────────────────────────┘  │
│         ↑                        │
│         │                        │
│    Application reads here        │
│    (same memory!)                │
└─────────────────────────────────┘

No copying:
├── NIC → Memory (DMA)
├── Application → Same memory (direct access)
└── Memory → NIC (DMA)

Benefits:
├── No CPU cycles wasted
├── No memory bandwidth wasted
└── Lower latency
```

---

## Core DPDK Concepts

### 1. Environment Abstraction Layer (EAL)

**Purpose**: Provides common interface across platforms

```c
// Initialize DPDK
int ret = rte_eal_init(argc, argv);

// EAL handles:
├── Memory initialization (hugepages)
├── PCI device discovery
├── Thread management
├── CPU affinity
└── Platform-specific details

// Launch function on specific core
rte_eal_remote_launch(lcore_main, NULL, 1);
```

**EAL Parameters**:
```bash
./app -l 0-3 -n 4 -- <app-specific args>
       │    │
       │    └── Memory channels
       └── Cores to use (0,1,2,3)
```

### 2. Memory Pool (mempool)

**Purpose**: Pre-allocated memory for packets

```c
// Create memory pool
struct rte_mempool *pool;
pool = rte_pktmbuf_pool_create(
    "MBUF_POOL",        // Name
    NUM_MBUFS,          // Number of elements (e.g., 8192)
    CACHE_SIZE,         // Per-core cache
    0,                  // Private data size
    RTE_MBUF_DEFAULT_BUF_SIZE,  // Buffer size
    rte_socket_id()     // NUMA socket
);

// Allocate packet buffer
struct rte_mbuf *pkt = rte_pktmbuf_alloc(pool);

// Free packet buffer (returns to pool)
rte_pktmbuf_free(pkt);
```

**Why Mempool?**:
```
Pre-allocation benefits:
├── No malloc/free overhead
├── Cache-friendly (per-core cache)
├── Lock-free allocation
└── Predictable performance

Packet lifecycle:
1. Allocated from pool (fast)
2. Used by application
3. Returned to pool (fast)
4. Reused (no system call)
```

### 3. mbuf (Message Buffer)

**Purpose**: Packet buffer structure

```c
struct rte_mbuf {
    // Metadata
    uint16_t pkt_len;       // Total packet length
    uint16_t data_len;      // Data length in this segment
    void *buf_addr;         // Buffer virtual address
    
    // Offload flags
    uint64_t ol_flags;      // Offload features
    uint16_t vlan_tci;      // VLAN tag
    uint32_t rss_hash;      // RSS hash value
    
    // Chaining (for jumbo frames)
    struct rte_mbuf *next;  // Next segment
    
    // Room for packet data
    char data[...];
};

// Access packet data
char *pkt_data = rte_pktmbuf_mtod(mbuf, char *);
```

**mbuf Layout**:
```
┌────────────────────────────────┐
│ rte_mbuf structure (metadata)  │ ← Application sees this
├────────────────────────────────┤
│ Headroom (RTE_PKTMBUF_HEADROOM)│ ← For adding headers
├────────────────────────────────┤
│ Packet Data                    │ ← Actual packet
│ [Eth][IP][TCP][Payload]        │
├────────────────────────────────┤
│ Tailroom                       │ ← For appending data
└────────────────────────────────┘
```

### 4. Ring Buffers

**Purpose**: Lock-free queues for inter-thread communication

```c
// Create ring
struct rte_ring *ring;
ring = rte_ring_create(
    "PKT_RING",         // Name
    RING_SIZE,          // Size (must be power of 2)
    rte_socket_id(),    // NUMA socket
    RING_F_SP_ENQ | RING_F_SC_DEQ  // Single producer/consumer
);

// Enqueue (producer)
rte_ring_enqueue(ring, (void *)pkt);

// Dequeue (consumer)
void *pkt;
rte_ring_dequeue(ring, &pkt);

// Bulk operations (better performance)
rte_ring_enqueue_bulk(ring, pkts, n);
rte_ring_dequeue_bulk(ring, pkts, n);
```

**Ring Buffer Operation**:
```
Circular Buffer:
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 3 │ 4 │   │   │   │   │ 1 │ 2 │
└───┴───┴─↑─┴───┴───┴───┴─↑─┴───┘
          │               │
       Consumer        Producer
       (read)          (write)

Lock-free:
├── CAS (Compare-And-Swap) operations
├── No locks needed
├── High performance
└── Scales with cores
```

### 5. Poll Mode Drivers (PMDs)

**Purpose**: Userspace NIC drivers

```c
// Initialize PMD
rte_eth_dev_configure(port_id, n_rxq, n_txq, &port_conf);

// Setup RX queue
rte_eth_rx_queue_setup(port_id, queue_id, 
                       nb_desc, socket_id, 
                       &rx_conf, mempool);

// Setup TX queue
rte_eth_tx_queue_setup(port_id, queue_id,
                       nb_desc, socket_id,
                       &tx_conf);

// Start port
rte_eth_dev_start(port_id);

// Receive packets (polling)
nb_rx = rte_eth_rx_burst(port_id, queue_id,
                         pkts, BURST_SIZE);

// Transmit packets
nb_tx = rte_eth_tx_burst(port_id, queue_id,
                         pkts, nb_pkts);
```

**Supported NICs**:
```
Intel:
├── ixgbe (82599, X520, X540)
├── i40e (X710, XL710)
├── ice (E810)
└── ixgbevf (SR-IOV VF)

Mellanox:
├── mlx4 (ConnectX-3)
├── mlx5 (ConnectX-4/5/6)

Broadcom:
└── bnxt

Virtual:
├── virtio
├── vmxnet3
└── vhost
```

---

## Setting Up DPDK

### System Preparation

**1. Install Dependencies**:

```bash
# Ubuntu/Debian
sudo apt-get install build-essential python3-pip \
    libnuma-dev python3-pyelftools

# RHEL/CentOS
sudo yum install gcc python3-pip numactl-devel \
    python3-pyelftools
```

**2. Configure Hugepages**:

```bash
# Check current hugepage config
grep Huge /proc/meminfo

# Allocate 2MB hugepages (1024 pages = 2GB)
echo 1024 > /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages

# Or 1GB hugepages (4 pages = 4GB)
echo 4 > /sys/kernel/mm/hugepages/hugepages-1048576kB/nr_hugepages

# Make permanent in /etc/sysctl.conf
vm.nr_hugepages = 1024

# Mount hugetlbfs (usually automatic)
mkdir -p /mnt/huge
mount -t hugetlbfs nodev /mnt/huge

# Add to /etc/fstab for persistence
nodev /mnt/huge hugetlbfs defaults 0 0
```

**3. Bind NIC to DPDK Driver**:

```bash
# Load kernel modules
modprobe uio
modprobe uio_pci_generic
# Or for better isolation:
modprobe vfio-pci

# Bind NIC to DPDK-compatible driver
# Using dpdk-devbind.py script
./dpdk-devbind.py --status  # Check current status

# Bind to uio_pci_generic
./dpdk-devbind.py --bind=uio_pci_generic 0000:01:00.0

# Or bind to vfio-pci (recommended)
./dpdk-devbind.py --bind=vfio-pci 0000:01:00.0

# Unbind (return to kernel)
./dpdk-devbind.py --bind=i40e 0000:01:00.0
```

### Building DPDK

**Method 1: Meson (Modern)**:

```bash
# Download DPDK
wget https://fast.dpdk.org/rel/dpdk-21.11.tar.xz
tar xf dpdk-21.11.tar.xz
cd dpdk-21.11

# Build with meson
meson build
cd build
ninja
sudo ninja install

# Set environment
export PKG_CONFIG_PATH=/usr/local/lib64/pkgconfig
```

**Method 2: Make (Legacy)**:

```bash
# Set target
export RTE_TARGET=x86_64-native-linux-gcc
export RTE_SDK=/path/to/dpdk

# Build
make install T=$RTE_TARGET
```

### Simple DPDK Application

```c
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>

#define RX_RING_SIZE 1024
#define TX_RING_SIZE 1024
#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

static struct rte_mempool *mbuf_pool;

// Main packet processing loop
static int lcore_main(void *arg) {
    uint16_t port = 0;
    struct rte_mbuf *bufs[BURST_SIZE];
    
    printf("Core %u forwarding packets.\n", rte_lcore_id());
    
    while (1) {
        // Receive packets
        const uint16_t nb_rx = rte_eth_rx_burst(port, 0,
                                                bufs, BURST_SIZE);
        
        if (unlikely(nb_rx == 0))
            continue;
        
        // Simple forwarding: send packets back out
        const uint16_t nb_tx = rte_eth_tx_burst(port, 0,
                                                bufs, nb_rx);
        
        // Free unsent packets
        if (unlikely(nb_tx < nb_rx)) {
            for (uint16_t i = nb_tx; i < nb_rx; i++)
                rte_pktmbuf_free(bufs[i]);
        }
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    // Initialize EAL
    int ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "EAL initialization failed\n");
    
    // Create mbuf pool
    mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL",
        NUM_MBUFS, MBUF_CACHE_SIZE, 0,
        RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    
    // Configure port
    uint16_t port = 0;
    struct rte_eth_conf port_conf = {};
    rte_eth_dev_configure(port, 1, 1, &port_conf);
    
    // Setup RX queue
    rte_eth_rx_queue_setup(port, 0, RX_RING_SIZE,
        rte_eth_dev_socket_id(port), NULL, mbuf_pool);
    
    // Setup TX queue
    rte_eth_tx_queue_setup(port, 0, TX_RING_SIZE,
        rte_eth_dev_socket_id(port), NULL);
    
    // Start port
    rte_eth_dev_start(port);
    
    // Launch processing on cores
    rte_eal_mp_remote_launch(lcore_main, NULL, CALL_MAIN);
    
    return 0;
}
```

**Compile**:

```bash
gcc -o dpdk_fwd dpdk_fwd.c \
    $(pkg-config --cflags --libs libdpdk)
```

**Run**:

```bash
sudo ./dpdk_fwd -l 0-1 -n 4 -- <app args>
```

---

## DPDK in Practice

### Real-World Applications

**1. VPP (Vector Packet Processing)**:
```
Cisco's high-performance packet processing
├── Built on DPDK
├── Software router/switch
├── Multi-Gbps throughput
└── Used in NFV deployments
```

**2. DPDK-based OVS (Open vSwitch)**:
```
Accelerated virtual switching:
├── Standard OVS: ~5 Mpps
├── OVS-DPDK: ~15-20 Mpps
└── 3-4× performance improvement
```

**3. SPDK (Storage Performance Development Kit)**:
```
Similar to DPDK but for storage:
├── NVMe over Fabrics
├── Userspace storage stack
└── Millions of IOPS
```

**4. Snort/Suricata with DPDK**:
```
IDS/IPS with DPDK acceleration:
├── Multi-Gbps threat detection
├── Minimal packet loss
└── Real-time analysis
```

### DPDK with Containers

**Docker**:

```dockerfile
FROM ubuntu:20.04

# Install DPDK
RUN apt-get update && \
    apt-get install -y dpdk dpdk-dev

# Copy application
COPY dpdk_app /app/

# Privileged mode needed for hugepages/devices
CMD ["/app/dpdk_app", "-l", "0-1", "-n", "4"]
```

**Run with privileges**:

```bash
docker run --privileged \
    -v /mnt/huge:/mnt/huge \
    -v /dev:/dev \
    dpdk_container
```

**Kubernetes with SR-IOV + DPDK**:

```yaml
apiVersion: v1
kind: Pod
spec:
  containers:
  - name: dpdk-app
    image: dpdk_container
    securityContext:
      privileged: true
    resources:
      requests:
        hugepages-1Gi: 2Gi
        intel.com/intel_sriov: '2'
      limits:
        hugepages-1Gi: 2Gi
        intel.com/intel_sriov: '2'
    volumeMounts:
    - name: hugepage
      mountPath: /mnt/huge
  volumes:
  - name: hugepage
    emptyDir:
      medium: HugePages
```

---

## Performance

### Benchmarks

**Packet Throughput**:

```
Hardware: Intel X710 (10 Gbps), 64-byte packets

Linux kernel (no tuning):
└── ~3 Mpps per core

Linux kernel (tuned, NAPI):
└── ~5 Mpps per core

DPDK:
└── ~14.88 Mpps per core (line rate!)

Result: 3-5× improvement
```

**Latency**:

```
Round-trip latency (software forwarding):

Linux kernel:
└── ~50-100 microseconds

DPDK:
└── ~5-10 microseconds

Result: 10× improvement
```

**CPU Efficiency**:

```
10 Gbps throughput:

Linux kernel:
└── 4-6 CPU cores

DPDK:
└── 1-2 CPU cores

Result: 3× better CPU efficiency
```

### Scaling

**Multi-Core Scaling**:

```
1 core:  14.88 Mpps
2 cores: 29.76 Mpps (100% scaling)
4 cores: 59.52 Mpps (100% scaling)
8 cores: ~100 Mpps (limited by NIC)

Near-linear scaling with cores!
```

---

## DPDK Use Cases

### 1. Network Function Virtualization (NFV)

```
Virtual Network Functions on DPDK:
├── vRouter (routing)
├── vFirewall
├── vLoadBalancer
├── vIDS/IPS
└── vDPI (Deep Packet Inspection)

Benefits:
├── 10-100 Gbps performance
├── Software flexibility
├── Cost savings vs hardware
└── Rapid deployment
```

### 2. Software Load Balancers

```
DPDK-based load balancers:
├── HAProxy with DPDK
├── Facebook's Katran
├── Google's Maglev

Performance:
├── Millions of connections/sec
├── Sub-millisecond latency
└── Handles DDoS traffic
```

### 3. 5G/Packet Core

```
5G infrastructure:
├── UPF (User Plane Function)
├── High packet rates
├── Low latency requirements
└── DPDK enables software implementation
```

### 4. High-Frequency Trading

```
Financial systems:
├── Ultra-low latency required
├── Sub-microsecond network processing
├── DPDK provides predictable performance
└── Kernel bypass critical
```

### 5. Content Delivery Networks (CDN)

```
Edge servers:
├── High request rates
├── DPDK-accelerated HTTP
├── Better cost/performance
└── Smaller footprint
```

---

## DPDK vs Traditional Networking

### Comparison Summary

| Aspect | Linux Kernel | DPDK |
|--------|-------------|------|
| **Mode** | Kernel space | User space |
| **I/O Model** | Interrupt-driven | Polling |
| **Throughput (10G)** | 3-5 Mpps/core | 14.88 Mpps/core |
| **Latency** | 50-100 µs | 5-10 µs |
| **CPU Efficiency** | Lower | Higher (3×) |
| **Memory Copies** | Multiple | Zero-copy |
| **Context Switches** | Many | None |
| **CPU Dedication** | Shared | Dedicated (polling) |
| **Application Complexity** | Simple (sockets) | Complex (DPDK APIs) |
| **Debugging** | Standard tools | Specialized tools |
| **Flexibility** | High | Medium |
| **Learning Curve** | Low | High |

### When to Use DPDK

**Use DPDK when**:
```
✓ Need maximum throughput (>5 Mpps)
✓ Need low latency (<10 µs)
✓ Packet processing is main workload
✓ Have dedicated CPU cores
✓ Application can be rewritten
✓ NFV/packet processing use case

Examples:
├── Software routers
├── Firewalls
├── Load balancers
├── DPI engines
└── Packet capture/analysis
```

**Use Kernel Networking when**:
```
✓ Standard application (web, DB, etc.)
✓ Performance adequate (<5 Gbps)
✓ Need full protocol stack
✓ Want simple development
✓ Share CPU with other work
✓ Standard sockets interface

Examples:
├── Web servers
├── Databases
├── Application servers
└── General enterprise apps
```

---

## Summary

**DPDK** achieves maximum network performance by:
- **Kernel bypass**: No kernel involvement
- **Polling**: No interrupts, no context switches
- **Zero-copy**: Direct memory access
- **Hugepages**: Reduced TLB misses
- **CPU pinning**: Dedicated cores, better cache

**Performance gains**:
- 3-5× higher throughput vs kernel
- 10× lower latency
- Near-linear multi-core scaling
- Line-rate processing possible

**Key concepts**:
- **EAL**: Environment abstraction
- **mempool**: Pre-allocated packet buffers
- **mbuf**: Packet buffer structure
- **Ring**: Lock-free queues
- **PMD**: Poll Mode Drivers (userspace)

**Trade-offs**:
- Complexity: Harder to develop than sockets
- CPU dedication: Cores busy-waiting
- Portability: Requires DPDK-compatible NICs
- Debugging: More difficult than kernel

**Perfect for**:
- NFV (Network Function Virtualization)
- Software routers/switches/firewalls
- High-performance packet processing
- Low-latency networking
- Situations where network is the bottleneck

DPDK represents the ultimate in software network performance, achieving near-hardware speeds through aggressive optimization and kernel bypass!

---