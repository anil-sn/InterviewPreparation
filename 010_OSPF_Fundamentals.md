# OSPF Fundamentals: Understanding the Core Protocol

## What OSPF Actually Is

Open Shortest Path First (OSPF) is a link-state routing protocol that operates on a simple but powerful principle: every router in an area maintains an identical database of the network topology, then independently calculates the shortest path to every destination. This differs fundamentally from distance-vector protocols where routers trust their neighbors' advertisements blindly.

The protocol exists in two versions. OSPFv2 (RFC 2328) handles IPv4, while OSPFv3 (RFC 5340) was designed for IPv6 but can also carry IPv4 routes through extensions. The core algorithm remains identical, but the packet formats and addressing mechanisms differ to accommodate the respective IP versions.

## The Core Design Philosophy

OSPF was designed to solve specific problems that plagued earlier protocols like RIP. RIP's 15-hop limit made it unusable in large networks, and its slow convergence (counting to infinity) caused routing loops that persisted for minutes. OSPF eliminates these issues through complete topology awareness.

When you enable OSPF on a router, that router doesn't just learn about networks from its neighbors. Instead, it learns about every single link (hence "link-state") in the entire routing domain. Each router knows which routers exist, which links connect them, what the cost of each link is, and the state (up or down) of every connection. With this complete picture, routing loops become mathematically impossible because every router calculates paths from the same authoritative data.

## Areas: The Hierarchical Design

OSPF divides networks into areas to manage scale. Without areas, every router would need to maintain topology information for potentially thousands of routers, which becomes computationally expensive and memory-intensive. Areas create boundaries that limit the scope of topology flooding.

Area 0, called the backbone area, sits at the center of the hierarchy. All other areas must connect to Area 0, either directly or through virtual links. This design isn't arbitrary aesthetic choice; it's a mathematical necessity for the protocol's correctness. The two-level hierarchy (backbone and regular areas) ensures that routing information flows in a predictable pattern and prevents the formation of routing inconsistencies.

When a router connects to multiple areas, it becomes an Area Border Router (ABR). ABRs maintain separate topology databases for each area they connect to, which means an ABR connecting three areas maintains three distinct link-state databases. The ABR then summarizes routing information from non-backbone areas into Area 0, and redistributes Area 0 routes back into the non-backbone areas.

## Router Types and Their Roles

Internal Routers sit entirely within a single area. These routers have the simplest job: they maintain one topology database and calculate paths within their area.

Area Border Routers (ABRs) connect areas together and perform critical summarization functions. An ABR runs multiple instances of the SPF algorithm, one per area. It converts Type-1 and Type-2 LSAs (which describe intra-area topology) into Type-3 LSAs (which advertise inter-area routes as simple network/mask pairs). This conversion is where topology information gets abstracted into simple reachability information.

Autonomous System Boundary Routers (ASBRs) inject external routes into OSPF. These routers redistribute routes from other protocols (BGP, static routes, connected interfaces) into the OSPF domain. ASBRs generate Type-5 LSAs for external routes in regular areas, or Type-7 LSAs in Not-So-Stubby Areas (NSSAs).

Backbone Routers have at least one interface in Area 0. The backbone area cannot be empty; it must contain actual routers and links, not just virtual connectivity.

## Network Types and Neighbor Discovery

OSPF behaves differently on different network types because the protocol must adapt to the underlying Layer 2 characteristics. The network type determines how neighbors are discovered and how Designated Routers are elected.

On broadcast networks (Ethernet), OSPF uses multicast to discover neighbors automatically. When a router's interface comes up, it sends Hello packets to 224.0.0.5 (AllSPFRouters). All OSPF routers on that segment listen to this address and respond with their own Hellos. This creates a many-to-many neighbor relationship problem: on a segment with N routers, you'd have N(N-1)/2 adjacencies, and each link-state change would flood N(N-1)/2 times.

To solve this, OSPF elects a Designated Router (DR) and Backup Designated Router (BDR). All other routers (DROthers) form adjacencies only with the DR and BDR, reducing the adjacency count from N(N-1)/2 to approximately 2N. When a router needs to flood an update, it sends to 224.0.0.6 (AllDRouters), which the DR receives. The DR then floods the update back to 224.0.0.5 (AllSPFRouters), ensuring all routers receive it.

Point-to-point networks don't need DRs because there are only two routers. The adjacency forms directly between them, and they flood updates to each other without mediation.

Non-broadcast multi-access (NBMA) networks like Frame Relay require manual neighbor configuration because they can't use multicast for discovery. These networks often have partial mesh topologies where not all routers can communicate directly. OSPF can run in either NBMA mode (which elects a DR/BDR) or point-to-multipoint mode (which treats the network as a collection of point-to-point links).

## Hello Protocol: The Heartbeat

OSPF routers send Hello packets at regular intervals (10 seconds on broadcast/point-to-point, 30 seconds on NBMA). These packets serve multiple purposes beyond simple neighbor discovery.

Hello packets carry the router's Router ID, which uniquely identifies the router in the OSPF domain. The Router ID is typically the highest IP address on a loopback interface, or the highest IP on a physical interface if no loopbacks exist. This identifier remains constant even if interface addresses change.

The Hello packet also lists all neighbors the router has heard from recently. When Router A receives a Hello from Router B, and Router A sees its own Router ID in Router B's neighbor list, Router A knows that bidirectional communication works. This "two-way state" confirms that packets can successfully traverse the link in both directions.

Dead interval (typically 4x Hello interval) determines when a neighbor is considered failed. If no Hello arrives within this time, OSPF immediately declares the neighbor down and triggers LSA flooding to inform all routers of the topology change. This rapid failure detection (40 seconds default on broadcast networks) allows OSPF to converge quickly when links fail.

## LSA Types: Information Encoding

OSPF encodes topology and routing information in Link-State Advertisements (LSAs). Each LSA type serves a specific purpose in the hierarchical design.

Type-1 (Router LSA) describes a router's directly connected links within an area. Every router generates a Type-1 LSA that lists each interface, its state, cost, and the neighbor connected to it. These LSAs flood only within their origin area and form the foundation of the topology database.

Type-2 (Network LSA) describes a multi-access network segment. The Designated Router generates this LSA, listing all routers attached to the segment. This creates the "network node" in the topology graph, allowing SPF to model the shared medium correctly.

Type-3 (Summary LSA) carries inter-area routes. ABRs generate these to advertise networks from one area into another. Type-3 LSAs contain only network/mask and cost information, deliberately hiding the detailed topology. This abstraction limits SPF calculation scope and reduces the amount of information each router must process.

Type-4 (ASBR Summary LSA) informs routers how to reach an ASBR in another area. Since Type-5 LSAs flood throughout the routing domain and don't specify which area the ASBR resides in, Type-4 LSAs provide the path to reach the ASBR through the area hierarchy.

Type-5 (AS External LSA) describes routes from outside the OSPF domain. ASBRs generate these when redistributing external routes. Type-5 LSAs flood everywhere except stub areas, which explicitly reject external routes to reduce overhead.

Type-7 (NSSA External LSA) provides a compromise for NSSAs (Not-So-Stubby Areas). These areas need some external route injection while maintaining the benefits of stub areas. Type-7 LSAs flood only within the NSSA, and the NSSA ABR translates them to Type-5 LSAs before injecting them into Area 0.

## OSPFv3 Differences

OSPFv3 maintains the same fundamental algorithm as OSPFv2 but changes the packet format and addressing model to accommodate IPv6. The protocol now runs per-link rather than per-subnet, which allows multiple OSPF instances on a single physical interface.

Link-local addresses handle all protocol communication. Neighbors exchange Hellos and database synchronization using link-local addresses, which means the routers don't need globally routable IPv6 addresses to form adjacencies. This separation of control plane (link-local) and data plane (global addresses) improves flexibility.

LSA format changed to be address-family agnostic. OSPFv3 LSAs don't contain IP addresses in the body; instead, they reference links and routers by Router ID and Interface ID. Separate LSA types (Type-8 Link LSA and Type-9 Intra-Area-Prefix LSA) carry the actual IPv6 prefixes, allowing the core topology LSAs to remain protocol-neutral.

The Instance ID field allows multiple OSPF processes on a single link. Two routers will only form an adjacency if their Instance IDs match, enabling ships-in-the-night routing where different OSPF topologies coexist on shared infrastructure.

Authentication moved from the OSPF packet header to IPsec. OSPFv3 relies on IPv6's built-in security mechanisms rather than implementing its own. This offloads authentication and encryption to the operating system's IPsec stack, which is typically better optimized and more thoroughly vetted than application-layer security.

## Metric Calculation

OSPF uses cost as its metric, where cost equals a reference bandwidth divided by the interface bandwidth. The default reference is 100 Mbps, so a 10 Mbps Ethernet link has cost 10, while a 100 Mbps link has cost 1.

This automatic cost calculation breaks down with modern high-speed interfaces. A 1 Gbps link and a 100 Gbps link both calculate to cost 1 with the default reference bandwidth. Network operators must manually increase the reference bandwidth (to 1 Tbps or higher) to differentiate modern interface speeds, or assign costs manually.

Cost accumulates along a path. When calculating the shortest path from router A to network Z, OSPF sums the costs of all outbound interfaces along the path. Inbound interface costs are never considered; the packet has already traversed that link by the time it reaches the router.

Equal-cost multipath (ECMP) occurs when multiple paths have identical total costs. OSPF installs all equal-cost paths into the routing table, and the forwarding plane load-balances traffic across them. The number of ECMP paths supported varies by platform, typically ranging from 4 to 128 paths on modern routers.

## The Database Exchange Process

When two routers form a new adjacency, they must synchronize their link-state databases. This synchronization follows a specific sequence to ensure efficiency and correctness.

Database Description (DBD) packets exchange database summaries. Each router sends DBD packets listing the LSA headers (Type, ID, Sequence Number) in its database. The routers compare these headers against their own databases to identify missing or outdated LSAs.

Link-State Request (LSR) packets explicitly request specific LSAs. If Router A sees that Router B has a newer version of an LSA (higher sequence number), Router A sends an LSR asking for that LSA.

Link-State Update (LSU) packets carry the actual LSA contents. Router B responds to the LSR by sending an LSU containing the requested LSAs. Each LSA in the LSU must be individually acknowledged.

Link-State Acknowledgment (LSAck) packets confirm LSA receipt. For every LSA received, a router must send an acknowledgment. On broadcast networks, the DR can send delayed acknowledgments that cover multiple LSAs in a single packet, reducing overhead.

This four-packet exchange (DBD, LSR, LSU, LSAck) ensures reliable database synchronization over unreliable network links. OSPF implements its own reliability mechanism rather than depending on TCP, allowing it to run directly over IP.

## Sequence Numbers and LSA Aging

Every LSA contains a 32-bit sequence number that starts at 0x80000001 and increments with each new version. When a router needs to update an LSA (due to a topology change), it increments the sequence number and floods the new version.

Routers use sequence numbers to determine which LSA version is newer. Higher sequence numbers are always newer, with one exception: 0x80000001 is newer than 0x7FFFFFFF (wrapping case). This wrapping occurs approximately every 69.9 minutes if a router updates an LSA once per second continuously.

LSA age increments from 0 to 3600 seconds (MaxAge). Each router increments the age of LSAs in its database once per second. When an LSA reaches MaxAge, the router floods it one final time with age=MaxAge, then removes it from the database. This aging mechanism ensures that stale LSAs don't persist indefinitely if the originating router fails.

Routers refresh their self-originated LSAs every 30 minutes (LSRefreshTime) by flooding them with incremented sequence numbers and age reset to 0. This refresh prevents the LSAs from aging out during normal operation.

## Summary

OSPF builds a complete topology map through LSA flooding, calculates shortest paths independently using Dijkstra's algorithm, and organizes networks hierarchically through areas. The protocol's complexity comes from its need to handle diverse network types, ensure reliable database synchronization, and maintain consistency across potentially thousands of routers. Understanding these fundamentals provides the foundation for comprehending the state machine, SPF calculation, and advanced features that build upon this base.

# OSPF Link State Routing Protocol Fundamentals

## What Link State Routing Actually Means

OSPF operates on a fundamentally different principle than distance-vector protocols. Instead of trusting what neighbors say about distances to destinations, each router builds a complete map of the network topology. Think of it as the difference between asking for directions (distance-vector) versus studying a complete map before planning your route (link-state).

Every OSPF router maintains an identical database called the Link State Database (LSDB). This database contains information about every router, every network segment, and how they connect. When any change occurs anywhere in the network, that change is flooded to all routers so everyone updates their map simultaneously.

## The Core Problem OSPF Solves

Traditional distance-vector protocols suffer from slow convergence and counting-to-infinity problems. When a link fails, routers might spend minutes exchanging incorrect routing information back and forth. OSPF eliminates this by ensuring every router knows the complete topology. When a link fails, every router recalculates routes from the same accurate map. There is no ambiguity, no waiting for updates to propagate hop-by-hop.

## How Link State Actually Works

