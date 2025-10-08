# Understanding IS-IS Protocol: Core Architecture and Operation

## What IS-IS Really Is (And Why It Matters)

Before we dive into mechanisms and algorithms, let's understand what problem IS-IS solves. When you have a network with dozens or hundreds of routers, how does each router know the best path to every destination? IS-IS is a link-state routing protocol that answers this question by having every router build a complete map of the network topology, then calculate optimal paths using that map.

Here's the critical insight: IS-IS doesn't tell you "to reach network X, go through router Y." Instead, it tells you "here's what the entire network looks like—every router, every link, every cost." Then you calculate the best path yourself. This is fundamentally different from distance-vector protocols where routers trust what their neighbors tell them about distant destinations.

## The OSI Heritage (Why IS-IS Looks Strange)

IS-IS originated in the OSI protocol suite, designed to route CLNP (Connectionless Network Protocol) packets. When the industry moved to IP, engineers needed to adapt IS-IS rather than abandon it. This created "Integrated IS-IS," which routes IP traffic using an OSI-designed protocol.

This history explains several peculiarities you'll encounter. IS-IS doesn't use IP addresses to identify routers—it uses System IDs, which are OSI-style identifiers. IS-IS packets aren't carried in IP packets; they're encapsulated directly in data-link frames (like Ethernet). This makes IS-IS a Layer 2 protocol that happens to make Layer 3 routing decisions. Think of it this way: while OSPF routers talk to each other using IP addresses, IS-IS routers talk using MAC addresses.

This design choice has profound implications. Since IS-IS doesn't depend on IP, it can route IPv4, IPv6, or any other protocol simply by carrying different information in its advertisements. The protocol mechanism stays the same; only the payload changes.

## The Two-Level Hierarchy (Building Scalable Networks)

IS-IS divides networks into areas, but not quite like OSPF does. In IS-IS, the hierarchy is cleaner and more intuitive. Every IS-IS router operates at one of three levels: Level 1, Level 2, or Level 1-2.

A Level 1 router knows everything about its own area but nothing about other areas. It maintains a detailed map of every router and link within its area. When it needs to send traffic outside its area, it doesn't need to know the external topology—it just forwards packets to the nearest Level 1-2 router. This is similar to how you might know every street in your neighborhood but rely on highway signs to reach other cities.

Level 2 routers form the backbone of the network. They connect different areas together and know how to reach every area, but they don't care about the internal details of each area. A Level 2 router maintains a map showing which areas exist and how to reach them, but it doesn't store every individual route within those areas.

Level 1-2 routers participate in both worlds. They maintain two separate databases: one containing the detailed map of their local area (Level 1 database) and another containing the area-level topology (Level 2 database). These routers act as border gateways between areas. When a Level 1 router in area 49.0001 needs to reach a network in area 49.0002, it sends packets to a Level 1-2 router, which consults its Level 2 database to forward the packet toward the destination area.

Here's what makes this elegant: Level 1 routers don't need to know anything about the backbone topology. They don't need to understand how areas interconnect. They trust that Level 1-2 routers will handle inter-area routing. This dramatically reduces memory requirements and speeds up convergence for access-layer devices.

## Network Entity Titles (The Strange Addressing Scheme)

IS-IS doesn't identify routers with IP addresses. Instead, it uses a Network Entity Title (NET), which is an OSI-style address. A typical NET looks like: `49.0001.1921.6800.1001.00`

Let's decode this structure. The NET consists of three parts: Area ID, System ID, and NSEL (N-Selector).

The Area ID (49.0001 in this example) identifies which area this router belongs to. All Level 1 routers in the same area must have identical Area IDs, or they won't form adjacencies. The "49" is an AFI (Authority and Format Identifier) indicating this is a private address space—similar to RFC 1918 addresses in IP. Most deployments use 49 for simplicity.

The System ID (1921.6800.1001) uniquely identifies this specific router within the network. This is typically 6 bytes (represented as 12 hexadecimal digits, often written with dots for readability). Many network engineers derive the System ID from a loopback IP address. For example, if the loopback is 192.168.1.1, you might use 1921.6800.1001 as the System ID. Why? Because loopback interfaces don't go down when physical interfaces fail, providing stability.

The NSEL (00) is always zero for routers. In OSI terminology, a zero NSEL indicates you're addressing the entire system rather than a specific service. You'll always see .00 at the end of a router's NET.

Understanding NETs is crucial because every IS-IS configuration starts by assigning a NET to the router. This single configuration line tells the router its identity and area membership simultaneously.

## How Routers Discover Each Other

IS-IS routers discover neighbors by sending Hello messages called IIHs (IS-IS Hello PDUs). Unlike OSPF's relatively complex neighbor state machine with eight states, IS-IS keeps it simpler with three core states: Down, Initializing, and Up.

When an IS-IS interface comes up, the router starts sending IIH packets. These hellos are sent directly at Layer 2 to a multicast MAC address (01:80:C2:00:00:14 for Level 1, 01:80:C2:00:00:15 for Level 2). Notice these aren't IP multicast addresses—this is pure Layer 2 communication.

The IIH packet contains critical information: the router's System ID, the area it belongs to (for Level 1), the interfaces it's sending from, and a list of neighbors it currently sees. When router A receives an IIH from router B, it adds B to its own neighbor list. When router B receives A's next IIH and sees itself listed in A's neighbor list, B knows the communication is bidirectional. At this point, the adjacency moves to the Up state.

