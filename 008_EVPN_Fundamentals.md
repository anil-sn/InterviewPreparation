# EVPN : Understanding the Protocol From Ground Up

## What This Collection Covers

This set of documents provides complete, detailed coverage of EVPN—Ethernet VPN—from fundamental concepts to operational deployment. Each document focuses on a specific aspect of EVPN, building knowledge progressively. The documents are written to be read in sequence or used individually as references.

EVPN is not simple. It solves complex problems in data center networking through careful protocol design that leverages BGP's proven scalability. Understanding EVPN requires understanding the problems it solves, the mechanisms it uses, and how these mechanisms interact. These documents provide that understanding without sugarcoating the complexity.

## Document Sequence and Purpose

### EVPN Fundamentals - Problems and Solutions

This document establishes why EVPN exists by examining the problems with traditional Layer 2 networking that made EVPN necessary. The VLAN limitation of 4,096 segments is not a minor inconvenience—it is a complete barrier to building modern multi-tenant infrastructure. Flood-and-learn does not just waste bandwidth—it makes large networks unmanageable. ARP broadcasts do not merely create overhead—they can bring networks down through quadratic scaling.

Each problem is explained in detail, showing exactly why traditional approaches fail at scale. Then the document shows how EVPN solves each problem. The VNI provides 16 million segments. Proactive advertisement eliminates flooding. ARP suppression makes broadcasts unnecessary. The fundamental shift from reactive to proactive operation is the insight that makes EVPN work.

Reading this document first establishes the context. You understand not just what EVPN does but why it does it. The protocol features make sense because you see the problems they solve.

### EVPN Route Type 2 - MAC/IP Advertisement

Type 2 routes are the heart of EVPN. Every MAC address generates a Type 2 route. Every forwarding decision relies on information from Type 2 routes. This document explains the anatomy of Type 2 routes—what fields they contain and what each field means.

The document traces how Type 2 routes are generated when endpoints appear, how they propagate via BGP, and how receiving VTEPs process them to populate forwarding tables. The complete flow from server boot to forwarding table population is shown step by step.

Type 2 routes handle MAC mobility explicitly through route advertisements and withdrawals. The document explains how mobility works, why it is reliable, and how it differs from traditional implicit learning. Understanding Type 2 routes is understanding how EVPN replaces flood-and-learn with proactive distribution.

### EVPN Route Type 3 - Inclusive Multicast Route

While Type 2 handles most traffic through direct forwarding, broadcast and multicast traffic still exists. Type 3 routes establish which VTEPs participate in which VNIs, enabling selective broadcast distribution.

The document explains ingress replication—how the source VTEP creates copies for each destination—and how this differs from multicast distribution. It shows how broadcast traffic is handled efficiently without flooding throughout the entire network.

Type 3 routes also handle unknown unicast as a fallback. In well-operating networks, unknown unicast is rare because Type 2 advertisements provide proactive learning, but Type 3 ensures even edge cases work correctly.

### EVPN Route Type 5 - IP Prefix Routes

VNIs provide isolation, but complete isolation is often too restrictive. Applications in different VNIs need controlled communication. Type 5 routes enable inter-VNI routing by advertising IP prefixes rather than individual hosts.

The document explains symmetric and asymmetric routing models and how Type 5 routes support both. It shows how distributed anycast gateway eliminates centralized routing bottlenecks. Type 5 routes extend EVPN from pure Layer 2 to integrated Layer 2/Layer 3 networking.

### EVPN Route Types 1 and 4 - Multihoming Support

Multihoming—connecting a device to multiple switches—requires coordination to avoid loops and duplication. Type 1 and Type 4 routes provide this coordination.

The document explains Ethernet Segment Identifiers, how switches discover shared access to multihomed devices, and how Designated Forwarder election ensures broadcasts are delivered once rather than duplicated. Split-horizon filtering prevents forwarding loops.

Most deployments will not heavily use multihoming, but for critical infrastructure requiring redundancy, these route types are essential.

### EVPN and BGP Integration

EVPN runs on top of BGP. Understanding this integration is crucial. The document explains why BGP was chosen—its proven scalability, existing operational knowledge, and vendor support.

Multi-Protocol BGP extensions allow the same BGP session to carry both underlay IP routing and overlay EVPN information. Route reflector architecture scales BGP to thousands of leaves without requiring full mesh sessions. Route Targets control which VTEPs import which routes, keeping control plane state manageable.

The document shows BGP session establishment, route encoding in BGP UPDATE messages, and how BGP's mechanisms handle graceful restart and failure detection.

### EVPN Complete Flow - Step by Step

This document traces EVPN deployment from bare metal to production traffic. Starting with switches that have no configuration, it builds underlay connectivity, establishes BGP sessions, enables EVPN, configures VNIs, and verifies traffic flow.

Each step is explained with the actual configuration required and verification commands to confirm each layer is working. The document shows Type 3 advertisement when VNIs are configured, Type 2 advertisement when endpoints appear, ARP suppression working, and data plane forwarding succeeding.

This practical walkthrough makes EVPN concrete rather than abstract. You see exactly how the protocol is deployed and verified in real networks.

### EVPN Troubleshooting and Failure Analysis

When EVPN is not working, systematic troubleshooting finds the problem. This document provides a methodology for diagnosing issues.

Troubleshooting proceeds bottom-up. Underlay problems prevent everything else from working. BGP session problems prevent route distribution. Route advertisement or import problems prevent forwarding table population. Data plane problems prevent traffic flow despite correct control plane state.

The document covers common misconfigurations, expected behavior during failures, and a systematic workflow for problem identification. EVPN troubleshooting is not guesswork—each layer can be verified, and problems can be isolated methodically.

## Key Concepts Across Documents

Several concepts appear throughout multiple documents because they are fundamental to EVPN:

**Proactive vs Reactive**: Traditional networking is reactive—waiting for traffic and responding to it. EVPN is proactive—advertising information before traffic flows. This shift eliminates flooding and provides instant knowledge of network state.

**Control Plane and Data Plane Separation**: BGP distributes control plane information about where MACs are located. VXLAN data plane encapsulates and forwards traffic based on this information. The separation allows each to be optimized independently.

**Route Targets for Filtering**: Not every VTEP needs to know about every MAC in every VNI. Route Targets allow selective import, keeping control plane state manageable even in very large networks.

**BGP as Foundation**: Extending BGP rather than creating a new protocol leverages existing scalability, tools, and operational knowledge. The same route reflectors and session management serve both underlay and overlay.

**VNI for Segmentation**: The 24-bit VNI space eliminates VLAN's numerical constraint completely. Segmentation is no longer a limiting factor in network design.

## How to Use This Collection

For learning EVPN from scratch, read the documents in order. Start with the problems and solutions document to understand why EVPN exists. Work through the route type documents to understand how information is distributed. Read the BGP integration document to understand the control plane. Follow the complete flow document to see practical deployment. Reference the troubleshooting document when issues arise.

For reference, each document stands alone. Need to understand Type 2 routes? Read that document. Deploying multihoming? Read the Type 1 and Type 4 document. Troubleshooting an issue? The troubleshooting document provides systematic guidance.

The documents avoid bullet points in favor of flowing prose that builds concepts progressively. They prioritize brutal clarity over comfort—complexity is not hidden, problems are not minimized, and failures are examined honestly. The goal is genuine understanding, not superficial familiarity.

## What Makes EVPN Work

EVPN works because it was designed to solve real problems at scale. The protocol did not emerge from theoretical exploration but from practical necessity. Cloud providers and large data centers hit the limits of traditional networking and needed something better.

The key insight is proactive advertisement. By advertising MAC locations before traffic flows, EVPN eliminates the entire class of problems caused by reactive learning and flooding. The bandwidth saved, the latency predictability gained, and the scale enabled are not incremental improvements—they are transformative.

BGP as the distribution mechanism was the right choice. BGP scales to Internet size, handles policy elegantly through Route Targets, and is universally supported. Extending BGP rather than creating a new protocol reduced deployment friction and leveraged existing infrastructure.

The route types are carefully designed for their specific purposes. Type 2 for MAC learning, Type 3 for broadcast distribution, Type 5 for routing, Types 1 and 4 for multihoming—each serves a distinct function, and together they provide complete functionality.

## Where EVPN Fits

EVPN is the control plane for overlay networks. It distributes the information that makes VXLAN data plane forwarding work efficiently. It integrates with BGP for underlay routing, creating a unified control plane that handles both physical network routing and virtual network operation.

For AI data centers, EVPN provides the scalable network segmentation required to isolate different training jobs, different users, and different environments. The elimination of flooding and broadcast storms ensures predictable performance for latency-sensitive collective operations. The ability to create millions of network segments enables the multi-tenancy required for shared infrastructure.

Understanding EVPN means understanding modern data center networking. The protocol has become the standard not because of marketing or vendor lock-in but because it solves real problems better than alternatives. These documents provide the understanding needed to deploy, operate, and troubleshoot EVPN networks effectively.

# EVPN Fundamentals: The Problems That Demanded a Solution

## What EVPN Actually Is

EVPN is Ethernet VPN. It is a control plane protocol that runs on top of BGP. The protocol distributes Layer 2 and Layer 3 reachability information across a network. When we say it runs on BGP, we mean it extends BGP by defining a new address family called L2VPN EVPN. This address family allows BGP to carry Ethernet MAC addresses, IP-to-MAC mappings, and virtual network membership information instead of just IP prefixes.

EVPN was standardized in RFC 7432. It was created to solve specific, painful problems that made traditional data center networking unable to scale. Understanding EVPN means understanding these problems first. The protocol exists because the old ways failed catastrophically at the scales modern infrastructure demands.

## Problem One: VLAN Limitation is a Hard Wall

Traditional Ethernet networks use VLANs to create isolated broadcast domains. VLAN stands for Virtual LAN. The idea is simple: you configure devices into VLAN 10, and they can communicate with each other but are isolated from devices in VLAN 20. This segmentation provides security, traffic isolation, and organizational structure.

The VLAN ID is carried in the Ethernet frame header using the 802.1Q standard. The VLAN ID field is exactly 12 bits. This is not a configuration choice or a vendor limitation. This is the standard. Twelve bits gives you 4,096 possible values. Some of these values are reserved, leaving approximately 4,094 usable VLANs.

For a small business with 50 employees, 4,094 VLANs seems like infinity. For a data center hosting 10,000 customers, each requiring their own isolated network, 4,094 is laughably inadequate. You need more than twice that number just for the customers, and you have not allocated VLANs for management, storage, or internal services yet.

Cloud providers have this problem multiplied. Amazon Web Services has millions of customers. Each customer deploys multiple applications. Each application might need multiple network segments for different tiers. The VLAN ID space is exhausted before you even begin serving customers.

Container orchestration platforms face similar constraints. Kubernetes might create hundreds or thousands of network segments for different applications and namespaces. The 4,096 limit becomes a constraint that forces ugly workarounds—VLAN reuse, complex segmentation strategies, or simply telling customers no.

This is not a problem you can engineer around within the VLAN framework. The 12-bit field is part of the Ethernet standard. Changing it would require changing every network interface card, every switch, and every piece of software that processes Ethernet frames. This is not happening. The industry chose a different path: abandon VLANs for segmentation and build something new.

## EVPN Solution: The VNI Breaks the Ceiling

EVPN, when used with VXLAN encapsulation, introduces the VNI—VXLAN Network Identifier. The VNI is a 24-bit field. Twenty-four bits provides 16,777,216 possible values. This is not 4,096. This is not double or triple the VLAN limit. This is four thousand times larger.

With 16 million VNIs available, the segmentation constraint disappears. A cloud provider can assign each customer a unique VNI and still have capacity for millions more customers. A container platform can create VNIs dynamically for every application without worrying about exhaustion. The numerical limitation that made VLANs unworkable at scale simply does not exist with VNIs.

EVPN distributes information about VNIs across the network. When a device joins VNI 10100, EVPN advertisements inform all relevant switches. When MAC addresses appear in VNI 10100, EVPN distributes their locations. The VNI provides isolation equivalent to VLANs but without the crippling numerical constraint.

## Problem Two: Flood-and-Learn is Bandwidth Suicide

Traditional Ethernet switches learn MAC addresses by observing traffic. When a switch receives a frame, it examines the source MAC address and records which port that frame arrived on. Over time, the switch builds a table mapping MAC addresses to ports. This is called learning.

When a switch needs to forward a frame to a destination MAC it has not learned yet, it has no choice but to flood. Flooding means sending the frame out every port except the port it arrived on. The hope is that one of those ports leads to the destination, which will then respond, allowing the switch to learn the destination's location.

This flood-and-learn mechanism breaks down catastrophically at scale. Consider a data center with 100 switches. When a new server boots up, no switch knows its MAC address yet. The first frame destined for this server triggers flooding on the local switch. That switch forwards copies to neighboring switches. Those switches flood to their neighbors. The frame propagates throughout the network until it reaches the destination or exhausts all possibilities.

In a 100-switch network, a single unknown MAC causes potentially hundreds of frame copies to be transmitted. If you have 10,000 servers booting up, migrating, or otherwise generating unknown MACs, the flood traffic becomes enormous. You are consuming gigabits or terabits of bandwidth sending traffic to places that do not need it and will discard it.

The learning process is also reactive and slow. There is a delay between when a MAC first appears and when all switches learn about it. During this delay, traffic to that MAC must be flooded. For latency-sensitive applications, this unpredictable delay is unacceptable. Sometimes traffic forwards directly in microseconds. Sometimes it floods for seconds while learning completes. This inconsistency makes performance unpredictable.

Virtualized environments make this worse. Virtual machines migrate between hosts constantly. Each migration invalidates learned MAC addresses. Containers are created and destroyed continuously. Each change requires relearning. In a dynamic data center, the network is in perpetual learning mode, constantly flooding, constantly consuming bandwidth for no useful purpose.

Broadcast traffic amplifies the problem. When any device sends a broadcast frame, every switch in the broadcast domain must replicate and forward it to every port. In a 100-switch network, one broadcast becomes tens of thousands of frame transmissions. If multiple devices broadcast simultaneously, the aggregate load can overwhelm switches, causing packet loss and cascading failures. This is why large flat Layer 2 networks are notoriously unstable.

## EVPN Solution: Proactive Advertisement Eliminates Flooding

EVPN inverts the learning model. Instead of switches reactively learning by observing traffic, switches proactively advertise what they know. When a server boots up and sends its first frame, the local switch learns the server's MAC address through normal Ethernet learning. But instead of stopping there, the switch immediately creates a BGP route—specifically, an EVPN Type 2 route—advertising this MAC address.