The process has three distinct phases. First, routers discover their neighbors and form adjacencies. Second, they exchange complete topology information to synchronize their databases. Third, each router independently runs Dijkstra's algorithm on the identical database to compute shortest paths.

The neighbor discovery happens through Hello packets sent every 10 seconds by default. These packets contain the router's identity and list of neighbors it has already heard from. When router A receives a Hello from router B that lists A as a neighbor, they agree they have bidirectional communication. This is the 2-Way state.

Once neighbors reach 2-Way state, they begin database synchronization. One router becomes the master and the other the slave in this exchange. The master sends Database Description packets that contain a summary of every piece of topology information it knows. The slave compares this against its own database and requests any missing pieces using Link State Request packets. The master responds with Link State Update packets containing the full details. This continues until both routers have identical databases and reach the Full state.

## The Link State Database Structure

The LSDB is not a flat collection of routes. It is a structured database of Link State Advertisements (LSAs), each describing a specific piece of topology. A Router LSA describes all links connected to a particular router with their costs. A Network LSA describes a multi-access network segment like an Ethernet LAN and lists all routers attached to it. Summary LSAs describe routes to networks in other areas. External LSAs describe routes redistributed from other routing protocols.

Each LSA has a sequence number, age, and checksum. The sequence number starts at 0x80000001 and increments each time the LSA is updated. Routers always prefer the LSA with the higher sequence number. The age starts at 0 when created and increments every second. When an LSA reaches MaxAge (3600 seconds), it is purged from the database. Every 30 minutes, routers refresh their own LSAs by incrementing the sequence number and resetting the age to 0.

## Dijkstra's Algorithm Application

Once the LSDB is synchronized, each router runs the Shortest Path First (SPF) algorithm, which is Dijkstra's algorithm. The router places itself at the root of a tree. It examines all LSAs describing links from the root and adds the nearest neighbors to a candidate list with their costs. It selects the lowest-cost candidate, marks it as permanent, and examines that router's links. This continues until all routers are placed in the tree.

The result is a shortest-path tree with the calculating router at the root. From this tree, the router extracts the next-hop and cost to reach every destination network. These become entries in the routing table.

## Cost Calculation Reality

OSPF cost is not arbitrary. By default, cost equals reference bandwidth divided by interface bandwidth. The reference bandwidth defaults to 100 Mbps. A 10 Mbps interface has cost 10. A 100 Mbps interface has cost 1. But here is the problem: Gigabit and 10 Gigabit interfaces also have cost 1 because the formula produces fractional results that OSPF rounds down to 1.

In modern networks, you must manually configure the reference bandwidth to 10000 or higher to differentiate high-speed links. Otherwise, OSPF cannot distinguish between 100 Mbps, 1 Gbps, and 10 Gbps links. They all appear equally costly.

## Implementation Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_ROUTERS 20
#define MAX_LINKS 10
#define MAX_AGE 3600
#define INITIAL_SEQ 0x80000001

/* Link structure representing a connection to a neighbor */
typedef struct {
    char neighbor_id[16];
    int cost;
} Link;

/* Link State Advertisement */
typedef struct {
    char router_id[16];
    unsigned int sequence_number;
    int age;
    Link links[MAX_LINKS];
    int link_count;
} LSA;

/* Link State Database */
typedef struct {
    LSA database[MAX_ROUTERS];
    int lsa_count;
} LSDB;

/* Routing table entry */
typedef struct {
    char destination[16];
    char next_hop[16];
    int cost;
} RouteEntry;

/* OSPF Router structure */
typedef struct {
    char router_id[16];
    LSDB lsdb;
    Link interfaces[MAX_LINKS];
    int interface_count;
    RouteEntry routing_table[MAX_ROUTERS];
    int route_count;
    unsigned int sequence_number;
} OSPFRouter;

/* Priority queue node for Dijkstra's algorithm */
typedef struct {
    char router_id[16];
    int distance;
} PQNode;

/* Initialize LSDB */
void init_lsdb(LSDB *lsdb) {
    lsdb->lsa_count = 0;
}

/* Initialize OSPF Router */
void init_router(OSPFRouter *router, const char *router_id) {
    strcpy(router->router_id, router_id);
    init_lsdb(&router->lsdb);
    router->interface_count = 0;
    router->route_count = 0;
    router->sequence_number = INITIAL_SEQ;
}

/* Check if LSA is expired */
bool is_lsa_expired(LSA *lsa) {
    return lsa->age >= MAX_AGE;
}

/* Find LSA in database by router ID */
int find_lsa_index(LSDB *lsdb, const char *router_id) {
    for (int i = 0; i < lsdb->lsa_count; i++) {
        if (strcmp(lsdb->database[i].router_id, router_id) == 0) {
            return i;
        }
    }
    return -1;
}

/* Add or update LSA in database - returns true if database changed */
bool add_lsa(LSDB *lsdb, LSA *lsa) {
    int index = find_lsa_index(lsdb, lsa->router_id);
    
    /* New LSA */
    if (index == -1) {
        if (lsdb->lsa_count >= MAX_ROUTERS) {
            return false;
        }
        lsdb->database[lsdb->lsa_count] = *lsa;
        lsdb->lsa_count++;
        return true;
    }
    
    LSA *existing = &lsdb->database[index];
    
    /* Newer sequence number wins */
    if (lsa->sequence_number > existing->sequence_number) {
        *existing = *lsa;
        return true;
    }
    
    /* Same sequence, younger age wins */
    if (lsa->sequence_number == existing->sequence_number && 
        lsa->age < existing->age) {
        *existing = *lsa;
        return true;
    }
    
    return false;
}

/* Add interface to router */
void add_interface(OSPFRouter *router, const char *neighbor_id, int cost) {
    if (router->interface_count >= MAX_LINKS) {
        return;
    }
    
    strcpy(router->interfaces[router->interface_count].neighbor_id, neighbor_id);
    router->interfaces[router->interface_count].cost = cost;
    router->interface_count++;
}

/* Generate Router LSA */
LSA generate_router_lsa(OSPFRouter *router) {
    LSA lsa;
    
    strcpy(lsa.router_id, router->router_id);
    lsa.sequence_number = router->sequence_number;
    lsa.age = 0;
    lsa.link_count = router->interface_count;
    
    for (int i = 0; i < router->interface_count; i++) {
        lsa.links[i] = router->interfaces[i];
    }
    
    router->sequence_number++;
    return lsa;
}

/* Receive and process LSA */
bool receive_lsa(OSPFRouter *router, LSA *lsa) {
    return add_lsa(&router->lsdb, lsa);
}

/* Find link cost in LSA */
int find_link_cost(LSA *lsa, const char *neighbor_id) {
    for (int i = 0; i < lsa->link_count; i++) {
        if (strcmp(lsa->links[i].neighbor_id, neighbor_id) == 0) {
            return lsa->links[i].cost;
        }
    }
    return -1;
}

/* Simple priority queue implementation for Dijkstra */
typedef struct {
    PQNode nodes[MAX_ROUTERS];
    int size;
} PriorityQueue;

void pq_init(PriorityQueue *pq) {
    pq->size = 0;
}

void pq_push(PriorityQueue *pq, const char *router_id, int distance) {
    if (pq->size >= MAX_ROUTERS) return;
    
    strcpy(pq->nodes[pq->size].router_id, router_id);
    pq->nodes[pq->size].distance = distance;
    pq->size++;
    
    /* Bubble up */
    int i = pq->size - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->nodes[i].distance >= pq->nodes[parent].distance) break;
        
        PQNode temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[parent];
        pq->nodes[parent] = temp;
        i = parent;
    }
}

PQNode pq_pop(PriorityQueue *pq) {
    PQNode result = pq->nodes[0];
    pq->size--;
    
    if (pq->size > 0) {
        pq->nodes[0] = pq->nodes[pq->size];
        
        /* Bubble down */
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            
            if (left < pq->size && 
                pq->nodes[left].distance < pq->nodes[smallest].distance) {
                smallest = left;
            }
            if (right < pq->size && 
                pq->nodes[right].distance < pq->nodes[smallest].distance) {
                smallest = right;
            }
            
            if (smallest == i) break;
            
            PQNode temp = pq->nodes[i];
            pq->nodes[i] = pq->nodes[smallest];
            pq->nodes[smallest] = temp;
            i = smallest;
        }
    }
    
    return result;
}

bool pq_is_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

/* Calculate SPF using Dijkstra's algorithm */
void calculate_spf(OSPFRouter *router) {
    char visited[MAX_ROUTERS][16];
    int visited_count = 0;
    
    int distances[MAX_ROUTERS];
    char router_ids[MAX_ROUTERS][16];
    char previous[MAX_ROUTERS][16];
    int node_count = 0;
    
    PriorityQueue pq;
    pq_init(&pq);
    
    /* Initialize with source router */
    strcpy(router_ids[0], router->router_id);
    distances[0] = 0;
    previous[0][0] = '\0';
    node_count = 1;
    
    pq_push(&pq, router->router_id, 0);
    
    /* Dijkstra's algorithm */
    while (!pq_is_empty(&pq)) {
        PQNode current = pq_pop(&pq);
        
        /* Check if already visited */
        bool is_visited = false;
        for (int i = 0; i < visited_count; i++) {
            if (strcmp(visited[i], current.router_id) == 0) {
                is_visited = true;
                break;
            }
        }
        if (is_visited) continue;
        
        strcpy(visited[visited_count], current.router_id);
        visited_count++;
        
        /* Find LSA for current router */
        int lsa_idx = find_lsa_index(&router->lsdb, current.router_id);
        if (lsa_idx == -1) continue;
        
        LSA *current_lsa = &router->lsdb.database[lsa_idx];
        if (is_lsa_expired(current_lsa)) continue;
        
        /* Examine all neighbors */
        for (int i = 0; i < current_lsa->link_count; i++) {
            const char *neighbor = current_lsa->links[i].neighbor_id;
            int link_cost = current_lsa->links[i].cost;
            
            /* Check if neighbor already visited */
            bool neighbor_visited = false;
            for (int j = 0; j < visited_count; j++) {
                if (strcmp(visited[j], neighbor) == 0) {
                    neighbor_visited = true;
                    break;
                }
            }
            if (neighbor_visited) continue;
            
            int new_distance = current.distance + link_cost;
            
            /* Find neighbor in distances array */
            int neighbor_idx = -1;
            for (int j = 0; j < node_count; j++) {
                if (strcmp(router_ids[j], neighbor) == 0) {
                    neighbor_idx = j;
                    break;
                }
            }
            
            /* New node or found shorter path */
            if (neighbor_idx == -1) {
                strcpy(router_ids[node_count], neighbor);
                distances[node_count] = new_distance;
                strcpy(previous[node_count], current.router_id);
                neighbor_idx = node_count;
                node_count++;
                pq_push(&pq, neighbor, new_distance);
            } else if (new_distance < distances[neighbor_idx]) {
                distances[neighbor_idx] = new_distance;
                strcpy(previous[neighbor_idx], current.router_id);
                pq_push(&pq, neighbor, new_distance);
            }
        }
    }
    
    /* Build routing table from SPF results */
    router->route_count = 0;
    
    for (int i = 0; i < node_count; i++) {
        if (strcmp(router_ids[i], router->router_id) == 0) {
            continue;
        }
        
        /* Trace back to find next hop */
        char next_hop[16];
        strcpy(next_hop, router_ids[i]);
        
        char current[16];
        strcpy(current, router_ids[i]);
        
        while (strlen(previous[i]) > 0) {
            /* Find index of current in arrays */
            int curr_idx = -1;
            for (int j = 0; j < node_count; j++) {
                if (strcmp(router_ids[j], current) == 0) {
                    curr_idx = j;
                    break;
                }
            }
            
            if (curr_idx == -1 || strlen(previous[curr_idx]) == 0) break;
            
            if (strcmp(previous[curr_idx], router->router_id) != 0) {
                strcpy(current, previous[curr_idx]);
            } else {
                strcpy(next_hop, current);
                break;
            }
        }
        
        strcpy(router->routing_table[router->route_count].destination, router_ids[i]);
        strcpy(router->routing_table[router->route_count].next_hop, next_hop);
        router->routing_table[router->route_count].cost = distances[i];
        router->route_count++;
    }
}

/* Print routing table */
void print_routing_table(OSPFRouter *router) {
    printf("%s Routing Table:\n", router->router_id);
    printf("%-12s %-12s %-6s\n", "Destination", "Next Hop", "Cost");
    printf("--------------------------------\n");
    
    for (int i = 0; i < router->route_count; i++) {
        printf("%-12s %-12s %-6d\n",
               router->routing_table[i].destination,
               router->routing_table[i].next_hop,
               router->routing_table[i].cost);
    }
    printf("\n");
}