This two-way handshake is important. Just because A can hear B doesn't mean B can hear A—perhaps there's a misconfigured interface or a unidirectional link failure. IS-IS won't route traffic over the link until both sides confirm they can communicate bidirectionally.

For an adjacency to form successfully, routers must agree on several parameters. For Level 1 adjacencies, both routers must be in the same area—their Area IDs must match. For Level 2 adjacencies, area membership doesn't matter since Level 2 routers form the backbone across area boundaries. However, both routers must support the same Level (both must be Level 2 capable). If authentication is configured, the authentication keys must match. If MTU sizes differ significantly, problems may arise with LSP flooding.

## Link-State PDUs (How Topology Information Spreads)

Once adjacencies form, routers exchange topology information using Link-State PDUs (LSPs). Think of an LSP as a router's business card—it announces "here's who I am, here's what I'm connected to, and here's the cost to reach my neighbors."

Each router generates its own LSP describing itself and its local links. When router A creates or updates its LSP, it floods this LSP to all neighbors. Those neighbors store the LSP in their database and forward it to their neighbors (except back to A). This flooding continues until every router in the area has a copy.

LSPs are identified by two critical fields: the LSP ID and a sequence number. The LSP ID consists of the originating router's System ID plus additional bytes. The sequence number starts at 1 when a router boots and increments every time the router needs to advertise new information. If a router's interface goes down, it generates a new LSP with a higher sequence number advertising the updated topology.

When a router receives an LSP, it compares the sequence number with what it has in its database. If the received LSP has a higher sequence number, it's newer—the router updates its database and floods the LSP to its neighbors. If the received LSP has a lower sequence number, it's stale—the router sends back its newer copy. If the sequence numbers match, the LSPs are identical, and no action is needed. This mechanism ensures all routers eventually converge on the same topology view.

To manage LSP synchronization efficiently, IS-IS uses two additional PDU types: CSNPs (Complete Sequence Number PDUs) and PSNPs (Partial Sequence Number PDUs).

A CSNP is like an index of all LSPs a router currently has in its database. On point-to-point links, routers exchange CSNPs periodically to verify they have identical databases. If router A sends a CSNP and router B notices it's missing an LSP that A has, B requests that LSP using a PSNP. On broadcast networks, the Designated Intermediate System (which we'll discuss shortly) sends CSNPs periodically, allowing all routers to stay synchronized.

A PSNP explicitly requests specific LSPs. If a router realizes it's missing LSP X (perhaps after comparing sequence numbers in a CSNP), it sends a PSNP asking for LSP X. This provides a reliable retransmission mechanism without requiring acknowledgment of every single LSP.

## The Designated Intermediate System

On broadcast networks like Ethernet, IS-IS elects a Designated Intermediate System (DIS). This is conceptually similar to OSPF's Designated Router, but with important differences.

Without a DIS, consider what happens when five routers connect to the same Ethernet segment. Each router would form adjacencies with the other four, resulting in ten adjacencies total. Each router would generate an LSP describing all four of its neighbors. When you calculate shortest paths, you'd need to examine all these interconnected relationships.

The DIS simplifies this dramatically by creating a pseudonode—a virtual node representing the broadcast segment itself. Instead of every router forming adjacencies with every other router, all routers form adjacencies with the pseudonode. The five routers now have five adjacencies (each router to the pseudonode) instead of ten. The DIS generates an LSP for the pseudonode describing which routers are connected to the segment.

DIS election is simple: the router with the highest priority wins, with ties broken by the highest MAC address. Unlike OSPF, there's no backup DIS. If the current DIS fails, a new DIS is elected immediately (within seconds), and it starts generating pseudonode LSPs. This fast election and preemption make the lack of a backup acceptable.

Here's a crucial difference from OSPF: when a new router joins the segment with a higher priority, it immediately becomes the DIS. In OSPF, the DR doesn't change unless it fails completely. IS-IS's preemptive election prevents situations where a suboptimal router remains DR just because it was first.

## The Link-State Database

Every IS-IS router maintains a link-state database (LSDB) containing all LSPs it has received. This database is literally a map of the network. For Level 1 routers, the database contains all LSPs from routers in their area. For Level 2 routers, it contains LSPs describing the backbone topology. For Level 1-2 routers, there are two separate databases—one for each level.

When you look at the LSDB, you can see every router in the area, every link between routers, and the cost of each link. This is the raw material the SPF algorithm uses to calculate routes.

The database is synchronized across all routers through the flooding mechanism we discussed. If router A's LSDB differs from router B's, the CSNPs and PSNPs will detect this discrepancy and request the missing information. Given enough time without failures, all routers converge to identical LSDBs.

## Shortest Path First Algorithm

Once a router has a complete LSDB, it needs to determine the best path to every destination. This is where the Shortest Path First (SPF) algorithm—also known as Dijkstra's algorithm—comes into play.

The SPF algorithm treats the network as a graph where routers are nodes and links are edges with associated costs. Starting from itself as the root, the router calculates the lowest-cost path to every other node in the graph. This produces a shortest-path tree with the router at the center.

Here's how the algorithm works conceptually. Initially, only the calculating router is in the "known" set. The algorithm examines all links directly attached to the known router and identifies the neighbor with the lowest total cost. This neighbor is added to the known set. The process repeats: examine all links from the known set to unknown nodes, add the lowest-cost unknown node to the known set, and continue until all nodes are processed.