This advertisement is distributed via BGP to all other switches in the network. Within seconds, every switch knows about the new MAC address and knows exactly which switch it is behind. When any switch needs to forward traffic to this MAC, it already has the information. It does not flood. It does not wait for learning. It immediately encapsulates the frame and sends it directly to the correct destination switch.

The elimination of flooding is not partial. It is total. In normal operation, once EVPN routes have propagated, there is no flooding. A switch either knows where a MAC is located, or the MAC does not exist. There is no intermediate state of uncertainty that requires flooding. The bandwidth that was wasted on flood traffic is now available for actual data.

The performance predictability improves dramatically. Every forwarding decision happens in the same way—table lookup, encapsulation, direct forwarding. There are no special cases where flooding adds delay. Latency becomes consistent and deterministic. For AI training workloads where collective operations require synchronized communication, this predictability is critical.

## Problem Three: ARP Broadcasts Create Quadratic Scaling

The Address Resolution Protocol maps IP addresses to MAC addresses. When a device wants to send an IP packet but does not know the destination's MAC address, it broadcasts an ARP request. The broadcast says "whoever has IP address X, please respond with your MAC address." Every device in the broadcast domain receives this request, but only the device with that IP address responds.

In a small network, ARP overhead is negligible. In a large network, it becomes a significant problem. Consider a data center with 10,000 servers. Each server periodically performs ARP lookups for various connections—perhaps once per second on average. That is 10,000 ARP broadcasts per second flooding through the network.

Every one of these 10,000 broadcasts must be received by all 10,000 servers. Each server's network interface must interrupt the CPU to process the ARP packet, determine it is not the target, and discard it. Ten thousand servers each processing 10,000 ARP packets per second means 100 million ARP packet receptions per second. This consumes CPU cycles that could be used for actual work.

The network itself must carry this broadcast traffic. Switches must replicate each broadcast to all ports. With 10,000 servers across hundreds of switches, each ARP broadcast generates potentially tens of thousands of frame transmissions across the network. This consumes bandwidth that could carry application data.

The scaling is worse than linear. If you double the number of servers, you double the number of ARP broadcasts being generated, and you double the number of destinations each broadcast must reach. The total ARP traffic quadruples. This quadratic scaling means ARP overhead grows faster than the network itself grows.

ARP storms can bring networks down entirely. If a misconfigured device or a malicious actor generates excessive ARP broadcasts, every device in the broadcast domain is overwhelmed processing these broadcasts. Switches struggle to replicate and forward the flood of traffic. Legitimate traffic is delayed or dropped. The network degrades or fails completely.

## EVPN Solution: ARP Suppression Makes Broadcasts Unnecessary

EVPN Type 2 routes carry both MAC addresses and IP addresses. When a switch learns a MAC address from a server's frame, it often can also learn the associated IP address from the same frame or from ARP traffic the server generates. The EVPN advertisement includes both pieces of information.

Every switch receiving this EVPN route now knows the IP-to-MAC binding. The switch builds a table mapping IP addresses to MAC addresses, populated by EVPN advertisements rather than by observing ARP traffic.

When a server sends an ARP request, the local switch intercepts it. Before forwarding the broadcast, the switch checks its EVPN-learned IP-to-MAC table. If the table contains the requested IP address, the switch generates an ARP reply immediately using the information from EVPN. The switch sends this reply to the requesting server. The ARP request is never broadcast. It never leaves the local switch.

From the server's perspective, it sent an ARP request and received an ARP reply, exactly as it would in a traditional network. The server does not know and does not care that the reply came from the switch rather than from the actual destination. The protocol worked correctly, but the broadcast was suppressed.

This ARP suppression scales linearly instead of quadratically. As the network grows, the number of EVPN advertisements grows linearly with the number of endpoints. But the number of ARP broadcasts approaches zero because most are suppressed locally. A 10,000-server network generates perhaps tens of thousands of EVPN routes, but generates virtually no ARP broadcast traffic.

The CPU overhead on servers drops dramatically. Servers no longer receive thousands of irrelevant ARP broadcasts per second. Their network interfaces no longer interrupt the CPU to process and discard these broadcasts. The CPU is free to do actual work.

The network bandwidth previously consumed by ARP broadcasts is reclaimed. The switches are not replicating and forwarding broadcasts. The bandwidth is available for application data. In a high-performance network serving AI workloads, every bit of bandwidth matters, and ARP suppression ensures bandwidth is not wasted on unnecessary control traffic.

## Problem Four: MAC Mobility is Chaos Without Explicit Signaling

Virtual machines and containers do not stay in one place. They migrate between hosts for load balancing, for maintenance, or because orchestration systems reschedule them. When a workload moves, its MAC address and IP address move with it. The network must adapt to this new location quickly and correctly, or traffic fails.

Traditional networks handle mobility poorly. When a VM moves from Host A to Host B, initially the network still believes the VM is at Host A because that is what it learned. Traffic continues flowing to Host A, where the VM no longer exists. The traffic is black-holed.

Eventually, when the VM sends a frame from its new location at Host B, switches begin learning the new location. But this learning is gradual and asymmetric. The local switch at Host B learns immediately. Switches that see traffic from the VM learn when they observe it. Switches that do not see this traffic might retain stale information for minutes until their MAC table entries age out.

During this transition period, the network is in an inconsistent state. Some switches forward traffic to Host B correctly. Other switches still forward to Host A incorrectly. Traffic succeeds intermittently based on which switches are involved in the forwarding path. This intermittent failure is difficult to troubleshoot and unpredictable in its impact.

There is no explicit signal that a MAC has moved. Switches must infer movement by observing that a MAC address previously seen on one port now appears on a different port. Some switches might interpret this as a topology change. Others might ignore it as a transient error. The behavior is platform-dependent and often unreliable.

Race conditions occur when both the old and new locations still have state. If Host A has not yet aged out the MAC, and Host B has just learned it, both locations might respond to traffic, causing duplication or confusion. Layer 2 protocols do not handle these edge cases gracefully.

## EVPN Solution: Explicit Withdrawal and Readvertisement

When a VM moves in an EVPN network, the mobility is signaled explicitly through BGP route updates. The sequence is precise and deterministic.

When the VM appears at Host B and sends its first frame, the switch at Host B learns the MAC address locally. This switch immediately generates an EVPN Type 2 route advertising this MAC with Host B as the next-hop. This route is distributed via BGP to all switches in the network.

Switches receive this new route. They already have an existing Type 2 route for this MAC pointing to Host A. BGP's route selection process compares the routes. Typically, the newer route is preferred. Switches update their forwarding tables to point to Host B as the new location.

Meanwhile, the switch at Host A no longer sees traffic from this MAC because the VM has moved. After a short aging timeout, the switch realizes the MAC is no longer local. It withdraws its previously advertised EVPN Type 2 route. This withdrawal is distributed via BGP to all switches.

The combination of the new advertisement from Host B and the withdrawal from Host A ensures clean transition. All switches converge on Host B as the current location. The transition typically completes in seconds. There is no extended period of inconsistent state.

BGP's mechanisms handle race conditions. If switches temporarily receive routes from both locations, BGP's route selection ensures they make consistent choices. All switches select the same best route based on deterministic criteria. There are no scenarios where some switches choose Host A while others choose Host B.

The explicit signaling makes mobility observable and debuggable. Looking at BGP route updates, you can see exactly when a MAC moved, from where to where, and which switches updated their forwarding. This visibility is impossible with implicit learning, where you can only infer what happened by observing traffic patterns.

EVPN transforms mobility from a chaotic, unreliable process into a clean, well-defined protocol operation. Workloads can move freely, and the network adapts correctly and quickly. This reliability is essential for modern infrastructure where workload mobility is constant and expected.

## The Fundamental Shift: Reactive to Proactive

The core insight behind EVPN is the shift from reactive to proactive operation. Traditional networking is reactive. Switches wait for traffic to appear, then react to it by learning, flooding, or generating broadcasts. The network is always catching up to reality.

EVPN is proactive. Switches announce what they know the moment they know it. Other switches receive these announcements before any traffic flows. When traffic does flow, the forwarding decision is already made based on previously received information. The network is ready for traffic before traffic arrives.

This proactive model eliminates the entire class of problems caused by reactive learning. There are no learning delays because learning happens preemptively. There is no flooding because destinations are known before forwarding begins. There are no broadcasts because IP-to-MAC bindings are distributed proactively.

The proactive model scales better. In a reactive model, every new endpoint generates flood traffic that grows with network size. In a proactive model, every new endpoint generates one advertisement that propagates via BGP. BGP is proven to scale to hundreds of thousands of routes. The control plane cost grows linearly while the data plane benefit is elimination of flooding entirely.

The proactive model is more deterministic. Reactive learning depends on traffic patterns—what traffic has been seen, what has been learned recently, what has aged out. Different switches might have different learned state based on different traffic exposure. Proactive advertisement ensures all switches have the same information. The network state is consistent everywhere.

EVPN exists because traditional Layer 2 networking broke at scale. The problems were not minor inefficiencies. They were fundamental barriers to building large, performant, reliable networks. EVPN solves these problems through careful protocol design that leverages BGP's proven scalability while addressing the specific needs of Ethernet networks. Understanding these problems and solutions is understanding why EVPN has become the standard for modern data center networking.

# EVPN Route Type 2: The Core of MAC Learning

## What Type 2 Does

Route Type 2 is called the MAC/IP Advertisement Route. This route type advertises that a specific MAC address, optionally with its associated IP address, is reachable through a specific location in the network. Type 2 is the most fundamental and most frequently used EVPN route type. Every endpoint that appears in the network generates a Type 2 route. Every MAC address you want other switches to know about requires a Type 2 advertisement.

When a switch learns a MAC address locally—perhaps a server boots up and sends its first frame—the switch creates a Type 2 route. This route says "I am switch X at IP address Y, and I have MAC address Z in VNI W, and it is associated with IP address A." This route is sent via BGP to other switches. Those switches receive the route and install forwarding table entries. Now when any switch needs to send traffic to MAC address Z, it knows exactly where to send it.

Type 2 routes replace the flood-and-learn mechanism of traditional Ethernet. Instead of switches learning MAC addresses by observing traffic and flooding unknowns, switches learn proactively through Type 2 advertisements. This eliminates flooding and provides instant knowledge of MAC locations across the entire network.

## The Anatomy of a Type 2 Route

Understanding what information a Type 2 route carries is essential for understanding what it accomplishes. The route contains multiple fields, each serving a specific purpose.

The Route Distinguisher makes the route globally unique. In BGP, routes are identified by their prefix. For IP routes, the prefix is the IP address and mask. For EVPN routes, different route types need different identification mechanisms. The Route Distinguisher is an 8-byte value prepended to the route that ensures uniqueness even if multiple switches advertise the same MAC address in different contexts. The RD is typically automatically generated from the switch's router ID and a locally unique number.

The Ethernet Segment Identifier indicates whether this MAC address is behind a multihomed link. If the value is zero, the MAC is single-homed—connected to only one switch. If the value is non-zero, multiple switches share access to this MAC, and they use the ESI to coordinate. For most simple deployments, the ESI is zero because servers connect to a single switch.

The Ethernet Tag ID provides additional segmentation within a VNI. In most VXLAN deployments, this field is set to zero. Some deployment models use non-zero values to create additional hierarchy, but this is uncommon. For practical purposes, you will typically see Ethernet Tag ID as zero in Type 2 routes.

The MAC Address Length is always 48 bits for standard Ethernet. This field exists for protocol completeness but always has the same value.

The MAC Address is the actual 48-bit MAC address being advertised. This is the core information: which MAC address this route is about.

The IP Address Length can be zero, 32 bits for IPv4, or 128 bits for IPv6. If zero, no IP address is being advertised with this MAC. If non-zero, the route includes the IP address associated with this MAC.

The IP Address is the associated IP address if the length is non-zero. This is crucial for ARP suppression. By advertising both MAC and IP in a single route, EVPN provides complete Layer 2 and Layer 3 information.

The MPLS Label fields carry the VNI in VXLAN deployments. BGP routes were originally designed for MPLS networks where labels indicate forwarding paths. EVPN reuses this field to carry the VXLAN Network Identifier. When a switch receives a Type 2 route, it extracts the VNI from this field to know which virtual network this MAC belongs to.

The BGP next-hop is set to the advertising switch's IP address—specifically, its VTEP loopback address. This tells receiving switches where to send VXLAN-encapsulated traffic destined for this MAC.

## How Type 2 Routes Are Generated

The generation process begins with local learning. A server connected to a switch sends a frame. This frame has a source MAC address. The switch receives this frame and examines the source MAC. Through standard Ethernet learning, the switch records which port the MAC appeared on. This is no different from how traditional switches learn MACs.

But EVPN extends this local learning with advertisement. Once the switch learns the MAC, it does not stop there. The switch checks whether this MAC should be advertised via EVPN. If the port is configured for EVPN—typically meaning it is part of a VNI—the switch proceeds with advertisement generation.

The switch constructs the Type 2 route. It fills in the MAC address from what it learned. It determines the VNI based on the port's configuration—which VNI this port belongs to. It sets the next-hop to its own VTEP IP address. If the switch can determine the IP address associated with this MAC—perhaps from inspecting the frame's contents if it is an IP packet, or from observing ARP traffic—it includes the IP address in the route.

The route is then injected into BGP. The switch's BGP process sends this route to its BGP peers—typically route reflectors in a leaf-spine fabric. The route carries the L2VPN EVPN address family indicator so BGP knows this is an EVPN route, not a regular IP route.

Route Targets are attached as extended communities. These RTs determine which other switches should import this route. The RT is typically derived from the VNI—for example, VNI 10100 might use RT 65000:10100. Switches configured to import this RT will process the route. Switches not interested in this VNI will ignore it based on RT filtering.

## How Type 2 Routes Are Processed

When a switch receives a Type 2 route via BGP, it processes the route in several steps. First, BGP's standard route selection process determines whether this route is the best route for this particular MAC/IP in this VNI. There might be multiple advertisements for the same MAC if it is multihomed or if it recently moved. BGP compares all routes and selects the best one according to standard BGP selection criteria.

If this route is selected as best, the switch examines the Route Targets. The switch has import filters configured—lists of RTs it is interested in. If the route's RTs match the import filter, the route is imported. If not, the route is ignored.

For imported routes, the switch extracts the relevant information. It pulls out the MAC address, the IP address if present, the VNI from the label field, and the next-hop VTEP IP address. With this information, the switch populates its forwarding tables.

The MAC table for the appropriate VNI is updated. An entry is created mapping the MAC address to the remote VTEP IP address. This entry says "to reach this MAC in this VNI, encapsulate traffic with VXLAN and send to this VTEP IP."