/* Main simulation */
int main() {
    /*
     * Network topology:
     *     R1 --- 10 --- R2
     *     |             |
     *     5            20
     *     |             |
     *     R3 --- 15 --- R4
     *     |
     *     8
     *     |
     *     R5
     */
    
    OSPFRouter routers[5];
    LSA lsas[5];
    
    /* Initialize routers */
    init_router(&routers[0], "R1");
    init_router(&routers[1], "R2");
    init_router(&routers[2], "R3");
    init_router(&routers[3], "R4");
    init_router(&routers[4], "R5");
    
    /* Configure interfaces (bidirectional links) */
    add_interface(&routers[0], "R2", 10);
    add_interface(&routers[0], "R3", 5);
    
    add_interface(&routers[1], "R1", 10);
    add_interface(&routers[1], "R4", 20);
    
    add_interface(&routers[2], "R1", 5);
    add_interface(&routers[2], "R4", 15);
    add_interface(&routers[2], "R5", 8);
    
    add_interface(&routers[3], "R2", 20);
    add_interface(&routers[3], "R3", 15);
    
    add_interface(&routers[4], "R3", 8);
    
    /* Generate Router LSAs */
    for (int i = 0; i < 5; i++) {
        lsas[i] = generate_router_lsa(&routers[i]);
        receive_lsa(&routers[i], &lsas[i]);
    }
    
    /* Flood all LSAs to all routers */
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            receive_lsa(&routers[i], &lsas[j]);
        }
    }
    
    /* Calculate SPF on each router */
    for (int i = 0; i < 5; i++) {
        calculate_spf(&routers[i]);
    }
    
    /* Display results */
    print_routing_table(&routers[0]);
    printf("==================================================\n\n");
    print_routing_table(&routers[4]);
    printf("==================================================\n\n");
    
    printf("Verification:\n");
    printf("R1 to R5 path: R1 -> R3 -> R5 (cost: 13)\n");
    printf("R5 to R2 path: R5 -> R3 -> R1 -> R2 (cost: 23)\n\n");
    
    return 0;
}
```

## Why This Implementation Matters

The code demonstrates the core link-state principle: every router has identical topology information and independently computes routes. When R1 needs to reach R5, it does not ask R3 how to get there. Instead, R1 runs Dijkstra's algorithm on its complete topology map and discovers the path R1-R3-R5 with cost 13. R3 simultaneously runs the same algorithm and computes its own routes. Both routers reach identical conclusions because they start with identical LSDBs.

The critical insight is that OSPF scales through this independence. When the network has 1000 routers, each router still only needs to run SPF once when the topology changes. There is no iterative message passing between routers trying to reach consensus. The consensus already exists in the synchronized LSDB.

# OSPF Area-Based Hierarchical Design

## The Scalability Problem OSPF Areas Solve

Running OSPF in a flat single-area topology works perfectly fine for small networks with maybe 50 routers. But when you scale to 500 or 5000 routers, the flat topology becomes a disaster. Every time any link flaps anywhere in the network, every single router must run SPF calculation. A router in New York recalculates its entire routing table because a link failed in Tokyo. This is wasteful and creates convergence storms where thousands of routers simultaneously consume CPU resources.

The second problem is memory. In a flat topology, every router maintains complete topology information about every other router. A network with 5000 routers might have 20000 links. Each router stores information about all 20000 links in its LSDB. This database grows quadratically with network size.

The third problem is LSA flooding. When a link changes state, the LSA describing that change must flood to every router in the network. In a large flat topology, a single link flap generates thousands of LSA flooding events as the update propagates everywhere.

## How Areas Fix These Problems

OSPF areas divide the network into isolated flooding domains. Routers within an area maintain complete topology information only about their own area. They do not know the detailed topology of other areas. When a link fails in Area 1, only routers in Area 1 run SPF. Routers in Area 2 and Area 3 remain completely unaffected.

Areas connect through special routers called Area Border Routers (ABRs). An ABR has interfaces in multiple areas. It maintains a separate LSDB for each area it connects to. The ABR does not flood detailed topology information between areas. Instead, it summarizes routes from one area and advertises only the summary into other areas.

## The Backbone Area Requirement

Area 0 is special. It is called the backbone area and has a mandatory role. All areas must connect to Area 0, either directly or through a virtual link. This is not a suggestion. It is a hard requirement of the OSPF protocol. The reason is simple: OSPF inter-area routing follows a hub-and-spoke model. Traffic from Area 1 to Area 2 must transit through Area 0.

Consider a network with Area 1, Area 0, and Area 2. An ABR connects Area 1 to Area 0. Another ABR connects Area 0 to Area 2. When a router in Area 1 needs to reach a network in Area 2, the packet travels from Area 1 to Area 0, then from Area 0 to Area 2. The packet cannot go directly from Area 1 to Area 2 even if a physical link exists between their ABRs, unless that link is in Area 0.

This design prevents routing loops between areas. Without the hub-and-spoke constraint, you could create scenarios where Area 1 prefers a path through Area 2, while Area 2 prefers a path through Area 1, causing a loop. The backbone requirement eliminates this possibility.

## How ABRs Actually Work

An ABR is simply a router with interfaces in multiple areas. One of those areas must be Area 0. The ABR runs separate SPF calculations for each area. It has a distinct LSDB for each area. The LSDBs are completely isolated. A Type-1 Router LSA in Area 1 never appears in the Area 2 LSDB.

After computing routes within each area, the ABR generates Type-3 Summary LSAs. These LSAs advertise reachability to networks without describing the detailed topology. For example, if Area 1 contains networks 10.1.0.0/16 and 10.2.0.0/16, the ABR generates Type-3 LSAs saying "I can reach 10.1.0.0/16 with cost X" and "I can reach 10.2.0.0/16 with cost Y". It does not describe which routers exist in Area 1 or how they connect.

The ABR floods these Type-3 LSAs into Area 0. Other ABRs in Area 0 receive these summaries and potentially re-advertise them into their non-backbone areas. This is how routes propagate across the entire OSPF domain.

## Route Preference and Inter-Area Path Selection

OSPF has a strict route preference hierarchy. Intra-area routes always win over inter-area routes. An intra-area route is a route to a destination within the same area. An inter-area route is a route learned through Type-3 Summary LSAs from another area.

Why this preference? Because intra-area routes come from complete topology information. The router ran SPF and computed the optimal path. Inter-area routes come from summaries where the detailed topology is hidden. OSPF trusts its own SPF calculation more than summaries from other areas.

When a router has multiple paths to the same destination through different ABRs, it sums the cost to reach each ABR plus the advertised cost in that ABR's Type-3 LSA. The lowest total cost wins. This is standard shortest-path logic but applied across area boundaries.

## Area Types and LSA Filtering

Standard areas allow all LSA types. But OSPF defines special area types that filter certain LSAs to reduce database size and processing overhead. These area types make tradeoffs between functionality and efficiency.

A stub area blocks Type-5 External LSAs. External routes come from redistribution from other routing protocols like BGP or static routes. In many networks, the external routing table is enormous. If your area does not need to know about thousands of external routes, making it a stub area eliminates that overhead. The ABR injects a default route into the stub area so routers can still reach external destinations, just through the ABR.

A totally stubby area blocks both Type-5 External LSAs and Type-3 Summary LSAs from other areas. The only inter-area routes are default routes injected by the ABR. This minimizes the LSDB to only intra-area topology. Totally stubby areas work well for simple stub networks that just need connectivity to the rest of the network without caring about specific routes.

Not-So-Stubby Area (NSSA) is a stub area that allows limited external route advertisement. Normal stub areas cannot redistribute external routes because they block Type-5 LSAs. But what if your stub area connects to a small external network you want to advertise? NSSA solves this by introducing Type-7 LSAs. These are external LSAs that exist only within the NSSA. When the ABR receives Type-7 LSAs, it can convert them to Type-5 LSAs and flood them into Area 0.

## Implementation Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROUTERS 50
#define MAX_AREAS 10
#define MAX_LINKS 20
#define MAX_ROUTES 100
#define MAX_LSAS 200

/* LSA Types */
typedef enum {
    LSA_TYPE_ROUTER = 1,
    LSA_TYPE_NETWORK = 2,
    LSA_TYPE_SUMMARY = 3,
    LSA_TYPE_ASBR_SUMMARY = 4,
    LSA_TYPE_EXTERNAL = 5
} LSAType;

/* Area Types */
typedef enum {
    AREA_TYPE_STANDARD,
    AREA_TYPE_STUB,
    AREA_TYPE_TOTALLY_STUB,
    AREA_TYPE_NSSA
} AreaType;

/* Router Types */
typedef enum {
    ROUTER_INTERNAL,
    ROUTER_ABR,
    ROUTER_ASBR
} RouterType;

/* Link structure */
typedef struct {
    char neighbor_id[16];
    int area_id;
    int cost;
} Link;

/* Route structure */
typedef struct {
    char destination[32];
    int area_id;
    int cost;
    char next_hop[16];
    bool is_inter_area;
} Route;

/* LSA structure */
typedef struct {
    LSAType type;
    char advertising_router[16];
    int area_id;
    char network[32];
    int cost;
    unsigned int sequence;
} LSA;

/* Per-Area LSDB */
typedef struct {
    int area_id;
    AreaType area_type;
    LSA lsas[MAX_LSAS];
    int lsa_count;
} AreaLSDB;

/* OSPF Router */
typedef struct {
    char router_id[16];
    RouterType type;
    Link links[MAX_LINKS];
    int link_count;
    AreaLSDB area_databases[MAX_AREAS];
    int area_count;
    Route routing_table[MAX_ROUTES];
    int route_count;
} OSPFRouter;

/* Initialize router */
void init_ospf_router(OSPFRouter *router, const char *router_id, RouterType type) {
    strcpy(router->router_id, router_id);
    router->type = type;
    router->link_count = 0;
    router->area_count = 0;
    router->route_count = 0;
}

/* Add area to router */
void add_area(OSPFRouter *router, int area_id, AreaType area_type) {
    if (router->area_count >= MAX_AREAS) return;
    
    AreaLSDB *area_db = &router->area_databases[router->area_count];
    area_db->area_id = area_id;
    area_db->area_type = area_type;
    area_db->lsa_count = 0;
    router->area_count++;
}

/* Find area database by area ID */
AreaLSDB* find_area_db(OSPFRouter *router, int area_id) {
    for (int i = 0; i < router->area_count; i++) {
        if (router->area_databases[i].area_id == area_id) {
            return &router->area_databases[i];
        }
    }
    return NULL;
}

/* Add link to router in specific area */
void add_link_in_area(OSPFRouter *router, const char *neighbor_id, 
                      int area_id, int cost) {
    if (router->link_count >= MAX_LINKS) return;
    
    Link *link = &router->links[router->link_count];
    strcpy(link->neighbor_id, neighbor_id);
    link->area_id = area_id;
    link->cost = cost;
    router->link_count++;
}

/* Add Router LSA to area database */
void add_router_lsa(AreaLSDB *area_db, const char *router_id, 
                    const char *network, int cost) {
    if (area_db->lsa_count >= MAX_LSAS) return;
    
    LSA *lsa = &area_db->lsas[area_db->lsa_count];
    lsa->type = LSA_TYPE_ROUTER;
    strcpy(lsa->advertising_router, router_id);
    lsa->area_id = area_db->area_id;
    strcpy(lsa->network, network);
    lsa->cost = cost;
    lsa->sequence = 1;
    area_db->lsa_count++;
}

/* Add Summary LSA to area database */
void add_summary_lsa(AreaLSDB *area_db, const char *abr_id,
                     const char *network, int cost) {
    if (area_db->lsa_count >= MAX_LSAS) return;
    
    LSA *lsa = &area_db->lsas[area_db->lsa_count];
    lsa->type = LSA_TYPE_SUMMARY;
    strcpy(lsa->advertising_router, abr_id);
    lsa->area_id = area_db->area_id;
    strcpy(lsa->network, network);
    lsa->cost = cost;
    lsa->sequence = 1;
    area_db->lsa_count++;
}

/* Check if LSA type is allowed in area */
bool is_lsa_allowed(AreaType area_type, LSAType lsa_type) {
    switch (area_type) {
        case AREA_TYPE_STANDARD:
            return true;
        
        case AREA_TYPE_STUB:
            return lsa_type != LSA_TYPE_EXTERNAL;
        
        case AREA_TYPE_TOTALLY_STUB:
            return lsa_type == LSA_TYPE_ROUTER || 
                   lsa_type == LSA_TYPE_NETWORK;
        
        case AREA_TYPE_NSSA:
            return lsa_type != LSA_TYPE_EXTERNAL;
        
        default:
            return false;
    }
}

/* Calculate intra-area routes for a specific area */
void calculate_intra_area_routes(OSPFRouter *router, int area_id) {
    AreaLSDB *area_db = find_area_db(router, area_id);
    if (!area_db) return;
    
    /* Simple intra-area route calculation */
    /* In real implementation, this would run full SPF */
    
    for (int i = 0; i < area_db->lsa_count; i++) {
        LSA *lsa = &area_db->lsas[i];
        
        if (lsa->type != LSA_TYPE_ROUTER) continue;
        if (strcmp(lsa->advertising_router, router->router_id) == 0) continue;
        
        /* Find direct link to this router */
        int link_cost = -1;
        for (int j = 0; j < router->link_count; j++) {
            if (router->links[j].area_id == area_id &&
                strcmp(router->links[j].neighbor_id, lsa->advertising_router) == 0) {
                link_cost = router->links[j].cost;
                break;
            }
        }
        
        if (link_cost == -1) continue;
        
        /* Add route */
        if (router->route_count >= MAX_ROUTES) continue;
        
        Route *route = &router->routing_table[router->route_count];
        strcpy(route->destination, lsa->network);
        route->area_id = area_id;
        route->cost = link_cost + lsa->cost;
        strcpy(route->next_hop, lsa->advertising_router);
        route->is_inter_area = false;
        router->route_count++;
    }
}

/* ABR: Generate summary LSAs for area networks */
void abr_generate_summaries(OSPFRouter *abr, int source_area, int dest_area) {
    if (abr->type != ROUTER_ABR) return;
    
    AreaLSDB *source_db = find_area_db(abr, source_area);
    AreaLSDB *dest_db = find_area_db(abr, dest_area);
    
    if (!source_db || !dest_db) return;
    
    /* Check if destination area allows summary LSAs */
    if (!is_lsa_allowed(dest_db->area_type, LSA_TYPE_SUMMARY)) {
        return;
    }
    
    /* Generate summary LSA for each network in source area */
    for (int i = 0; i < source_db->lsa_count; i++) {
        LSA *source_lsa = &source_db->lsas[i];
        
        if (source_lsa->type != LSA_TYPE_ROUTER) continue;
        
        /* Calculate cost from ABR to this network */
        int cost_to_network = 0;
        
        /* Find if ABR has direct link to network's router */
        for (int j = 0; j < abr->link_count; j++) {
            if (abr->links[j].area_id == source_area &&
                strcmp(abr->links[j].neighbor_id, 
                       source_lsa->advertising_router) == 0) {
                cost_to_network = abr->links[j].cost + source_lsa->cost;
                break;
            }
        }
        
        if (cost_to_network > 0) {
            add_summary_lsa(dest_db, abr->router_id, 
                          source_lsa->network, cost_to_network);
        }
    }
}

/* Calculate inter-area routes from summary LSAs */
void calculate_inter_area_routes(OSPFRouter *router, int area_id) {
    AreaLSDB *area_db = find_area_db(router, area_id);
    if (!area_db) return;
    
    for (int i = 0; i < area_db->lsa_count; i++) {
        LSA *lsa = &area_db->lsas[i];
        
        if (lsa->type != LSA_TYPE_SUMMARY) continue;
        
        /* Find cost to reach the ABR */
        int cost_to_abr = -1;
        for (int j = 0; j < router->link_count; j++) {
            if (router->links[j].area_id == area_id &&
                strcmp(router->links[j].neighbor_id, 
                       lsa->advertising_router) == 0) {
                cost_to_abr = router->links[j].cost;
                break;
            }
        }
        
        if (cost_to_abr == -1) continue;
        
        /* Check if we already have an intra-area route (which wins) */
        bool has_intra_area = false;
        for (int j = 0; j < router->route_count; j++) {
            if (strcmp(router->routing_table[j].destination, lsa->network) == 0 &&
                !router->routing_table[j].is_inter_area) {
                has_intra_area = true;
                break;
            }
        }
        
        if (has_intra_area) continue;
        
        /* Add inter-area route */
        if (router->route_count >= MAX_ROUTES) continue;
        
        Route *route = &router->routing_table[router->route_count];
        strcpy(route->destination, lsa->network);
        route->area_id = area_id;
        route->cost = cost_to_abr + lsa->cost;
        strcpy(route->next_hop, lsa->advertising_router);
        route->is_inter_area = true;
        router->route_count++;
    }
}

/* Install default route in stub area */
void install_stub_default_route(OSPFRouter *router, int stub_area_id, 
                               const char *abr_id, int cost) {
    if (router->route_count >= MAX_ROUTES) return;
    
    Route *route = &router->routing_table[router->route_count];
    strcpy(route->destination, "0.0.0.0/0");
    route->area_id = stub_area_id;
    route->cost = cost;
    strcpy(route->next_hop, abr_id);
    route->is_inter_area = true;
    router->route_count++;
}

/* Print routing table */
void print_routing_table(OSPFRouter *router) {
    printf("\n%s Routing Table (Type: %s):\n", 
           router->router_id,
           router->type == ROUTER_ABR ? "ABR" : 
           router->type == ROUTER_ASBR ? "ASBR" : "Internal");
    printf("%-20s %-8s %-8s %-15s %-10s\n", 
           "Destination", "Area", "Cost", "Next Hop", "Type");
    printf("---------------------------------------------------------------\n");
    
    for (int i = 0; i < router->route_count; i++) {
        Route *route = &router->routing_table[i];
        printf("%-20s %-8d %-8d %-15s %-10s\n",
               route->destination,
               route->area_id,
               route->cost,
               route->next_hop,
               route->is_inter_area ? "Inter-Area" : "Intra-Area");
    }
}

/* Main demonstration */
int main() {
    printf("OSPF Multi-Area Network Simulation\n");
    printf("===================================\n\n");
    
    /*
     * Network Topology:
     * 
     * Area 1:          Area 0 (Backbone):      Area 2:
     * R1 --- 10 --- ABR1 --- 5 --- ABR2 --- 15 --- R2
     * (10.1.1.0/24)    |                         (10.2.1.0/24)
     *                  |
     *                  8
     *                  |
     *                 R3
     *              (10.0.1.0/24)
     */
    
    OSPFRouter r1, abr1, abr2, r2, r3;
    
    /* Initialize routers */
    init_ospf_router(&r1, "R1", ROUTER_INTERNAL);
    init_ospf_router(&abr1, "ABR1", ROUTER_ABR);
    init_ospf_router(&abr2, "ABR2", ROUTER_ABR);
    init_ospf_router(&r2, "R2", ROUTER_INTERNAL);
    init_ospf_router(&r3, "R3", ROUTER_INTERNAL);
    
    /* Configure areas on routers */
    add_area(&r1, 1, AREA_TYPE_STANDARD);
    
    add_area(&abr1, 1, AREA_TYPE_STANDARD);
    add_area(&abr1, 0, AREA_TYPE_STANDARD);
    
    add_area(&abr2, 0, AREA_TYPE_STANDARD);
    add_area(&abr2, 2, AREA_TYPE_STUB);
    
    add_area(&r2, 2, AREA_TYPE_STUB);
    
    add_area(&r3, 0, AREA_TYPE_STANDARD);
    
    /* Configure links */
    add_link_in_area(&r1, "ABR1", 1, 10);
    add_link_in_area(&abr1, "R1", 1, 10);
    
    add_link_in_area(&abr1, "ABR2", 0, 5);
    add_link_in_area(&abr2, "ABR1", 0, 5);
    
    add_link_in_area(&abr1, "R3", 0, 8);
    add_link_in_area(&r3, "ABR1", 0, 8);
    
    add_link_in_area(&abr2, "R2", 2, 15);
    add_link_in_area(&r2, "ABR2", 2, 15);
    
    /* Populate LSDBs with Router LSAs */
    AreaLSDB *area1_r1 = find_area_db(&r1, 1);
    add_router_lsa(area1_r1, "R1", "10.1.1.0/24", 1);
    
    AreaLSDB *area1_abr1 = find_area_db(&abr1, 1);
    add_router_lsa(area1_abr1, "R1", "10.1.1.0/24", 1);
    
    AreaLSDB *area0_abr1 = find_area_db(&abr1, 0);
    add_router_lsa(area0_abr1, "R3", "10.0.1.0/24", 1);
    
    AreaLSDB *area0_abr2 = find_area_db(&abr2, 0);
    add_router_lsa(area0_abr2, "R3", "10.0.1.0/24", 1);
    
    AreaLSDB *area0_r3 = find_area_db(&r3, 0);
    add_router_lsa(area0_r3, "R3", "10.0.1.0/24", 1);
    
    AreaLSDB *area2_abr2 = find_area_db(&abr2, 2);
    add_router_lsa(area2_abr2, "R2", "10.2.1.0/24", 1);
    
    AreaLSDB *area2_r2 = find_area_db(&r2, 2);
    add_router_lsa(area2_r2, "R2", "10.2.1.0/24", 1);
    
    /* ABR1 generates summaries from Area 1 to Area 0 */
    abr_generate_summaries(&abr1, 1, 0);
    
    /* ABR2 receives summaries in Area 0 and generates into Area 2 */
    AreaLSDB *area0_for_abr2 = find_area_db(&abr2, 0);
    for (int i = 0; i < area0_abr1->lsa_count; i++) {
        if (area0_abr1->lsas[i].type == LSA_TYPE_SUMMARY) {
            /* Simulate LSA flooding to ABR2 */
            LSA summary = area0_abr1->lsas[i];
            area0_for_abr2->lsas[area0_for_abr2->lsa_count++] = summary;
        }
    }
    
    /* ABR2 generates summaries from Area 0 to Area 2 */
    /* Note: Area 2 is stub, so limited summaries */
    abr_generate_summaries(&abr2, 0, 2);
    
    /* Calculate routes on each router */
    calculate_intra_area_routes(&r1, 1);
    calculate_intra_area_routes(&r2, 2);
    calculate_intra_area_routes(&r3, 0);
    
    calculate_intra_area_routes(&abr1, 1);
    calculate_intra_area_routes(&abr1, 0);
    calculate_inter_area_routes(&abr1, 0);
    
    calculate_intra_area_routes(&abr2, 0);
    calculate_intra_area_routes(&abr2, 2);
    calculate_inter_area_routes(&abr2, 0);
    calculate_inter_area_routes(&abr2, 2);
    
    /* Install default route in stub area */
    install_stub_default_route(&r2, 2, "ABR2", 15);
    
    /* Display results */
    print_routing_table(&r1);
    print_routing_table(&abr1);
    print_routing_table(&r3);
    print_routing_table(&abr2);
    print_routing_table(&r2);
    
    printf("\n\nKey Observations:\n");
    printf("1. R1 (Area 1) has only intra-area routes\n");
    printf("2. ABR1 has routes from both Area 1 and Area 0\n");
    printf("3. R3 (Area 0) sees summary routes from Area 1\n");
    printf("4. R2 (Stub Area) has default route instead of specific inter-area routes\n");
    printf("5. All inter-area traffic must transit Area 0\n");
    
    return 0;
}
```