For Level 1-2 routers, this calculation happens twice—once for the Level 1 database and once for the Level 2 database. The Level 1 calculation determines best paths to destinations within the area. The Level 2 calculation determines best paths to other areas and external routes. When forwarding a packet, the router checks its routing table, which contains the results of both SPF calculations.

The cost of each link is configurable, typically based on bandwidth. A 10 Gbps link might have cost 1, while a 1 Gbps link has cost 10. By manipulating costs, you can influence traffic flow through the network—a fundamental traffic engineering technique.

## Route Installation and Forwarding

After SPF completes, the router knows the best next hop to reach every destination. This information is installed in the routing table. When a packet arrives, the router performs a longest-prefix match in the routing table and forwards the packet to the indicated next hop.

If multiple equal-cost paths exist to a destination—suppose the SPF calculation found two paths both costing 30—IS-IS can install all equal-cost paths in the routing table. This enables Equal-Cost Multipath (ECMP) forwarding, where traffic is distributed across multiple links, improving utilization and providing automatic load balancing.

Level 1 routers install a default route pointing toward the nearest Level 1-2 router for inter-area traffic. They don't need specific routes to other areas; the default route handles everything external. This keeps Level 1 routing tables small.

Level 1-2 routers can optionally "leak" specific routes from Level 2 into Level 1. This allows Level 1 routers to see external destinations without requiring full Level 2 information. Route leaking is typically used for important destinations where optimal routing is critical, while the default route handles everything else.

## The Power of Type-Length-Value Encoding

IS-IS's extensibility comes from its extensive use of Type-Length-Value (TLV) encoding. Instead of fixed-format packets where byte 10 always means the same thing, IS-IS uses variable-length TLV fields. Each TLV has a type code indicating what information it carries, a length field specifying how many bytes follow, and the value itself.

When IS-IS needed to support IPv6, engineers didn't redesign the protocol. They defined new TLVs for IPv6 reachability information. Routers that understand IPv6 TLVs process them; older routers ignore them without breaking. This same mechanism enabled Segment Routing support—new TLVs carry Segment IDs without changing the core protocol.

This is brutally elegant. One protocol instance can carry IPv4 routes, IPv6 routes, and MPLS labels simultaneously, each in appropriate TLVs. You don't run separate protocol instances for IPv4 and IPv6. You configure IS-IS once, and it handles both address families.

## Authentication and Security

IS-IS supports authentication to prevent unauthorized routers from injecting false routing information. Authentication can be configured at different levels: interface-level (for IIH packets) and area/domain-level (for LSPs and SNPs).

Without authentication, a malicious device could send bogus LSPs claiming to be a legitimate router, advertising itself as the best path to important destinations. All routers would update their LSDBs with this false information and start forwarding traffic to the attacker. Authentication prevents this by requiring a shared secret—only routers knowing the secret can participate in routing.

Modern deployments use HMAC-MD5 or stronger cryptographic authentication. Each packet includes a hash computed using the packet contents and the shared secret. Receiving routers recompute the hash and verify it matches. If the hashes don't match, the packet is dropped. An attacker without the secret cannot generate valid packets.

## Convergence and Failure Recovery

When something fails—a link goes down, a router crashes—how quickly does the network adapt? This is the convergence question, and it's critical for network availability.

When router A detects a link failure (perhaps by missing several consecutive hellos from neighbor B), it immediately generates a new LSP with an incremented sequence number, removing the failed link from its advertisements. This LSP floods through the network. Every router receiving it updates its LSDB and recalculates SPF. Within seconds, all routers have new shortest-path trees that route around the failure.

Modern IS-IS implementations include Fast Reroute (FRR) mechanisms using Loop-Free Alternates (LFAs). The idea is simple but powerful: during normal operation, the router pre-calculates backup paths. If the primary next hop fails, the router immediately switches to the pre-calculated backup without waiting for SPF to run. This can reduce convergence time from seconds to milliseconds.

The router analyzes its LSDB and asks: "If my primary path to destination X fails, which neighbor could I use as an alternate without creating a routing loop?" An LFA is a neighbor that has a path to the destination that doesn't route back through the calculating router. By pre-computing these alternates, the router eliminates the SPF calculation delay when failures occur.

## Scalability Considerations

As networks grow, several factors affect IS-IS scalability: the size of the LSDB, the number of neighbors, and the frequency of topology changes.

The two-level hierarchy addresses LSDB size. By keeping Level 1 databases limited to a single area and summarizing inter-area routing at Level 2, you prevent the LSDB from growing uncontrollably. An area with 50 routers might have 50 LSPs in its Level 1 database. Even if the network has 20 such areas (1,000 total routers), each Level 2 router only maintains LSPs for the backbone routers, not for every access router.

Route summarization further reduces LSP size. Instead of advertising ten specific prefixes (192.168.1.0/24 through 192.168.10.0/24), you advertise one summary (192.168.0.0/20). This reduces the routing table size and decreases SPF calculation time since fewer prefixes need processing.

LSP throttling controls how quickly routers respond to changes. Without throttling, rapid topology changes (link flapping) cause continuous SPF calculations, consuming CPU and potentially destabilizing the network. LSP throttling implements exponential backoff: after generating an LSP, the router waits before generating another. If changes continue, the wait time increases exponentially up to a maximum. This prevents router CPU exhaustion during instability.