If an IP address was included in the Type 2 route, the ARP suppression table is also updated. This table maps IP addresses to MAC addresses within VNIs. When a server sends an ARP request, the switch can consult this table and reply immediately without forwarding the broadcast.

The switch's control plane has now completed its job. The forwarding tables are populated. When actual traffic arrives destined for this MAC, the data plane performs a lookup in these tables and forwards accordingly. The Type 2 route served its purpose: distributing knowledge of the MAC's location so traffic can be forwarded efficiently.

## The Flow: From Server Boot to Traffic Forwarding

Let us trace the complete flow from a server appearing on the network to traffic being forwarded to it. This illustrates how Type 2 routes work in practice.

A server connects to Port 10 on Switch A. The server is configured to use VLAN 100, which is mapped to VNI 10100. The server boots up and begins network initialization. As part of this initialization, the server sends frames—perhaps DHCP requests, perhaps gratuitous ARP announcements.

Switch A receives these frames on Port 10. Switch A examines the source MAC address: AA:AA:AA:AA:AA:AA. Switch A's MAC learning logic records this: MAC AA:AA:AA:AA:AA:AA is on Port 10 in VLAN 100. This is standard Ethernet learning that has existed for decades.

Because Port 10 is configured as part of VNI 10100 with EVPN enabled, Switch A proceeds with EVPN advertisement. Switch A constructs a Type 2 route. The MAC address is AA:AA:AA:AA:AA:AA. The VNI is 10100. The next-hop is 10.0.1.1, which is Switch A's VTEP loopback IP address.

Switch A inspects the frame's contents. If the frame contains an IP packet, Switch A can extract the source IP address: 10.1.1.10. Switch A includes this in the Type 2 route. The route now carries both MAC and IP information.

Switch A's BGP process sends this Type 2 route to the route reflectors. The route is tagged with Route Target 65000:10100 based on the VNI. The route is distributed through the BGP network.

Switch B, located in a different rack, has servers in VNI 10100. Switch B is configured to import routes with RT 65000:10100. When the route reflector sends the Type 2 route to Switch B, Switch B examines the RTs, determines it should import this route, and processes it.

Switch B extracts the information: MAC AA:AA:AA:AA:AA:AA in VNI 10100 is reachable via VTEP 10.0.1.1. IP address 10.1.1.10 maps to this MAC. Switch B updates its MAC table and ARP suppression table with this information.

Now a server behind Switch B wants to communicate with IP address 10.1.1.10. The server sends an ARP request: "Who has 10.1.1.10?" Switch B intercepts this ARP request. Switch B checks its ARP suppression table, finds that 10.1.1.10 maps to MAC AA:AA:AA:AA:AA:AA, and immediately replies with this information. The ARP request is suppressed.

The server behind Switch B now sends traffic to MAC AA:AA:AA:AA:AA:AA. The frame arrives at Switch B. Switch B looks up this MAC in its MAC table for VNI 10100. The table says this MAC is at remote VTEP 10.0.1.1. Switch B performs VXLAN encapsulation: wraps the frame with VXLAN header containing VNI 10100, UDP header, and IP header with destination 10.0.1.1.

The encapsulated packet is routed through the underlay network from Switch B to Switch A. Switch A receives it, recognizes its own VTEP IP, decapsulates the packet, and delivers the original frame to Port 10 where the destination server is connected.

The server receives the frame. From the server's perspective, it is simply communicating with another server on the same Layer 2 network. The entire EVPN mechanism—the Type 2 route advertisement, the BGP distribution, the table population, the VXLAN encapsulation—is invisible to the endpoints. It just works.

## Type 2 Routes and MAC Mobility

When a MAC address moves—a VM migrates, a container is rescheduled—Type 2 routes handle the transition. The process is explicit and controlled, unlike traditional networks where mobility is inferred from traffic patterns.

Before the move, the MAC is behind Switch A. Switch A has advertised a Type 2 route with Switch A as the next-hop. All other switches have forwarding entries pointing to Switch A. Traffic flows correctly to Switch A, which delivers it to the MAC.

The VM migrates from behind Switch A to behind Switch C. The VM appears on a port on Switch C and begins sending traffic. Switch C sees the MAC address for the first time and learns it locally. Switch C does not know this MAC was previously elsewhere. Switch C simply knows it now has this MAC locally.

Switch C generates a Type 2 route for this MAC with Switch C as the next-hop. This route is distributed via BGP to all switches, including Switch A. The route carries the same MAC address but a different next-hop.

Other switches, like Switch B, receive this new Type 2 route. They already have an existing Type 2 route for this MAC pointing to Switch A. BGP's route selection process compares the routes. The routes have equal AS path length, equal other attributes. The tiebreaker is typically the most recently received route or the route from the switch with the lower router ID. Either way, BGP selects one route as best.

Typically, the newer route from Switch C is selected. Switch B updates its forwarding table. The MAC table entry for this MAC is changed from pointing to Switch A to pointing to Switch C. New traffic is now forwarded to Switch C.

Meanwhile, Switch A no longer sees traffic from this MAC because the VM has moved away. After an aging timeout—typically a few minutes—Switch A ages out the MAC from its local MAC table. When the MAC is aged out locally, Switch A withdraws its Type 2 route. This withdrawal is sent via BGP to all switches.

Switches receiving the withdrawal remove the old route from their BGP tables. If any switch was still using the old route, it now has only the new route from Switch C. The migration is complete. All switches forward to the correct new location.

The transition is clean. There might be a brief period where both Switch A and Switch C advertise the MAC, but BGP ensures all switches make consistent choices about which route to use. There are no scenarios where some switches forward to the old location while others forward to the new location. The network converges to consistent state quickly.

## Type 2 Routes for IPv6

Type 2 routes are not limited to IPv4. The IP Address field in the route can carry IPv6 addresses. The IP Address Length is set to 128 bits, and the full IPv6 address is included. The processing is identical—switches populate their tables with the MAC-to-IPv6 binding, and Neighbor Discovery solicitations can be suppressed just as ARP requests are suppressed.

IPv6 actually benefits more from EVPN than IPv4 does. IPv6 uses Neighbor Discovery instead of ARP, and Neighbor Discovery generates significant multicast traffic. Without EVPN, every Neighbor Solicitation must be sent as multicast to all nodes. With EVPN, these solicitations can be suppressed locally just as ARP is suppressed. The reduction in multicast overhead is substantial in large IPv6 networks.

## Type 2 Routes in Failure Scenarios

When a switch fails or when a link fails, Type 2 routes handle the failure through BGP's standard mechanisms. If Switch A fails completely, its BGP sessions to the route reflectors time out. The route reflectors detect the session failure and automatically withdraw all routes learned from Switch A. This includes all Type 2 routes for MACs that were behind Switch A.

Other switches receive these withdrawals via BGP. They remove the affected MAC entries from their forwarding tables. Traffic destined for these MACs now has no forwarding entry. The traffic is dropped until the MACs reappear elsewhere or until Switch A is restored.

If a link between Switch A and its upstream fails, Switch A itself remains operational and its BGP sessions remain up. However, Switch A can no longer reach the underlay network. Switch A's loopback IP address becomes unreachable. Other switches can still forward VXLAN traffic toward Switch A's IP, but the underlay cannot route these packets to Switch A.

In this scenario, the Type 2 routes remain advertised because BGP sessions are still up. The control plane state is correct, but the data plane fails because the underlay cannot deliver packets. Some deployments use BFD (Bidirectional Forwarding Detection) on the underlay links to detect failures quickly. When BFD detects a link failure, it can trigger BGP session closure, which then causes route withdrawals.

## Type 2 Route Scaling

The number of Type 2 routes in a network equals the number of MAC addresses being advertised. In a data center with 100,000 servers, each with one MAC address, there are 100,000 Type 2 routes. If servers have multiple interfaces or multiple VMs, the number multiplies accordingly.

BGP handles this scale comfortably. BGP was designed to handle Internet routing tables with over a million routes. A data center with hundreds of thousands of Type 2 routes is well within BGP's capabilities. The route storage in switch memory is the practical limit, but modern switches have sufficient memory for large MAC tables.

Route churn—the rate of routes being added and removed—can be more challenging than absolute route count. In environments with high VM or container churn where workloads are constantly created, moved, and destroyed, the rate of Type 2 route updates can be high. BGP must process these updates, switches must install and remove forwarding entries, and the control plane must converge quickly.

EVPN implementations optimize for this by batching updates when possible, rate-limiting advertisement generation to prevent overwhelming BGP, and using efficient data structures for table updates. Well-designed EVPN networks handle even high churn rates without performance degradation.

## Why Type 2 Is Central

Type 2 routes are the heart of EVPN. While other route types serve important functions, Type 2 is what makes the system work. Every MAC address that participates in EVPN generates a Type 2 route. Every forwarding decision relies on information learned from Type 2 routes. Understanding Type 2 is understanding how EVPN replaces flood-and-learn with proactive advertisement, and how this replacement enables scalable, efficient, predictable network operation.

# EVPN Route Type 3: Managing Broadcast and Multicast

## The Purpose of Type 3

Route Type 3 is called the Inclusive Multicast Ethernet Tag Route. The name reveals its function: it identifies which VTEPs want to receive multicast and broadcast traffic for a particular VNI. When a VTEP wants to participate in a VNI, it sends a Type 3 route advertising its membership. Other VTEPs receive this advertisement and add the advertising VTEP to their list of destinations for broadcast and multicast traffic in that VNI.

Type 3 routes solve the broadcast distribution problem. Even with EVPN's proactive MAC learning via Type 2 routes, legitimate broadcast traffic still exists. Servers send DHCP requests as broadcasts. Some protocols use broadcast for discovery. Unknown unicast traffic—traffic to MAC addresses not yet learned—must be handled somehow. Type 3 routes establish how to distribute this traffic without flooding it throughout the entire network.

Without Type 3 routes, VTEPs would not know which other VTEPs care about a particular VNI. If every VTEP broadcast to every other VTEP in the network regardless of VNI membership, the bandwidth waste would be enormous. Type 3 routes create VNI-specific distribution lists, ensuring broadcast traffic only goes where it is needed.

## The Anatomy of a Type 3 Route

A Type 3 route is simpler than a Type 2 route because it carries less information. The route identifies the advertising VTEP and which VNI it is advertising for.

The Route Distinguisher makes the route unique in the global BGP table. Multiple VTEPs can advertise Type 3 routes for the same VNI, and the RD ensures these routes do not conflict.

The Ethernet Tag ID is typically zero in VXLAN deployments. This field can create additional segmentation within a VNI in some deployment models, but most networks use a single tag value of zero.

The IP Address Length and IP Address identify the advertising VTEP. This is the VTEP's loopback IP address—the address used as the source for VXLAN encapsulation. This tells receiving VTEPs where to send broadcast traffic.

The VNI information is not directly in the route but is carried in extended communities through Route Targets. A Type 3 route for VNI 10100 is tagged with Route Target 65000:10100. VTEPs interested in VNI 10100 import routes with this RT.

The route optionally carries a tunnel type identifier. For VXLAN, this indicates ingress replication should be used. Other tunnel types might use multicast distribution, but VXLAN typically uses ingress replication.

## How Type 3 Routes Are Generated

Type 3 route generation happens when a VTEP activates participation in a VNI. This typically occurs during initial configuration when an administrator configures a VNI on the switch, or dynamically when the first endpoint in a VNI appears.

When VNI 10100 is configured on a VTEP, the VTEP checks whether it should advertise participation. If the VTEP has active ports mapped to this VNI, or if it is configured to participate regardless, the VTEP generates a Type 3 route.

The VTEP constructs the route with its own loopback IP address as the originating VTEP. It sets the Ethernet Tag ID to zero. It attaches Route Target 65000:10100 to indicate this advertisement is for VNI 10100. The tunnel type is set to ingress replication.

This route is injected into BGP and distributed to route reflectors, which redistribute it to all other VTEPs. Any VTEP that imports routes with RT 65000:10100 will receive this Type 3 route.

The Type 3 route remains advertised as long as the VTEP participates in the VNI. If the VTEP is reconfigured to remove the VNI, or if all endpoints in that VNI disappear, the VTEP withdraws the Type 3 route. Other VTEPs receive the withdrawal and remove the advertising VTEP from their broadcast lists.

## How Type 3 Routes Are Processed

When a VTEP receives a Type 3 route via BGP, it first checks whether it should import the route based on Route Targets. If the VTEP participates in the advertised VNI, it imports the route. If not, it ignores the route.

For imported Type 3 routes, the VTEP extracts the originating VTEP's IP address. The VTEP adds this IP to its broadcast replication list for the VNI. This list is stored in the VTEP's data structures—conceptually a table mapping VNI to list of remote VTEP IPs.

As the VTEP receives Type 3 routes from multiple other VTEPs, all for the same VNI, the broadcast list grows. If 10 VTEPs advertise Type 3 routes for VNI 10100, the receiving VTEP builds a list of 10 VTEP IPs. This list represents all the places broadcast traffic for VNI 10100 should be sent.

When the VTEP needs to broadcast traffic in VNI 10100, it consults this list. For each VTEP IP in the list, the VTEP creates a copy of the frame, encapsulates it in VXLAN with destination IP set to that VTEP IP, and sends it. This is called ingress replication—the source VTEP replicates the frame to all destinations.

## The Flow: Broadcast Distribution

Let us trace how broadcast traffic is handled using Type 3 routes. We have three VTEPs: VTEP-1, VTEP-2, and VTEP-3. All three participate in VNI 10100.

During configuration, each VTEP generates a Type 3 route for VNI 10100. VTEP-1 advertises its IP address 10.0.1.1. VTEP-2 advertises 10.0.1.2. VTEP-3 advertises 10.0.1.3. These routes are distributed via BGP.

VTEP-1 receives Type 3 routes from VTEP-2 and VTEP-3. VTEP-1 builds a broadcast list for VNI 10100: [10.0.1.2, 10.0.1.3]. Similarly, VTEP-2 builds the list [10.0.1.1, 10.0.1.3], and VTEP-3 builds [10.0.1.1, 10.0.1.2]. Each VTEP knows about all other VTEPs in the VNI.

A server behind VTEP-1 sends a broadcast frame. Perhaps it is a DHCP Discover message looking for a DHCP server. The frame arrives at VTEP-1 with destination MAC address FF:FF:FF:FF:FF:FF, which is the broadcast MAC.

VTEP-1 recognizes this as a broadcast that must be replicated. VTEP-1 checks its broadcast list for VNI 10100 and sees [10.0.1.2, 10.0.1.3]. VTEP-1 creates two copies of the frame.