## Critical Design Decisions

The area design makes a fundamental tradeoff. You gain scalability by hiding topology details between areas, but you lose optimal path selection. A router in Area 1 cannot see the detailed topology of Area 2, so it cannot compute the true shortest path to destinations in Area 2. It must trust the summary information from the ABR.

This can lead to suboptimal routing. Imagine Area 1 connects to Area 0 through two ABRs. Both ABRs advertise the same network in Area 2 with the same cost. The router in Area 1 picks one arbitrarily. But if the router could see Area 2's topology, it might discover that one ABR provides a much better path. This information is hidden by the area abstraction.

The backbone requirement also creates single points of failure. If Area 0 becomes partitioned, inter-area routing breaks. Areas can still route within themselves, but they lose connectivity to other areas. This is why Area 0 design requires careful attention to redundancy.

# OSPF Packet Types and Neighbor State Machine

## Why OSPF Needs Five Packet Types

OSPF is not a simple protocol that just announces routes. It must discover neighbors, negotiate master-slave relationships, synchronize databases, acknowledge updates, and detect failures. Each of these functions requires specific packet formats with different information. Using a single packet type for all purposes would create ambiguity and waste bandwidth. OSPF solves this with five distinct packet types, each optimized for its specific purpose.

The protocol must work reliably over unreliable networks. IP itself provides no guarantee that packets arrive or arrive in order. OSPF cannot assume the underlying network will handle reliability. Therefore, OSPF implements its own reliability mechanisms including explicit acknowledgments, retransmission timers, and sequence numbers. The packet types reflect this need for reliability.

## Hello Packets: The Foundation of Everything

Hello packets serve three critical functions. First, they discover neighbors. When a router starts, it has no knowledge of what other OSPF routers exist on its links. It sends Hello packets to the multicast address 224.0.0.5 (AllSPFRouters). Any OSPF router on that link receives the Hello and learns about the sender.

Second, Hello packets establish bidirectional communication. Just because router A hears router B does not mean B hears A. OSPF requires confirmed bidirectional communication before forming an adjacency. The Hello packet contains a list of all neighbors the sender has heard from. When A receives a Hello from B that lists A as a neighbor, A knows the communication is bidirectional.

Third, Hello packets maintain neighbor relationships and detect failures. Routers send Hello packets every HelloInterval seconds, which defaults to 10 seconds on broadcast networks. If a router does not receive a Hello from a neighbor within the DeadInterval (default 40 seconds, four times the HelloInterval), it declares that neighbor dead and triggers SPF recalculation.

The Hello packet also carries critical parameters that must match between neighbors: area ID, authentication information, stub area flag, and network mask. If these parameters do not match, routers will not form an adjacency. This prevents configuration errors from creating broken topologies.

## Database Description Packets: The Index