Mesh groups are used in fully-meshed non-broadcast networks to prevent LSP flooding storms. In a full mesh of N routers, without mesh groups, each LSP would be flooded N-1 times. With mesh groups, routers in the same group don't flood LSPs between themselves—they know they'll all receive the LSP anyway. This dramatically reduces unnecessary flooding in large-scale provider networks.

## Why These Details Matter

Understanding IS-IS at this level isn't academic. When you're troubleshooting why two routers won't form an adjacency, you need to know that Level 1 adjacencies require matching Area IDs. When you're designing a large network, you need to understand how the two-level hierarchy reduces LSDB size and speeds convergence. When you're implementing IPv6, you need to know that IS-IS handles it through TLVs without requiring a separate protocol instance.

The protocol's design choices—Layer 2 operation, OSI addressing, TLV extensibility, simple state machine—all have practical consequences. Some make IS-IS more complex to configure initially (NETs aren't as intuitive as IP addresses). Others make it more scalable and flexible (single protocol instance for multiple address families). Understanding these tradeoffs helps you decide when IS-IS is the right tool and how to deploy it effectively.

IS-IS has proven itself in the largest networks on the planet—major ISPs run IS-IS as their IGP specifically because it scales well and converges quickly. The protocol isn't perfect, but its design has aged remarkably well. The fundamentals we've covered here—LSP flooding, SPF calculation, two-level hierarchy—remain unchanged since the protocol's inception, while the TLV mechanism has allowed continuous evolution to support modern requirements.

This foundation prepares you to tackle advanced topics: Segment Routing for traffic engineering, BFD integration for sub-second failure detection, IPv6 multi-topology routing, and more. But these advanced features all build on the core concepts we've explored. Master these fundamentals, and the rest becomes much more approachable.

# IS-IS PDU Types and Packet Formats: Complete Breakdown

## Understanding Protocol Data Units

Before we dissect the actual packet structures, let's understand why IS-IS uses the term "PDU" rather than "packet" or "message." This terminology comes directly from the OSI reference model, where different layers use specific names for their data units. The physical layer deals with bits, the data-link layer with frames, the network layer with packets, the transport layer with segments or datagrams, and higher layers with messages or data. The term "Protocol Data Unit" is a generic term that can refer to the data unit at any layer.

IS-IS, being an OSI protocol adapted for IP, retained OSI terminology. When we say "IS-IS PDU," we're referring to the data structure that IS-IS creates and processes. These PDUs are encapsulated directly in data-link frames (like Ethernet frames), not in IP packets. This is a crucial architectural distinction we've discussed before, but it affects how we think about these data structures.

IS-IS defines several PDU types, each serving a specific purpose in the protocol operation. Understanding each PDU type's structure, purpose, and how routers process them is essential for deep IS-IS knowledge.

## The Common PDU Header

Every IS-IS PDU begins with a common header structure containing fields that identify and describe the PDU. This common header is 8 bytes long and appears at the start of every PDU type before any type-specific fields or variable-length information.

Let's examine each field in the common header in detail.

**Intradomain Routing Protocol Discriminator (1 byte)**

The first byte of every IS-IS PDU is the Intradomain Routing Protocol Discriminator, which has the hexadecimal value 0x83. This value identifies the PDU as belonging to the ISO 10589 IS-IS protocol, distinguishing it from other OSI protocols that might use the same data-link encapsulation.

When a router receives a frame and needs to determine what protocol it carries, it examines this discriminator. If it sees 0x83, it knows this is an IS-IS PDU and passes it to the IS-IS process for handling. If it sees a different value, the frame belongs to a different protocol.

This field exists because OSI defined many network layer protocols (CLNP, ES-IS, IS-IS, and others), and they needed a way to multiplex them over the same data-link layer. The discriminator serves as a protocol identifier at the OSI network layer.

**Length Indicator (1 byte)**

The second byte specifies the length of the entire PDU header in bytes. Different PDU types have different header lengths because they contain different fields. The Length Indicator tells the receiving router where the header ends and the variable-length payload (TLVs) begins.

For example, an IS-IS Hello PDU might have a header length of 27 bytes, while an LSP might have a header length of 27 bytes as well (they happen to be the same), but a CSNP has a different header length. The receiving router reads this field to know how many bytes constitute the fixed header before variable data begins.

This field only counts the fixed header, not the variable-length TLVs that follow. If a PDU has 27 bytes of fixed header followed by 100 bytes of TLVs, the Length Indicator is 27, not 127.

**Version/Protocol ID Extension (1 byte)**

The third byte contains the version of the IS-IS protocol. The current version is 1, so this byte is 0x01 in all modern IS-IS implementations. This field allows for future protocol versions while maintaining backward compatibility.

If IS-IS were ever to have a version 2 with incompatible changes, implementations could set this byte to 0x02. Routers receiving PDUs with version 2 could either process them (if they support version 2) or ignore them (if they only support version 1). In practice, IS-IS has remained at version 1 for decades because the TLV extensibility mechanism allows adding new features without changing the base protocol version.

**ID Length (1 byte)**

The fourth byte specifies how many bytes the System ID uses in this IS-IS domain. As we discussed in the addressing document, System IDs are typically 6 bytes, but the OSI architecture allowed flexibility. This field communicates the System ID length so routers can correctly parse addresses in the PDU.

In modern IS-IS deployments, this byte is almost always 0x06 (decimal 6), indicating 6-byte System IDs. Some very old or specialized implementations might use different lengths, but 6 bytes is the de facto standard.