For the first copy, VTEP-1 encapsulates with VXLAN header containing VNI 10100, UDP header, and IP header with source 10.0.1.1 and destination 10.0.1.2. This packet is sent into the underlay network toward VTEP-2.

For the second copy, VTEP-1 encapsulates with destination IP 10.0.1.3. This packet is sent toward VTEP-3.

VTEP-2 receives its copy. It decapsulates, sees VNI 10100, and delivers the broadcast frame to all local servers in VNI 10100. If a DHCP server is behind VTEP-2, it receives the broadcast and can respond.

VTEP-3 does the same with its copy. All VTEPs participating in VNI 10100 receive the broadcast. The broadcast was distributed efficiently—each VTEP received exactly one copy, and no VTEPs not participating in VNI 10100 received anything.

## Ingress Replication vs Multicast

Type 3 routes support two distribution mechanisms: ingress replication and multicast. Ingress replication, described above, has the source VTEP create individual unicast copies of the frame for each destination VTEP. This is simple and requires no special network support beyond basic IP routing.

Multicast distribution uses IP multicast for more efficient replication. With multicast, each VNI is mapped to a multicast group address. VTEPs interested in a VNI join the multicast group. When a VTEP needs to broadcast in a VNI, it sends a single VXLAN packet to the multicast group address. The network's multicast routing replicates the packet to all group members.

Multicast is more bandwidth-efficient in the core network because replication happens in switches rather than at the source. However, multicast requires the underlay network to support and be configured for IP multicast, which adds operational complexity. Many data centers avoid multicast due to this complexity.

Type 3 routes work with either mechanism. The tunnel type field in the Type 3 route indicates which method is used. For ingress replication, the field says "replicate at source." For multicast, the field carries the multicast group address to use. Most VXLAN deployments use ingress replication despite its higher bandwidth cost because operational simplicity is prioritized over bandwidth efficiency.

## Type 3 Routes and Unknown Unicast

Not all broadcast traffic is truly broadcast. Sometimes traffic has a unicast destination MAC, but the destination MAC has not been learned yet. This is called unknown unicast. Without EVPN, switches flood unknown unicast like broadcasts.

With EVPN, Type 2 routes mean destinations are known before traffic is sent. ARP suppression means most first-packet scenarios are handled without broadcasts. However, edge cases exist where unknown unicast still occurs—perhaps a host sends traffic before EVPN routes have propagated, or perhaps there is a configuration issue.

Type 3 routes handle unknown unicast just like broadcast. If a VTEP receives a unicast frame for a MAC address not in its MAC table, the VTEP treats it as unknown unicast and replicates it using the broadcast list built from Type 3 routes. This ensures the frame reaches all VTEPs in the VNI, and whichever VTEP has the destination locally will deliver it.

In well-operating EVPN networks, unknown unicast should be rare. Type 2 advertisements propagate quickly, and most MAC addresses are known before traffic flows. Unknown unicast serves as a fallback mechanism for edge cases rather than a primary forwarding path.

## Type 3 Routes and Multicast Optimization

Some applications use multicast deliberately for efficient one-to-many communication. A video streaming server might send multicast to multiple clients. Without optimization, EVPN treats application multicast as broadcast, replicating to all VTEPs in the VNI even though only some VTEPs have interested receivers.

EVPN can be extended with multicast snooping integration. The VTEPs learn which local hosts have joined which multicast groups by snooping IGMP messages. Type 3 routes can carry information about which multicast groups a VTEP has receivers for. This allows VTEPs to build selective distribution lists—only VTEPs with receivers for multicast group G receive traffic to group G.

This optimization requires additional protocol features beyond basic Type 3 routes. Not all EVPN implementations support it. For many networks, the performance benefit is not worth the added complexity. Broadcast and multicast traffic is typically a small fraction of total traffic, so even unoptimized distribution does not significantly impact performance.

## Type 3 Route Scaling

The number of Type 3 routes in a network is the number of VTEPs times the number of VNIs each VTEP participates in. If you have 100 VTEPs and 1,000 VNIs, and every VTEP participates in every VNI, you have 100,000 Type 3 routes. This is manageable for BGP but represents substantial state.

In practice, not every VTEP participates in every VNI. VTEPs only advertise Type 3 routes for VNIs that have active endpoints. If VNI 10100 only has endpoints behind 10 of the 100 VTEPs, only those 10 advertise Type 3 routes for that VNI. This reduces the total count substantially.

Route Target filtering further reduces the load. VTEPs only import Type 3 routes for VNIs they care about. A VTEP that does not participate in VNI 10100 filters out all Type 3 routes with RT 65000:10100. This keeps the control plane state manageable even in large networks.

## Type 3 Routes in Failure Scenarios

When a VTEP fails, its BGP sessions go down. The route reflectors withdraw all routes learned from that VTEP, including its Type 3 routes. Other VTEPs receive these withdrawals and remove the failed VTEP from their broadcast lists. Subsequent broadcasts do not replicate to the failed VTEP. The waste of sending traffic to an unreachable destination is avoided.

When a VTEP recovers, it reestablishes BGP sessions and readvertises its Type 3 routes. Other VTEPs receive these advertisements and add the recovered VTEP back to their broadcast lists. Broadcasts again reach the recovered VTEP. The failure and recovery are handled automatically through BGP's mechanisms.

If a VTEP loses underlay connectivity but keeps BGP sessions up, a more subtle problem occurs. The VTEP's Type 3 routes remain advertised because BGP is still working. Other VTEPs try to replicate broadcasts to this VTEP, but the underlay cannot deliver packets. The broadcasts are lost. This scenario requires underlay failure detection—using BFD or similar mechanisms—to trigger BGP session closure and route withdrawal.

## Why Type 3 Matters

Type 3 routes enable selective broadcast distribution. Without Type 3, every broadcast would either be sent to all VTEPs regardless of interest, wasting bandwidth, or be handled through complex multicast configuration. Type 3 provides a simple, scalable mechanism for VTEPs to signal interest and for broadcasts to be distributed only where needed. While Type 2 routes handle most traffic through direct forwarding, Type 3 routes ensure the broadcast corner cases work correctly.

# EVPN Route Type 5: Inter-VNI Routing

## The Purpose of Type 5

Route Type 5 is called the IP Prefix Route. While Type 2 routes advertise individual host MAC and IP addresses, Type 5 routes advertise IP subnets—entire prefixes that represent many hosts. Type 5 enables routing between different VNIs, allowing controlled communication between isolated network segments.

VNIs provide Layer 2 isolation by design. Traffic in VNI 10100 cannot reach VNI 10200 without explicit routing. This isolation is desirable for security and organization, but complete isolation is often too restrictive. A web application in VNI 10100 needs to query a database in VNI 10200. A management network in VNI 10300 needs to access servers in multiple application VNIs.

Type 5 routes provide the mechanism for inter-VNI routing. A gateway—either a dedicated router or a VTEP with routing functionality—advertises IP prefixes via Type 5 routes. These advertisements tell other VTEPs which IP subnets are reachable and how to route to them. VTEPs can then forward traffic destined for remote subnets to the gateway, which routes between VNIs.

## The Anatomy of a Type 5 Route

Type 5 routes carry more information than Type 2 or Type 3 routes because they describe routing paths, not just endpoint locations.

The Route Distinguisher makes the route unique in BGP. Multiple gateways might advertise the same IP prefix for different VNIs or different purposes, and the RD ensures these advertisements do not conflict.

The Ethernet Segment Identifier is typically zero for Type 5 routes because routing functions are not usually multihomed in the same way access ports are.

The Ethernet Tag ID is typically zero in VXLAN deployments, consistent with other route types.

The IP Prefix Length and IP Prefix are the core information. Unlike Type 2 routes that advertise /32 or /128 host addresses, Type 5 routes advertise subnets like 10.1.1.0/24 or 192.168.100.0/24. This single route represents all hosts in the subnet.

The Gateway IP Address specifies the next-hop for reaching this prefix. This might be a separate routing device's IP, or it might be the advertising VTEP's own IP if it performs routing itself.

The MPLS Label field carries the VNI associated with this prefix. This tells receiving VTEPs which VNI this subnet belongs to, which is crucial for symmetric routing models where both source and destination VTEPs perform routing.

Extended communities carry Route Targets that determine which VTEPs import these routes. For inter-VNI routing, a gateway might advertise Type 5 routes with RTs for multiple VNIs, allowing those VNIs to import routes for each other.

## Symmetric vs Asymmetric IRB

Inter-VNI routing can be implemented in two models: symmetric and asymmetric. Type 5 routes support both, but the models differ in how routing is performed.

In asymmetric routing, only one VTEP performs routing. Consider traffic from a host in VNI 10100 to a host in VNI 10200. The source VTEP receives the packet from the local host in VNI 10100. The source VTEP recognizes the destination is in a different subnet and routes the packet. It looks up the destination IP and determines it is in VNI 10200. The source VTEP changes the VNI to 10200, re-encapsulates, and sends the packet to the destination VTEP. The destination VTEP simply bridges the packet to the local host—no routing involved.

The return path is asymmetric. The host in VNI 10200 sends return traffic. The destination VTEP (now the source for return traffic) receives the packet in VNI 10200, performs routing to determine it should go to VNI 10100, changes the VNI, and forwards. The original source VTEP (now the destination for return traffic) simply bridges. Each direction involves routing at only one end, and it is a different end for each direction—hence asymmetric.

In symmetric routing, both VTEPs perform routing. The source VTEP routes from VNI 10100 to a transit VNI—perhaps VNI 99999. The packet is encapsulated with VNI 99999 and sent to the destination VTEP. The destination VTEP decapsulates, sees VNI 99999, and knows this requires routing. It routes from VNI 99999 to the destination VNI 10200 and delivers to the local host. Return traffic follows the same pattern: destination VTEP routes from VNI 10200 to VNI 99999, source VTEP routes from VNI 99999 to VNI 10100. Both directions involve routing at both ends—symmetric.

Type 5 routes enable both models by advertising which prefixes are in which VNIs. For asymmetric routing, Type 5 routes tell VTEPs the VNI for each destination prefix so they can route directly to the correct VNI. For symmetric routing, Type 5 routes advertise the transit VNI and establish the routing paths through it.

## How Type 5 Routes Are Generated

Type 5 route generation happens at routing boundary devices. These might be dedicated routers, or they might be VTEPs with integrated routing functionality. The device must be configured to route between VNIs and to advertise the reachability via EVPN.

A gateway serving VNI 10100 with subnet 10.1.1.0/24 and VNI 10200 with subnet 10.2.1.0/24 generates two Type 5 routes. The first advertises prefix 10.1.1.0/24 with VNI 10100. The second advertises 10.2.1.0/24 with VNI 10200. Both routes set the next-hop to the gateway's IP address.

The routes are tagged with appropriate Route Targets. For inter-VNI routing, the gateway might tag the 10.1.1.0/24 route with RT for VNI 10200, and tag the 10.2.1.0/24 route with RT for VNI 10100. This allows VNI 10100 to import routes for VNI 10200's subnet and vice versa, enabling bidirectional routing.

The routes are injected into BGP and distributed to VTEPs. Any VTEP importing these routes gains the ability to route traffic toward these subnets by forwarding to the gateway.

## How Type 5 Routes Are Processed

When a VTEP receives a Type 5 route via BGP, it checks the Route Targets against its import filters. If the VTEP should import the route—typically because it participates in a VNI that needs to route to the advertised prefix—it processes the route.

The VTEP extracts the IP prefix and the next-hop gateway IP. It installs an IP routing table entry: prefix 10.2.1.0/24 is reachable via gateway at IP address 10.0.5.5. If the route includes VNI information, the VTEP also records which VNI this prefix belongs to.

When a host behind the VTEP sends traffic to an IP in this prefix, the VTEP's routing logic matches the destination IP against its routing table. It finds the Type 5-learned entry and forwards the packet according to the routing model—either directly encapsulating with the destination VNI for asymmetric routing, or routing to the transit VNI and forwarding to the gateway for symmetric routing.

## The Flow: Inter-VNI Communication

Let us trace traffic from a host in VNI 10100 to a host in VNI 10200 using Type 5 routes. We will use asymmetric routing for this example.

Host-A is in VNI 10100 with IP 10.1.1.10 behind VTEP-1. Host-B is in VNI 10200 with IP 10.2.1.20 behind VTEP-2. A gateway has advertised Type 5 routes: 10.2.1.0/24 is in VNI 10200. VTEP-1 has imported this route because VNI 10100 is configured to route to VNI 10200.

Host-A wants to send a packet to 10.2.1.20. Host-A's default gateway is configured as 10.1.1.1, which is an anycast gateway IP implemented by VTEP-1. Host-A sends the packet to its gateway MAC address.

The packet arrives at VTEP-1. VTEP-1 recognizes the destination MAC is the gateway MAC. VTEP-1 performs IP routing. It looks up destination IP 10.2.1.20 in its routing table. The table has an entry from the Type 5 route: 10.2.1.0/24 is in VNI 10200.

VTEP-1 knows the destination is Host-B at 10.2.1.20 in VNI 10200. From Type 2 routes, VTEP-1 knows Host-B's MAC address is BB:BB:BB:BB:BB:BB and it is behind VTEP-2 at IP 10.0.1.2. VTEP-1 constructs a new frame with source MAC as the gateway MAC, destination MAC as BB:BB:BB:BB:BB:BB, containing the IP packet.

VTEP-1 encapsulates this frame with VXLAN header containing VNI 10200 (not 10100—the VNI has been changed as part of routing). The outer IP header has source 10.0.1.1 (VTEP-1) and destination 10.0.1.2 (VTEP-2). The packet is sent into the underlay.

VTEP-2 receives the packet, decapsulates, sees VNI 10200, and looks at the destination MAC BB:BB:BB:BB:BB:BB. VTEP-2's MAC table shows this MAC is on a local port. VTEP-2 delivers the frame to Host-B.

Host-B receives the frame and responds. Return traffic follows a similar but asymmetric path. Host-B sends to its gateway. VTEP-2 routes from VNI 10200 to VNI 10100, encapsulates with VNI 10100, and sends to VTEP-1. VTEP-1 delivers to Host-A. Both directions involved routing, but at different VTEPs—asymmetric.

## Distributed Anycast Gateway with Type 5

Many EVPN deployments use distributed anycast gateway. Instead of having a centralized router that all traffic must traverse, every VTEP acts as a gateway for its locally connected hosts. Type 5 routes enable this by distributing prefix information to all VTEPs.