After neighbors reach bidirectional communication, they must synchronize their LSDBs. The obvious approach would be for each router to send its entire database to the other. But this is wasteful. If both routers already have identical databases, they would exchange massive amounts of redundant information.

OSPF uses a more efficient approach. One router becomes the master and the other the slave. The master sends Database Description (DBD) packets containing headers of every LSA in its database. These headers include the LSA type, advertising router, sequence number, and age, but not the full LSA content. This is like sending a table of contents instead of the entire book.

The slave examines each LSA header and compares it to its own database. For each LSA where the header indicates a newer version than what the slave has, or for LSAs the slave does not have at all, the slave makes a note to request the full LSA later. The slave acknowledges each DBD packet before the master sends the next one. This creates a reliable exchange where both sides confirm they have synchronized their catalogs.

The master-slave election happens during the ExStart state. Both routers send DBD packets with special flags set. The router with the higher router ID becomes the master. This election is deterministic and always produces the same result, preventing any ambiguity.

## Link State Request Packets: Asking for Missing Pieces

After the DBD exchange completes, the slave has a complete list of LSAs where it needs the full information. It sends Link State Request (LSR) packets listing these LSAs. Each request specifies the LSA type, advertising router, and link state ID, which uniquely identifies an LSA.

The master responds with Link State Update packets containing the requested LSAs. If the slave still needs additional LSAs after receiving a response, it sends additional LSR packets. This continues until the slave has received all LSAs it needs.

LSR packets are also used during normal operation. When a router receives an LSA header in a DBD packet or an LSA reference in any context that indicates a newer LSA exists, it sends an LSR to obtain that LSA. This is how updates propagate through the network.

## Link State Update Packets: Carrying the Actual Data

Link State Update (LSU) packets carry one or more complete LSAs. This is the packet type that actually distributes topology information. When a link goes down, the router detecting the failure generates a new LSA describing its current state and sends it in an LSU packet.

LSU packets flood through the network. When a router receives an LSU, it examines each LSA. If the LSA is newer than what the router has in its database, the router installs the new LSA and forwards the LSU out all interfaces except the one it arrived on. This flooding ensures every router receives the update.

LSU packets can carry multiple LSAs to improve efficiency. When a router has multiple LSAs to send, it packs them into a single LSU packet rather than sending separate packets for each LSA. This reduces packet overhead and network bandwidth consumption.

The flooding mechanism includes loop prevention. Each LSA has a sequence number that increments with each update. Routers only accept LSAs with higher sequence numbers than what they currently have. If a router receives an LSA with an older or equal sequence number, it discards the LSA or sends back the newer version it has. This prevents old information from circulating indefinitely.

## Link State Acknowledgment Packets: Ensuring Reliability

OSPF must ensure that LSAs are reliably delivered. IP is unreliable, so OSPF implements its own acknowledgment mechanism. When a router receives an LSU packet, it must acknowledge receipt. The Link State Acknowledgment (LSAck) packet serves this purpose.

The LSAck packet contains a list of LSA headers, indicating which LSAs the sender acknowledges. If a router sends an LSU and does not receive an acknowledgment within the retransmission interval (default 5 seconds), it retransmits the LSU. This continues until acknowledgment is received or the neighbor is declared dead.

On broadcast networks, OSPF optimizes acknowledgments. Instead of every router sending individual acknowledgments, routers can send a single multicast acknowledgment to 224.0.0.5. This reduces the number of packets on the network. Alternatively, a router can implicitly acknowledge an LSA by sending its own LSU containing the same LSA. This implicit acknowledgment eliminates the need for a separate LSAck packet.

## The Neighbor State Machine

OSPF neighbors progress through eight states as they establish and maintain adjacencies. Understanding these states is essential for troubleshooting OSPF problems. When adjacencies fail to form, the states reveal exactly where the process broke.

Down is the initial state. The router has not received any Hello packets from the neighbor. This is also the state after a neighbor is declared dead due to not receiving Hellos within the DeadInterval.

Attempt applies only to NBMA networks where neighbors must be manually configured. The router sends unicast Hellos to the configured neighbor address. If no response arrives, the neighbor remains in Attempt state.

Init means the router has received a Hello packet from the neighbor, but that Hello did not list the router as a neighbor. This indicates one-way communication. The neighbor hears this router, but this router is not yet certain the neighbor can hear it.

2-Way state confirms bidirectional communication. The router received a Hello from the neighbor that lists the router as a neighbor. At this point, the routers have confirmed they can communicate. On broadcast networks, routers may stop here and not form full adjacencies. Only the Designated Router (DR) and Backup Designated Router (BDR) form full adjacencies with all routers. Other routers remain in 2-Way state with each other.

ExStart begins the database synchronization process. Routers negotiate master-slave relationship by exchanging DBD packets with no LSA information. The router with the higher router ID becomes master.

Exchange state is where routers send DBD packets containing LSA headers. Each router builds a list of LSAs it needs to request. The exchange continues until both routers have sent all their DBD packets and received acknowledgments.

Loading state occurs when routers send LSR packets requesting full LSAs for the entries they identified during Exchange. The router transitions to Loading when it has Link State Requests to send. As responses arrive via LSU packets, the list of pending requests shrinks.

Full state means databases are fully synchronized. The routers have identical LSDBs and are ready to route traffic. This is the goal state for all adjacencies. Routers remain in Full state as long as they continue receiving Hello packets and the adjacency remains healthy.

## Implementation Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_NEIGHBORS 20
#define MAX_LSA_HEADERS 100
#define HELLO_INTERVAL 10
#define DEAD_INTERVAL 40
#define RETX_INTERVAL 5

/* OSPF Packet Types */
typedef enum {
    PACKET_HELLO = 1,
    PACKET_DBD = 2,
    PACKET_LSR = 3,
    PACKET_LSU = 4,
    PACKET_LSACK = 5
} PacketType;

/* Neighbor States */
typedef enum {
    STATE_DOWN = 0,
    STATE_ATTEMPT = 1,
    STATE_INIT = 2,
    STATE_2WAY = 3,
    STATE_EXSTART = 4,
    STATE_EXCHANGE = 5,
    STATE_LOADING = 6,
    STATE_FULL = 7
} NeighborState;

/* LSA Header for DBD packets */
typedef struct {
    unsigned short age;
    unsigned char type;
    char link_state_id[16];
    char advertising_router[16];
    unsigned int sequence;
    unsigned short checksum;
} LSAHeader;

/* Hello Packet */
typedef struct {
    char router_id[16];
    int area_id;
    char network_mask[16];
    int hello_interval;
    int dead_interval;
    char neighbors[MAX_NEIGHBORS][16];
    int neighbor_count;
} HelloPacket;

/* Database Description Packet */
typedef struct {
    char router_id[16];
    unsigned int dd_sequence;
    bool is_master;
    bool is_more;
    bool is_init;
    LSAHeader headers[MAX_LSA_HEADERS];
    int header_count;
} DBDPacket;

/* Link State Request Entry */
typedef struct {
    unsigned char ls_type;
    char link_state_id[16];
    char advertising_router[16];
} LSRequest;

/* LSR Packet */
typedef struct {
    char router_id[16];
    LSRequest requests[MAX_LSA_HEADERS];
    int request_count;
} LSRPacket;

/* LSU Packet */
typedef struct {
    char router_id[16];
    LSAHeader lsas[MAX_LSA_HEADERS];
    int lsa_count;
} LSUPacket;

/* LSAck Packet */
typedef struct {
    char router_id[16];
    LSAHeader ack_headers[MAX_LSA_HEADERS];
    int ack_count;
} LSAckPacket;

/* OSPF Neighbor */
typedef struct {
    char neighbor_id[16];
    NeighborState state;
    time_t last_hello_received;
    bool is_master;
    unsigned int dd_sequence;
    LSRequest pending_requests[MAX_LSA_HEADERS];
    int pending_request_count;
    LSAHeader local_lsa_headers[MAX_LSA_HEADERS];
    int local_lsa_count;
} OSPFNeighbor;

/* OSPF Interface */
typedef struct {
    char router_id[16];
    int area_id;
    OSPFNeighbor neighbors[MAX_NEIGHBORS];
    int neighbor_count;
    LSAHeader lsdb_headers[MAX_LSA_HEADERS];
    int lsdb_header_count;
} OSPFInterface;

/* Initialize interface */
void init_interface(OSPFInterface *intf, const char *router_id, int area_id) {
    strcpy(intf->router_id, router_id);
    intf->area_id = area_id;
    intf->neighbor_count = 0;
    intf->lsdb_header_count = 0;
}

/* Add LSA header to local database */
void add_lsa_header(OSPFInterface *intf, unsigned char type, 
                   const char *adv_router, unsigned int sequence) {
    if (intf->lsdb_header_count >= MAX_LSA_HEADERS) return;
    
    LSAHeader *header = &intf->lsdb_headers[intf->lsdb_header_count];
    header->age = 0;
    header->type = type;
    strcpy(header->link_state_id, "0.0.0.0");
    strcpy(header->advertising_router, adv_router);
    header->sequence = sequence;
    header->checksum = 0;
    intf->lsdb_header_count++;
}

/* Find neighbor by ID */
OSPFNeighbor* find_neighbor(OSPFInterface *intf, const char *neighbor_id) {
    for (int i = 0; i < intf->neighbor_count; i++) {
        if (strcmp(intf->neighbors[i].neighbor_id, neighbor_id) == 0) {
            return &intf->neighbors[i];
        }
    }
    return NULL;
}

/* Create neighbor entry */
OSPFNeighbor* create_neighbor(OSPFInterface *intf, const char *neighbor_id) {
    if (intf->neighbor_count >= MAX_NEIGHBORS) return NULL;
    
    OSPFNeighbor *neighbor = &intf->neighbors[intf->neighbor_count];
    strcpy(neighbor->neighbor_id, neighbor_id);
    neighbor->state = STATE_DOWN;
    neighbor->last_hello_received = 0;
    neighbor->is_master = false;
    neighbor->dd_sequence = 0;
    neighbor->pending_request_count = 0;
    neighbor->local_lsa_count = 0;
    
    intf->neighbor_count++;
    return neighbor;
}

/* Process Hello packet */
void process_hello(OSPFInterface *intf, HelloPacket *hello) {
    printf("[%s] Received Hello from %s\n", intf->router_id, hello->router_id);
    
    OSPFNeighbor *neighbor = find_neighbor(intf, hello->router_id);
    if (!neighbor) {
        neighbor = create_neighbor(intf, hello->router_id);
        if (!neighbor) return;
    }
    
    neighbor->last_hello_received = time(NULL);
    
    /* Check if we are listed in neighbor's Hello */
    bool we_are_listed = false;
    for (int i = 0; i < hello->neighbor_count; i++) {
        if (strcmp(hello->neighbors[i], intf->router_id) == 0) {
            we_are_listed = true;
            break;
        }
    }
    
    /* State transitions */
    if (neighbor->state == STATE_DOWN) {
        neighbor->state = STATE_INIT;
        printf("[%s] Neighbor %s: DOWN -> INIT\n", 
               intf->router_id, neighbor->neighbor_id);
    }
    
    if (neighbor->state == STATE_INIT && we_are_listed) {
        neighbor->state = STATE_2WAY;
        printf("[%s] Neighbor %s: INIT -> 2-WAY (bidirectional confirmed)\n",
               intf->router_id, neighbor->neighbor_id);
        
        /* Start database synchronization */
        neighbor->state = STATE_EXSTART;
        printf("[%s] Neighbor %s: 2-WAY -> EXSTART\n",
               intf->router_id, neighbor->neighbor_id);
    }
}

/* Send Hello packet */
HelloPacket create_hello(OSPFInterface *intf) {
    HelloPacket hello;
    strcpy(hello.router_id, intf->router_id);
    hello.area_id = intf->area_id;
    strcpy(hello.network_mask, "255.255.255.0");
    hello.hello_interval = HELLO_INTERVAL;
    hello.dead_interval = DEAD_INTERVAL;
    
    /* List all neighbors in 2-Way or higher state */
    hello.neighbor_count = 0;
    for (int i = 0; i < intf->neighbor_count; i++) {
        if (intf->neighbors[i].state >= STATE_2WAY) {
            strcpy(hello.neighbors[hello.neighbor_count], 
                   intf->neighbors[i].neighbor_id);
            hello.neighbor_count++;
        }
    }
    
    return hello;
}