This field is particularly important for parsing LSP IDs and other address fields within the PDU. If the router sees ID Length = 6, it knows that when it encounters a System ID field, that field will be 6 bytes long. If it saw ID Length = 8, it would expect 8-byte System IDs.

**PDU Type (1 byte)**

The fifth byte identifies what type of PDU this is. This is arguably the most important field in the header because it tells the router how to interpret the rest of the PDU. Different PDU types have different structures after the common header.

The PDU Type values are:
- 0x0F: Level 1 LAN Hello PDU
- 0x10: Level 2 LAN Hello PDU  
- 0x11: Point-to-Point Hello PDU
- 0x12: Level 1 Link State PDU
- 0x14: Level 2 Link State PDU
- 0x18: Level 1 Complete Sequence Number PDU
- 0x19: Level 2 Complete Sequence Number PDU
- 0x1A: Level 1 Partial Sequence Number PDU
- 0x1B: Level 2 Partial Sequence Number PDU

Notice that most PDU types come in Level 1 and Level 2 variants (with separate type codes), while Point-to-Point Hellos are level-agnostic—the same hello type is used for both Level 1 and Level 2 on point-to-point links.

When a router receives a PDU, it checks the PDU Type field to determine whether this is a Hello, LSP, CSNP, or PSNP, and whether it's Level 1 or Level 2. Based on this, it invokes the appropriate processing logic.

**Version (1 byte)**

The sixth byte is another version field, redundant with the earlier Version/Protocol ID Extension field. It's also set to 0x01. This redundancy exists for historical reasons in the OSI protocol design. In modern IS-IS, this byte is always 1.

**Reserved (1 byte)**

The seventh byte is reserved for future use and set to 0x00. Implementations ignore this byte when receiving PDUs and set it to zero when sending. It provides room for future extensions if needed, though the TLV mechanism has proven sufficient for extensibility.

**Maximum Area Addresses (1 byte)**

The eighth and final byte of the common header indicates how many area addresses this router supports. Most routers support three area addresses (value 0x03), which allows for area renumbering scenarios where routers temporarily claim membership in multiple areas.

As we discussed in the addressing document, supporting multiple area addresses enables smooth area renumbering. During a transition, routers can be configured with both old and new area addresses, allowing adjacencies to form with neighbors using either address. Once all routers have the new address, the old address can be removed.

A router receiving a Hello PDU checks this field to ensure the neighbor supports at least as many area addresses as it has configured. If there's a mismatch (one router configured with 4 area addresses but its neighbor only supports 3), there could be problems forming adjacencies in certain scenarios.

## IS-IS Hello PDUs (IIH)

IS-IS Hello PDUs are used to discover neighbors and establish adjacencies. There are three types of hello PDUs: Level 1 LAN Hello, Level 2 LAN Hello, and Point-to-Point Hello. Let's examine each in detail.

### LAN Hello PDU Structure

LAN Hello PDUs (both Level 1 and Level 2) are sent on broadcast networks like Ethernet. After the 8-byte common header, a LAN Hello contains these fixed fields:

**Circuit Type (1 byte)**

This field indicates the level(s) the router is operating at on this interface. Values are:
- 0x01: Level 1 only
- 0x02: Level 2 only  
- 0x03: Level 1 and Level 2

This allows a receiving router to understand what levels its neighbor supports on this particular interface, which may differ from the neighbor's global configuration if interfaces are individually configured for specific levels.

**Source ID (ID Length bytes, typically 6)**

This field contains the System ID of the router sending this hello. This is how the receiving router learns the sender's identity. Remember, IS-IS doesn't use IP addresses to identify routers—it uses System IDs. The Source ID in the hello allows the receiver to know who sent this PDU.

If the ID Length (from the common header) is 6, this field is 6 bytes. If ID Length were 8, this field would be 8 bytes. This is why the ID Length field in the common header is important—it tells us how to parse variable-length fields like this one.

**Holding Time (2 bytes)**

The Holding Time specifies how many seconds the receiving router should consider this adjacency valid without receiving another hello. If the Holding Time expires without receiving a new hello from this neighbor, the router declares the neighbor down and tears down the adjacency.

Typical hello intervals are 10 seconds, with holding times of 30 seconds (3× the hello interval). This gives some margin for lost hellos. If the link is congested and one or two hellos are dropped, the adjacency doesn't flap as long as at least one hello arrives within the holding time.

The Holding Time is expressed as a 16-bit unsigned integer, allowing values from 0 to 65,535 seconds. A holding time of 0 has special meaning—it immediately tears down the adjacency. This is used when a router administratively shuts down an IS-IS interface; it sends a hello with holding time 0 to signal neighbors that the adjacency is going down.

**Priority (1 byte)**

The Priority field is used for DIS (Designated Intermediate System) election on broadcast networks. The router with the highest priority becomes the DIS, with ties broken by highest System ID. Priority ranges from 0 to 127.

A priority of 0 means "I refuse to become DIS." A router with priority 0 will never be elected DIS, even if it's the only router on the segment (in which case, no DIS would be elected, which could cause problems).

The default priority is typically 64, making it easy to configure some routers with higher priority (to prefer them as DIS) or lower priority (to make them backup choices).

Unlike OSPF's DR election, IS-IS DIS election is preemptive. If a router with higher priority joins the segment, it immediately becomes the new DIS, even if another router was already serving as DIS. This ensures the most suitable router always acts as DIS.