All VTEPs advertise the same gateway MAC and IP for a given subnet. When a host sends traffic to the gateway, the local VTEP intercepts it. If the traffic is for a local destination in the same VNI, the VTEP bridges it. If the traffic is for a remote subnet, the VTEP routes it using Type 5 routes.

This distributed model eliminates the traffic hairpin that occurs with centralized gateways. Traffic from VNI 10100 to VNI 10200 is routed at the source VTEP and delivered directly to the destination VTEP. There is no intermediate hop to a separate gateway device. This optimizes traffic flow and eliminates a potential bottleneck.

Type 5 routes make distributed gateway work by ensuring all VTEPs have the routing information they need. Each VTEP learns from Type 5 routes which prefixes exist in which VNIs. When routing is needed, the VTEP has the information to make forwarding decisions without consulting a centralized controller or database.

## Type 5 Routes for External Connectivity

Type 5 routes are not limited to inter-VNI routing within the data center. They can also advertise external routes—prefixes reachable outside the EVPN network. A border gateway connecting the data center to external networks advertises these external prefixes via Type 5 routes.

For example, a border gateway with connectivity to the Internet advertises a default route 0.0.0.0/0 via a Type 5 route. VTEPs importing this route gain a path to the Internet. When a host sends traffic to an Internet destination, the VTEP matches against the default route and forwards to the border gateway, which routes the traffic externally.

This integration of internal and external routing through the same EVPN control plane simplifies operations. The same BGP infrastructure distributes both inter-VNI routes and external routes. VTEPs learn all reachability information through EVPN without needing separate routing protocols.

## Type 5 Route Scaling

The number of Type 5 routes is typically much smaller than the number of Type 2 routes. Instead of advertising every host individually, Type 5 routes advertise entire subnets. A subnet with 250 hosts requires one Type 5 route, not 250 Type 2 routes.

In a data center with 100 VNIs, each with one or two subnets, you might have 150 Type 5 routes total. This is far fewer than the potentially millions of Type 2 routes for individual hosts. Type 5 routes contribute minimally to control plane load.

However, Type 5 routes for external prefixes can be more numerous. If the border gateway imports full Internet routing table and redistributes it via Type 5 routes, you have hundreds of thousands of routes. Most deployments avoid this by advertising only a default route or a small set of summary routes for external destinations.

## Type 5 Routes in Failure Scenarios

When a gateway advertising Type 5 routes fails, its BGP sessions go down. The route reflectors withdraw the gateway's Type 5 routes. VTEPs receive the withdrawals and remove the routing table entries. Traffic requiring inter-VNI routing can no longer reach the failed gateway and fails.

Redundancy is achieved by having multiple gateways advertise the same prefixes. If two gateways both advertise 10.2.1.0/24 via Type 5 routes, VTEPs learn both routes. BGP selects one as best, typically based on metrics or AS path. If the primary gateway fails and its routes are withdrawn, the backup route from the second gateway becomes active. Traffic automatically fails over.

For fully distributed anycast gateway, failure of one VTEP does not affect routing capability. Every VTEP advertises the gateway function via its own Type 5 routes (or handles routing internally without explicit advertisements). Loss of one VTEP only affects hosts directly connected to it, not the overall routing capability of the network.

## Why Type 5 Matters

Type 5 routes extend EVPN from pure Layer 2 overlay to integrated Layer 2 and Layer 3. While VNIs provide isolation, Type 5 routes provide controlled connectivity between isolated segments. Without Type 5, inter-VNI routing would require external routers and complex configurations. With Type 5, routing is integrated into the EVPN control plane, leveraging the same BGP infrastructure that distributes MAC and ARP information. The result is a unified system that handles both switching and routing through a consistent, scalable protocol.

# EVPN and BGP: How the Control Plane Actually Works

## Why BGP Was Chosen

EVPN needed a distribution mechanism to propagate MAC addresses, IP-to-MAC bindings, and virtual network membership information across the data center. The protocol designers could have created a completely new protocol from scratch, but they chose instead to extend BGP. This was not a casual decision. BGP brings specific characteristics that made it the right choice for EVPN.

BGP scales to Internet size. The global Internet routing table contains over a million prefixes distributed among tens of thousands of autonomous systems. BGP handles this scale routinely. A data center with hundreds of thousands of MAC addresses and thousands of switches is large, but it is modest compared to the Internet. If BGP can handle global routing, it can certainly handle a data center.

BGP is well understood. Network engineers already know BGP from managing Internet connectivity. Monitoring tools, troubleshooting procedures, and operational knowledge all exist. Using BGP for EVPN means leveraging this existing expertise rather than requiring teams to learn an entirely new protocol.

BGP has robust vendor support. Every network equipment manufacturer implements BGP. Interoperability between vendors is proven. Choosing BGP for EVPN means EVPN can work across multi-vendor networks without requiring proprietary protocols or complex gateways.

BGP's flexibility was crucial. BGP was designed to be extensible through address families and extended communities. The protocol can carry different types of information beyond traditional IP routing. EVPN leverages this extensibility by defining a new address family specifically for Ethernet information while reusing BGP's session management, route distribution, and policy mechanisms.

## Multi-Protocol BGP Extensions

BGP originally carried only IPv4 unicast routing information. When IPv6 emerged, BGP needed to carry IPv6 routes as well. Rather than creating a separate protocol, the industry extended BGP with Multi-Protocol extensions, documented in RFC 4760.

Multi-Protocol BGP (MP-BGP) allows a single BGP session to carry multiple types of information simultaneously. Each type of information is identified by an Address Family Identifier (AFI) and Sub-Address Family Identifier (SAFI). IPv4 unicast uses AFI 1, SAFI 1. IPv6 unicast uses AFI 2, SAFI 1. EVPN uses AFI 25, SAFI 70.

During BGP session establishment, routers exchange capability advertisements. One capability is the list of address families the router supports. If both routers support the L2VPN EVPN address family, they can exchange EVPN routes over that session. The same BGP session can simultaneously carry IPv4 routes for underlay connectivity and EVPN routes for overlay MAC distribution.

This multiplexing means a single BGP infrastructure serves multiple purposes. The same BGP sessions, the same route reflectors, the same operational model support both underlay routing and overlay control plane. This unification simplifies operations compared to running separate protocols for different functions.

## BGP Session Establishment for EVPN

Before any EVPN routes can be exchanged, BGP sessions must be established. The process follows standard BGP procedures with the addition of EVPN capabilities.

Two leaf switches, Leaf-1 and Leaf-2, need to exchange EVPN information. In a typical data center design, they do not peer directly with each other. Instead, both peer with route reflectors—typically the spine switches. Leaf-1 establishes a BGP session to Spine-1, and Leaf-2 also establishes a session to Spine-1.

The session establishment begins with TCP. BGP runs over TCP port 179. Leaf-1 initiates a TCP connection to Spine-1's IP address on port 179. Once the TCP handshake completes, BGP can proceed.

Leaf-1 sends a BGP OPEN message. This message contains Leaf-1's AS number, router ID, and importantly, a list of capabilities Leaf-1 supports. One capability is Multi-Protocol Extensions. Another capability lists the address families: IPv4 unicast for underlay routing, and L2VPN EVPN for overlay control plane.

Spine-1 receives the OPEN message and examines the capabilities. If Spine-1 also supports L2VPN EVPN, it responds with its own OPEN message listing its capabilities, including L2VPN EVPN. If both sides support the same address families, the session can carry those types of routes.

The BGP session moves through OPEN-SENT, OPEN-CONFIRM, and finally ESTABLISHED state. Only in ESTABLISHED state can routes be exchanged. Once established, Leaf-1 can send EVPN routes to Spine-1, and Spine-1 can reflect these routes to Leaf-2 and other leaves.

## Route Reflector Architecture

Full mesh BGP does not scale. If every leaf must peer with every other leaf, a network with N leaves requires N*(N-1)/2 BGP sessions. With 100 leaves, that is 4,950 sessions. Configuration, management, and troubleshooting this many sessions is impractical.

Route reflectors solve this scaling problem. A route reflector is a BGP router that receives routes from clients and redistributes them to other clients. Leaves peer with route reflectors, not with each other. The number of sessions grows linearly rather than quadratically.

In a typical leaf-spine fabric, spine switches act as route reflectors. Each leaf establishes BGP sessions to multiple spines for redundancy. If a leaf peers with two spines and there are 100 leaves, that is 200 total leaf-to-spine sessions. This is manageable compared to thousands of leaf-to-leaf sessions.

When Leaf-1 sends an EVPN Type 2 route to Spine-1, Spine-1 sees that Leaf-1 is configured as a route reflector client. Spine-1's route reflector function receives the route and immediately reflects it to all other route reflector clients. This means Spine-1 sends the route to Leaf-2, Leaf-3, and all other leaves.

If multiple spines act as route reflectors, they peer with each other. Spine-1 and Spine-2 have an iBGP session between them. When Spine-1 receives a route from Leaf-1, it reflects the route to its clients and also sends it to Spine-2. Spine-2 then reflects to its own clients. This ensures all leaves receive all routes regardless of which spine they peer with.

Route reflectors introduce a hierarchy but maintain full route distribution. Every leaf learns about every EVPN route, just as they would with full mesh, but the session count is manageable. The route reflector architecture is proven at Internet scale and works equally well for EVPN.

## EVPN Route Encoding in BGP

EVPN routes are carried in BGP UPDATE messages, but the encoding is different from traditional IP routes. Standard BGP UPDATE messages carry Network Layer Reachability Information (NLRI) describing IP prefixes. EVPN uses MP-BGP NLRI format to carry Ethernet information.

An EVPN Type 2 route is encoded as follows. The MP-BGP UPDATE message includes an MP_REACH_NLRI attribute. This attribute has the AFI set to 25 (L2VPN) and SAFI set to 70 (EVPN). The next-hop is set to the advertising VTEP's loopback IP address.

The NLRI itself contains the EVPN route information. The first byte is the route type—for Type 2, this is value 2. Following bytes encode the route-specific information: Route Distinguisher, Ethernet Segment Identifier, Ethernet Tag ID, MAC Address Length, MAC Address, IP Address Length, IP Address, and MPLS Labels.

Extended communities carry additional information. Route Targets determine import/export policy. The VNI is often carried in the MPLS Label field, but some implementations also use extended communities specifically for VXLAN VNI advertisement. The specific encoding details vary slightly between implementations, but the conceptual model is consistent.

When a receiving router processes the BGP UPDATE, it parses the MP_REACH_NLRI, extracts the EVPN route type and information, checks Route Targets against import filters, and if accepted, installs the information in its EVPN tables. The router's control plane translates this EVPN information into data plane forwarding entries.

## Route Targets and Import/Export Control

Route Targets are critical for controlling which VTEPs learn which routes. In a large network with thousands of VNIs and millions of MAC addresses, distributing every route to every VTEP would waste memory and processing. Route Targets provide filtering so VTEPs only learn routes for VNIs they participate in.

Each EVPN route carries one or more Route Target extended communities. A Type 2 route for a MAC in VNI 10100 might be tagged with RT 65000:10100. This RT indicates the route belongs to VNI 10100.

VTEPs configure import and export policies based on RTs. A VTEP configured with "import target RT 65000:10100" will import any routes tagged with this RT. A VTEP not configured for this RT will ignore these routes. Export policy determines which RTs to attach when advertising routes.

In simple deployments, the import/export configuration is automatic. When an administrator configures VNI 10100 on a VTEP, the system automatically generates import and export statements for RT 65000:10100. Routes generated for MACs in this VNI are tagged with this RT on export. Routes with this RT are imported. The VTEP participates in VNI 10100 without manual RT configuration.

In complex deployments with inter-VNI routing or advanced policies, RTs can be manually configured. A gateway routing between VNI 10100 and VNI 10200 might import both RT 65000:10100 and RT 65000:10200, allowing it to learn routes from both VNIs. It might export routes with both RTs, allowing both VNIs to learn routing information. This fine-grained control enables sophisticated traffic engineering and policy implementation.

## BGP Route Selection for EVPN

When a VTEP receives multiple BGP routes for the same destination—perhaps because of multihoming or MAC mobility—BGP's route selection process determines which route to use. The selection algorithm is the same as for standard BGP routes, applied to EVPN routes.

First, routes with highest LOCAL_PREF are preferred. This is rarely used in data center eBGP but can influence route selection in some designs.

Second, routes with shorter AS_PATH are preferred. In leaf-spine eBGP, Type 2 routes from other leaves have AS path length of 2 (originating leaf AS, then spine AS). Direct routes learned locally have effectively zero AS path. Local learning always wins over remote advertisements.

Third, routes learned via eBGP are preferred over iBGP. Data center designs typically use eBGP exclusively, so this criterion does not apply.

Fourth, routes with lowest IGP metric to the next-hop are preferred. If multiple spines reflect the same route to a leaf, the leaf prefers the route via the spine with the best underlay reachability.

Finally, the router ID tiebreaker ensures deterministic selection when all else is equal. If two route reflectors advertise identical routes, the leaf consistently selects based on router ID.

This selection process ensures consistent forwarding. All VTEPs make the same best-path decision for a given route. MAC mobility works cleanly because when a MAC moves and a new route is advertised with more recent timestamp or better path, all VTEPs converge on the new route.

## Graceful Restart and Convergence

BGP sessions occasionally need to restart—software upgrades, configuration changes, or debugging might require restarting the BGP process. Without special handling, a BGP restart withdraws all routes, disrupts forwarding, and takes seconds to minutes to fully recover as routes are relearned and redistributed.

Graceful restart mechanisms minimize this disruption. When a BGP speaker signals it supports graceful restart, its peers preserve the routes learned from it even if the session goes down. The peers continue forwarding using the existing routes while giving the restarting speaker time to recover.

When the restarting speaker comes back up and reestablishes sessions, it sends all its routes again. The peers compare the newly received routes to the preserved routes. If they match, the transition is hitless—forwarding continued using the old routes, and now using the identical new routes, with no disruption. If routes changed, the peers install the updates and adjust forwarding accordingly.

For EVPN, graceful restart means a leaf switch can be upgraded without causing all its MAC addresses to disappear from the network. Spines preserve the Type 2 routes during the restart. Traffic continues flowing to those MACs. When the leaf recovers and readvertises the routes, the network is already in correct state.

## Failure Detection and Recovery

BGP uses keepalive messages to detect failures. BGP peers exchange keepalive messages periodically—typically every 60 seconds. If a peer does not receive a keepalive for the hold time—typically 180 seconds—it declares the session dead.