/* Process DBD packet */
void process_dbd(OSPFInterface *intf, DBDPacket *dbd) {
    printf("[%s] Received DBD from %s (seq=%u, master=%d, more=%d)\n",
           intf->router_id, dbd->router_id, dbd->dd_sequence, 
           dbd->is_master, dbd->is_more);
    
    OSPFNeighbor *neighbor = find_neighbor(intf, dbd->router_id);
    if (!neighbor) return;
    
    if (neighbor->state == STATE_EXSTART) {
        /* Master-Slave negotiation */
        if (strcmp(intf->router_id, dbd->router_id) > 0) {
            neighbor->is_master = false;
            printf("[%s] I am MASTER in relationship with %s\n",
                   intf->router_id, neighbor->neighbor_id);
        } else {
            neighbor->is_master = true;
            printf("[%s] I am SLAVE in relationship with %s\n",
                   intf->router_id, neighbor->neighbor_id);
        }
        
        neighbor->state = STATE_EXCHANGE;
        neighbor->dd_sequence = dbd->dd_sequence;
        printf("[%s] Neighbor %s: EXSTART -> EXCHANGE\n",
               intf->router_id, neighbor->neighbor_id);
    }
    
    if (neighbor->state == STATE_EXCHANGE) {
        /* Compare received LSA headers with local database */
        neighbor->pending_request_count = 0;
        
        for (int i = 0; i < dbd->header_count; i++) {
            bool need_lsa = true;
            
            /* Check if we have this LSA */
            for (int j = 0; j < intf->lsdb_header_count; j++) {
                if (strcmp(dbd->headers[i].advertising_router,
                          intf->lsdb_headers[j].advertising_router) == 0 &&
                    dbd->headers[i].type == intf->lsdb_headers[j].type) {
                    
                    /* We have it, check if newer */
                    if (dbd->headers[i].sequence <= intf->lsdb_headers[j].sequence) {
                        need_lsa = false;
                    }
                    break;
                }
            }
            
            if (need_lsa && neighbor->pending_request_count < MAX_LSA_HEADERS) {
                LSRequest *req = &neighbor->pending_requests[neighbor->pending_request_count];
                req->ls_type = dbd->headers[i].type;
                strcpy(req->link_state_id, dbd->headers[i].link_state_id);
                strcpy(req->advertising_router, dbd->headers[i].advertising_router);
                neighbor->pending_request_count++;
            }
        }
        
        if (!dbd->is_more && neighbor->pending_request_count > 0) {
            neighbor->state = STATE_LOADING;
            printf("[%s] Neighbor %s: EXCHANGE -> LOADING (%d LSAs needed)\n",
                   intf->router_id, neighbor->neighbor_id, 
                   neighbor->pending_request_count);
        } else if (!dbd->is_more && neighbor->pending_request_count == 0) {
            neighbor->state = STATE_FULL;
            printf("[%s] Neighbor %s: EXCHANGE -> FULL (databases synchronized)\n",
                   intf->router_id, neighbor->neighbor_id);
        }
    }
}

/* Create DBD packet */
DBDPacket create_dbd(OSPFInterface *intf, OSPFNeighbor *neighbor, bool is_init) {
    DBDPacket dbd;
    strcpy(dbd.router_id, intf->router_id);
    dbd.dd_sequence = neighbor->dd_sequence;
    dbd.is_master = !neighbor->is_master;
    dbd.is_init = is_init;
    dbd.header_count = 0;
    
    if (!is_init) {
        /* Include LSA headers from local database */
        for (int i = 0; i < intf->lsdb_header_count && dbd.header_count < MAX_LSA_HEADERS; i++) {
            dbd.headers[dbd.header_count++] = intf->lsdb_headers[i];
        }
        dbd.is_more = false;
    } else {
        dbd.is_more = true;
    }
    
    return dbd;
}

/* Process LSR packet */
void process_lsr(OSPFInterface *intf, LSRPacket *lsr) {
    printf("[%s] Received LSR from %s (%d requests)\n",
           intf->router_id, lsr->router_id, lsr->request_count);
    
    /* In real implementation, would send LSU with requested LSAs */
    printf("[%s] Sending LSU with %d LSAs to %s\n",
           intf->router_id, lsr->request_count, lsr->router_id);
}

/* Process LSU packet */
void process_lsu(OSPFInterface *intf, LSUPacket *lsu) {
    printf("[%s] Received LSU from %s (%d LSAs)\n",
           intf->router_id, lsu->router_id, lsu->lsa_count);
    
    OSPFNeighbor *neighbor = find_neighbor(intf, lsu->router_id);
    if (!neighbor) return;
    
    /* Install new LSAs */
    for (int i = 0; i < lsu->lsa_count; i++) {
        /* Check if this LSA satisfies a pending request */
        for (int j = 0; j < neighbor->pending_request_count; j++) {
            if (lsu->lsas[i].type == neighbor->pending_requests[j].ls_type &&
                strcmp(lsu->lsas[i].advertising_router, 
                       neighbor->pending_requests[j].advertising_router) == 0) {
                
                /* Remove from pending requests */
                for (int k = j; k < neighbor->pending_request_count - 1; k++) {
                    neighbor->pending_requests[k] = neighbor->pending_requests[k + 1];
                }
                neighbor->pending_request_count--;
                break;
            }
        }
    }
    
    /* Check if loading is complete */
    if (neighbor->state == STATE_LOADING && neighbor->pending_request_count == 0) {
        neighbor->state = STATE_FULL;
        printf("[%s] Neighbor %s: LOADING -> FULL (all LSAs received)\n",
               intf->router_id, neighbor->neighbor_id);
    }
    
    /* Send acknowledgment */
    printf("[%s] Sending LSAck to %s\n", intf->router_id, lsu->router_id);
}

/* Simulate adjacency formation */
void simulate_adjacency_formation() {
    printf("OSPF Adjacency Formation Simulation\n");
    printf("====================================\n\n");
    
    /* Create two routers */
    OSPFInterface r1, r2;
    init_interface(&r1, "1.1.1.1", 0);
    init_interface(&r2, "2.2.2.2", 0);
    
    /* Add some LSA headers to each router */
    add_lsa_header(&r1, 1, "1.1.1.1", 0x80000001);
    add_lsa_header(&r1, 1, "3.3.3.3", 0x80000001);
    
    add_lsa_header(&r2, 1, "2.2.2.2", 0x80000001);
    add_lsa_header(&r2, 1, "4.4.4.4", 0x80000001);
    
    printf("\n--- Phase 1: Hello Exchange ---\n\n");
    
    /* R1 sends Hello to R2 */
    HelloPacket hello1 = create_hello(&r1);
    printf("[R1] Sending Hello\n");
    process_hello(&r2, &hello1);
    
    /* R2 sends Hello to R1 (without R1 listed yet) */
    HelloPacket hello2 = create_hello(&r2);
    printf("\n[R2] Sending Hello\n");
    process_hello(&r1, &hello2);
    
    /* R1 sends Hello again (now with R2 listed) */
    hello1 = create_hello(&r1);
    printf("\n[R1] Sending Hello (with R2 listed)\n");
    process_hello(&r2, &hello1);
    
    printf("\n--- Phase 2: Master-Slave Negotiation ---\n\n");
    
    /* R1 sends initial DBD */
    OSPFNeighbor *r1_neighbor = find_neighbor(&r1, "2.2.2.2");
    r1_neighbor->dd_sequence = 12345;
    DBDPacket dbd1 = create_dbd(&r1, r1_neighbor, true);
    printf("[R1] Sending initial DBD\n");
    process_dbd(&r2, &dbd1);
    
    /* R2 sends initial DBD */
    OSPFNeighbor *r2_neighbor = find_neighbor(&r2, "1.1.1.1");
    r2_neighbor->dd_sequence = 54321;
    DBDPacket dbd2 = create_dbd(&r2, r2_neighbor, true);
    printf("\n[R2] Sending initial DBD\n");
    process_dbd(&r1, &dbd2);
    
    printf("\n--- Phase 3: Database Description Exchange ---\n\n");
    
    /* Master (R2) sends DBD with LSA headers */
    r2_neighbor->dd_sequence++;
    dbd2 = create_dbd(&r2, r2_neighbor, false);
    printf("[R2] Sending DBD with LSA headers\n");
    process_dbd(&r1, &dbd2);
    
    /* Slave (R1) sends DBD with LSA headers */
    dbd1 = create_dbd(&r1, r1_neighbor, false);
    printf("\n[R1] Sending DBD with LSA headers\n");
    process_dbd(&r2, &dbd1);
    
    printf("\n--- Phase 4: LSA Exchange ---\n\n");
    
    /* R1 requests LSAs from R2 */
    if (r1_neighbor->pending_request_count > 0) {
        LSRPacket lsr1;
        strcpy(lsr1.router_id, "1.1.1.1");
        lsr1.request_count = r1_neighbor->pending_request_count;
        for (int i = 0; i < r1_neighbor->pending_request_count; i++) {
            lsr1.requests[i] = r1_neighbor->pending_requests[i];
        }
        printf("[R1] Sending LSR\n");
        process_lsr(&r2, &lsr1);
        
        /* R2 responds with LSU */
        LSUPacket lsu2;
        strcpy(lsu2.router_id, "2.2.2.2");
        lsu2.lsa_count = lsr1.request_count;
        for (int i = 0; i < lsu2.lsa_count; i++) {
            lsu2.lsas[i].type = lsr1.requests[i].ls_type;
            strcpy(lsu2.lsas[i].advertising_router, lsr1.requests[i].advertising_router);
            lsu2.lsas[i].sequence = 0x80000001;
        }
        printf("\n[R2] Responding with LSU\n");
        process_lsu(&r1, &lsu2);
    }
    
    /* R2 requests LSAs from R1 */
    if (r2_neighbor->pending_request_count > 0) {
        LSRPacket lsr2;
        strcpy(lsr2.router_id, "2.2.2.2");
        lsr2.request_count = r2_neighbor->pending_request_count;
        for (int i = 0; i < r2_neighbor->pending_request_count; i++) {
            lsr2.requests[i] = r2_neighbor->pending_requests[i];
        }
        printf("\n[R2] Sending LSR\n");
        process_lsr(&r1, &lsr2);
        
        /* R1 responds with LSU */
        LSUPacket lsu1;
        strcpy(lsu1.router_id, "1.1.1.1");
        lsu1.lsa_count = lsr2.request_count;
        for (int i = 0; i < lsu1.lsa_count; i++) {
            lsu1.lsas[i].type = lsr2.requests[i].ls_type;
            strcpy(lsu1.lsas[i].advertising_router, lsr2.requests[i].advertising_router);
            lsu1.lsas[i].sequence = 0x80000001;
        }
        printf("\n[R1] Responding with LSU\n");
        process_lsu(&r2, &lsu1);
    }
    
    printf("\n--- Adjacency Formation Complete ---\n\n");
    printf("Final States:\n");
    printf("R1 neighbor 2.2.2.2: %s\n", 
           r1_neighbor->state == STATE_FULL ? "FULL" : "NOT FULL");
    printf("R2 neighbor 1.1.1.1: %s\n",
           r2_neighbor->state == STATE_FULL ? "FULL" : "NOT FULL");
}