**LAN ID (ID Length + 1 bytes, typically 7)**

The LAN ID identifies the broadcast network and consists of the current DIS's System ID plus a 1-byte pseudonode number. For example, if the DIS has System ID `1921.6800.1001` and assigns pseudonode number `01` to this segment, the LAN ID is `1921.6800.1001.01`.

The pseudonode number allows a single DIS to create multiple pseudonodes if it's the DIS on multiple segments. Each segment gets a unique pseudonode number.

The LAN ID serves as a stable identifier for the broadcast segment. Even if the DIS changes (a new router with higher priority joins), the new DIS will take over as DIS and update the LAN ID to reflect its own System ID. Routers on the segment see the LAN ID change in subsequent hellos and adjust their LSPs accordingly.

After these fixed fields, the LAN Hello contains variable-length TLV fields carrying additional information. We'll discuss TLVs in detail later, but important TLVs in hellos include:
- Area Addresses TLV: Lists the area addresses the sender claims membership in
- IS Neighbors TLV: Lists the System IDs of neighbors the sender has heard hellos from  
- Authentication TLV: Contains authentication information if authentication is enabled
- IP Interface Address TLV: Lists IP addresses configured on this interface
- Protocols Supported TLV: Indicates which protocols (IPv4, IPv6) the router supports

### Point-to-Point Hello PDU Structure

Point-to-Point Hello PDUs are used on point-to-point links (like serial interfaces, GRE tunnels, or point-to-point Ethernet). They're simpler than LAN hellos because point-to-point links don't need DIS election.

After the 8-byte common header, a Point-to-Point Hello contains:

**Circuit Type (1 byte)**

Same as in LAN hellos, indicating Level 1 only, Level 2 only, or both.

**Source ID (ID Length bytes, typically 6)**

The System ID of the sender, just like in LAN hellos.

**Holding Time (2 bytes)**

Same as in LAN hellos, specifying how long this adjacency remains valid without additional hellos.

**PDU Length (2 bytes)**

The total length of the PDU in bytes, including the common header, fixed fields, and all TLVs. This allows the receiver to know when the PDU ends, which is necessary for proper parsing, especially if the PDU is followed by padding or other data in the frame.

**Local Circuit ID (1 byte)**

An identifier for the local end of the circuit. This is assigned by the sending router and helps distinguish multiple parallel links between the same pair of routers. If routers R1 and R2 have three parallel links, R1 might assign Local Circuit IDs 1, 2, and 3 to differentiate them.

After these fixed fields come TLVs, similar to LAN hellos but without DIS-related information since point-to-point links don't have DIS elections.

### The Three-Way Handshake on Point-to-Point Links

Point-to-Point hellos support a three-way handshake to ensure bidirectional communication before forming the adjacency. This prevents problems with unidirectional links.

The three-way handshake uses a TLV (Type 240, Point-to-Point Three-Way Adjacency State TLV) that contains:
- Adjacency State: Down, Initializing, or Up
- Extended Local Circuit ID: A 4-byte identifier for the local circuit
- Neighbor System ID: The System ID of the neighbor (once learned)
- Neighbor Extended Local Circuit ID: The neighbor's Extended Local Circuit ID (once learned)