Three-minute failure detection is too slow for modern data centers. Bidirectional Forwarding Detection (BFD) provides sub-second failure detection. BFD sends lightweight hello packets every 50-300 milliseconds. If BFD does not receive expected hellos, it declares the path failed within a second.

BGP is configured to use BFD for session liveness. When BFD detects a failure, it immediately notifies BGP. BGP tears down the session and withdraws all routes learned from the failed peer. This withdrawal propagates through the network in seconds.

For EVPN, rapid failure detection means MAC addresses behind a failed VTEP are removed from forwarding tables quickly. Traffic destined for these MACs fails rather than being black-holed. When the VTEP recovers, routes are readvertised, and forwarding resumes. The failure and recovery are handled automatically through BGP's mechanisms.

## The Unified Control Plane

The integration of EVPN with BGP creates a unified control plane for the data center. The same BGP sessions carry underlay routing information—which switches can reach which IPs—and overlay information—which MACs are behind which VTEPs. The same route reflectors distribute both types of routes. The same operational procedures apply to both.

This unification is powerful. Operators use the same tools to monitor EVPN and underlay routing. Troubleshooting follows the same patterns—check BGP sessions, examine route advertisements, verify import/export policies. The learning curve for EVPN is reduced because so much is already familiar from traditional BGP operations.

The architectural decision to extend BGP rather than create a new protocol has proven successful. EVPN scales to tens of thousands of endpoints. It interoperates across vendors. It integrates seamlessly with existing BGP infrastructure. The protocol achieves its goals—proactive MAC learning, efficient distribution, scalable operation—by building on BGP's proven foundation.

# EVPN Complete Flow: From Power-On to Production

## Starting From Nothing

We begin with bare metal. Three leaf switches and two spine switches are racked, cabled, and powered on. The physical network exists but nothing is configured. We will build EVPN functionality step by step, understanding what happens at each stage and why.

The goal is a working EVPN network where servers in different racks can communicate transparently, with efficient MAC learning, suppressed broadcasts, and proper isolation. We will configure underlay routing, establish BGP sessions, enable EVPN, and verify traffic flow. This is how EVPN actually gets built in the real world.

## Step One: Basic IP Connectivity

Before anything else, switches need basic IP connectivity. Each switch needs an IP address on its loopback interface and IP addresses on physical interfaces connecting to other switches.

On Leaf-1, we configure loopback0 with IP 10.0.1.1/32. This loopback represents Leaf-1's identity in the network. It will serve as the VTEP source address for VXLAN encapsulation. The /32 mask indicates this is a host address, not a subnet.

On the physical interface connecting Leaf-1 to Spine-1, we configure IP 10.255.1.1/31. The /31 mask is point-to-point addressing—only two addresses exist, one for each end of the link. No broadcast address, no network address, just two hosts. This is efficient for point-to-point links.

On Spine-1's corresponding interface, we configure 10.255.1.0/31. Now Leaf-1 can ping Spine-1's interface at 10.255.1.0. Basic IP connectivity exists on this link.

We repeat this configuration for all links. Leaf-1 to Spine-2 uses 10.255.1.3/31 and 10.255.1.2/31. Leaf-2 to Spine-1 uses 10.255.2.1/31 and 10.255.2.0/31. Every physical link gets a /31 subnet. Every switch gets a loopback.

At this point, switches can ping their directly connected neighbors but cannot reach switches multiple hops away. Leaf-1 can ping its connected spines but cannot reach Leaf-2. We need routing.

## Step Two: Underlay BGP Configuration

We configure BGP to distribute loopback reachability. The underlay uses eBGP with each switch in its own autonomous system. Leaf-1 is AS 65001, Leaf-2 is AS 65002, Leaf-3 is AS 65003. Spines are AS 65100 and AS 65101. Using separate ASs enables automatic loop prevention.

On Leaf-1, we enter BGP configuration mode and configure AS number 65001. We configure the router-id as 10.0.1.1, matching the loopback. We advertise the loopback subnet 10.0.1.1/32 into BGP.

We configure BGP neighbors for each spine connection. Neighbor 10.255.1.0 (Spine-1) is remote AS 65100. Neighbor 10.255.1.2 (Spine-2) is remote AS 65101. For each neighbor, we activate the IPv4 unicast address family, allowing exchange of IPv4 routes.

On Spine-1, we configure AS 65100 and router-id 10.0.100.1. We configure neighbors for each leaf connection—Leaf-1 at 10.255.1.1, Leaf-2 at 10.255.2.1, Leaf-3 at 10.255.3.1. All neighbors are activated for IPv4 unicast.

The BGP sessions establish. We can verify with "show bgp summary" showing sessions in Established state. Leaf-1 advertises its loopback 10.0.1.1/32 to both spines. The spines receive this advertisement and reflect it to all other leaves. Leaf-2 and Leaf-3 learn about 10.0.1.1/32 via BGP.

Now Leaf-1 can ping Leaf-2's loopback 10.0.1.2. The underlay is functional. We have IP routing between all switches. This underlay will carry VXLAN-encapsulated overlay traffic.

## Step Three: EVPN BGP Configuration

Now we extend the BGP sessions to support EVPN. On Leaf-1, under the existing BGP configuration, we add the L2VPN EVPN address family. For each neighbor, we activate the L2VPN EVPN address family in addition to IPv4 unicast. The same BGP session now carries both underlay routes and EVPN routes.

On Spine-1, we activate L2VPN EVPN for each neighbor. Critically, we configure these neighbors as route-reflector clients. This tells Spine-1 to reflect EVPN routes between leaves. Without route-reflector configuration, iBGP rules would prevent route propagation.

The BGP sessions already exist in Established state. The addition of the new address family triggers capability negotiation. If both sides support L2VPN EVPN, the capability exchange succeeds and the session can now carry EVPN routes.

We verify with "show bgp l2vpn evpn summary" showing the same neighbors now active for the EVPN address family. No EVPN routes exist yet because we have not configured any VNIs or endpoints.

## Step Four: VNI Configuration

We configure VNI 10100 on Leaf-1. The configuration specifies VNI 10100, maps it to VLAN 100 locally, and enables EVPN for this VNI. The exact syntax varies by platform, but conceptually we are telling Leaf-1: "Ports in VLAN 100 are part of VNI 10100, and you should advertise endpoints in this VNI via EVPN."

We also configure the VTEP source interface as loopback0. When Leaf-1 encapsulates VXLAN packets, it will use 10.0.1.1 as the source IP address.

The moment we activate VNI 10100, Leaf-1 generates an EVPN Type 3 route. This route says "I am VTEP at 10.0.1.1 and I participate in VNI 10100." The route is tagged with Route Target 65001:10100 (automatically derived from the VNI and AS number). The route is sent via BGP to both spines.

Spines receive the Type 3 route and reflect it to other leaves. However, Leaf-2 and Leaf-3 are not yet configured for VNI 10100. They receive the Type 3 route but ignore it based on Route Target filtering. They are not importing RT 65001:10100 because they are not participating in VNI 10100.

We configure VNI 10100 on Leaf-2 similarly. Leaf-2 generates its own Type 3 route: "I am VTEP at 10.0.1.2 and I participate in VNI 10100." This route is tagged with RT 65002:10100 and distributed via BGP.

Both leaves now import each other's Type 3 routes. Leaf-1 learns that VTEP 10.0.1.2 participates in VNI 10100. Leaf-2 learns that VTEP 10.0.1.1 participates. They build broadcast replication lists for VNI 10100. If either needs to send broadcast or unknown unicast in this VNI, they know where to replicate.

## Step Five: First Endpoint Appears

We connect Server-A to Port 10 on Leaf-1. We configure Port 10 as an access port in VLAN 100 (mapped to VNI 10100). Server-A boots up with IP address 10.1.1.10 and MAC address AA:AA:AA:AA:AA:AA.

Server-A sends its first frame—perhaps a DHCP Discover broadcast. The frame has source MAC AA:AA:AA:AA:AA:AA and destination MAC FF:FF:FF:FF:FF:FF (broadcast). The frame arrives at Leaf-1 on Port 10.

Leaf-1 learns the source MAC through standard Ethernet learning. The MAC table for VLAN 100 gets an entry: MAC AA:AA:AA:AA:AA:AA is on Port 10. This is local learning, same as traditional switches.