int main() {
    simulate_adjacency_formation();
    return 0;
}
```

## The Critical Role of Sequence Numbers

OSPF's reliability depends entirely on sequence numbers. Without them, the protocol would have no way to determine which LSA is newer. Imagine two routers with different versions of the same LSA. How do they decide which is correct? The sequence number provides total ordering.

Sequence numbers start at 0x80000001 and increment to 0x7FFFFFFF. When a router generates a new version of an LSA, it increments the sequence number. The highest sequence number always wins. This simple rule ensures consistency across the network.

The protocol must handle sequence number wraparound. After reaching 0x7FFFFFFF, the sequence number cannot simply wrap to 0x80000001 because that would make the new LSA appear older than the previous one. OSPF solves this by prematurely aging the old LSA to MaxAge, flushing it from the network, and then starting fresh with 0x80000001. This forced flush ensures the old sequence numbers are purged before new ones begin.

# OSPF LSA Types Deep Dive

## Why Multiple LSA Types Exist

A flat LSA structure where everything is the same type would be wasteful and ambiguous. Different routing information has different flooding scopes and different meanings. A router's directly connected links should flood only within an area. Summary routes between areas should flood differently than external routes from other protocols. OSPF solves this with distinct LSA types, each with specific flooding rules and semantic meaning.

The LSA type determines where the LSA can flood. Type 1 and Type 2 LSAs flood only within a single area. They describe intra-area topology and have no meaning outside their area. Type 3, 4, and 5 LSAs flood more broadly, carrying inter-area and external routing information. This scoping prevents unnecessary flooding and keeps LSDBs appropriately sized.

## Type 1 Router LSA: The Foundation

Every router generates a Type 1 Router LSA describing itself. This LSA lists all of the router's active OSPF interfaces and their states. For each interface, the LSA includes the interface type (point-to-point, broadcast, etc.), the attached network or neighbor, and the cost of using that interface.

The Router LSA is the most fundamental piece of OSPF topology information. When you run SPF algorithm, you primarily process Router LSAs. The algorithm starts at your own Router LSA and follows links to other Router LSAs, building the shortest-path tree. Without Router LSAs, SPF cannot function.

A Router LSA floods only within its area. A router in Area 1 generates a Router LSA that floods to all routers in Area 1. That LSA never crosses into Area 0 or any other area. This is how areas maintain topology independence. Other areas know summary routes into Area 1, but they never see the detailed Router LSAs from Area 1.

The Router LSA includes flags indicating router capabilities. The E-bit indicates whether the router can process External LSAs. Stub area routers clear this bit because they reject Type 5 LSAs. The B-bit indicates the router is an Area Border Router. The V-bit indicates the router is an endpoint of a virtual link. These flags help other routers understand what role this router plays.

## Type 2 Network LSA: Multi-Access Representation

Type 2 Network LSAs exist only on multi-access networks like Ethernet where multiple routers connect to the same segment. Point-to-point links have no Network LSA because the relationship is fully described by the two Router LSAs at each end.

The Designated Router generates the Network LSA for its segment. This LSA lists all routers attached to the segment that have formed full adjacencies with the DR. The Network LSA acts as a pseudo-node in the SPF graph. Instead of creating a full mesh of links between all routers on the segment, SPF creates links from each router to the Network LSA, and the Network LSA implicitly connects them all.

Without the Network LSA optimization, a broadcast segment with N routers would require N*(N-1)/2 link descriptions in the Router LSAs. With 10 routers, that is 45 links. The Network LSA reduces this to N links: each router has one link to the pseudo-node. This dramatically simplifies the topology database.

The Network LSA contains the network mask of the segment. This mask is critical for determining which IP addresses belong to this network. When SPF computes routes, it needs to know the exact network prefixes, not just the router IDs.

## Type 3 Summary LSA: Inter-Area Routes

Type 3 Summary LSAs are how routes propagate between areas. An Area Border Router generates Type 3 LSAs to advertise networks from one area into another area. These LSAs do not describe topology. They simply state "network X.X.X.X/Y is reachable through me with cost Z."

The key insight is that Type 3 LSAs hide topology details. When an ABR advertises a network from Area 1 into Area 0, routers in Area 0 do not learn about the internal structure of Area 1. They only know the ABR can reach that network and the cost to do so. This information hiding is what makes areas scale.

Type 3 LSAs flood throughout their destination area but stop at area boundaries. When ABR1 connects Area 0 and Area 1, and ABR2 connects Area 0 and Area 2, here is what happens: ABR1 generates Type 3 LSAs from Area 1 and floods them into Area 0. ABR2 receives those LSAs in Area 0. ABR2 can then generate new Type 3 LSAs and flood them into Area 2, allowing Area 2 to learn about Area 1 networks. But the Type 3 LSAs that flooded Area 0 never directly enter Area 2. ABR2 must explicitly re-advertise them.

This multi-step process enforces the hub-and-spoke routing model. Routes from Area 1 to Area 2 must transit Area 0. Even if ABR1 and ABR2 are directly connected, if that link is not in Area 0, it cannot carry inter-area traffic.

The cost in a Type 3 LSA is the cost from the ABR to the destination network within the source area. When a router in the destination area receives the Type 3 LSA, it adds the cost to reach the ABR to the cost in the LSA. This sum gives the total cost to reach the destination network.

## Type 4 ASBR Summary LSA: Finding the External Router

Type 4 LSAs solve a specific problem: how do routers learn the path to an Autonomous System Boundary Router that is redistributing external routes? Type 5 External LSAs contain the network prefixes from outside OSPF, but they do not explain how to reach the ASBR that originated them.

An ABR generates a Type 4 LSA when it learns about an ASBR in one of its areas. The Type 4 LSA advertises the ASBR's router ID and the cost to reach it. This LSA floods into other areas connected to that ABR. Routers in those areas now know how to reach the ASBR.

Consider an ASBR in Area 1 that redistributes BGP routes. The ASBR generates Type 5 LSAs describing those external networks. These Type 5 LSAs flood throughout the entire OSPF domain. But routers in Area 2 receive the Type 5 LSAs and need to know: how do I reach the ASBR listed in this Type 5 LSA? The Type 4 LSA provides that answer. It tells Area 2 routers "the ASBR is reachable through ABR1 with cost X."

Without Type 4 LSAs, external routes would be unusable outside the ASBR's area. Routers would know the external networks exist but have no path to the ASBR that can forward packets to those networks.

## Type 5 External LSA: Beyond OSPF

Type 5 External LSAs describe routes from outside the OSPF routing domain. When you redistribute static routes, BGP routes, or routes from another IGP into OSPF, they become Type 5 LSAs. These LSAs flood throughout the entire OSPF domain, crossing all area boundaries except stub areas.

Type 5 LSAs have a unique flooding scope. They are the only LSA type that floods everywhere (except where explicitly blocked by stub area configuration). This makes sense because external routes are not tied to any specific area. A BGP route redistributed in Area 1 is equally relevant to routers in Area 2 and Area 3.

The External LSA includes a metric type field with two values: Type 1 (E1) and Type 2 (E2). This distinction is critical. Type 2 is the default and means the cost to reach the external network is just the external cost listed in the LSA. The internal OSPF cost to reach the ASBR is not added. Type 1 means the total cost is the external cost plus the internal cost to reach the ASBR.

Why have two metric types? Type 2 makes sense when the external metric is much larger than internal OSPF metrics. If you redistribute a BGP route with metric 200, and your internal OSPF costs are 1 to 10, adding the internal cost barely changes the total. Type 2 keeps the external metric dominant. Type 1 makes sense when you want the internal path to the ASBR to influence route selection. If two ASBRs advertise the same external network, Type 1 causes OSPF to prefer the closer ASBR.

The External LSA can include a forwarding address. This address tells routers to forward packets to a specific next-hop rather than to the ASBR itself. This optimizes the path when the ASBR is not the actual next-hop for the external network.

## Type 7 NSSA External LSA: Stub Area Workaround

Type 7 LSAs solve the problem of stub areas that need to advertise external routes. Normal stub areas cannot contain an ASBR because they block Type 5 External LSAs. But what if a stub area connects to a small external network you want to advertise into OSPF?

Not-So-Stubby Area (NSSA) allows limited external route advertisement within a stub area. The ASBR in the NSSA generates Type 7 LSAs instead of Type 5 LSAs. These Type 7 LSAs flood within the NSSA like any intra-area LSA. They stop at the area boundary.

The ABR connecting the NSSA to the backbone examines the Type 7 LSAs. It converts selected Type 7 LSAs into Type 5 LSAs and floods them into Area 0. From there, the Type 5 LSAs flood normally throughout the OSPF domain. This conversion allows the NSSA's external routes to be advertised while still maintaining the stub area's benefits of blocking other external routes.

Not all Type 7 LSAs are converted. The ABR may filter certain Type 7 LSAs or may perform route summarization. The Type 7 LSA includes a P-bit indicating whether the ABR should translate it to Type 5. The ASBR sets this bit based on local policy.

## Type 9, 10, 11 Opaque LSAs: Extension Mechanism

Opaque LSAs provide a generic extension mechanism for OSPF. The original OSPF specification defined LSA types 1 through 5. As new features emerged, the protocol needed a way to carry additional information without defining entirely new LSA types for each feature.

Type 9 Opaque LSAs have link-local scope. They flood only on the link where they originate and do not propagate beyond that link. Type 10 Opaque LSAs have area-local scope, flooding throughout an area like Type 1 and 2 LSAs. Type 11 Opaque LSAs have AS-wide scope, flooding throughout the OSPF domain like Type 5 LSAs.

The Opaque LSA contains a type field within its contents that identifies what kind of information it carries. MPLS Traffic Engineering uses Type 10 Opaque LSAs to advertise link bandwidth, reservable bandwidth, and administrative groups. Grace LSAs for Graceful Restart use Type 9 Opaque LSAs. Future OSPF extensions can define new opaque sub-types without modifying the base protocol.

Routers that do not understand a particular opaque LSA type still flood it correctly according to its scope. They treat it as opaque data and simply forward it. This allows new features to be deployed gradually without requiring simultaneous upgrades across the network.

## Implementation Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LSA 100
#define MAX_LINKS 20

/* LSA Types */
typedef enum {
    LSA_TYPE_ROUTER = 1,
    LSA_TYPE_NETWORK = 2,
    LSA_TYPE_SUMMARY = 3,
    LSA_TYPE_ASBR_SUMMARY = 4,
    LSA_TYPE_EXTERNAL = 5,
    LSA_TYPE_NSSA_EXTERNAL = 7,
    LSA_TYPE_OPAQUE_LINK = 9,
    LSA_TYPE_OPAQUE_AREA = 10,
    LSA_TYPE_OPAQUE_AS = 11
} LSAType;

/* External Metric Type */
typedef enum {
    METRIC_TYPE_1 = 1,
    METRIC_TYPE_2 = 2
} ExternalMetricType;

/* Router Link Types */
typedef enum {
    LINK_TYPE_P2P = 1,
    LINK_TYPE_TRANSIT = 2,
    LINK_TYPE_STUB = 3,
    LINK_TYPE_VIRTUAL = 4
} RouterLinkType;

/* Router Link */
typedef struct {
    RouterLinkType type;
    char link_id[16];
    char link_data[16];
    int metric;
} RouterLink;

/* Type 1: Router LSA */
typedef struct {
    char advertising_router[16];
    int area_id;
    unsigned int sequence;
    bool is_abr;
    bool is_asbr;
    RouterLink links[MAX_LINKS];
    int link_count;
} RouterLSA;

/* Type 2: Network LSA */
typedef struct {
    char advertising_router[16];  /* The DR */
    int area_id;
    char network_mask[16];
    char attached_routers[MAX_LINKS][16];
    int router_count;
    unsigned int sequence;
} NetworkLSA;

/* Type 3: Summary LSA */
typedef struct {
    char advertising_router[16];  /* The ABR */
    int area_id;
    char network[32];
    char netmask[16];
    int metric;
    unsigned int sequence;
} SummaryLSA;

/* Type 4: ASBR Summary LSA */
typedef struct {
    char advertising_router[16];  /* The ABR */
    int area_id;
    char asbr_id[16];
    int metric;
    unsigned int sequence;
} ASBRSummaryLSA;

/* Type 5: External LSA */
typedef struct {
    char advertising_router[16];  /* The ASBR */
    char network[32];
    char netmask[16];
    ExternalMetricType metric_type;
    int metric;
    char forwarding_address[16];
    int external_route_tag;
    unsigned int sequence;
} ExternalLSA;

/* Type 7: NSSA External LSA */
typedef struct {
    char advertising_router[16];
    int area_id;
    char network[32];
    char netmask[16];
    ExternalMetricType metric_type;
    int metric;
    bool p_bit;  /* Propagate bit */
    unsigned int sequence;
} NSSAExternalLSA;

/* LSA Database */
typedef struct {
    RouterLSA router_lsas[MAX_LSA];
    int router_lsa_count;
    
    NetworkLSA network_lsas[MAX_LSA];
    int network_lsa_count;
    
    SummaryLSA summary_lsas[MAX_LSA];
    int summary_lsa_count;
    
    ASBRSummaryLSA asbr_summary_lsas[MAX_LSA];
    int asbr_summary_lsa_count;
    
    ExternalLSA external_lsas[MAX_LSA];
    int external_lsa_count;
    
    NSSAExternalLSA nssa_lsas[MAX_LSA];
    int nssa_lsa_count;
} LSADatabase;

/* Initialize LSA database */
void init_lsa_database(LSADatabase *db) {
    db->router_lsa_count = 0;
    db->network_lsa_count = 0;
    db->summary_lsa_count = 0;
    db->asbr_summary_lsa_count = 0;
    db->external_lsa_count = 0;
    db->nssa_lsa_count = 0;
}

/* Create Type 1 Router LSA */
RouterLSA create_router_lsa(const char *router_id, int area_id, 
                            bool is_abr, bool is_asbr) {
    RouterLSA lsa;
    strcpy(lsa.advertising_router, router_id);
    lsa.area_id = area_id;
    lsa.sequence = 0x80000001;
    lsa.is_abr = is_abr;
    lsa.is_asbr = is_asbr;
    lsa.link_count = 0;
    return lsa;
}

/* Add link to Router LSA */
void add_router_link(RouterLSA *lsa, RouterLinkType type, 
                    const char *link_id, const char *link_data, int metric) {
    if (lsa->link_count >= MAX_LINKS) return;
    
    RouterLink *link = &lsa->links[lsa->link_count];
    link->type = type;
    strcpy(link->link_id, link_id);
    strcpy(link->link_data, link_data);
    link->metric = metric;
    lsa->link_count++;
}

/* Create Type 2 Network LSA */
NetworkLSA create_network_lsa(const char *dr_id, int area_id, 
                              const char *netmask) {
    NetworkLSA lsa;
    strcpy(lsa.advertising_router, dr_id);
    lsa.area_id = area_id;
    strcpy(lsa.network_mask, netmask);
    lsa.router_count = 0;
    lsa.sequence = 0x80000001;
    return lsa;
}

/* Add router to Network LSA */
void add_attached_router(NetworkLSA *lsa, const char *router_id) {
    if (lsa->router_count >= MAX_LINKS) return;
    strcpy(lsa->attached_routers[lsa->router_count], router_id);
    lsa->router_count++;
}

/* Create Type 3 Summary LSA */
SummaryLSA create_summary_lsa(const char *abr_id, int area_id,
                              const char *network, const char *netmask, 
                              int metric) {
    SummaryLSA lsa;
    strcpy(lsa.advertising_router, abr_id);
    lsa.area_id = area_id;
    strcpy(lsa.network, network);
    strcpy(lsa.netmask, netmask);
    lsa.metric = metric;
    lsa.sequence = 0x80000001;
    return lsa;
}

/* Create Type 4 ASBR Summary LSA */
ASBRSummaryLSA create_asbr_summary_lsa(const char *abr_id, int area_id,
                                       const char *asbr_id, int metric) {
    ASBRSummaryLSA lsa;
    strcpy(lsa.advertising_router, abr_id);
    lsa.area_id = area_id;
    strcpy(lsa.asbr_id, asbr_id);
    lsa.metric = metric;
    lsa.sequence = 0x80000001;
    return lsa;
}

/* Create Type 5 External LSA */
ExternalLSA create_external_lsa(const char *asbr_id, const char *network,
                               const char *netmask, ExternalMetricType type,
                               int metric) {
    ExternalLSA lsa;
    strcpy(lsa.advertising_router, asbr_id);
    strcpy(lsa.network, network);
    strcpy(lsa.netmask, netmask);
    lsa.metric_type = type;
    lsa.metric = metric;
    strcpy(lsa.forwarding_address, "0.0.0.0");
    lsa.external_route_tag = 0;
    lsa.sequence = 0x80000001;
    return lsa;
}

/* Create Type 7 NSSA External LSA */
NSSAExternalLSA create_nssa_external_lsa(const char *asbr_id, int area_id,
                                        const char *network, const char *netmask,
                                        ExternalMetricType type, int metric,
                                        bool propagate) {
    NSSAExternalLSA lsa;
    strcpy(lsa.advertising_router, asbr_id);
    lsa.area_id = area_id;
    strcpy(lsa.network, network);
    strcpy(lsa.netmask, netmask);
    lsa.metric_type = type;
    lsa.metric = metric;
    lsa.p_bit = propagate;
    lsa.sequence = 0x80000001;
    return lsa;
}

/* Add LSAs to database */
void add_router_lsa(LSADatabase *db, RouterLSA *lsa) {
    if (db->router_lsa_count >= MAX_LSA) return;
    db->router_lsas[db->router_lsa_count++] = *lsa;
}

void add_network_lsa(LSADatabase *db, NetworkLSA *lsa) {
    if (db->network_lsa_count >= MAX_LSA) return;
    db->network_lsas[db->network_lsa_count++] = *lsa;
}

void add_summary_lsa(LSADatabase *db, SummaryLSA *lsa) {
    if (db->summary_lsa_count >= MAX_LSA) return;
    db->summary_lsas[db->summary_lsa_count++] = *lsa;
}

void add_asbr_summary_lsa(LSADatabase *db, ASBRSummaryLSA *lsa) {
    if (db->asbr_summary_lsa_count >= MAX_LSA) return;
    db->asbr_summary_lsas[db->asbr_summary_lsa_count++] = *lsa;
}

void add_external_lsa(LSADatabase *db, ExternalLSA *lsa) {
    if (db->external_lsa_count >= MAX_LSA) return;
    db->external_lsas[db->external_lsa_count++] = *lsa;
}

void add_nssa_external_lsa(LSADatabase *db, NSSAExternalLSA *lsa) {
    if (db->nssa_lsa_count >= MAX_LSA) return;
    db->nssa_lsas[db->nssa_lsa_count++] = *lsa;
}

/* Print LSA database */
void print_lsa_database(LSADatabase *db, const char *router_name) {
    printf("\n========== %s LSDB ==========\n", router_name);
    
    printf("\n--- Type 1: Router LSAs ---\n");
    for (int i = 0; i < db->router_lsa_count; i++) {
        RouterLSA *lsa = &db->router_lsas[i];
        printf("Router: %s, Area: %d, ABR: %s, ASBR: %s\n",
               lsa->advertising_router, lsa->area_id,
               lsa->is_abr ? "Yes" : "No",
               lsa->is_asbr ? "Yes" : "No");
        for (int j = 0; j < lsa->link_count; j++) {
            printf("  Link: ID=%s, Metric=%d\n",
                   lsa->links[j].link_id, lsa->links[j].metric);
        }
    }
    
    printf("\n--- Type 2: Network LSAs ---\n");
    for (int i = 0; i < db->network_lsa_count; i++) {
        NetworkLSA *lsa = &db->network_lsas[i];
        printf("DR: %s, Area: %d, Mask: %s\n",
               lsa->advertising_router, lsa->area_id, lsa->network_mask);
        printf("  Attached routers: ");
        for (int j = 0; j < lsa->router_count; j++) {
            printf("%s ", lsa->attached_routers[j]);
        }
        printf("\n");
    }
    
    printf("\n--- Type 3: Summary LSAs ---\n");
    for (int i = 0; i < db->summary_lsa_count; i++) {
        SummaryLSA *lsa = &db->summary_lsas[i];
        printf("ABR: %s, Area: %d, Network: %s, Metric: %d\n",
               lsa->advertising_router, lsa->area_id,
               lsa->network, lsa->metric);
    }
    
    printf("\n--- Type 4: ASBR Summary LSAs ---\n");
    for (int i = 0; i < db->asbr_summary_lsa_count; i++) {
        ASBRSummaryLSA *lsa = &db->asbr_summary_lsas[i];
        printf("ABR: %s, Area: %d, ASBR: %s, Metric: %d\n",
               lsa->advertising_router, lsa->area_id,
               lsa->asbr_id, lsa->metric);
    }
    
    printf("\n--- Type 5: External LSAs ---\n");
    for (int i = 0; i < db->external_lsa_count; i++) {
        ExternalLSA *lsa = &db->external_lsas[i];
        printf("ASBR: %s, Network: %s, Type: E%d, Metric: %d\n",
               lsa->advertising_router, lsa->network,
               lsa->metric_type, lsa->metric);
    }
    
    printf("\n--- Type 7: NSSA External LSAs ---\n");
    for (int i = 0; i < db->nssa_lsa_count; i++) {
        NSSAExternalLSA *lsa = &db->nssa_lsas[i];
        printf("ASBR: %s, Area: %d, Network: %s, Type: N%d, Metric: %d, P-bit: %s\n",
               lsa->advertising_router, lsa->area_id, lsa->network,
               lsa->metric_type, lsa->metric, lsa->p_bit ? "Set" : "Clear");
    }
}

/* Simulate LSA generation and flooding */
int main() {
    printf("OSPF LSA Types Demonstration\n");
    printf("=============================\n");
    
    /* Create LSA databases for different routers */
    LSADatabase area1_router_db, area0_router_db, area2_router_db;
    init_lsa_database(&area1_router_db);
    init_lsa_database(&area0_router_db);
    init_lsa_database(&area2_router_db);
    
    printf("\n--- Scenario: Multi-area OSPF with external routes ---\n");
    printf("Area 1: R1 (Internal Router)\n");
    printf("Area 0: ABR1, R2 (Backbone Router), ABR2\n");
    printf("Area 2: R3 (NSSA), ASBR1 (redistributes external routes)\n\n");
    
    /* Area 1: R1 generates Router LSA */
    RouterLSA r1_lsa = create_router_lsa("1.1.1.1", 1, false, false);
    add_router_link(&r1_lsa, LINK_TYPE_TRANSIT, "10.1.1.1", "10.1.1.1", 10);
    add_router_link(&r1_lsa, LINK_TYPE_STUB, "192.168.1.0", "255.255.255.0", 1);
    add_router_lsa(&area1_router_db, &r1_lsa);
    
    /* Area 1: ABR1 generates Router LSA (exists in both Area 1 and Area 0) */
    RouterLSA abr1_area1_lsa = create_router_lsa("2.2.2.2", 1, true, false);
    add_router_link(&abr1_area1_lsa, LINK_TYPE_TRANSIT, "10.1.1.1", "10.1.1.2", 10);
    add_router_lsa(&area1_router_db, &abr1_area1_lsa);
    
    /* Area 1: DR generates Network LSA */
    NetworkLSA net1_lsa = create_network_lsa("1.1.1.1", 1, "255.255.255.0");
    add_attached_router(&net1_lsa, "1.1.1.1");
    add_attached_router(&net1_lsa, "2.2.2.2");
    add_network_lsa(&area1_router_db, &net1_lsa);
    
    /* Area 0: Backbone routers */
    RouterLSA abr1_area0_lsa = create_router_lsa("2.2.2.2", 0, true, false);
    add_router_link(&abr1_area0_lsa, LINK_TYPE_P2P, "3.3.3.3", "10.0.1.1", 5);
    add_router_lsa(&area0_router_db, &abr1_area0_lsa);
    
    RouterLSA r2_lsa = create_router_lsa("3.3.3.3", 0, false, false);
    add_router_link(&r2_lsa, LINK_TYPE_P2P, "2.2.2.2", "10.0.1.2", 5);
    add_router_link(&r2_lsa, LINK_TYPE_P2P, "4.4.4.4", "10.0.2.1", 8);
    add_router_lsa(&area0_router_db, &r2_lsa);
    
    RouterLSA abr2_area0_lsa = create_router_lsa("4.4.4.4", 0, true, false);
    add_router_link(&abr2_area0_lsa, LINK_TYPE_P2P, "3.3.3.3", "10.0.2.2", 8);
    add_router_lsa(&area0_router_db, &abr2_area0_lsa);
    
    /* ABR1 generates Summary LSA into Area 0 for Area 1 networks */
    SummaryLSA sum1_lsa = create_summary_lsa("2.2.2.2", 0, 
                                             "192.168.1.0", "255.255.255.0", 11);
    add_summary_lsa(&area0_router_db, &sum1_lsa);
    
    /* Area 2 (NSSA): ASBR1 generates Type 7 NSSA External LSA */
    RouterLSA asbr1_lsa = create_router_lsa("5.5.5.5", 2, false, true);
    add_router_link(&asbr1_lsa, LINK_TYPE_P2P, "4.4.4.4", "10.2.1.1", 15);
    add_router_lsa(&area2_router_db, &asbr1_lsa);
    
    NSSAExternalLSA nssa_lsa = create_nssa_external_lsa("5.5.5.5", 2,
                                                        "200.1.1.0", "255.255.255.0",
                                                        METRIC_TYPE_2, 20, true);
    add_nssa_external_lsa(&area2_router_db, &nssa_lsa);
    
    /* ABR2 generates Type 4 ASBR Summary into Area 0 */
    ASBRSummaryLSA asbr_sum_lsa = create_asbr_summary_lsa("4.4.4.4", 0, "5.5.5.5", 15);
    add_asbr_summary_lsa(&area0_router_db, &asbr_sum_lsa);
    
    /* ABR2 converts Type 7 to Type 5 and floods into Area 0 */
    ExternalLSA ext_lsa = create_external_lsa("5.5.5.5", "200.1.1.0", 
                                             "255.255.255.0", METRIC_TYPE_2, 20);
    add_external_lsa(&area0_router_db, &ext_lsa);
    
    /* Type 5 also floods into Area 1 (not a stub area) */
    add_external_lsa(&area1_router_db, &ext_lsa);
    
    /* ABR2 generates Summary LSA into Area 2 for Area 1 networks */
    SummaryLSA sum2_lsa = create_summary_lsa("4.4.4.4", 2,
                                             "192.168.1.0", "255.255.255.0", 24);
    add_summary_lsa(&area2_router_db, &sum2_lsa);
    
    /* Display all LSDBs */
    print_lsa_database(&area1_router_db, "Area 1 Router");
    print_lsa_database(&area0_router_db, "Area 0 Router");
    print_lsa_database(&area2_router_db, "Area 2 NSSA Router");
    
    printf("\n\n=== Key Observations ===\n");
    printf("1. Type 1 Router LSAs flood only within their area\n");
    printf("2. Type 2 Network LSAs describe multi-access segments\n");
    printf("3. Type 3 Summary LSAs carry inter-area routes\n");
    printf("4. Type 4 ASBR Summary LSAs help locate the ASBR\n");
    printf("5. Type 5 External LSAs flood throughout the domain\n");
    printf("6. Type 7 NSSA LSAs are converted to Type 5 at area boundary\n");
    printf("7. Area 2 (NSSA) has no Type 5 LSAs, only Type 7\n");
    
    return 0;
}
```

## The Practical Impact of LSA Design

LSA type selection directly affects network scalability and convergence behavior. When you configure an area as totally stubby, you eliminate Type 3 and Type 5 LSAs from that area's LSDB. The routers in that area now have dramatically smaller databases. A router in a totally stubby area with 50 routers might have 200 LSAs instead of 20000. SPF calculations complete in milliseconds instead of seconds.

The cost is loss of path optimality. That router only knows default routes for reaching other areas and external networks. It cannot choose the best path based on detailed topology. This tradeoff is acceptable in many stub networks where there is only one or two exit points anyway. The optimal path is obvious without detailed routing information.

Type 5 LSAs create the largest LSDBs because they flood everywhere. In networks with thousands of external routes from BGP, the Type 5 LSAs can dominate the LSDB size. This is why stub areas and NSSA exist. They provide mechanisms to limit Type 5 flooding while maintaining necessary connectivity.