Initially, router A sends hellos with state Down, because it hasn't seen any hellos from B yet. When B receives A's hello, it moves to state Initializing and starts including A's System ID and Extended Local Circuit ID in its hellos. When A receives B's hello and sees itself mentioned (its System ID and Extended Local Circuit ID appear in B's hello), A knows the link is bidirectional. A moves to state Up. When B receives A's hello with state Up, B also moves to state Up.

This three-way handshake ensures both routers agree the link is bidirectional before beginning LSP exchange. It's more robust than the two-way handshake on LAN segments.

## Link State PDUs (LSPs)

Link State PDUs are the heart of IS-IS. They carry topology information and are flooded throughout the network so every router can build a complete topology database. Let's examine the LSP structure in detail.

After the 8-byte common header, an LSP contains:

**PDU Length (2 bytes)**

The total length of the LSP in bytes, including header and TLVs. LSPs can be quite large if they carry many TLVs (many reachable prefixes, many neighbors, etc.), so this field is important for parsing.

**Remaining Lifetime (2 bytes)**

This field specifies how many seconds this LSP remains valid. When a router originates an LSP, it sets the Remaining Lifetime to a maximum value (typically 1200 seconds, or 20 minutes). As the LSP is stored in databases and forwarded, routers decrement the Remaining Lifetime.

When an LSP's Remaining Lifetime reaches 0, it expires and is purged from the database. This prevents stale information from persisting if a router crashes without explicitly withdrawing its LSPs.

The Remaining Lifetime mechanism is crucial for database consistency. If a router goes offline suddenly (power failure, crash), its LSPs will eventually age out across the network. Other routers will recalculate SPF without the dead router's information, automatically routing around the failure.

**LSP ID (ID Length + 2 bytes, typically 8)**

The LSP ID uniquely identifies this LSP. It consists of three parts:
- System ID (6 bytes): The System ID of the router that originated this LSP
- Pseudonode ID (1 byte): 0x00 for normal LSPs, non-zero for pseudonode LSPs created by the DIS
- LSP Number (1 byte): Fragment number, starting at 0x00 for the first fragment

For example, LSP ID `1921.6800.1001.00-00` means:
- Originated by router with System ID 1921.6800.1001
- Not a pseudonode (pseudonode ID is 00)
- First fragment (LSP Number is 00)

LSP ID `1921.6800.1001.02-00` means:
- Same originating router
- Pseudonode ID is 02 (this is a pseudonode LSP for LAN segment 02)
- First fragment

LSP ID `1921.6800.1001.00-01` means:
- Same originating router  
- Not a pseudonode
- Second fragment (LSP Number is 01)

The LSP ID is fundamental to LSP flooding and database synchronization. Routers compare LSP IDs to determine if they have a particular LSP in their database.

**Sequence Number (4 bytes)**

The Sequence Number is a 32-bit unsigned integer that increases each time the router generates a new version of this LSP. When the router first boots, it starts with sequence number 1. Each time the router needs to update its LSP (an interface goes down, a prefix is added, etc.), it increments the sequence number and re-floods the LSP.

Receiving routers compare sequence numbers to determine which version of an LSP is newer. If router A receives an LSP with sequence number 100 and already has the same LSP with sequence number 98, router A knows the received LSP is newer. It replaces the old LSP with the new one and floods the new LSP to its neighbors.

If router A receives an LSP with sequence number 100 but already has sequence number 102, router A knows its stored version is newer. It sends its newer LSP back to the sender, helping synchronize databases.

Sequence numbers eventually wrap around after reaching the maximum 32-bit value (4,294,967,295). This is handled gracefully—after reaching the maximum, the sequence number resets to 1, and a special procedure called "sequence number reset" ensures routers don't confuse old LSPs with the newly wrapped sequence numbers.

**Checksum (2 bytes)**

The Checksum provides error detection for the LSP. It's calculated over the entire LSP (excluding the checksum field itself) using an algorithm defined in the OSI standards. If a router receives an LSP and the checksum doesn't match, the LSP is corrupted and discarded.

The checksum protects against transmission errors and memory corruption. IS-IS doesn't rely on data-link layer CRC alone because LSPs are stored in memory for extended periods, and memory corruption could change LSP contents. The checksum allows detecting such corruption.

**P (Partition Repair) Flag (1 bit)**

This bit indicates whether the router supports partition repair, an advanced IS-IS feature for handling Level 2 backbone partitions. Most implementations set this to 0 (partition repair not supported) because partition repair is complex and rarely needed in practice.

**ATT (Attached) Bits (4 bits)**

These four bits indicate whether this Level 1 router has connectivity to other areas. There are four bits to potentially indicate attachment to different types of external destinations (default, delay-optimized, expense-optimized, error-optimized), though in practice only the default bit is used.

When set to 1, the ATT bit tells Level 1 routers "I am a Level 1-2 router with connectivity to the backbone; use me for inter-area traffic." This is the mechanism that causes Level 1 routers to install default routes, as we discussed in the router types document.

**LSPDBOL (LSP Database Overload) Flag (1 bit)**

When set to 1, this bit indicates the router is in database overload condition. This means the router's LSDB has become too large and the router can no longer reliably maintain full topology information. Other routers will avoid using this router for transit traffic, only sending traffic to this router if it's the destination.

Database overload is a graceful degradation mechanism. Rather than crashing or producing incorrect routing when memory runs out, the router sets the overload bit and continues operating in a limited capacity.

**IS Type (2 bits)**

These bits indicate the router type:
- 0x01: Level 1
- 0x03: Level 2

This tells receiving routers whether this is a Level 1 LSP or Level 2 LSP (which is also indicated by the PDU Type in the common header, so this is somewhat redundant).

After these fixed header fields, the LSP contains TLVs carrying the actual topology information: neighbor adjacencies, reachable IP prefixes, TE information, Segment Routing data, etc. The TLVs are where the real routing information lives.

## Complete Sequence Number PDUs (CSNPs)

CSNPs are used to synchronize LSP databases between routers. They contain a summary of all LSPs the sending router has in its database, allowing the receiving router to compare and identify any missing or outdated LSPs.

After the common header, a CSNP contains:

**PDU Length (2 bytes)**

Total length of the CSNP including all fields.

**Source ID (ID Length + 1 bytes, typically 7)**

The System ID of the router sending this CSNP plus a pseudonode ID (which is 0x00 for CSNPs originated by routers, non-zero if originated by a DIS on behalf of a pseudonode—though in practice, the router originates CSNPs, not pseudonodes).

**Start LSP ID (ID Length + 2 bytes, typically 8)**

CSNPs can cover a range of LSP IDs. The Start LSP ID indicates the beginning of the range. A CSNP might describe LSPs from `0000.0000.0000.00-00` to `FFFF.FFFF.FFFF.FF-FF` (covering all possible LSP IDs).

**End LSP ID (ID Length + 2 bytes, typically 8)**

The ending LSP ID of the range covered by this CSNP. Together with the Start LSP ID, this defines which LSPs this CSNP describes.

After these fixed fields comes a TLV containing LSP Entries. Each entry in the LSP Entries TLV lists:
- LSP ID
- Sequence Number
- Remaining Lifetime  
- Checksum

This is a summary of each LSP in the range. The CSNP doesn't contain the full LSP contents, just this metadata that allows receivers to verify they have the correct version of each LSP.

When a router receives a CSNP, it compares the listed LSPs with its own database:
- If the CSNP lists an LSP the receiver doesn't have, the receiver sends a PSNP requesting that LSP
- If the CSNP lists an LSP with a lower sequence number than the receiver has, the receiver sends its newer version to the CSNP sender
- If the CSNP lists an LSP with the same sequence number the receiver has, no action is needed (databases are synchronized for that LSP)
- If the CSNP lists an LSP with a higher sequence number than the receiver has, the receiver waits for the sender to flood the newer LSP

On broadcast networks, the DIS sends CSNPs periodically (typically every 10 seconds) listing all LSPs in its database. All other routers on the segment listen to these CSNPs and verify their databases match. This provides ongoing synchronization and catches any LSPs that might have been lost during flooding.

On point-to-point links, both routers send CSNPs periodically to each other, allowing bidirectional synchronization.

## Partial Sequence Number PDUs (PSNPs)

PSNPs are used to explicitly request specific LSPs and to acknowledge LSP receipt. After the common header, a PSNP structure is similar to a CSNP:

**PDU Length (2 bytes)**

Total length of the PSNP.

**Source ID (ID Length + 1 bytes, typically 7)**

System ID of the sending router plus pseudonode ID.

After the fixed fields comes an LSP Entries TLV, but unlike CSNPs which list all LSPs in a range, PSNPs only list specific LSPs that need action.

When used for requesting LSPs, the PSNP lists LSPs the sender wants to receive. For example, if a router notices from a CSNP that it's missing LSP `1921.6800.1001.00-00`, it sends a PSNP requesting that specific LSP.

When used for acknowledgment on point-to-point links, the PSNP lists LSPs that were successfully received. This provides reliable flooding—the sender knows the neighbor received the LSP and doesn't need to retransmit.

On broadcast networks, PSNPs are used primarily for requesting missing LSPs. Acknowledgment is implicit—if the DIS's periodic CSNP includes the LSP, all routers know the LSP was successfully flooded.

## Type-Length-Value Fields

The real power and extensibility of IS-IS comes from TLVs (Type-Length-Value fields). After the fixed header in most PDU types, the remainder of the PDU consists of TLVs encoding various types of information.

Each TLV has a simple structure:
- Type (1 byte): Identifies what kind of information this TLV carries
- Length (1 byte): How many bytes of value data follow
- Value (variable, up to 255 bytes): The actual data

The Type byte identifies the TLV. For example:
- Type 1: Area Addresses
- Type 2: IS Neighbors (for LSPs)
- Type 6: IS Neighbors (for Hello PDUs)
- Type 8: Padding
- Type 10: Authentication Information
- Type 128: IP Internal Reachability Information
- Type 129: IP External Reachability Information
- Type 130: IP Interface Address
- Type 132: IPv4 Interface Address
- Type 135: Extended IP Reachability
- Type 236: IPv6 Reachability
- Type 242: IS-IS Router Capability

And many more. New TLV types can be defined as needed for new features, which is why IS-IS has remained extensible for decades.

The Length byte specifies how many bytes of value data follow. This allows the receiver to skip TLVs it doesn't understand. If a router receives a TLV with Type 255 (which it doesn't recognize), it reads the Length byte, skips that many bytes, and continues parsing the next TLV.