Because EVPN is enabled for this VNI, Leaf-1 generates an EVPN Type 2 route. The route contains MAC address AA:AA:AA:AA:AA:AA. If Leaf-1 can determine the IP address from the frame contents or from subsequent ARP traffic, it includes IP 10.1.1.10. The VNI 10100 is encoded in the MPLS label field. The BGP next-hop is set to 10.0.1.1 (Leaf-1's VTEP address). The route is tagged with RT 65001:10100.

This Type 2 route is sent via BGP to both spines. The spines reflect it to Leaf-2. Leaf-2 imports the route because it participates in VNI 10100 and imports RT 65001:10100.

Leaf-2 processes the Type 2 route. It extracts: MAC AA:AA:AA:AA:AA:AA in VNI 10100 is reachable via remote VTEP 10.0.1.1. Leaf-2 installs a MAC table entry for VNI 10100 showing this MAC points to remote VTEP. It also extracts the IP address 10.1.1.10 and installs an ARP suppression entry mapping this IP to the MAC.

Now Leaf-2 knows about Server-A even though no traffic has flowed between the leaves. The Type 2 advertisement proactively distributed this information.

## Step Six: Second Endpoint and ARP Suppression

We connect Server-B to Port 15 on Leaf-2. Server-B has IP 10.1.1.20 and MAC BB:BB:BB:BB:BB:BB, also in VLAN 100 (VNI 10100). The same process occurs: Leaf-2 learns locally, generates Type 2 route, distributes via BGP. Leaf-1 receives and installs the information about Server-B.

Server-B wants to communicate with Server-A at IP 10.1.1.10. Server-B's operating system needs the MAC address for 10.1.1.10. Server-B sends an ARP request: "Who has 10.1.1.10? Tell BB:BB:BB:BB:BB:BB."

The ARP request arrives at Leaf-2. Leaf-2 recognizes this as an ARP request. Before forwarding it as a broadcast, Leaf-2 checks its ARP suppression table. The table has an entry from the Type 2 route: IP 10.1.1.10 maps to MAC AA:AA:AA:AA:AA:AA.

Leaf-2 generates an ARP reply locally. The reply says "10.1.1.10 is at AA:AA:AA:AA:AA:AA" and is sent to Server-B. Server-B receives the reply and caches the ARP entry. The ARP request never left Leaf-2. It was not encapsulated in VXLAN, it was not sent to Leaf-1, it did not reach Server-A. ARP suppression worked.

Server-B now knows the destination MAC and can send actual data.

## Step Seven: Data Plane Traffic Flow

Server-B creates an Ethernet frame destined for Server-A. Source MAC is BB:BB:BB:BB:BB:BB, destination MAC is AA:AA:AA:AA:AA:AA. The payload is an IP packet from 10.1.1.20 to 10.1.1.10. This frame is sent out Server-B's network interface and arrives at Leaf-2 Port 15.

Leaf-2 receives the frame on Port 15, which is in VLAN 100, mapped to VNI 10100. Leaf-2 looks at the destination MAC AA:AA:AA:AA:AA:AA. Leaf-2 checks its MAC table for VNI 10100. The table has an entry from the Type 2 route: this MAC is at remote VTEP 10.0.1.1.

Leaf-2 performs VXLAN encapsulation. The original Ethernet frame is wrapped with VXLAN header containing VNI 10100. A UDP header is added with source port (randomly selected or hashed) and destination port 4789 (standard VXLAN port). An outer IP header is added with source IP 10.0.1.2 (Leaf-2's VTEP address) and destination IP 10.0.1.1 (Leaf-1's VTEP address). Finally, an outer Ethernet header is added for the next-hop routing.

This encapsulated packet enters the underlay network. Leaf-2 looks up destination IP 10.0.1.1 in its IP routing table. The table shows multiple paths via Spine-1 and Spine-2 (ECMP). Leaf-2 performs a hash on the packet headers and selects Spine-1. The packet is forwarded to Spine-1.

Spine-1 receives the packet. It looks up destination 10.0.1.1, finds the route pointing toward Leaf-1, and forwards. The packet arrives at Leaf-1.

Leaf-1 receives a packet with destination IP 10.0.1.1, its own loopback. Leaf-1 examines the packet. It sees UDP destination port 4789 and recognizes VXLAN traffic. Leaf-1 strips the outer Ethernet, IP, and UDP headers. It examines the VXLAN header, sees VNI 10100, and strips the VXLAN header.

Leaf-1 is left with the original Ethernet frame. It looks at destination MAC AA:AA:AA:AA:AA:AA. Leaf-1's local MAC table for VLAN 100 (mapped from VNI 10100) shows this MAC is on Port 10. Leaf-1 forwards the frame out Port 10.

Server-A receives the frame. From Server-A's perspective, it simply received an Ethernet frame from another device on the same network. The VXLAN encapsulation, the BGP control plane, the EVPN advertisements—all invisible to the endpoint.

## Step Eight: Return Traffic

Server-A responds. The process is identical but in reverse. Server-A sends a frame to MAC BB:BB:BB:BB:BB:BB. Leaf-1 receives it, looks up the destination MAC, finds it is at remote VTEP 10.0.1.2 (from Type 2 route), encapsulates with VXLAN and destination IP 10.0.1.2, sends through the underlay to Leaf-2, which decapsulates and delivers to Server-B.

Bidirectional communication works. The traffic flow is symmetric and efficient. No broadcasts occurred except for the initial ARP request, which was suppressed. No flooding occurred. Every forwarding decision was direct based on information proactively learned via EVPN.

## Step Nine: MAC Mobility

Server-A migrates from Leaf-1 to Leaf-3. Perhaps it is a virtual machine that live-migrated, or perhaps the physical connection was moved. Server-A appears on Port 20 of Leaf-3, which is also configured for VNI 10100.

Server-A sends a frame from its new location. Leaf-3 receives the frame, learns MAC AA:AA:AA:AA:AA:AA locally on Port 20. Leaf-3 generates a Type 2 route advertising this MAC with next-hop 10.0.1.3 (Leaf-3's VTEP address).

This Type 2 route is distributed via BGP. Leaf-1 and Leaf-2 both receive it. They already have Type 2 routes for this MAC pointing to Leaf-1. BGP's route selection process compares the routes. Typically, the newer advertisement or the one with a better path is selected. Let us say the new route from Leaf-3 is selected.

Leaf-2 updates its MAC table. The entry for MAC AA:AA:AA:AA:AA:AA is changed from remote VTEP 10.0.1.1 to remote VTEP 10.0.1.3. New traffic from Server-B to Server-A is now encapsulated with destination IP 10.0.1.3 and reaches Leaf-3, which delivers to Server-A at its new location.

Meanwhile, Leaf-1 no longer sees traffic from Server-A. After the MAC aging timeout expires, Leaf-1 ages out the MAC from its local table. When the MAC ages out, Leaf-1 withdraws its Type 2 route. The withdrawal is distributed via BGP. All leaves remove the old route. Only the new route from Leaf-3 remains. Mobility is complete.

## Step Ten: VNI Isolation

We configure VNI 10200 on Leaf-1 and Leaf-2, mapped to VLAN 200 locally. A server in VNI 10200 connects to Leaf-1. The same EVPN process occurs—Type 3 advertisements, Type 2 advertisements for MACs in VNI 10200.

Traffic in VNI 10200 uses the same VXLAN encapsulation mechanism but with VNI field set to 10200 instead of 10100. When Leaf-1 receives VXLAN traffic, it examines the VNI and delivers to the appropriate local VLAN. VNI 10100 traffic goes to VLAN 100 ports. VNI 10200 traffic goes to VLAN 200 ports.

Server-A in VNI 10100 cannot reach servers in VNI 10200. The VNIs provide complete Layer 2 isolation. Even though both VNIs use the same physical infrastructure, the same switches, the same VXLAN encapsulation mechanism, traffic cannot cross between them. Each VNI is a separate broadcast domain with separate MAC learning.

If inter-VNI routing is needed, it requires explicit configuration with Type 5 routes or centralized gateways, which we have not configured. The default is complete isolation.

## Verification Commands

Throughout this process, we verify each step. After underlay BGP configuration, "show bgp summary" confirms sessions are established. "show bgp ipv4 unicast" shows learned loopback routes.

After EVPN configuration, "show bgp l2vpn evpn summary" confirms EVPN sessions. "show bgp l2vpn evpn" displays advertised and received EVPN routes. We see Type 3 routes for VNI membership and Type 2 routes for MAC addresses.

At the data plane level, "show mac address-table" or equivalent displays locally learned MACs and remotely learned MACs. Remote MACs show the VTEP IP they are behind. "show vxlan tunnel" or equivalent displays active VXLAN tunnels to remote VTEPs.

Packet captures on uplink interfaces show VXLAN-encapsulated traffic. We see outer IP headers with source and destination as VTEP loopbacks, UDP port 4789, and VXLAN headers with the VNI. Inside is the original Ethernet frame unchanged.

These verification steps confirm each layer is working: underlay routing, BGP sessions, EVPN advertisements, MAC learning, and data plane forwarding.

## What We Built

We started with bare switches and built a complete EVPN network. The underlay provides IP routing between VTEPs using BGP. The overlay provides virtual Layer 2 networks using VXLAN. The control plane distributes MAC and IP information using EVPN over BGP. Endpoints communicate transparently, unaware of the complexity beneath.

This step-by-step progression is how EVPN networks are actually deployed. You configure the underlay first because the overlay depends on it. You establish BGP sessions before sending EVPN routes. You configure VNIs before connecting endpoints. Each step builds on the previous, and each step can be verified before proceeding.

Understanding this flow—from power-on to production traffic—is understanding EVPN not as an abstract protocol but as a concrete system that solves real problems in real networks.

# EVPN Route Types 1 and 4: Multihoming Support

## The Multihoming Problem

Multihoming means a single device connects to multiple switches simultaneously. This is done for redundancy. If a server has two network interfaces, one connected to Switch A and another connected to Switch B, the server can continue operating even if one switch fails or one link breaks. Multihoming is common in enterprise environments and increasingly used in data centers for critical services.

Traditional Layer 2 networks handle multihoming poorly. When a multihomed device sends traffic, both switches learn its MAC address. When traffic destined for this device arrives at either switch, that switch forwards it locally—no problem. But when broadcast traffic needs to be sent to the device, both switches forward the broadcast, causing the device to receive duplicates. Worse, if the device sends a broadcast, both switches might forward it back to each other, creating loops and potential broadcast storms.

Active-active multihoming, where both links carry traffic simultaneously, requires even more coordination. Both switches must know about each other's connection to the same device. They must avoid forwarding traffic between themselves for this device. They must coordinate which switch handles broadcast traffic to avoid duplication. Without explicit protocol support, achieving stable active-active multihoming is fragile and error-prone.

EVPN solves these problems with explicit multihoming support through Route Type 1 and Route Type 4. These route types allow switches to discover they share access to the same device, coordinate their behavior, and provide redundancy without loops or duplication.

## The Ethernet Segment Identifier

The foundation of EVPN multihoming is the Ethernet Segment Identifier, or ESI. An ESI is a 10-byte value that uniquely identifies a multihomed link or device. When a device connects to multiple switches, all those switches are configured with the same ESI for the ports connecting to that device.

The ESI tells switches: "These ports, even though they are on different switches, connect to the same physical device or network segment." With this knowledge, switches can coordinate. They know that traffic from this ESI appearing on any of them is from the same source. They know broadcast traffic should only be forwarded once across all links to this ESI, not once per switch.

ESI configuration can be manual or automatic. Manual configuration requires the administrator to assign the same ESI value to the appropriate ports on each switch. Automatic methods use protocols like LACP to detect that ports are part of a link aggregation group connecting to the same device, and automatically assign a consistent ESI based on the LACP system ID.

The ESI value zero has special meaning: it indicates the port is single-homed, not multihomed. Most ports in most networks are single-homed, so ESI 0 is the most common value. Only ports that are part of multihomed configurations use non-zero ESIs.

## Route Type 1: Ethernet Auto-Discovery

Route Type 1 is called the Ethernet Auto-Discovery (AD) route. Its purpose is to advertise that a particular switch has a port in a particular Ethernet Segment. This allows other switches to discover which switches share access to the same multihomed device.

The route contains the Route Distinguisher for uniqueness, the ESI of the multihomed link, the Ethernet Tag ID (typically zero), and an MPLS label that identifies the switch. The BGP next-hop is set to the advertising switch's IP address.

When Switch A and Switch B both connect to the same multihomed server with ESI 00:00:00:00:00:01, both switches advertise Type 1 routes for this ESI. Switch A's route says "I am Switch A, and I have ports in ESI 00:00:00:00:00:01." Switch B's route says "I am Switch B, and I have ports in ESI 00:00:00:00:00:01."

Both switches receive each other's Type 1 routes. They examine the ESI and recognize they share access to the same Ethernet Segment. This discovery is automatic—no manual configuration is needed to tell each switch about the other. The EVPN control plane handles the discovery.

Once switches discover they are part of the same Ethernet Segment, they can coordinate their behavior. They establish an Ethernet Segment, a logical construct representing the multihomed link. Within this Ethernet Segment, the switches must elect a Designated Forwarder for broadcast and multicast traffic, which is where Route Type 4 comes in.

## Route Type 4: Ethernet Segment Route

Route Type 4 is called the Ethernet Segment route. It serves a similar discovery function to Type 1 but with a different purpose. While Type 1 announces membership in an Ethernet Segment per VNI, Type 4 announces membership at the Ethernet Segment level independent of VNIs.

Type 4 routes contain the Route Distinguisher, the ESI, and the originating router's IP address. When Switch A advertises a Type 4 route for ESI 00:00:00:00:00:01, it is saying "I am Switch A, and I am one of the switches connected to this Ethernet Segment."

The key function of Type 4 routes is to enable Designated Forwarder (DF) election. When multiple switches connect to the same multihomed device, they cannot all forward broadcast and multicast traffic to that device. If they did, the device would receive duplicate frames. Only one switch should forward broadcasts to the multihomed device—the Designated Forwarder.

The DF election algorithm uses information from Type 4 routes. The switches connected to an Ethernet Segment exchange Type 4 routes. Based on these routes, they run the election algorithm. The algorithm might use lowest IP address, lowest router ID, or some other deterministic criterion. All switches run the same algorithm on the same information, so they all reach the same conclusion about which switch is the DF.

In many deployments, the DF is elected per VNI rather than globally. This means for VNI 10100, Switch A might be the DF, while for VNI 10200, Switch B might be the DF. This provides load distribution—different VNIs use different switches for broadcast forwarding, spreading the load rather than concentrating it on one switch.

## Multihoming in Action

Let us trace how multihoming works with EVPN. A server connects to Switch A via Port 10 and to Switch B via Port 15. Both ports are configured with ESI 00:00:00:00:00:01. The server uses active-active bonding, meaning both links carry traffic simultaneously.

During configuration, both switches advertise Type 1 routes for this ESI. Both also advertise Type 4 routes. Switch A and Switch B receive each other's advertisements and discover they share access to this Ethernet Segment.

The switches run the DF election algorithm. Let us say Switch A is elected as the DF for VNI 10100. Switch A is responsible for forwarding broadcast and multicast traffic in VNI 10100 to the server. Switch B will not forward broadcasts for this VNI to the server, even though it has a working connection.

The server sends a frame. Because of its bonding configuration, the frame might exit via the link to Switch A or the link to Switch B, depending on the bonding algorithm. Let us say the frame exits via Switch B.

Switch B receives the frame on Port 15. Switch B learns the source MAC locally and generates an EVPN Type 2 route. Critically, the Type 2 route includes the ESI 00:00:00:00:00:01 in the Ethernet Segment Identifier field. This tells other VTEPs that this MAC is behind a multihomed link.

Switch A, which also connects to the same server, receives the Type 2 route. Switch A sees the ESI matches the ESI of its own Port 10. Switch A knows this MAC is reachable via the shared Ethernet Segment. If traffic arrives at Switch A for this MAC, Switch A can forward it out Port 10 directly—it does not need to send it across the network to Switch B first.

A remote VTEP, Switch C, receives the Type 2 route from Switch B. Switch C installs a forwarding entry pointing to Switch B's VTEP address. If Switch C needs to send traffic to this MAC, it encapsulates and sends to Switch B. However, Switch C also received the Type 1 routes from both Switch A and Switch B for this ESI. Switch C knows both switches can reach this MAC. If Switch B fails, Switch C can switch to using Switch A.

Now consider broadcast traffic. Switch C needs to send a broadcast in VNI 10100. Switch C has Type 3 routes from Switch A and Switch B indicating both participate in VNI 10100. Without multihoming, Switch C would replicate the broadcast to both Switch A and Switch B. With multihoming, Switch C knows both switches are part of the same Ethernet Segment from the Type 1 routes. Switch C only sends the broadcast to the Designated Forwarder—Switch A. This avoids the server receiving duplicate broadcasts.

## Split-Horizon Filtering

Another critical function enabled by Type 1 routes is split-horizon filtering. When the multihomed server sends a frame, it might exit via Switch A or Switch B. Let us say it exits via Switch A.

Switch A receives the frame, learns the MAC, and generates a Type 2 route. This route is distributed to all VTEPs, including Switch B. Switch B receives the route and sees it is for a MAC in the Ethernet Segment it shares with Switch A.

If a remote VTEP sends traffic to this MAC, it might send to Switch B (based on its load distribution or because it did not learn Switch A's route yet). The traffic arrives at Switch B. Switch B needs to forward to the server. Switch B could forward out Port 15 directly. But Switch B could also encapsulate and send to Switch A, letting Switch A forward out Port 10.

Split-horizon filtering prevents loops in this scenario. Switch B knows this MAC is on the shared Ethernet Segment from the ESI in the Type 2 route. If Switch B receives VXLAN traffic from another VTEP for this MAC, Switch B forwards it locally. But if Switch B receives VXLAN traffic from Switch A for this MAC, Switch B drops it. The assumption is that if Switch A is sending traffic for a MAC in their shared Ethernet Segment, Switch A already tried to deliver it locally and failed. Switch B forwarding it back to the server would create a loop.

This split-horizon behavior is automatic once the Ethernet Segment is established via Type 1 routes. The switches use the ESI information to identify traffic that should not be forwarded between them.

## Active-Active vs Active-Standby

EVPN multihoming supports both active-active and active-standby modes. In active-active, both links carry traffic simultaneously. The server sends and receives through both Switch A and Switch B. Both switches forward unicast traffic. Only the DF forwards broadcasts to avoid duplication, but both switches forward unicast.

In active-standby, only one link carries traffic under normal conditions. Switch A is active, Switch B is standby. The server sends and receives only through Switch A. Switch B's link is idle. If Switch A fails, the server fails over to Switch B, and Switch B becomes active.

The EVPN mechanisms work the same for both modes. Type 1 and Type 4 routes are advertised regardless of mode. The DF election occurs regardless. The difference is in how the multihomed device itself treats the links, which is outside EVPN's control. EVPN provides the infrastructure for either mode to work correctly.

## Fast Convergence on Failure

When Switch A fails, the multihomed server still has connectivity via Switch B. But remote VTEPs need to know about the failure so they stop sending traffic to Switch A. EVPN handles this through BGP withdrawal.

When Switch A's BGP sessions go down due to the failure, the route reflectors withdraw all routes from Switch A. This includes the Type 1 and Type 4 routes for the Ethernet Segment and the Type 2 routes for MACs learned on Switch A.

Remote VTEPs receive the withdrawals. They remove Switch A from their forwarding tables. If they were sending traffic to Switch A for the multihomed server, they switch to Switch B (which they know about from Switch B's Type 1 routes and the ESI in the Type 2 route).

The convergence time is determined by BGP failure detection plus route propagation. With BFD for fast failure detection, this can be sub-second. The multihomed device experiences brief traffic loss during convergence but then continues operating normally through the surviving link.

## When Multihoming Is Not Needed

Most deployments do not use EVPN multihoming. Most servers connect to a single switch. Virtual machines on hypervisors do not need multihoming—the hypervisor handles local redundancy, and VMs are single-homed to the hypervisor's virtual switch. Container networks are typically single-homed.

Multihoming is used for critical infrastructure where link or switch failure cannot be tolerated. Storage systems, load balancers, critical databases—these might use multihoming. The deployment complexity and operational overhead of multihoming are only justified when the redundancy is truly necessary.

For single-homed ports, ESI is set to zero, Type 1 and Type 4 routes are not advertised, and the DF election does not occur. The simpler single-homed forwarding model is sufficient and avoids unnecessary complexity.

## Route Types 1 and 4: Specialized But Important

Type 1 and Type 4 routes serve a specialized purpose: enabling EVPN multihoming. Most networks will not see heavy use of these route types because most ports are single-homed. But for deployments that need active-active multihoming with the redundancy and load distribution it provides, these route types are essential.

They solve the coordination problem: how do multiple switches connected to the same device avoid forwarding loops, avoid broadcast duplication, and provide seamless failover? The answer is explicit signaling through Type 1 and Type 4 routes, allowing switches to discover shared Ethernet Segments, elect Designated Forwarders, and implement split-horizon filtering. The result is robust, standards-based multihoming that works across vendors and scales to large deployments.

# EVPN Troubleshooting: Finding and Fixing Problems

## The Troubleshooting Mindset

EVPN troubleshooting requires understanding that the system has three distinct layers: the underlay network providing IP connectivity, the BGP control plane distributing EVPN routes, and the VXLAN data plane forwarding encapsulated traffic. A problem in any layer can manifest as application-level failure, but the root cause must be identified at the correct layer. Treating a control plane problem as a data plane problem wastes time and leads nowhere.

The troubleshooting approach is systematic. Start by defining the problem precisely. "The network does not work" is useless. "Server A at IP 10.1.1.10 behind Leaf-1 cannot reach Server B at IP 10.1.1.20 behind Leaf-2 in VNI 10100" is specific and actionable. With a precise problem statement, you can form hypotheses about what might be wrong and test them methodically.

Always verify the layers bottom-up. If the underlay is broken, the overlay cannot function. If BGP sessions are down, EVPN routes cannot propagate. If routes are not being advertised or imported, forwarding tables are incomplete. Only when control plane state is verified correct should you investigate data plane forwarding. This layered approach prevents chasing symptoms while missing the actual cause.

## Underlay Problems: The Foundation Must Work

The most fundamental requirement is underlay IP connectivity. VTEPs must be able to route IP packets to each other's loopback addresses. If this basic requirement fails, VXLAN encapsulation cannot deliver packets regardless of how correctly EVPN is configured.

Test underlay connectivity with ping. From Leaf-1, ping Leaf-2's loopback address. If the ping fails, VXLAN traffic cannot flow between these leaves. The problem is in the underlay routing, not in EVPN or VXLAN.

Check the underlay routing table. On Leaf-1, examine the IP routing table for routes to other leaves' loopbacks. If the route exists and points to the correct next-hop, the underlay control plane is working. If the route is missing, check underlay BGP. Are BGP sessions established? Are loopback prefixes being advertised? Are routes being received but not installed?

Common underlay problems include BGP session failures due to misconfigured neighbor IP addresses, wrong AS numbers, or authentication mismatches. Interface configurations might be wrong—MTU mismatches, interface administratively down, or physical layer problems. Routing protocol issues like filtering or incorrect route advertisements prevent route propagation.

MTU problems are particularly insidious. VXLAN adds approximately 50 bytes of overhead. If the underlay MTU is 1500 bytes and inner frames are also 1500 bytes, encapsulated packets will be 1550 bytes and too large. This causes fragmentation or packet drops depending on the "Don't Fragment" bit setting. The symptom is that small packets work but large packets fail. The solution is configuring jumbo frames (9000-byte MTU) on all underlay links.

Verify MTU with ping using large packet sizes. Ping with 1500-byte packets should succeed. If it fails, MTU is insufficient. Increase underlay MTU to at least 1600 bytes, preferably 9000 bytes for headroom.

## BGP Session Problems: Routes Cannot Flow

EVPN routes are distributed via BGP. If BGP sessions are not established or are flapping, route distribution fails. Check BGP session state with "show bgp summary" or equivalent commands. Sessions should be in Established state. If sessions are in any other state—Idle, Connect, Active, OpenSent, OpenConfirm—they are not functional.

If a session is stuck in Connect or Active state, the TCP connection is failing. Check IP connectivity between the BGP neighbors. Verify that TCP port 179 is not blocked by firewalls. Check that the configured neighbor IP address is correct and reachable.

If the session reaches OpenSent but fails there, the OPEN messages are being exchanged but negotiation is failing. Common causes include AS number mismatch (configured AS does not match what the neighbor expects), unsupported capabilities (one side requires something the other does not support), or authentication failure if MD5 authentication is configured.

For EVPN specifically, verify that the L2VPN EVPN address family is configured and activated on both sides of the session. A common mistake is configuring the address family but forgetting to activate it for the neighbor. The session establishes for IPv4 unicast but EVPN routes are not exchanged because the address family is not active.

Check which address families are negotiated with "show bgp neighbor X" commands. The output shows which address families are active for the neighbor. Both IPv4 unicast (for underlay) and L2VPN EVPN (for overlay) should be active.

Route reflector configuration must be correct. If leaves are configured as route reflector clients but the spine does not have route reflector configuration enabled, routes will not be reflected between leaves. Verify route reflector configuration on spines and client configuration on leaves.

## Route Advertisement Problems: Routes Not Being Sent

If BGP sessions are established but expected EVPN routes are not appearing, the problem is in route generation or advertisement. Start by checking whether routes are being generated locally.

For Type 2 routes, verify that MAC addresses are being learned locally. Check the local MAC table on the switch. If a MAC address is not in the local table, it will not be advertised via EVPN. The problem might be that the server is not sending traffic, the port is misconfigured, or VLAN/VNI mapping is wrong.

If the MAC is learned locally but no Type 2 route exists, check EVPN configuration for that VNI. Is EVPN enabled for this VNI? Is the VNI configured correctly? Different platforms have different configuration requirements—some require explicit EVPN enablement, others enable it automatically when VNI is configured.

Check the BGP table for locally originated EVPN routes. The command varies by platform but is typically something like "show bgp l2vpn evpn" with filters for locally originated routes. If the route is in the local BGP table but not being sent to neighbors, check export policy or route filtering.

Route Targets control which routes are advertised. If the locally generated route does not have the correct RT extended communities attached, other VTEPs will not import it. Check that RT configuration matches between the advertising VTEP and importing VTEPs. The RT should be derived from the VNI—typically something like RT:ASN:VNI.

## Route Import Problems: Routes Not Being Accepted

Routes might be advertised correctly but not imported by receiving VTEPs. Check the BGP table on the receiving VTEP. If the route appears in "show bgp l2vpn evpn" output, it was received via BGP. If the route does not appear, the problem is earlier—route advertisement or BGP session issues.

If the route appears in BGP but not in the forwarding table, the problem is import filtering. Check Route Target import configuration. The receiving VTEP must be configured to import routes with the RT that the advertising VTEP is using. If VNI 10100 uses RT 65000:10100, VTEPs participating in VNI 10100 must import this RT.

Some platforms automatically configure RT import/export based on VNI configuration. Others require explicit configuration. Verify that import/export is configured appropriately for your platform.

BGP route selection might prevent route installation. If multiple routes for the same MAC exist, only the best route is used. Check all received routes for a particular MAC. If a route is not being used, compare it to the route that is being used to understand why BGP selected one over the other.

Check for route limits. Some platforms have limits on the number of EVPN routes they can handle. If these limits are exceeded, new routes are rejected. This is rare in modern hardware but can occur in older or lower-end switches.

## Data Plane Problems: Forwarding Table State

If control plane state is correct—routes are advertised, received, and imported—but traffic still fails, the problem is in data plane forwarding. The control plane populated the forwarding tables, but the data plane is not using them correctly.

Check the MAC table on the source VTEP. For the destination MAC, is there an entry? Does it point to the correct remote VTEP? The MAC table should show remote MACs with VTEP IP addresses as next-hops.

If the MAC table is correct, verify VXLAN tunnel state. VXLAN tunnels between VTEPs are usually established automatically when needed, but some platforms require explicit configuration. Check that tunnels exist to the remote VTEPs where destination MACs are located.

Capture packets at the source VTEP to verify encapsulation is happening. When traffic is sent from the local server to the remote MAC, the source VTEP should encapsulate it in VXLAN. If encapsulation does not occur, the VTEP is not recognizing the traffic as requiring encapsulation, perhaps due to incorrect VLAN/VNI mapping.

If packets are encapsulated, capture on the uplink to verify they are being sent with correct headers. The outer IP destination should be the remote VTEP's loopback. The VXLAN header should contain the correct VNI. The inner frame should be unchanged from what the server sent.

Check the destination VTEP. Are encapsulated packets arriving? If not, the underlay is not routing them correctly—back to underlay troubleshooting. If packets arrive but are not decapsulated, check VXLAN configuration. Is the VTEP listening on UDP port 4789? Is it configured to handle VXLAN traffic?

If decapsulation occurs but the frame is not delivered to the local server, check the local MAC table and port configuration. The destination MAC should be in the local table pointing to a local port in the correct VLAN.

## ARP Suppression Not Working

ARP suppression relies on Type 2 routes carrying both MAC and IP information. If ARP is not being suppressed, first verify that Type 2 routes include IP addresses. Check the route with "show bgp l2vpn evpn route-type 2" or equivalent. The route should show both MAC and IP.

If the route has only MAC but no IP, the advertising VTEP did not learn the IP address. This happens if the VTEP only sees Layer 2 frames without inspecting their contents, or if the IP address learning feature is disabled. Enable IP address learning on the advertising VTEP.

If routes include IP addresses but ARP is still not suppressed, verify that ARP suppression is enabled on the receiving VTEP. Some platforms require explicit configuration to enable ARP suppression. Without this configuration, even though the VTEP has the IP-to-MAC binding from EVPN, it does not intercept ARP requests.

Check that the ARP suppression table is populated. Commands vary by platform, but look for ARP cache or ARP suppression table showing IP-to-MAC mappings learned from EVPN. If the table is empty despite having Type 2 routes with IP addresses, there is a configuration or software issue preventing table population.

## Broadcast or Multicast Not Working

Broadcast and multicast rely on Type 3 routes establishing VNI membership. If broadcasts are not reaching all expected destinations, verify Type 3 route advertisement and import.

Check that all VTEPs participating in a VNI are advertising Type 3 routes for that VNI. Each VTEP should advertise its loopback IP as a participant in the VNI. If a VTEP is not advertising a Type 3 route, check VNI configuration and ensure the VNI is active.

On the source VTEP, check the broadcast replication list for the VNI. This should show all remote VTEP IPs that received Type 3 routes for this VNI. If a VTEP is missing from the list, its Type 3 route was not imported—check RT filtering.

If the replication list is correct but broadcasts still do not reach some VTEPs, check the data plane forwarding. When a broadcast is sent, the source VTEP should create copies for each VTEP in the replication list. Packet captures should show multiple VXLAN packets being sent, one to each destination VTEP.

For multihomed scenarios, verify that only the Designated Forwarder is sending broadcasts to the multihomed device. If both switches send broadcasts, the device receives duplicates. Check Type 4 route advertisement and DF election. The non-DF switch should not forward broadcasts for the VNI even though it participates in the VNI.

## Failure Scenarios and Expected Behavior

Understanding how EVPN should behave during failures helps distinguish normal failover from actual problems. When a VTEP fails completely—power loss or catastrophic hardware failure—its BGP sessions to route reflectors time out. This takes 3 minutes with default keepalive timers, or sub-second with BFD.

When the session times out, route reflectors withdraw all routes from the failed VTEP. Other VTEPs receive withdrawals for all Type 2, Type 3, and Type 5 routes from the failed VTEP. They remove affected entries from their forwarding tables. Traffic to MACs that were behind the failed VTEP now has no forwarding entry and is dropped. This is expected—those endpoints are unreachable because their access switch is dead.

When the VTEP recovers, it reestablishes BGP sessions and readvertises all routes. Other VTEPs receive the advertisements and reinstall forwarding entries. Traffic resumes. The failure and recovery are handled automatically through BGP mechanisms with no manual intervention required.

Link failures are more subtle. If a leaf-spine link fails, the leaf loses one of its BGP sessions. If the leaf peers with multiple spines (best practice), it still has connectivity via remaining spines. Routes continue to flow through the working sessions. Data plane traffic redistributes across working uplinks via ECMP. The failure is transparent to endpoints.

If all uplinks from a leaf fail, the leaf becomes isolated from the underlay. BGP sessions to all spines go down. Route reflectors withdraw the leaf's routes. Other VTEPs can no longer reach endpoints behind the isolated leaf. The isolated leaf still has its own local forwarding—servers behind it can communicate with each other—but no inter-rack traffic works. This is expected with complete isolation.

## Common Misconfigurations

Certain mistakes appear repeatedly in EVPN deployments. VLAN/VNI mapping mismatches are common. The VNI must be consistently mapped to the same VLAN across all VTEPs, or traffic in different VLANs is placed in the same VNI, breaking isolation. Verify that VNI 10100 maps to VLAN 100 (or whatever VLAN is used) on all leaves.

Route Target mismatches prevent route import. If one VTEP advertises with RT 65000:10100 but another VTEP imports RT 65001:10100, routes are not imported. Check that RT configuration is consistent. In most deployments, RT is auto-derived from VNI, which avoids this mistake.

Loopback addresses used as VTEP source must be unique and reachable. If two VTEPs use the same loopback IP, traffic is delivered incorrectly. If a loopback is not advertised into underlay BGP, it is not reachable, and VXLAN traffic cannot be delivered.

BGP AS number configuration errors prevent session establishment. In eBGP designs where each leaf is its own AS, the AS numbers must be configured correctly and consistently. If Leaf-1 is configured as AS 65001 but a spine thinks it is AS 65002, the session will not establish.

Forgetting to activate address families after configuring them is common. BGP configuration might define the L2VPN EVPN address family but not activate it for the neighbor. The session works for IPv4 unicast but not for EVPN. Always verify that the address family is both configured and activated.

## Systematic Troubleshooting Workflow

When faced with an EVPN problem, follow this workflow:

Define the problem precisely. Which endpoints cannot communicate? Which VNI? Which VTEPs are involved?

Verify underlay connectivity. Can the involved VTEPs ping each other's loopbacks? Check with ping tests and routing table inspection.

Verify BGP sessions. Are sessions to route reflectors established? Are both IPv4 unicast and L2VPN EVPN address families active?

Verify route advertisement. Are the expected EVPN routes being generated on the source VTEP? Check local BGP table for Type 2, Type 3, or Type 5 routes as appropriate.

Verify route propagation. Are routes reaching the destination VTEP? Check BGP table on destination VTEP.

Verify route import. Are routes being imported and installed in forwarding tables? Check RT configuration and forwarding table state.

Verify data plane forwarding. Are packets being encapsulated correctly? Are they traversing the underlay? Are they being decapsulated and delivered?

At each step, if the verification fails, the problem is identified. Fix the problem at that layer before proceeding. If a step succeeds, move to the next step. This systematic approach efficiently narrows down the problem rather than randomly trying fixes.

EVPN troubleshooting is not guesswork. The protocol's operation is well-defined, and the state at each layer can be inspected. With systematic verification of each layer and careful examination of configuration and state, problems can be identified and resolved methodically.