The Value contains the actual data. Its format depends on the TLV Type. For example:
- In an Area Addresses TLV (Type 1), the value contains one or more area addresses
- In an IP Internal Reachability TLV (Type 128), the value contains IP prefix information (prefix, mask, metric)
- In an IS Neighbors TLV (Type 2), the value contains System IDs of neighboring routers

## Putting It All Together

When a router sends an IS-IS Hello on a broadcast network, the packet structure looks like this (conceptually):

```
[8-byte Common Header]
  - Protocol Discriminator: 0x83
  - Length Indicator: 27
  - Version: 0x01
  - ID Length: 0x06
  - PDU Type: 0x0F (Level 1 LAN Hello)
  - Version: 0x01
  - Reserved: 0x00
  - Max Area Addresses: 0x03
  
[19-byte Fixed Hello Fields]
  - Circuit Type: 0x01
  - Source ID: 1921.6800.1001 (6 bytes)
  - Holding Time: 30 seconds (2 bytes)
  - PDU Length: 120 bytes (2 bytes, including all TLVs)
  - Priority: 64 (1 byte)
  - LAN ID: 1921.6800.1002.01 (7 bytes)
  
[Variable-Length TLVs]
  - Area Addresses TLV (Type 1)
    - Type: 0x01
    - Length: 0x03
    - Value: 49.0001
  - IS Neighbors TLV (Type 6)
    - Type: 0x06
    - Length: varies
    - Value: list of neighbor System IDs
  - Protocols Supported TLV (Type 129)
    - Type: 0x81
    - Length: 0x02
    - Value: 0xCC (IPv4), 0x8E (IPv6)
  - IP Interface Address TLV (Type 132)
    - Type: 0x84
    - Length: 0x04
    - Value: 192.168.1.1 (4 bytes)
```

The total packet size is 8 (common header) + 19 (fixed hello fields) + variable (sum of all TLV lengths).

Understanding this structure is crucial for troubleshooting. When packet captures show malformed PDUs, you need to identify which field is incorrect. When configuration produces unexpected behavior, understanding what goes into each PDU type helps you diagnose the issue.

The modularity of the PDU design—common header, type-specific fixed fields, then extensible TLVs—has proven remarkably successful. It provides enough structure for consistent parsing while allowing unlimited extensibility through new TLV types.