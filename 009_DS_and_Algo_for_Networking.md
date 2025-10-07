# Hash Tables in Networking: Fast Lookups at Scale

## Understanding the Core Problem

When a network device receives a packet, it faces a time-critical decision: where should this packet go next? This question must be answered in microseconds, sometimes nanoseconds. The device needs to look up information based on various keys—destination IP addresses, MAC addresses, port numbers, or combinations thereof. A naive approach using linear search through a list would check each entry one by one until finding a match. For a routing table with ten thousand entries, this means potentially ten thousand comparisons per packet. At line rate speeds of millions of packets per second, this approach collapses immediately.

Hash tables solve this fundamental problem by transforming the lookup operation from linear time to near-constant time. Instead of searching through entries sequentially, a hash table uses mathematical computation to jump directly to the location where data should be stored or retrieved.

## The Hash Function: Computing Location from Data

At the heart of every hash table lies the hash function—a deterministic algorithm that converts input data into an array index. When you need to store or retrieve information about IP address 192.168.1.1, the hash function computes a number that serves as the index into the underlying array. The same input always produces the same output, ensuring you can find what you stored.

Consider a simple example with a routing table implemented as a hash table with 1024 slots. When a packet arrives destined for 10.0.5.23, the hash function processes this IP address and might produce the value 387. The routing information for this destination lives in slot 387 of the array. No searching required—the hash function calculated the exact location.

The quality of a hash function determines the performance of the entire structure. A good hash function distributes keys uniformly across the available slots. If every IP address hashes to slot 0, you've gained nothing—all entries pile up in one location, forcing linear search through a chain. Network devices typically use carefully designed hash functions that consider the bit patterns common in network addresses and spread them evenly.

```c
// Simple hash function for IPv4 addresses
uint32_t hash_ipv4(uint32_t ip_addr, uint32_t table_size) {
    // Use multiplicative hashing with a prime number
    // This spreads similar IP addresses across the table
    uint32_t hash = ip_addr * 2654435761U;  // Knuth's multiplicative constant
    
    // Mix the bits further using XOR and bit rotation
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
    
    return hash % table_size;
}

// More sophisticated hash for better distribution
uint32_t hash_ipv4_murmur(uint32_t ip_addr, uint32_t table_size) {
    uint32_t h = ip_addr;
    
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    
    return h % table_size;
}

// Example usage
void demonstrate_hash_function() {
    uint32_t table_size = 1024;
    
    // 192.168.1.1 in network byte order
    uint32_t ip1 = (192 << 24) | (168 << 16) | (1 << 8) | 1;
    uint32_t index1 = hash_ipv4(ip1, table_size);
    
    // 192.168.1.2 - similar IP, should hash to different slot
    uint32_t ip2 = (192 << 24) | (168 << 16) | (1 << 8) | 2;
    uint32_t index2 = hash_ipv4(ip2, table_size);
    
    printf("IP 192.168.1.1 hashes to slot %u\n", index1);
    printf("IP 192.168.1.2 hashes to slot %u\n", index2);
}
```

## Collision Handling: When Two Keys Compete

Mathematical reality imposes a constraint: if your hash table has 1024 slots but you need to store 10,000 entries, multiple keys must share slots. Even with fewer entries than slots, the birthday paradox guarantees collisions. When two different IP addresses hash to the same index, the system needs a strategy to store both.

Chaining represents the most common collision resolution strategy in networking applications. Each slot in the hash table array doesn't store a single entry—it stores a pointer to a linked list. When multiple keys hash to the same location, they form a chain at that slot. Looking up an entry requires computing the hash to find the right slot, then traversing the chain to find the exact match. If the hash function distributes keys well, chains remain short, and lookup time stays close to constant. If the hash function distributes poorly, chains grow long, and performance degrades toward linear search.

```c
// Hash table entry for chaining
typedef struct hash_entry {
    uint32_t key;              // IP address or other identifier
    void *value;               // Pointer to the actual data
    struct hash_entry *next;   // Next entry in the chain
} hash_entry_t;

// Hash table structure with chaining
typedef struct hash_table {
    hash_entry_t **buckets;    // Array of chain heads
    uint32_t size;             // Number of buckets
    uint32_t count;            // Number of entries
} hash_table_t;

// Create a new hash table
hash_table_t* create_hash_table(uint32_t size) {
    hash_table_t *table = malloc(sizeof(hash_table_t));
    table->size = size;
    table->count = 0;
    table->buckets = calloc(size, sizeof(hash_entry_t*));
    return table;
}

// Insert an entry with chaining
void hash_table_insert(hash_table_t *table, uint32_t key, void *value) {
    uint32_t index = hash_ipv4(key, table->size);
    
    // Check if key already exists
    hash_entry_t *entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->key == key) {
            entry->value = value;  // Update existing entry
            return;
        }
        entry = entry->next;
    }
    
    // Create new entry at head of chain
    hash_entry_t *new_entry = malloc(sizeof(hash_entry_t));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->count++;
}

// Lookup an entry
void* hash_table_lookup(hash_table_t *table, uint32_t key) {
    uint32_t index = hash_ipv4(key, table->size);
    
    hash_entry_t *entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return NULL;  // Not found
}

// Delete an entry
int hash_table_delete(hash_table_t *table, uint32_t key) {
    uint32_t index = hash_ipv4(key, table->size);
    
    hash_entry_t *entry = table->buckets[index];
    hash_entry_t *prev = NULL;
    
    while (entry != NULL) {
        if (entry->key == key) {
            if (prev == NULL) {
                table->buckets[index] = entry->next;
            } else {
                prev->next = entry->next;
            }
            free(entry);
            table->count--;
            return 1;  // Success
        }
        prev = entry;
        entry = entry->next;
    }
    
    return 0;  // Not found
}
```

Open addressing takes a different approach by storing all entries directly in the array without chains. When a collision occurs, the system probes for the next available slot using a deterministic sequence. Linear probing simply checks the next slot, then the next, until finding an empty space or the target entry. Quadratic probing increases the step size quadratically. Double hashing uses a second hash function to compute the probe sequence.

```c
// Hash table entry for open addressing
typedef struct oa_entry {
    uint32_t key;
    void *value;
    int occupied;  // 0 = empty, 1 = occupied, -1 = deleted
} oa_entry_t;

// Open addressing hash table
typedef struct oa_hash_table {
    oa_entry_t *entries;
    uint32_t size;
    uint32_t count;
} oa_hash_table_t;

// Create open addressing hash table
oa_hash_table_t* create_oa_table(uint32_t size) {
    oa_hash_table_t *table = malloc(sizeof(oa_hash_table_t));
    table->size = size;
    table->count = 0;
    table->entries = calloc(size, sizeof(oa_entry_t));
    return table;
}

// Linear probing insert
void oa_insert(oa_hash_table_t *table, uint32_t key, void *value) {
    if (table->count >= table->size * 0.75) {
        // Table too full, should resize
        return;
    }
    
    uint32_t index = hash_ipv4(key, table->size);
    uint32_t original_index = index;
    
    do {
        if (table->entries[index].occupied <= 0) {
            // Empty or deleted slot
            table->entries[index].key = key;
            table->entries[index].value = value;
            table->entries[index].occupied = 1;
            table->count++;
            return;
        } else if (table->entries[index].key == key) {
            // Update existing
            table->entries[index].value = value;
            return;
        }
        
        // Linear probe to next slot
        index = (index + 1) % table->size;
    } while (index != original_index);
}

// Linear probing lookup
void* oa_lookup(oa_hash_table_t *table, uint32_t key) {
    uint32_t index = hash_ipv4(key, table->size);
    uint32_t original_index = index;
    
    do {
        if (table->entries[index].occupied == 0) {
            return NULL;  // Empty slot, key not found
        }
        if (table->entries[index].occupied == 1 && 
            table->entries[index].key == key) {
            return table->entries[index].value;
        }
        
        index = (index + 1) % table->size;
    } while (index != original_index);
    
    return NULL;
}

// Double hashing for better probe distribution
uint32_t secondary_hash(uint32_t key, uint32_t table_size) {
    // Second hash must never return 0
    return 1 + (key % (table_size - 1));
}

void* oa_lookup_double_hash(oa_hash_table_t *table, uint32_t key) {
    uint32_t index = hash_ipv4(key, table->size);
    uint32_t step = secondary_hash(key, table->size);
    uint32_t original_index = index;
    
    do {
        if (table->entries[index].occupied == 0) {
            return NULL;
        }
        if (table->entries[index].occupied == 1 && 
            table->entries[index].key == key) {
            return table->entries[index].value;
        }
        
        index = (index + step) % table->size;
    } while (index != original_index);
    
    return NULL;
}
```

## Routing Tables: The Canonical Application

When a router receives a packet, it must determine the next hop based on the destination IP address. Traditional routing tables store entries mapping destination networks to next-hop information. A hash table implementation allows the router to perform this lookup in constant time rather than searching through potentially millions of routes.

```c
// Routing table entry structure
typedef struct route_entry {
    uint32_t dest_ip;          // Destination IP (host address for exact match)
    uint32_t next_hop;         // Next hop IP address
    uint32_t interface_id;     // Outgoing interface
    uint32_t metric;           // Route metric
} route_entry_t;

// Routing table using hash table
typedef struct routing_table {
    hash_table_t *table;
} routing_table_t;

// Create routing table
routing_table_t* create_routing_table(uint32_t size) {
    routing_table_t *rt = malloc(sizeof(routing_table_t));
    rt->table = create_hash_table(size);
    return rt;
}

// Add a route (exact match only for this simple example)
void add_route(routing_table_t *rt, uint32_t dest_ip, 
               uint32_t next_hop, uint32_t interface_id, uint32_t metric) {
    route_entry_t *entry = malloc(sizeof(route_entry_t));
    entry->dest_ip = dest_ip;
    entry->next_hop = next_hop;
    entry->interface_id = interface_id;
    entry->metric = metric;
    
    hash_table_insert(rt->table, dest_ip, entry);
}

// Lookup route
route_entry_t* lookup_route(routing_table_t *rt, uint32_t dest_ip) {
    return (route_entry_t*)hash_table_lookup(rt->table, dest_ip);
}

// Example usage
void routing_example() {
    routing_table_t *rt = create_routing_table(1024);
    
    // Add routes
    uint32_t dest1 = (10 << 24) | (0 << 16) | (1 << 8) | 1;
    uint32_t nexthop1 = (10 << 24) | (0 << 16) | (1 << 8) | 254;
    add_route(rt, dest1, nexthop1, 1, 10);
    
    uint32_t dest2 = (192 << 24) | (168 << 16) | (1 << 8) | 1;
    uint32_t nexthop2 = (192 << 24) | (168 << 16) | (1 << 8) | 254;
    add_route(rt, dest2, nexthop2, 2, 5);
    
    // Lookup
    route_entry_t *route = lookup_route(rt, dest1);
    if (route) {
        printf("Found route: next_hop interface %u, metric %u\n", 
               route->interface_id, route->metric);
    }
}
```

The routing table hash uses the destination IP address as the key. When the router learns about a new route through BGP or OSPF, it computes the hash of the destination prefix and stores the routing information at that location. When forwarding a packet, the router hashes the destination address, jumps to that slot, and retrieves the next-hop information.

However, IP routing introduces a complication: longest prefix matching. A packet destined for 10.1.2.3 might match both 10.0.0.0/8 and 10.1.0.0/16, and the router must select the most specific match. Basic hash tables don't naturally support prefix matching—they excel at exact matches. This limitation drives routing tables toward more sophisticated data structures like tries for the actual prefix matching, while hash tables serve supporting roles for exact-match lookups in related tables.

## Neighbor Tables: Exact Match at Layer 2

The neighbor table—called the ARP cache in IPv4 or the neighbor cache in IPv6—maps Layer 3 addresses to Layer 2 MAC addresses. When a device needs to send a packet to 192.168.1.5 on the local network, it consults the neighbor table to find the corresponding MAC address. This represents a perfect hash table application because it requires exact matching, not prefix matching.

```c
// Neighbor table entry (ARP cache)
typedef struct neighbor_entry {
    uint32_t ip_addr;          // IPv4 address
    uint8_t mac_addr[6];       // MAC address
    uint32_t interface_id;     // Interface where neighbor is reachable
    time_t last_updated;       // For aging
    int state;                 // REACHABLE, STALE, DELAY, PROBE
} neighbor_entry_t;

#define NEIGHBOR_REACHABLE 1
#define NEIGHBOR_STALE 2
#define NEIGHBOR_DELAY 3

// Neighbor table
typedef struct neighbor_table {
    hash_table_t *table;
    time_t timeout;            // Entry timeout in seconds
} neighbor_table_t;

// Create neighbor table
neighbor_table_t* create_neighbor_table(uint32_t size, time_t timeout) {
    neighbor_table_t *nt = malloc(sizeof(neighbor_table_t));
    nt->table = create_hash_table(size);
    nt->timeout = timeout;
    return nt;
}

// Add or update neighbor
void neighbor_add(neighbor_table_t *nt, uint32_t ip_addr, 
                  uint8_t *mac_addr, uint32_t interface_id) {
    neighbor_entry_t *entry = (neighbor_entry_t*)hash_table_lookup(nt->table, ip_addr);
    
    if (entry == NULL) {
        entry = malloc(sizeof(neighbor_entry_t));
        entry->ip_addr = ip_addr;
        hash_table_insert(nt->table, ip_addr, entry);
    }
    
    memcpy(entry->mac_addr, mac_addr, 6);
    entry->interface_id = interface_id;
    entry->last_updated = time(NULL);
    entry->state = NEIGHBOR_REACHABLE;
}

// Lookup neighbor
neighbor_entry_t* neighbor_lookup(neighbor_table_t *nt, uint32_t ip_addr) {
    neighbor_entry_t *entry = (neighbor_entry_t*)hash_table_lookup(nt->table, ip_addr);
    
    if (entry == NULL) {
        return NULL;
    }
    
    // Check if entry has expired
    time_t now = time(NULL);
    if (now - entry->last_updated > nt->timeout) {
        hash_table_delete(nt->table, ip_addr);
        free(entry);
        return NULL;
    }
    
    return entry;
}

// Age out old entries (called periodically)
void neighbor_age(neighbor_table_t *nt) {
    time_t now = time(NULL);
    
    for (uint32_t i = 0; i < nt->table->size; i++) {
        hash_entry_t *entry = nt->table->buckets[i];
        hash_entry_t *prev = NULL;
        
        while (entry != NULL) {
            neighbor_entry_t *neighbor = (neighbor_entry_t*)entry->value;
            
            if (now - neighbor->last_updated > nt->timeout) {
                // Remove expired entry
                hash_entry_t *to_delete = entry;
                entry = entry->next;
                
                if (prev == NULL) {
                    nt->table->buckets[i] = entry;
                } else {
                    prev->next = entry;
                }
                
                free(neighbor);
                free(to_delete);
                nt->table->count--;
            } else {
                prev = entry;
                entry = entry->next;
            }
        }
    }
}

// Example: Handle ARP reply
void handle_arp_reply(neighbor_table_t *nt, uint32_t ip, uint8_t *mac, uint32_t iface) {
    printf("Received ARP reply: ");
    printf("%u.%u.%u.%u is at %02x:%02x:%02x:%02x:%02x:%02x\n",
           (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, 
           (ip >> 8) & 0xFF, ip & 0xFF,
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    neighbor_add(nt, ip, mac, iface);
}
```

## Flow Tables: Stateful Tracking

Modern network devices often maintain flow state—information about active connections passing through the device. A firewall tracks TCP connections, a NAT device maps internal addresses to external ports, and a load balancer maintains session affinity. All these applications need fast lookup of flow state based on packet header information.

```c
// 5-tuple for flow identification
typedef struct flow_tuple {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
} flow_tuple_t;

// Flow state entry
typedef struct flow_entry {
    flow_tuple_t tuple;
    uint64_t packet_count;
    uint64_t byte_count;
    time_t first_seen;
    time_t last_seen;
    int state;                 // TCP state for connection tracking
    void *app_data;           // Application-specific data
} flow_entry_t;

// Hash function for 5-tuple
uint32_t hash_flow_tuple(flow_tuple_t *tuple, uint32_t table_size) {
    // Combine all five fields into a single hash
    uint32_t hash = 0;
    
    // Mix source IP
    hash ^= tuple->src_ip;
    hash = (hash << 5) | (hash >> 27);  // Rotate left 5 bits
    
    // Mix destination IP
    hash ^= tuple->dst_ip;
    hash = (hash << 5) | (hash >> 27);
    
    // Mix ports (combined into single 32-bit value)
    uint32_t ports = (tuple->src_port << 16) | tuple->dst_port;
    hash ^= ports;
    hash = (hash << 5) | (hash >> 27);
    
    // Mix protocol
    hash ^= tuple->protocol;
    
    // Final mixing
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
    
    return hash % table_size;
}

// Compare two flow tuples for equality
int flow_tuple_equal(flow_tuple_t *a, flow_tuple_t *b) {
    return a->src_ip == b->src_ip &&
           a->dst_ip == b->dst_ip &&
           a->src_port == b->src_port &&
           a->dst_port == b->dst_port &&
           a->protocol == b->protocol;
}

// Flow table structure
typedef struct flow_table {
    hash_entry_t **buckets;
    uint32_t size;
    uint32_t count;
} flow_table_t;

// Create flow table
flow_table_t* create_flow_table(uint32_t size) {
    flow_table_t *table = malloc(sizeof(flow_table_t));
    table->size = size;
    table->count = 0;
    table->buckets = calloc(size, sizeof(hash_entry_t*));
    return table;
}

// Lookup or create flow
flow_entry_t* flow_lookup_or_create(flow_table_t *table, flow_tuple_t *tuple) {
    uint32_t index = hash_flow_tuple(tuple, table->size);
    
    // Search for existing flow
    hash_entry_t *entry = table->buckets[index];
    while (entry != NULL) {
        flow_entry_t *flow = (flow_entry_t*)entry->value;
        if (flow_tuple_equal(&flow->tuple, tuple)) {
            return flow;  // Found existing flow
        }
        entry = entry->next;
    }
    
    // Create new flow
    flow_entry_t *new_flow = malloc(sizeof(flow_entry_t));
    memcpy(&new_flow->tuple, tuple, sizeof(flow_tuple_t));
    new_flow->packet_count = 0;
    new_flow->byte_count = 0;
    new_flow->first_seen = time(NULL);
    new_flow->last_seen = new_flow->first_seen;
    new_flow->state = 0;
    new_flow->app_data = NULL;
    
    // Insert into hash table
    hash_entry_t *new_entry = malloc(sizeof(hash_entry_t));
    new_entry->key = index;  // Store index as key for this example
    new_entry->value = new_flow;
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    table->count++;
    
    return new_flow;
}

// Process a packet and update flow state
void process_packet(flow_table_t *table, uint32_t src_ip, uint32_t dst_ip,
                   uint16_t src_port, uint16_t dst_port, uint8_t protocol,
                   uint32_t packet_len) {
    flow_tuple_t tuple = {
        .src_ip = src_ip,
        .dst_ip = dst_ip,
        .src_port = src_port,
        .dst_port = dst_port,
        .protocol = protocol
    };
    
    flow_entry_t *flow = flow_lookup_or_create(table, &tuple);
    
    // Update statistics
    flow->packet_count++;
    flow->byte_count += packet_len;
    flow->last_seen = time(NULL);
}

// Example: Track TCP flows
void firewall_example() {
    flow_table_t *flows = create_flow_table(4096);
    
    // Simulate incoming packets
    process_packet(flows, 
                   (192 << 24) | (168 << 16) | (1 << 8) | 100,  // src
                   (10 << 24) | (0 << 16) | (0 << 8) | 1,       // dst
                   45678, 80, 6, 1500);  // src_port, dst_port, TCP, length
    
    process_packet(flows,
                   (192 << 24) | (168 << 16) | (1 << 8) | 100,
                   (10 << 24) | (0 << 16) | (0 << 8) | 1,
                   45678, 80, 6, 1500);  // Same flow, second packet
    
    printf("Flow table contains %u flows\n", flows->count);
}
```

The 5-tuple hash must distribute flows evenly to avoid hot spots where many connections hash to the same slot. Network traffic often shows patterns—many connections to the same destination IP or from the same source subnet. A poorly designed hash function might place all web traffic to a popular website in the same chain, degrading lookup performance for those flows while leaving other slots empty.

## Load Factor and Resizing

As a hash table fills, collision probability increases. With 100 entries in 1024 slots, collisions remain rare. With 900 entries, most insertions collide. The load factor—entries divided by slots—quantifies this fullness. Network devices typically maintain load factors between 0.5 and 0.75 to balance memory usage against lookup performance.

When the load factor exceeds the threshold, the hash table must resize. Resizing allocates a larger array—often doubling the size—and rehashes all existing entries into the new array.

```c
// Resize hash table when load factor exceeds threshold
void hash_table_resize(hash_table_t *table) {
    uint32_t old_size = table->size;
    uint32_t new_size = old_size * 2;
    
    printf("Resizing hash table from %u to %u buckets\n", old_size, new_size);
    
    // Allocate new bucket array
    hash_entry_t **new_buckets = calloc(new_size, sizeof(hash_entry_t*));
    
    // Rehash all existing entries
    for (uint32_t i = 0; i < old_size; i++) {
        hash_entry_t *entry = table->buckets[i];
        
        while (entry != NULL) {
            hash_entry_t *next = entry->next;
            
            // Compute new index
            uint32_t new_index = hash_ipv4(entry->key, new_size);
            
            // Insert at head of new bucket
            entry->next = new_buckets[new_index];
            new_buckets[new_index] = entry;
            
            entry = next;
        }
    }
    
    // Replace old buckets with new
    free(table->buckets);
    table->buckets = new_buckets;
    table->size = new_size;
}

// Insert with automatic resizing
void hash_table_insert_with_resize(hash_table_t *table, uint32_t key, void *value) {
    // Check load factor
    float load_factor = (float)table->count / (float)table->size;
    if (load_factor > 0.75) {
        hash_table_resize(table);
    }
    
    hash_table_insert(table, key, value);
}

// Incremental rehashing for real-time systems
typedef struct incremental_hash_table {
    hash_entry_t **old_buckets;
    hash_entry_t **new_buckets;
    uint32_t old_size;
    uint32_t new_size;
    uint32_t rehash_index;    // Current bucket being rehashed
    uint32_t count;
    int resizing;             // 1 if resize in progress
} inc_hash_table_t;

// Start incremental resize
void inc_resize_start(inc_hash_table_t *table) {
    table->new_size = table->old_size * 2;
    table->new_buckets = calloc(table->new_size, sizeof(hash_entry_t*));
    table->rehash_index = 0;
    table->resizing = 1;
}

// Rehash a few buckets (called between packet processing)
void inc_rehash_step(inc_hash_table_t *table, int buckets_per_step) {
    if (!table->resizing) return;
    
    for (int i = 0; i < buckets_per_step && table->rehash_index < table->old_size; i++) {
        hash_entry_t *entry = table->old_buckets[table->rehash_index];
        
        while (entry != NULL) {
            hash_entry_t *next = entry->next;
            uint32_t new_index = hash_ipv4(entry->key, table->new_size);
            
            entry->next = table->new_buckets[new_index];
            table->new_buckets[new_index] = entry;
            
            entry = next;
        }
        
        table->rehash_index++;
    }
    
    // Check if rehashing complete
    if (table->rehash_index >= table->old_size) {
        free(table->old_buckets);
        table->old_buckets = table->new_buckets;
        table->old_size = table->new_size;
        table->new_buckets = NULL;
        table->resizing = 0;
    }
}

// Lookup during incremental resize (checks both tables)
void* inc_lookup(inc_hash_table_t *table, uint32_t key) {
    if (table->resizing) {
        // Check if this bucket has been rehashed
        uint32_t old_index = hash_ipv4(key, table->old_size);
        
        if (old_index >= table->rehash_index) {
            // Not yet rehashed, check old table
            hash_entry_t *entry = table->old_buckets[old_index];
            while (entry != NULL) {
                if (entry->key == key) return entry->value;
                entry = entry->next;
            }
        }
        
        // Check new table
        uint32_t new_index = hash_ipv4(key, table->new_size);
        hash_entry_t *entry = table->new_buckets[new_index];
        while (entry != NULL) {
            if (entry->key == key) return entry->value;
            entry = entry->next;
        }
    } else {
        // Normal lookup
        uint32_t index = hash_ipv4(key, table->old_size);
        hash_entry_t *entry = table->old_buckets[index];
        while (entry != NULL) {
            if (entry->key == key) return entry->value;
            entry = entry->next;
        }
    }
    
    return NULL;
}
```

## Cache Line Optimization

Modern CPUs don't fetch single bytes from memory—they fetch cache lines, typically 64 bytes. When your hash function computes an index and the CPU retrieves that entry, it pulls an entire cache line into CPU cache. Network device implementations structure hash table entries carefully to align with cache line boundaries.

```c
// Cache-aligned hash table entry (64 bytes)
typedef struct __attribute__((aligned(64))) cache_aligned_entry {
    uint32_t key;              // 4 bytes
    uint32_t next_index;       // 4 bytes (index instead of pointer)
    uint32_t value_data[12];   // 48 bytes of inline data
    uint32_t padding;          // 4 bytes padding
} cache_aligned_entry_t;      // Total: 64 bytes (1 cache line)

// Compact entry for better cache utilization
typedef struct compact_entry {
    uint32_t key;              // 4 bytes
    uint32_t value;            // 4 bytes (or pointer)
    uint32_t next;             // 4 bytes
    uint32_t reserved;         // 4 bytes
} compact_entry_t;            // Total: 16 bytes (4 per cache line)

// Array-based hash table for better cache performance
typedef struct cache_friendly_table {
    compact_entry_t *entries;  // Dense array
    uint32_t *buckets;         // Bucket heads (indices into entries array)
    uint32_t size;
    uint32_t capacity;
    uint32_t free_head;        // Head of free list
} cache_friendly_table_t;
```

## Concurrent Access in Multi-Core Systems

Modern network devices run on multi-core processors, with different cores processing different packets simultaneously. Multiple cores may attempt to read or modify the same hash table concurrently.

```c
#include <pthread.h>
#include <stdatomic.h>

// Hash table with per-bucket locks
typedef struct locked_hash_table {
    hash_entry_t **buckets;
    pthread_rwlock_t *locks;   // One lock per bucket (or per group of buckets)
    uint32_t size;
    uint32_t count;
    uint32_t lock_count;       // Number of locks (typically size/16)
} locked_hash_table_t;

// Create hash table with locks
locked_hash_table_t* create_locked_table(uint32_t size) {
    locked_hash_table_t *table = malloc(sizeof(locked_hash_table_t));
    table->size = size;
    table->count = 0;
    table->lock_count = size / 16;  // One lock per 16 buckets
    
    table->buckets = calloc(size, sizeof(hash_entry_t*));
    table->locks = malloc(table->lock_count * sizeof(pthread_rwlock_t));
    
    for (uint32_t i = 0; i < table->lock_count; i++) {
        pthread_rwlock_init(&table->locks[i], NULL);
    }
    
    return table;
}

// Get lock index for a bucket
static inline uint32_t get_lock_index(locked_hash_table_t *table, uint32_t bucket) {
    return (bucket / 16) % table->lock_count;
}

// Thread-safe insert
void locked_insert(locked_hash_table_t *table, uint32_t key, void *value) {
    uint32_t index = hash_ipv4(key, table->size);
    uint32_t lock_idx = get_lock_index(table, index);
    
    pthread_rwlock_wrlock(&table->locks[lock_idx]);
    
    // Check for existing key
    hash_entry_t *entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->key == key) {
            entry->value = value;
            pthread_rwlock_unlock(&table->locks[lock_idx]);
            return;
        }
        entry = entry->next;
    }
    
    // Insert new entry
    hash_entry_t *new_entry = malloc(sizeof(hash_entry_t));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
    
    __atomic_fetch_add(&table->count, 1, __ATOMIC_SEQ_CST);
    
    pthread_rwlock_unlock(&table->locks[lock_idx]);
}

// Thread-safe lookup
void* locked_lookup(locked_hash_table_t *table, uint32_t key) {
    uint32_t index = hash_ipv4(key, table->size);
    uint32_t lock_idx = get_lock_index(table, index);
    
    pthread_rwlock_rdlock(&table->locks[lock_idx]);
    
    hash_entry_t *entry = table->buckets[index];
    while (entry != NULL) {
        if (entry->key == key) {
            void *value = entry->value;
            pthread_rwlock_unlock(&table->locks[lock_idx]);
            return value;
        }
        entry = entry->next;
    }
    
    pthread_rwlock_unlock(&table->locks[lock_idx]);
    return NULL;
}

// Lock-free lookup using RCU-style approach (simplified)
typedef struct lockfree_entry {
    uint32_t key;
    void *value;
    _Atomic(struct lockfree_entry*) next;
} lockfree_entry_t;

typedef struct lockfree_table {
    _Atomic(lockfree_entry_t*) *buckets;
    uint32_t size;
    _Atomic uint32_t count;
} lockfree_table_t;

// Lock-free lookup (readers never block)
void* lockfree_lookup(lockfree_table_t *table, uint32_t key) {
    uint32_t index = hash_ipv4(key, table->size);
    
    lockfree_entry_t *entry = atomic_load(&table->buckets[index]);
    while (entry != NULL) {
        if (entry->key == key) {
            return entry->value;
        }
        entry = atomic_load(&entry->next);
    }
    
    return NULL;
}

// Lock-free insert using compare-and-swap
int lockfree_insert(lockfree_table_t *table, uint32_t key, void *value) {
    uint32_t index = hash_ipv4(key, table->size);
    
    lockfree_entry_t *new_entry = malloc(sizeof(lockfree_entry_t));
    new_entry->key = key;
    new_entry->value = value;
    
    // Try to insert at head of list
    lockfree_entry_t *expected = atomic_load(&table->buckets[index]);
    do {
        atomic_store(&new_entry->next, expected);
    } while (!atomic_compare_exchange_weak(&table->buckets[index], 
                                           &expected, new_entry));
    
    atomic_fetch_add(&table->count, 1);
    return 1;
}
```

## Practical Performance Characteristics

```c
// Performance measurement example
#include <time.h>

void measure_hash_table_performance() {
    hash_table_t *table = create_hash_table(10000);
    
    // Insert 7500 entries (load factor 0.75)
    clock_t start = clock();
    for (uint32_t i = 0; i < 7500; i++) {
        uint32_t ip = (10 << 24) | (i & 0xFFFFFF);
        route_entry_t *entry = malloc(sizeof(route_entry_t));
        entry->dest_ip = ip;
        entry->next_hop = ip + 1;
        hash_table_insert(table, ip, entry);
    }
    clock_t end = clock();
    double insert_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Inserted 7500 entries in %.6f seconds\n", insert_time);
    printf("Average insert time: %.2f microseconds\n", 
           (insert_time * 1000000) / 7500);
    
    // Measure lookup performance
    start = clock();
    for (uint32_t i = 0; i < 100000; i++) {
        uint32_t ip = (10 << 24) | ((i * 7) % 7500);  // Random lookups
        route_entry_t *entry = hash_table_lookup(table, ip);
        (void)entry;  // Prevent optimization
    }
    end = clock();
    double lookup_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Performed 100000 lookups in %.6f seconds\n", lookup_time);
    printf("Average lookup time: %.2f nanoseconds\n", 
           (lookup_time * 1000000000) / 100000);
    
    // Measure chain lengths
    uint32_t max_chain = 0;
    uint32_t total_chain = 0;
    uint32_t non_empty = 0;
    
    for (uint32_t i = 0; i < table->size; i++) {
        uint32_t chain_len = 0;
        hash_entry_t *entry = table->buckets[i];
        while (entry != NULL) {
            chain_len++;
            entry = entry->next;
        }
        if (chain_len > 0) {
            non_empty++;
            total_chain += chain_len;
            if (chain_len > max_chain) {
                max_chain = chain_len;
            }
        }
    }
    
    printf("Non-empty buckets: %u / %u\n", non_empty, table->size);
    printf("Average chain length: %.2f\n", 
           (float)total_chain / non_empty);
    printf("Maximum chain length: %u\n", max_chain);
    printf("Load factor: %.2f\n", (float)table->count / table->size);
}
```

## Conclusion

Hash tables represent the workhorse data structure for exact-match lookups in networking. They convert potentially expensive search operations into fast constant-time access, enabling network devices to maintain and query large tables of state at line-rate speeds. From routing table optimization to flow tracking to neighbor resolution, hash tables appear throughout the network stack wherever devices need to map keys to values quickly.

The effectiveness of a hash table implementation depends on careful attention to hash function quality, collision handling strategy, load factor management, and memory layout optimization. Network software engineers must understand these factors deeply because the difference between a good hash table and a poor one directly impacts packet forwarding performance, throughput, and latency. Master hash tables, and you've mastered a fundamental building block of modern networking.

# Tries and Longest Prefix Matching in IP Routing

## The Fundamental Problem Hash Tables Cannot Solve

When a router receives a packet destined for 172.16.45.78, it must make a forwarding decision based on its routing table. Unlike hash table lookups that require exact key matches, IP routing demands something far more nuanced: longest prefix matching. The routing table might contain entries for 172.16.0.0/16, 172.16.45.0/24, and 172.16.45.64/26. All three prefixes match the destination address, but the router must select the most specific one—the longest matching prefix—which is 172.16.45.64/26 in this case.

Hash tables fail spectacularly at this task. You cannot hash 172.16.45.78 and expect to find 172.16.45.0/24. The mathematical properties that make hash tables fast—converting arbitrary keys into array indices—work only for exact matches. Routing requires a fundamentally different data structure, one that exploits the hierarchical nature of IP addresses and can efficiently navigate through progressively more specific prefixes. The trie delivers exactly this capability.

## Understanding the Trie Structure

A trie, pronounced "try" and derived from the word retrieval, organizes data in a tree where each edge represents a decision based on a single bit or character of the key. For IP routing, a binary trie uses the bits of an IP address to navigate from the root toward the leaves. Each node in the tree has at most two children: a left child for bit value 0 and a right child for bit value 1.

Consider building a trie for three routes: 10.0.0.0/8, 10.1.0.0/16, and 10.1.2.0/24. The root node represents the starting point before examining any bits. The first bit of 10.0.0.0 is 0 (since 10 in binary is 00001010), so we create a left child. We continue building the tree by examining successive bits, creating nodes as needed. When we reach the eighth bit (completing the /8 prefix), we mark that node as containing a route. The trie structure naturally captures the hierarchy: 10.1.0.0/16 exists as a descendant of 10.0.0.0/8, reflecting that it represents a more specific subset of that address space.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Binary trie node for IP routing
typedef struct trie_node {
    struct trie_node *left;      // Bit 0
    struct trie_node *right;     // Bit 1
    void *route_info;            // NULL if no route at this node
    uint8_t prefix_len;          // Length of prefix at this node (0 if no route)
} trie_node_t;

// Route information stored at trie nodes
typedef struct route_info {
    uint32_t prefix;
    uint8_t prefix_len;
    uint32_t next_hop;
    uint32_t interface_id;
    uint32_t metric;
} route_info_t;

// Create a new trie node
trie_node_t* create_trie_node() {
    trie_node_t *node = (trie_node_t*)calloc(1, sizeof(trie_node_t));
    return node;
}

// Extract bit at position from IP address (position 0 is leftmost/MSB)
static inline int get_bit(uint32_t addr, int position) {
    return (addr >> (31 - position)) & 1;
}

// Insert a route into the trie
void trie_insert(trie_node_t *root, uint32_t prefix, uint8_t prefix_len,
                 uint32_t next_hop, uint32_t interface_id, uint32_t metric) {
    trie_node_t *current = root;
    
    // Navigate through the trie based on prefix bits
    for (int i = 0; i < prefix_len; i++) {
        int bit = get_bit(prefix, i);
        
        if (bit == 0) {
            if (current->left == NULL) {
                current->left = create_trie_node();
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = create_trie_node();
            }
            current = current->right;
        }
    }
    
    // Store route information at this node
    if (current->route_info == NULL) {
        current->route_info = malloc(sizeof(route_info_t));
    }
    
    route_info_t *info = (route_info_t*)current->route_info;
    info->prefix = prefix;
    info->prefix_len = prefix_len;
    info->next_hop = next_hop;
    info->interface_id = interface_id;
    info->metric = metric;
    current->prefix_len = prefix_len;
}

// Longest prefix match lookup
route_info_t* trie_lookup(trie_node_t *root, uint32_t dest_addr) {
    trie_node_t *current = root;
    route_info_t *best_match = NULL;
    
    // Traverse the trie following the destination address bits
    for (int i = 0; i < 32 && current != NULL; i++) {
        // If current node has a route, remember it as potential match
        if (current->route_info != NULL) {
            best_match = (route_info_t*)current->route_info;
        }
        
        // Follow the appropriate branch
        int bit = get_bit(dest_addr, i);
        if (bit == 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    // Check if final node has a route
    if (current != NULL && current->route_info != NULL) {
        best_match = (route_info_t*)current->route_info;
    }
    
    return best_match;
}

// Example usage
void basic_trie_example() {
    trie_node_t *root = create_trie_node();
    
    // Insert routes: 10.0.0.0/8, 10.1.0.0/16, 10.1.2.0/24
    uint32_t prefix1 = 10U << 24;  // 10.0.0.0
    trie_insert(root, prefix1, 8, (10U << 24) | 1, 1, 10);
    
    uint32_t prefix2 = (10U << 24) | (1U << 16);  // 10.1.0.0
    trie_insert(root, prefix2, 16, (10U << 24) | (1U << 16) | 1, 2, 5);
    
    uint32_t prefix3 = (10U << 24) | (1U << 16) | (2U << 8);  // 10.1.2.0
    trie_insert(root, prefix3, 24, (10U << 24) | (1U << 16) | (2U << 8) | 1, 3, 3);
    
    // Lookup 10.1.2.55 - should match 10.1.2.0/24
    uint32_t dest = (10U << 24) | (1U << 16) | (2U << 8) | 55;
    route_info_t *route = trie_lookup(root, dest);
    
    if (route) {
        printf("Matched prefix: %u.%u.%u.%u/%u\n",
               (route->prefix >> 24) & 0xFF,
               (route->prefix >> 16) & 0xFF,
               (route->prefix >> 8) & 0xFF,
               route->prefix & 0xFF,
               route->prefix_len);
        printf("Next hop interface: %u\n", route->interface_id);
    }
}
```

## The Lookup Algorithm: Walking the Tree

Performing a longest prefix match requires traversing the trie from root to leaf, guided by the bits of the destination address. The algorithm maintains a pointer to the best match found so far. As it descends the tree, whenever it encounters a node containing route information, it updates the best match. This captures the essential property of longest prefix matching: a more specific route deeper in the tree overrides less specific routes higher up.

Starting at the root with destination address 10.1.2.55, the algorithm examines the first bit. For IP address 10.1.2.55, the binary representation begins with 00001010 00000001 00000010 00110111. The first bit is 0, so we follow the left pointer. We continue this process, checking each successive bit and following the corresponding branch. At the eighth bit, we encounter the node for 10.0.0.0/8 and record it as our current best match. Continuing onward, at bit 16 we find 10.1.0.0/16 and update our best match. At bit 24, we discover 10.1.2.0/24 and update once more. Even if the trie contains additional nodes beyond bit 24 for other destinations, they cannot improve our match—we've already found the most specific prefix that covers 10.1.2.55.

The algorithm's elegance lies in its simplicity: follow the bits, remember the last route encountered, and return it when the path ends. The data structure itself encodes the prefix hierarchy, eliminating any need for complex comparison logic.

## Memory Explosion and Path Compression

The binary trie suffers from a crippling weakness: memory consumption. A routing table with 800,000 routes might seem manageable, but consider what happens in the trie. An IPv4 address contains 32 bits, potentially requiring 32 levels of tree depth. In the worst case, every route could create 32 nodes. With 800,000 routes, you face up to 25 million nodes, each consuming perhaps 32 bytes for pointers and metadata. That's 800 megabytes for what should be perhaps 50 megabytes of actual route data.

The problem intensifies with sparse prefixes. A single route for 192.168.45.0/24 creates 24 nodes just to reach the leaf, most of which serve no purpose except connecting the root to that distant leaf. These long chains of single-child nodes waste both memory and lookup time—we must traverse every node even though the decision is predetermined.

Path compression attacks this waste directly by collapsing chains of single-child nodes into a single node that stores multiple bits. Instead of 24 separate nodes for 192.168.45.0/24, we create one node that stores "skip 24 bits checking pattern 192.168.45.0". During lookup, we compare multiple bits at once against the stored pattern. If they match, we continue. If they don't, the prefix doesn't match.

```c
// Path-compressed trie node (Patricia trie)
typedef struct patricia_node {
    struct patricia_node *left;
    struct patricia_node *right;
    void *route_info;
    uint32_t bit_position;       // Which bit to test
    uint32_t skip_value;         // Bits to match before this position
    uint8_t skip_len;            // How many bits to skip
} patricia_node_t;

// Create Patricia trie node
patricia_node_t* create_patricia_node() {
    patricia_node_t *node = (patricia_node_t*)calloc(1, sizeof(patricia_node_t));
    return node;
}

// Extract multiple bits from address
uint32_t get_bits(uint32_t addr, int start, int len) {
    return (addr >> (32 - start - len)) & ((1U << len) - 1);
}

// Check if skip pattern matches
int skip_matches(uint32_t addr, uint32_t skip_value, int start, int len) {
    uint32_t addr_bits = get_bits(addr, start, len);
    return addr_bits == skip_value;
}

// Insert into Patricia trie (simplified version)
void patricia_insert(patricia_node_t *root, uint32_t prefix, uint8_t prefix_len,
                     uint32_t next_hop, uint32_t interface_id) {
    patricia_node_t *current = root;
    int bit_pos = 0;
    
    while (bit_pos < prefix_len) {
        // Check if we can skip multiple bits
        int can_skip = 1;
        int skip_count = 0;
        
        // Find longest sequence where only one branch exists
        patricia_node_t *temp = current;
        while (bit_pos + skip_count < prefix_len && can_skip) {
            int bit = get_bit(prefix, bit_pos + skip_count);
            patricia_node_t *next = (bit == 0) ? temp->left : temp->right;
            
            if (next == NULL) {
                skip_count++;
            } else {
                can_skip = 0;
            }
            temp = next;
        }
        
        // Create compressed node if we can skip bits
        if (skip_count > 1) {
            patricia_node_t *new_node = create_patricia_node();
            new_node->bit_position = bit_pos;
            new_node->skip_len = skip_count;
            new_node->skip_value = get_bits(prefix, bit_pos, skip_count);
            
            int bit = get_bit(prefix, bit_pos);
            if (bit == 0) {
                current->left = new_node;
            } else {
                current->right = new_node;
            }
            current = new_node;
            bit_pos += skip_count;
        } else {
            // Normal single-bit navigation
            int bit = get_bit(prefix, bit_pos);
            if (bit == 0) {
                if (current->left == NULL) {
                    current->left = create_patricia_node();
                }
                current = current->left;
            } else {
                if (current->right == NULL) {
                    current->right = create_patricia_node();
                }
                current = current->right;
            }
            bit_pos++;
        }
    }
    
    // Store route at final node
    if (current->route_info == NULL) {
        current->route_info = malloc(sizeof(route_info_t));
    }
    route_info_t *info = (route_info_t*)current->route_info;
    info->prefix = prefix;
    info->prefix_len = prefix_len;
    info->next_hop = next_hop;
    info->interface_id = interface_id;
}

// Patricia trie lookup with skip checking
route_info_t* patricia_lookup(patricia_node_t *root, uint32_t dest_addr) {
    patricia_node_t *current = root;
    route_info_t *best_match = NULL;
    int bit_pos = 0;
    
    while (current != NULL && bit_pos < 32) {
        // Check skip pattern if present
        if (current->skip_len > 0) {
            if (!skip_matches(dest_addr, current->skip_value, 
                            current->bit_position, current->skip_len)) {
                break;  // Skip pattern doesn't match, stop here
            }
            bit_pos = current->bit_position + current->skip_len;
        }
        
        // Update best match if route exists at this node
        if (current->route_info != NULL) {
            best_match = (route_info_t*)current->route_info;
        }
        
        // Navigate to next node
        if (bit_pos < 32) {
            int bit = get_bit(dest_addr, bit_pos);
            current = (bit == 0) ? current->left : current->right;
            bit_pos++;
        } else {
            break;
        }
    }
    
    return best_match;
}
```

## Multi-bit Tries: Trading Memory for Speed

Binary tries make one decision per bit, requiring up to 32 memory accesses for IPv4 lookups. Each memory access costs dozens of CPU cycles. Even with path compression reducing nodes, the serial nature of bit-by-bit traversal creates a performance bottleneck. Multi-bit tries solve this by examining multiple bits simultaneously, dramatically reducing tree height at the cost of increased node size.

A 4-bit stride trie examines four bits at each step, requiring at most 8 steps for IPv4 instead of 32. Each node contains 16 children (2^4 = 16) instead of 2. At each level, we extract four bits from the destination address and use them as an index into the child array. This transformation trades memory for speed: nodes become larger, but we reach decisions faster.

Consider a stride-8 trie that examines entire bytes. Each node contains 256 child pointers, consuming significant memory, but IPv4 lookup requires only four memory accesses—one per byte. This becomes even more critical for IPv6 with its 128-bit addresses. A binary trie requires 128 steps; a stride-8 trie requires only 16.

```c
// Multi-bit trie node (stride-based)
#define STRIDE_BITS 4
#define STRIDE_SIZE (1 << STRIDE_BITS)  // 16 for 4-bit stride

typedef struct multibit_node {
    struct multibit_node *children[STRIDE_SIZE];
    void *route_info[STRIDE_SIZE];  // Each child position can have a route
    uint8_t route_len[STRIDE_SIZE]; // Prefix length for each route
} multibit_node_t;

// Create multi-bit trie node
multibit_node_t* create_multibit_node() {
    multibit_node_t *node = (multibit_node_t*)calloc(1, sizeof(multibit_node_t));
    return node;
}

// Extract stride bits from address
static inline uint32_t get_stride(uint32_t addr, int stride_index) {
    int bit_offset = stride_index * STRIDE_BITS;
    return (addr >> (32 - bit_offset - STRIDE_BITS)) & (STRIDE_SIZE - 1);
}

// Insert into multi-bit trie
void multibit_insert(multibit_node_t *root, uint32_t prefix, uint8_t prefix_len,
                     uint32_t next_hop, uint32_t interface_id, uint32_t metric) {
    multibit_node_t *current = root;
    int bit_pos = 0;
    int stride_index = 0;
    
    // Navigate to the correct level
    while (bit_pos + STRIDE_BITS <= prefix_len) {
        uint32_t stride_bits = get_stride(prefix, stride_index);
        
        if (current->children[stride_bits] == NULL) {
            current->children[stride_bits] = create_multibit_node();
        }
        
        current = current->children[stride_bits];
        bit_pos += STRIDE_BITS;
        stride_index++;
    }
    
    // Handle remaining bits (prefix not aligned to stride)
    if (bit_pos < prefix_len) {
        int remaining_bits = prefix_len - bit_pos;
        uint32_t base_stride = get_stride(prefix << (STRIDE_BITS - remaining_bits), stride_index);
        
        // Prefix expansion: a /17 prefix in a 4-bit stride must cover
        // multiple child entries (bits 17-20 can be anything)
        int expansion_count = 1 << (STRIDE_BITS - remaining_bits);
        
        for (int i = 0; i < expansion_count; i++) {
            uint32_t stride_value = (base_stride & ~(expansion_count - 1)) | i;
            
            if (current->route_info[stride_value] == NULL ||
                current->route_len[stride_value] < prefix_len) {
                
                if (current->route_info[stride_value] == NULL) {
                    current->route_info[stride_value] = malloc(sizeof(route_info_t));
                }
                
                route_info_t *info = (route_info_t*)current->route_info[stride_value];
                info->prefix = prefix;
                info->prefix_len = prefix_len;
                info->next_hop = next_hop;
                info->interface_id = interface_id;
                info->metric = metric;
                current->route_len[stride_value] = prefix_len;
            }
        }
    } else {
        // Prefix exactly aligned to stride boundary
        uint32_t stride_bits = get_stride(prefix, stride_index - 1);
        
        if (current->route_info[stride_bits] == NULL) {
            current->route_info[stride_bits] = malloc(sizeof(route_info_t));
        }
        
        route_info_t *info = (route_info_t*)current->route_info[stride_bits];
        info->prefix = prefix;
        info->prefix_len = prefix_len;
        info->next_hop = next_hop;
        info->interface_id = interface_id;
        info->metric = metric;
    }
}

// Multi-bit trie lookup
route_info_t* multibit_lookup(multibit_node_t *root, uint32_t dest_addr) {
    multibit_node_t *current = root;
    route_info_t *best_match = NULL;
    int stride_index = 0;
    
    // Walk through strides
    while (current != NULL && stride_index < (32 / STRIDE_BITS)) {
        uint32_t stride_bits = get_stride(dest_addr, stride_index);
        
        // Check if there's a route at this position
        if (current->route_info[stride_bits] != NULL) {
            best_match = (route_info_t*)current->route_info[stride_bits];
        }
        
        // Move to next level
        current = current->children[stride_bits];
        stride_index++;
    }
    
    return best_match;
}

// Example with stride-8 (byte-level) trie
#define STRIDE8_BITS 8
#define STRIDE8_SIZE 256

typedef struct stride8_node {
    struct stride8_node *children[STRIDE8_SIZE];
    void *route_info[STRIDE8_SIZE];
} stride8_node_t;

// Lookup in stride-8 trie (4 steps for IPv4)
route_info_t* stride8_lookup(stride8_node_t *root, uint32_t dest_addr) {
    stride8_node_t *current = root;
    route_info_t *best_match = NULL;
    
    // Extract each byte and navigate
    uint8_t byte0 = (dest_addr >> 24) & 0xFF;
    uint8_t byte1 = (dest_addr >> 16) & 0xFF;
    uint8_t byte2 = (dest_addr >> 8) & 0xFF;
    uint8_t byte3 = dest_addr & 0xFF;
    
    // Level 1
    if (current->route_info[byte0] != NULL) {
        best_match = (route_info_t*)current->route_info[byte0];
    }
    if (current->children[byte0] == NULL) return best_match;
    current = current->children[byte0];
    
    // Level 2
    if (current->route_info[byte1] != NULL) {
        best_match = (route_info_t*)current->route_info[byte1];
    }
    if (current->children[byte1] == NULL) return best_match;
    current = current->children[byte1];
    
    // Level 3
    if (current->route_info[byte2] != NULL) {
        best_match = (route_info_t*)current->route_info[byte2];
    }
    if (current->children[byte2] == NULL) return best_match;
    current = current->children[byte2];
    
    // Level 4
    if (current->route_info[byte3] != NULL) {
        best_match = (route_info_t*)current->route_info[byte3];
    }
    
    return best_match;
}
```

## Prefix Expansion and Its Costs

Multi-bit tries introduce a subtle complication when prefix lengths don't align with stride boundaries. Consider inserting 10.1.0.0/17 into a stride-4 trie. After processing the first 16 bits (four strides), we've reached the node for 10.1.0.0/16. Now we need to store information for "the first bit of the third byte is 0, and the remaining bits can be anything."

The solution is prefix expansion: replicate the route across all stride values that match the prefix. For a /17 prefix in a stride-4 world, the remaining 1 bit specifies half of the next stride's 16 possibilities. We store the route information in eight child positions: 0000, 0001, 0010, 0011, 0100, 0101, 0110, 0111. All these values have the first bit as 0, matching our /17 prefix.

This expansion multiplies memory consumption. A /25 prefix misaligned with 8-bit strides requires storing the route in 128 child positions. Routing tables with many such prefixes can explode in size. The trade-off becomes stark: binary tries use minimal memory but require many memory accesses; stride-8 tries require few memory accesses but consume massive memory due to prefix expansion. Most production implementations choose intermediate strides like 4 or 6 bits, balancing these opposing forces.

```c
// Demonstration of prefix expansion overhead
void demonstrate_prefix_expansion() {
    multibit_node_t *root = create_multibit_node();
    
    // Insert /16 prefix (aligned to stride) - no expansion needed
    uint32_t prefix_aligned = (10U << 24) | (1U << 16);
    multibit_insert(root, prefix_aligned, 16, 0, 1, 1);
    
    // Insert /17 prefix (misaligned) - requires expansion
    uint32_t prefix_misaligned = (10U << 24) | (1U << 16);
    multibit_insert(root, prefix_misaligned, 17, 0, 2, 1);
    
    // Count how many route entries were created
    multibit_node_t *node = root;
    
    // Navigate to 10.1.x.x
    uint32_t stride0 = get_stride(prefix_aligned, 0);  // First 4 bits of 10
    uint32_t stride1 = get_stride(prefix_aligned, 1);  // Next 4 bits
    uint32_t stride2 = get_stride(prefix_aligned, 2);  // First 4 bits of 1
    uint32_t stride3 = get_stride(prefix_aligned, 3);  // Next 4 bits of 1
    
    node = root->children[stride0];
    if (node) node = node->children[stride1];
    if (node) node = node->children[stride2];
    if (node) node = node->children[stride3];
    
    if (node) {
        int expansion_count = 0;
        for (int i = 0; i < STRIDE_SIZE; i++) {
            if (node->route_info[i] != NULL) {
                expansion_count++;
            }
        }
        printf("Prefix expansion created %d route entries\n", expansion_count);
        printf("Memory overhead: %dx compared to single entry\n", expansion_count);
    }
}
```

## Level Compression for IPv6

IPv6's 128-bit addresses make naive trie implementations completely impractical. A binary trie would require 128 levels; even a stride-8 trie needs 16 levels. However, real-world IPv6 routing tables exhibit strong structure. Most prefixes are /32, /48, or /64, corresponding to allocations to RIRs, ISPs, and end sites. The intermediate space remains sparse.

Level compression exploits this sparsity by using different stride sizes at different tree levels. The root might use stride-16, examining the first two bytes of the address in a single step. This handles the /32 prefixes common at the RIR level efficiently. Subsequent levels might use stride-8 or stride-4, adjusting to the prefix distribution at each depth. This variable-stride approach minimizes both memory consumption and lookup steps by optimizing each level independently.

```c
// Multi-level stride trie for IPv6
typedef struct ipv6_trie_level {
    int stride_bits;           // Stride size at this level
    int max_children;          // 2^stride_bits
} ipv6_trie_level_t;

// Variable-stride IPv6 trie configuration
// Level 0: stride-16 (0-16 bits) - handles RIR allocations (/32)
// Level 1: stride-16 (16-32 bits)
// Level 2: stride-16 (32-48 bits) - handles ISP allocations (/48)
// Level 3: stride-16 (48-64 bits) - handles end site allocations (/64)
// Levels 4-7: stride-16 (64-128 bits) - interface IDs
ipv6_trie_level_t ipv6_levels[] = {
    {16, 65536},  // Level 0: bytes 0-1
    {16, 65536},  // Level 1: bytes 2-3
    {16, 65536},  // Level 2: bytes 4-5
    {16, 65536},  // Level 3: bytes 6-7
    {16, 65536},  // Level 4: bytes 8-9
    {16, 65536},  // Level 5: bytes 10-11
    {16, 65536},  // Level 6: bytes 12-13
    {16, 65536},  // Level 7: bytes 14-15
};

typedef struct ipv6_node {
    struct ipv6_node **children;   // Dynamically sized based on level
    void **route_info;             // Route at each child position
    int level;                     // Which level this node is at
} ipv6_node_t;

// Create IPv6 trie node for specific level
ipv6_node_t* create_ipv6_node(int level) {
    ipv6_node_t *node = (ipv6_node_t*)calloc(1, sizeof(ipv6_node_t));
    node->level = level;
    
    int max_children = ipv6_levels[level].max_children;
    node->children = (ipv6_node_t**)calloc(max_children, sizeof(ipv6_node_t*));
    node->route_info = (void**)calloc(max_children, sizeof(void*));
    
    return node;
}

// Extract stride value for IPv6 address at given level
uint32_t get_ipv6_stride(uint8_t *addr, int level) {
    int stride_bits = ipv6_levels[level].stride_bits;
    int byte_offset = 0;
    
    for (int i = 0; i < level; i++) {
        byte_offset += ipv6_levels[i].stride_bits / 8;
    }
    
    if (stride_bits == 16) {
        return (addr[byte_offset] << 8) | addr[byte_offset + 1];
    }
    // Other stride sizes would be handled here
    return 0;
}

// Lookup in IPv6 variable-stride trie
route_info_t* ipv6_lookup(ipv6_node_t *root, uint8_t *dest_addr) {
    ipv6_node_t *current = root;
    route_info_t *best_match = NULL;
    int level = 0;
    
    while (current != NULL && level < 8) {
        uint32_t stride_value = get_ipv6_stride(dest_addr, level);
        
        // Check for route at this position
        if (current->route_info[stride_value] != NULL) {
            best_match = (route_info_t*)current->route_info[stride_value];
        }
        
        // Move to next level
        current = current->children[stride_value];
        level++;
    }
    
    return best_match;
}
```

## Practical Optimizations in Production Systems

Real routing systems implement numerous additional optimizations beyond the basic trie algorithms. Route caching maintains a hash table of recently looked-up addresses, bypassing the trie entirely for frequent destinations. Since most traffic concentrates on a small number of destinations—web servers, DNS servers, popular services—a cache with just a few thousand entries can achieve hit rates above 95 percent.

Leaf pushing reorganizes the trie by copying route information downward to reduce tree height for common lookups. If a /8 prefix has no more specific children for a large portion of its address space, we can push that route down to deeper levels, allowing faster lookups that terminate earlier. This trades memory for speed, creating duplicate route entries throughout the tree.

```c
// Route cache with LRU eviction
#define ROUTE_CACHE_SIZE 4096

typedef struct route_cache_entry {
    uint32_t dest_addr;
    route_info_t *route;
    uint64_t access_count;
    struct route_cache_entry *lru_prev;
    struct route_cache_entry *lru_next;
} route_cache_entry_t;

typedef struct route_cache {
    route_cache_entry_t **hash_table;
    route_cache_entry_t *lru_head;
    route_cache_entry_t *lru_tail;
    uint32_t size;
    uint64_t hits;
    uint64_t misses;
} route_cache_t;

// Simple hash for cache
uint32_t cache_hash(uint32_t addr) {
    addr ^= addr >> 16;
    addr *= 0x85ebca6b;
    addr ^= addr >> 13;
    return addr % ROUTE_CACHE_SIZE;
}

// Create route cache
route_cache_t* create_route_cache() {
    route_cache_t *cache = (route_cache_t*)calloc(1, sizeof(route_cache_t));
    cache->hash_table = (route_cache_entry_t**)calloc(ROUTE_CACHE_SIZE, 
                                                       sizeof(route_cache_entry_t*));
    return cache;
}

// Move entry to head of LRU list (most recently used)
void lru_touch(route_cache_t *cache, route_cache_entry_t *entry) {
    if (cache->lru_head == entry) return;  // Already at head
    
    // Remove from current position
    if (entry->lru_prev) entry->lru_prev->lru_next = entry->lru_next;
    if (entry->lru_next) entry->lru_next->lru_prev = entry->lru_prev;
    if (cache->lru_tail == entry) cache->lru_tail = entry->lru_prev;
    
    // Insert at head
    entry->lru_prev = NULL;
    entry->lru_next = cache->lru_head;
    if (cache->lru_head) cache->lru_head->lru_prev = entry;
    cache->lru_head = entry;
    if (cache->lru_tail == NULL) cache->lru_tail = entry;
}

// Lookup with caching
route_info_t* cached_lookup(route_cache_t *cache, trie_node_t *trie_root, 
                           uint32_t dest_addr) {
    uint32_t hash = cache_hash(dest_addr);
    route_cache_entry_t *entry = cache->hash_table[hash];
    
    // Check cache
    while (entry != NULL) {
        if (entry->dest_addr == dest_addr) {
            cache->hits++;
            entry->access_count++;
            lru_touch(cache, entry);
            return entry->route;
        }
        entry = entry->lru_next;
    }
    
    // Cache miss - do full trie lookup
    cache->misses++;
    route_info_t *route = trie_lookup(trie_root, dest_addr);
    
    if (route != NULL) {
        // Add to cache
        route_cache_entry_t *new_entry = (route_cache_entry_t*)malloc(sizeof(route_cache_entry_t));
        new_entry->dest_addr = dest_addr;
        new_entry->route = route;
        new_entry->access_count = 1;
        
        // Evict LRU entry if cache full
        if (cache->size >= ROUTE_CACHE_SIZE) {
            route_cache_entry_t *lru = cache->lru_tail;
            if (lru) {
                // Remove from hash table
                uint32_t lru_hash = cache_hash(lru->dest_addr);
                route_cache_entry_t **slot = &cache->hash_table[lru_hash];
                while (*slot != lru) slot = &(*slot)->lru_next;
                *slot = NULL;
                
                // Remove from LRU list
                if (lru->lru_prev) lru->lru_prev->lru_next = NULL;
                cache->lru_tail = lru->lru_prev;
                free(lru);
                cache->size--;
            }
        }
        
        // Insert new entry
        new_entry->lru_next = cache->hash_table[hash];
        cache->hash_table[hash] = new_entry;
        lru_touch(cache, new_entry);
        cache->size++;
    }
    
    return route;
}

// Display cache statistics
void print_cache_stats(route_cache_t *cache) {
    uint64_t total = cache->hits + cache->misses;
    double hit_rate = (double)cache->hits / total * 100.0;
    
    printf("Cache Statistics:\n");
    printf("  Size: %u entries\n", cache->size);
    printf("  Hits: %lu\n", cache->hits);
    printf("  Misses: %lu\n", cache->misses);
    printf("  Hit rate: %.2f%%\n", hit_rate);
}
```

## Hardware Implementations and TCAM

Software tries running on general-purpose CPUs achieve lookup times measured in hundreds of nanoseconds. Hardware implementations in ASICs reach single-digit nanoseconds by implementing the trie structure in dedicated circuits with massive parallelism. Ternary Content Addressable Memory (TCAM) offers an even more radical approach: hardware that searches all entries simultaneously.

TCAM stores each route as a combination of value bits and mask bits. For 10.1.0.0/16, the TCAM entry contains value 10.1.0.0 and mask 255.255.0.0. When you present a destination address to the TCAM, it compares that address against every stored entry in parallel, returning all matches. Priority encoding selects the longest prefix among the matches. This achieves constant-time lookup regardless of table size, at the cost of expensive, power-hungry hardware. A TCAM chip capable of storing 500,000 routes costs thousands of dollars and consumes 50 watts or more.

```c
// Software simulation of TCAM behavior
#define MAX_TCAM_ENTRIES 1000

typedef struct tcam_entry {
    uint32_t value;       // Route prefix
    uint32_t mask;        // Prefix mask
    uint8_t prefix_len;   // For priority encoding
    void *route_info;
    int valid;
} tcam_entry_t;

typedef struct tcam_table {
    tcam_entry_t entries[MAX_TCAM_ENTRIES];
    int entry_count;
} tcam_table_t;

// Create TCAM table
tcam_table_t* create_tcam() {
    tcam_table_t *tcam = (tcam_table_t*)calloc(1, sizeof(tcam_table_t));
    return tcam;
}

// Compute mask from prefix length
uint32_t prefix_len_to_mask(uint8_t prefix_len) {
    if (prefix_len == 0) return 0;
    return ~((1U << (32 - prefix_len)) - 1);
}

// Insert into TCAM
void tcam_insert(tcam_table_t *tcam, uint32_t prefix, uint8_t prefix_len,
                 uint32_t next_hop, uint32_t interface_id) {
    if (tcam->entry_count >= MAX_TCAM_ENTRIES) return;
    
    tcam_entry_t *entry = &tcam->entries[tcam->entry_count];
    entry->value = prefix;
    entry->mask = prefix_len_to_mask(prefix_len);
    entry->prefix_len = prefix_len;
    entry->valid = 1;
    
    entry->route_info = malloc(sizeof(route_info_t));
    route_info_t *info = (route_info_t*)entry->route_info;
    info->prefix = prefix;
    info->prefix_len = prefix_len;
    info->next_hop = next_hop;
    info->interface_id = interface_id;
    
    tcam->entry_count++;
}

// TCAM lookup - parallel search simulation
route_info_t* tcam_lookup(tcam_table_t *tcam, uint32_t dest_addr) {
    route_info_t *best_match = NULL;
    uint8_t longest_prefix = 0;
    
    // In hardware, this would happen in parallel across all entries
    for (int i = 0; i < tcam->entry_count; i++) {
        tcam_entry_t *entry = &tcam->entries[i];
        
        if (!entry->valid) continue;
        
        // Check if dest_addr matches this entry
        if ((dest_addr & entry->mask) == (entry->value & entry->mask)) {
            // Match found - check if it's more specific
            if (entry->prefix_len > longest_prefix) {
                longest_prefix = entry->prefix_len;
                best_match = (route_info_t*)entry->route_info;
            }
        }
    }
    
    return best_match;
}

// Benchmark TCAM vs Trie
void benchmark_lookup_methods() {
    // Create and populate both structures
    trie_node_t *trie_root = create_trie_node();
    tcam_table_t *tcam = create_tcam();
    
    // Insert 1000 random routes
    for (int i = 0; i < 1000; i++) {
        uint32_t prefix = rand();
        uint8_t prefix_len = 16 + (rand() % 16);  // /16 to /31
        uint32_t next_hop = rand();
        
        trie_insert(trie_root, prefix, prefix_len, next_hop, 1, 1);
        tcam_insert(tcam, prefix, prefix_len, next_hop, 1);
    }
    
    // Benchmark trie lookups
    clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        uint32_t dest = rand();
        route_info_t *route = trie_lookup(trie_root, dest);
        (void)route;
    }
    clock_t end = clock();
    double trie_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    // Benchmark TCAM lookups
    start = clock();
    for (int i = 0; i < 100000; i++) {
        uint32_t dest = rand();
        route_info_t *route = tcam_lookup(tcam, dest);
        (void)route;
    }
    end = clock();
    double tcam_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Trie lookup: %.2f ns per lookup\n", 
           (trie_time / 100000) * 1000000000);
    printf("TCAM lookup: %.2f ns per lookup\n", 
           (tcam_time / 100000) * 1000000000);
}
```

## Route Updates and Dynamic Modifications

Unlike static data structures, routing tables change constantly. BGP updates arrive announcing new prefixes or withdrawing old ones. OSPF recalculates routes when link costs change. The trie must support efficient insertions and deletions without disrupting ongoing lookups. In a multi-threaded router, some threads perform lookups while others update routes.

Insertion allocates new nodes as needed while traversing the trie, then stores the route information at the appropriate leaf. Deletion must be more careful. Simply removing a leaf node works only if no other routes depend on the path to that node. If deleting 10.1.2.0/24 leaves its parent node with no other children and no route of its own, we should delete that parent too, recursively pruning the tree. This cleanup prevents memory leaks from accumulating dead branches.

```c
// Delete a route from the trie
int trie_delete(trie_node_t *root, uint32_t prefix, uint8_t prefix_len) {
    trie_node_t *path[33];  // Store path for cleanup
    int path_len = 0;
    trie_node_t *current = root;
    path[path_len++] = current;
    
    // Navigate to the target node
    for (int i = 0; i < prefix_len; i++) {
        int bit = get_bit(prefix, i);
        
        if (bit == 0) {
            if (current->left == NULL) return 0;  // Route not found
            current = current->left;
        } else {
            if (current->right == NULL) return 0;  // Route not found
            current = current->right;
        }
        
        path[path_len++] = current;
    }
    
    // Check if route exists at this node
    if (current->route_info == NULL) {
        return 0;  // No route at this node
    }
    
    // Delete the route info
    free(current->route_info);
    current->route_info = NULL;
    current->prefix_len = 0;
    
    // Cleanup: walk back up and prune empty nodes
    for (int i = path_len - 1; i > 0; i--) {
        trie_node_t *node = path[i];
        
        // Keep node if it has a route or has children
        if (node->route_info != NULL || 
            node->left != NULL || 
            node->right != NULL) {
            break;
        }
        
        // Node is empty, remove it from parent
        trie_node_t *parent = path[i - 1];
        if (parent->left == node) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
        
        free(node);
    }
    
    return 1;  // Success
}

// Update a route (change next hop or metric)
void trie_update(trie_node_t *root, uint32_t prefix, uint8_t prefix_len,
                 uint32_t new_next_hop, uint32_t new_metric) {
    trie_node_t *current = root;
    
    // Navigate to target node
    for (int i = 0; i < prefix_len; i++) {
        int bit = get_bit(prefix, i);
        current = (bit == 0) ? current->left : current->right;
        if (current == NULL) return;  // Route not found
    }
    
    // Update route if it exists
    if (current->route_info != NULL) {
        route_info_t *info = (route_info_t*)current->route_info;
        info->next_hop = new_next_hop;
        info->metric = new_metric;
    }
}

// Thread-safe update with read-copy-update (RCU) pattern
typedef struct rcu_trie {
    trie_node_t *root;
    pthread_rwlock_t lock;
} rcu_trie_t;

// Deep copy a trie branch for safe modification
trie_node_t* copy_trie_branch(trie_node_t *node) {
    if (node == NULL) return NULL;
    
    trie_node_t *copy = create_trie_node();
    
    if (node->route_info != NULL) {
        copy->route_info = malloc(sizeof(route_info_t));
        memcpy(copy->route_info, node->route_info, sizeof(route_info_t));
        copy->prefix_len = node->prefix_len;
    }
    
    // Note: shallow copy of children for now
    // Full RCU would copy entire branch
    copy->left = node->left;
    copy->right = node->right;
    
    return copy;
}

// Safe update in multi-threaded environment
void rcu_trie_update(rcu_trie_t *rcu_trie, uint32_t prefix, uint8_t prefix_len,
                     uint32_t next_hop, uint32_t interface_id, uint32_t metric) {
    pthread_rwlock_wrlock(&rcu_trie->lock);
    
    // Create modified copy of affected branch
    trie_node_t *new_root = copy_trie_branch(rcu_trie->root);
    
    // Modify the copy
    trie_insert(new_root, prefix, prefix_len, next_hop, interface_id, metric);
    
    // Atomic pointer swap
    trie_node_t *old_root = rcu_trie->root;
    rcu_trie->root = new_root;
    
    pthread_rwlock_unlock(&rcu_trie->lock);
    
    // Old root can be freed after grace period
    // (when all readers have moved to new version)
    // In production, this would use proper RCU synchronization
}
```

## Conclusion

Tries solve the longest prefix matching problem that hash tables cannot touch. By organizing routes in a tree structure that mirrors the hierarchical nature of IP addresses, tries enable efficient lookups that naturally find the most specific matching prefix. The progression from binary tries to path-compressed Patricia tries to multi-bit stride tries illustrates the continuous trade-off between memory consumption and lookup speed that dominates routing table design.

Production routers combine multiple techniques: multi-bit tries for the main lookup structure, route caches for frequent destinations, and sometimes TCAM for performance-critical prefix ranges. Understanding these data structures deeply matters because routing performance directly determines how fast packets flow through networks. A poorly implemented trie can throttle a router's forwarding capacity by orders of magnitude. Master tries, and you understand how modern routers achieve the impossible: making millions of complex routing decisions per second while maintaining tables with hundreds of thousands of prefixes.

# Trees in Networking: RB-Trees and B-Trees

## Why Balanced Trees Matter in Network Systems

Network devices maintain countless time-ordered operations: TCP retransmission timers waiting to fire, ARP entries scheduled for refresh, routing protocol hello packets due for transmission, flow entries marked for expiration. A naive approach stores these events in an unsorted list, checking every entry periodically to find expired items. With thousands of active timers, this linear scan consumes milliseconds of CPU time every second—time stolen from packet forwarding.

Balanced trees solve this by maintaining events in sorted order while guaranteeing logarithmic time for insertions, deletions, and finding the minimum. When a new timer needs scheduling, inserting it into a tree of 10,000 entries requires approximately 14 comparisons instead of potentially 10,000. Finding the next timer to expire becomes a simple tree traversal to the leftmost node rather than examining every entry. This transformation from linear to logarithmic time complexity makes the difference between a router that processes packets at line rate and one that chokes on its own bookkeeping.

## Red-Black Trees: The Timer Wheel Alternative

Red-Black trees represent a self-balancing binary search tree where each node carries a color—red or black—along with its data. These colors enforce balance invariants through a set of rules that prevent the tree from degenerating into a linked list. No path from root to leaf ever exceeds twice the length of any other path, guaranteeing O(log n) operations. This predictable performance makes RB-trees ideal for the kernel timer subsystems that network stacks depend on.

The balancing rules seem arbitrary at first glance but enforce a mathematical relationship between tree height and node count. A red node cannot have a red child. Every path from root to leaf must contain the same number of black nodes. The root must be black. These simple rules ensure the tree remains approximately balanced after every insertion or deletion. When an operation threatens to violate the rules, the tree performs rotations and recoloring to restore them.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Red-Black tree node
typedef enum { RED, BLACK } rb_color_t;

typedef struct rb_node {
    uint64_t key;              // Expiration time (microseconds)
    void *data;                // Timer callback data
    rb_color_t color;
    struct rb_node *left;
    struct rb_node *right;
    struct rb_node *parent;
} rb_node_t;

typedef struct rb_tree {
    rb_node_t *root;
    rb_node_t *nil;            // Sentinel node
    uint32_t count;
} rb_tree_t;

// Timer callback function type
typedef void (*timer_callback_t)(void *data);

// Timer structure stored in RB-tree
typedef struct timer_entry {
    timer_callback_t callback;
    void *user_data;
    uint64_t expiration;       // Absolute expiration time
    int canceled;
} timer_entry_t;

// Create RB-tree
rb_tree_t* create_rb_tree() {
    rb_tree_t *tree = (rb_tree_t*)malloc(sizeof(rb_tree_t));
    
    // Create sentinel nil node (black)
    tree->nil = (rb_node_t*)malloc(sizeof(rb_node_t));
    tree->nil->color = BLACK;
    tree->nil->left = tree->nil->right = tree->nil->parent = tree->nil;
    
    tree->root = tree->nil;
    tree->count = 0;
    return tree;
}

// Create a new node
rb_node_t* create_rb_node(rb_tree_t *tree, uint64_t key, void *data) {
    rb_node_t *node = (rb_node_t*)malloc(sizeof(rb_node_t));
    node->key = key;
    node->data = data;
    node->color = RED;  // New nodes start red
    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;
    return node;
}

// Left rotation
void rb_left_rotate(rb_tree_t *tree, rb_node_t *x) {
    rb_node_t *y = x->right;
    
    // Turn y's left subtree into x's right subtree
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }
    
    // Link x's parent to y
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    
    // Put x on y's left
    y->left = x;
    x->parent = y;
}

// Right rotation
void rb_right_rotate(rb_tree_t *tree, rb_node_t *y) {
    rb_node_t *x = y->left;
    
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;
    }
    
    x->parent = y->parent;
    if (y->parent == tree->nil) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    
    x->right = y;
    y->parent = x;
}

// Fix violations after insertion
void rb_insert_fixup(rb_tree_t *tree, rb_node_t *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            rb_node_t *y = z->parent->parent->right;  // Uncle
            
            if (y->color == RED) {
                // Case 1: Uncle is red - recolor
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Case 2: z is right child - left rotate
                    z = z->parent;
                    rb_left_rotate(tree, z);
                }
                // Case 3: z is left child - recolor and right rotate
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_right_rotate(tree, z->parent->parent);
            }
        } else {
            // Mirror cases
            rb_node_t *y = z->parent->parent->left;  // Uncle
            
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rb_right_rotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// Insert a node
void rb_insert(rb_tree_t *tree, uint64_t key, void *data) {
    rb_node_t *z = create_rb_node(tree, key, data);
    rb_node_t *y = tree->nil;
    rb_node_t *x = tree->root;
    
    // Find insertion position
    while (x != tree->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    
    // Insert node
    z->parent = y;
    if (y == tree->nil) {
        tree->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    
    tree->count++;
    
    // Fix violations
    rb_insert_fixup(tree, z);
}

// Find minimum node in subtree
rb_node_t* rb_minimum(rb_tree_t *tree, rb_node_t *x) {
    while (x->left != tree->nil) {
        x = x->left;
    }
    return x;
}

// Replace subtree rooted at u with subtree rooted at v
void rb_transplant(rb_tree_t *tree, rb_node_t *u, rb_node_t *v) {
    if (u->parent == tree->nil) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// Fix violations after deletion
void rb_delete_fixup(rb_tree_t *tree, rb_node_t *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            rb_node_t *w = x->parent->right;  // Sibling
            
            if (w->color == RED) {
                // Case 1: Sibling is red
                w->color = BLACK;
                x->parent->color = RED;
                rb_left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            
            if (w->left->color == BLACK && w->right->color == BLACK) {
                // Case 2: Sibling's children are black
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    // Case 3: Sibling's right child is black
                    w->left->color = BLACK;
                    w->color = RED;
                    rb_right_rotate(tree, w);
                    w = x->parent->right;
                }
                // Case 4: Sibling's right child is red
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rb_left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            // Mirror cases
            rb_node_t *w = x->parent->left;
            
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rb_right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rb_left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rb_right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Delete a node
void rb_delete(rb_tree_t *tree, rb_node_t *z) {
    rb_node_t *y = z;
    rb_node_t *x;
    rb_color_t y_original_color = y->color;
    
    if (z->left == tree->nil) {
        x = z->right;
        rb_transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        rb_transplant(tree, z, z->left);
    } else {
        y = rb_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;
        
        if (y->parent == z) {
            x->parent = y;
        } else {
            rb_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        
        rb_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    tree->count--;
    
    if (y_original_color == BLACK) {
        rb_delete_fixup(tree, x);
    }
    
    free(z);
}

// Get current time in microseconds
uint64_t get_time_usec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}

// Timer subsystem using RB-tree
typedef struct timer_system {
    rb_tree_t *timers;
    uint64_t next_timer_id;
} timer_system_t;

// Create timer system
timer_system_t* create_timer_system() {
    timer_system_t *ts = (timer_system_t*)malloc(sizeof(timer_system_t));
    ts->timers = create_rb_tree();
    ts->next_timer_id = 1;
    return ts;
}

// Schedule a timer
uint64_t schedule_timer(timer_system_t *ts, uint64_t delay_usec,
                       timer_callback_t callback, void *user_data) {
    uint64_t expiration = get_time_usec() + delay_usec;
    
    timer_entry_t *entry = (timer_entry_t*)malloc(sizeof(timer_entry_t));
    entry->callback = callback;
    entry->user_data = user_data;
    entry->expiration = expiration;
    entry->canceled = 0;
    
    rb_insert(ts->timers, expiration, entry);
    
    return ts->next_timer_id++;
}

// Process expired timers
int process_expired_timers(timer_system_t *ts) {
    uint64_t now = get_time_usec();
    int processed = 0;
    
    while (ts->timers->root != ts->timers->nil) {
        // Get minimum (earliest expiring timer)
        rb_node_t *min = rb_minimum(ts->timers, ts->timers->root);
        
        if (min->key > now) {
            break;  // No more expired timers
        }
        
        // Timer has expired
        timer_entry_t *entry = (timer_entry_t*)min->data;
        
        if (!entry->canceled) {
            // Execute callback
            entry->callback(entry->user_data);
            processed++;
        }
        
        // Remove from tree
        rb_delete(ts->timers, min);
        free(entry);
    }
    
    return processed;
}

// Example timer callbacks
void tcp_retransmit_callback(void *data) {
    int connection_id = *(int*)data;
    printf("TCP retransmit timer fired for connection %d\n", connection_id);
}

void arp_refresh_callback(void *data) {
    uint32_t ip = *(uint32_t*)data;
    printf("ARP refresh for %u.%u.%u.%u\n",
           (ip >> 24) & 0xFF, (ip >> 16) & 0xFF,
           (ip >> 8) & 0xFF, ip & 0xFF);
}

// Demonstration
void demonstrate_timer_system() {
    timer_system_t *ts = create_timer_system();
    
    // Schedule various timers
    int conn1 = 101;
    schedule_timer(ts, 50000, tcp_retransmit_callback, &conn1);  // 50ms
    
    int conn2 = 102;
    schedule_timer(ts, 100000, tcp_retransmit_callback, &conn2); // 100ms
    
    uint32_t ip = (192 << 24) | (168 << 16) | (1 << 8) | 1;
    schedule_timer(ts, 30000, arp_refresh_callback, &ip);        // 30ms
    
    printf("Scheduled 3 timers. Tree contains %u entries.\n", ts->timers->count);
    
    // Simulate time passing
    usleep(60000);  // Sleep 60ms
    
    printf("Processing expired timers...\n");
    int processed = process_expired_timers(ts);
    printf("Processed %d timers. %u remaining.\n", processed, ts->timers->count);
}
```

## Rotations and Rebalancing Operations

The magic of RB-trees lies in rotations—local tree restructuring operations that preserve the binary search tree property while changing the tree's shape. A left rotation on node X makes X's right child the new subtree root, moving X down to become the left child. The operation touches only a handful of pointers and completes in constant time regardless of tree size.

When insertion creates two consecutive red nodes, violating the red-black property, the tree performs rotations and recoloring to restore balance. The algorithm examines the uncle node—the sibling of the new node's parent. If the uncle is red, recoloring suffices: flip the parent and uncle to black and the grandparent to red. If the uncle is black, rotations become necessary. The specific rotation depends on whether the new node forms a left-left, left-right, right-left, or right-right chain with its ancestors.

Deletion proves more complex, potentially requiring multiple rotations as fixes propagate up the tree. When removing a black node, the tree's black-height property breaks. The algorithm restores it by restructuring the sibling's subtree, potentially converting red nodes to black or performing rotations to borrow black-height from adjacent branches. Despite this complexity, the algorithm guarantees completion within O(log n) operations, maintaining the tree's performance characteristics.

## Timer Systems in Network Stacks

Linux kernel network stack uses RB-trees extensively for TCP timer management. Each TCP connection maintains multiple timers: retransmission timeout, delayed ACK, keepalive, and time-wait expiration. With thousands of concurrent connections, the system schedules tens of thousands of timers. The RB-tree implementation allows the kernel to find the next expiring timer in O(log n) time—a simple traversal to the leftmost node—and process it efficiently.

The alternative, a timer wheel, buckets timers by expiration time but suffers from resolution limitations. A timer wheel with microsecond precision requires millions of buckets, consuming excessive memory. A wheel with millisecond precision cannot accurately schedule sub-millisecond events. RB-trees provide unlimited precision, scheduling timers for any future time with consistent logarithmic performance.

```c
// Complete TCP timer example
typedef enum {
    TCP_TIMER_RETRANSMIT,
    TCP_TIMER_DELACK,
    TCP_TIMER_KEEPALIVE,
    TCP_TIMER_TIMEWAIT
} tcp_timer_type_t;

typedef struct tcp_connection {
    int conn_id;
    uint32_t local_ip;
    uint32_t remote_ip;
    uint16_t local_port;
    uint16_t remote_port;
    
    // Timer tracking
    rb_node_t *retransmit_timer;
    rb_node_t *delack_timer;
    rb_node_t *keepalive_timer;
    rb_node_t *timewait_timer;
} tcp_connection_t;

typedef struct tcp_timer_data {
    tcp_connection_t *conn;
    tcp_timer_type_t type;
} tcp_timer_data_t;

// TCP timer handler
void tcp_timer_handler(void *data) {
    tcp_timer_data_t *timer_data = (tcp_timer_data_t*)data;
    tcp_connection_t *conn = timer_data->conn;
    
    switch (timer_data->type) {
        case TCP_TIMER_RETRANSMIT:
            printf("Conn %d: Retransmitting unacked data\n", conn->conn_id);
            // Trigger retransmission logic
            break;
            
        case TCP_TIMER_DELACK:
            printf("Conn %d: Sending delayed ACK\n", conn->conn_id);
            // Send accumulated ACK
            break;
            
        case TCP_TIMER_KEEPALIVE:
            printf("Conn %d: Sending keepalive probe\n", conn->conn_id);
            // Send keepalive packet
            break;
            
        case TCP_TIMER_TIMEWAIT:
            printf("Conn %d: TIME_WAIT expired, closing\n", conn->conn_id);
            // Close connection
            break;
    }
}

// Set TCP retransmit timer
void tcp_set_retransmit_timer(timer_system_t *ts, tcp_connection_t *conn, 
                               uint64_t rto_usec) {
    tcp_timer_data_t *data = (tcp_timer_data_t*)malloc(sizeof(tcp_timer_data_t));
    data->conn = conn;
    data->type = TCP_TIMER_RETRANSMIT;
    
    schedule_timer(ts, rto_usec, tcp_timer_handler, data);
}

// Simulate TCP connection with timers
void simulate_tcp_timers() {
    timer_system_t *ts = create_timer_system();
    
    // Create TCP connection
    tcp_connection_t conn = {
        .conn_id = 12345,
        .local_ip = (192 << 24) | (168 << 16) | (1 << 8) | 100,
        .remote_ip = (10 << 24) | (0 << 16) | (0 << 8) | 1,
        .local_port = 54321,
        .remote_port = 80
    };
    
    // Schedule timers
    tcp_set_retransmit_timer(ts, &conn, 200000);  // 200ms RTO
    
    tcp_timer_data_t *delack_data = malloc(sizeof(tcp_timer_data_t));
    delack_data->conn = &conn;
    delack_data->type = TCP_TIMER_DELACK;
    schedule_timer(ts, 40000, tcp_timer_handler, delack_data);  // 40ms delayed ACK
    
    tcp_timer_data_t *keepalive_data = malloc(sizeof(tcp_timer_data_t));
    keepalive_data->conn = &conn;
    keepalive_data->type = TCP_TIMER_KEEPALIVE;
    schedule_timer(ts, 7200000000, tcp_timer_handler, keepalive_data);  // 2 hours
    
    printf("TCP connection established with timers\n");
    printf("RB-tree contains %u timers\n", ts->timers->count);
    
    // Process timers as they expire
    for (int i = 0; i < 5; i++) {
        usleep(50000);  // 50ms
        int processed = process_expired_timers(ts);
        if (processed > 0) {
            printf("Tick %d: Processed %d timers\n", i, processed);
        }
    }
}
```

## B-Trees: Optimizing for Disk Access

While RB-trees excel for in-memory operations, B-trees dominate when data resides on disk. The fundamental difference lies in access patterns: RAM allows fetching a single byte with the same cost as fetching thousands, while disks penalize small random accesses severely. Reading one byte from disk might require a 5-millisecond seek. Reading a 4-kilobyte block requires the same seek plus a negligible sequential transfer time. B-trees exploit this by storing hundreds of keys per node, reducing tree height and minimizing disk accesses.

A B-tree node contains multiple keys sorted in order, with each key separating two child pointers. A node storing 100 keys has 101 child pointers. Searching within a node uses binary search across those 100 keys—fast because they fit in a single disk block and thus a single read operation. Finding a key in a B-tree of one million entries requires at most log₁₀₀(1,000,000) = 3 disk accesses. A binary tree would require log₂(1,000,000) ≈ 20 disk accesses. This reduction from 20 to 3 disk operations transforms query latency from 100 milliseconds to 15 milliseconds.

```c
// B-tree node structure
#define BTREE_ORDER 5  // Maximum children per node (order 5 = max 4 keys)
#define MAX_KEYS (BTREE_ORDER - 1)
#define MIN_KEYS ((BTREE_ORDER + 1) / 2 - 1)

typedef struct btree_node {
    int num_keys;                          // Current number of keys
    uint32_t keys[MAX_KEYS];               // Keys in sorted order
    void *values[MAX_KEYS];                // Associated values
    struct btree_node *children[BTREE_ORDER];  // Child pointers
    int is_leaf;                           // 1 if leaf node
    struct btree_node *parent;             // Parent pointer
} btree_node_t;

typedef struct btree {
    btree_node_t *root;
    int height;
    int count;
} btree_t;

// Create B-tree
btree_t* create_btree() {
    btree_t *tree = (btree_t*)malloc(sizeof(btree_t));
    tree->root = NULL;
    tree->height = 0;
    tree->count = 0;
    return tree;
}

// Create B-tree node
btree_node_t* create_btree_node(int is_leaf) {
    btree_node_t *node = (btree_node_t*)malloc(sizeof(btree_node_t));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    node->parent = NULL;
    
    for (int i = 0; i < BTREE_ORDER; i++) {
        node->children[i] = NULL;
    }
    
    return node;
}

// Search for key in B-tree
void* btree_search(btree_node_t *node, uint32_t key) {
    if (node == NULL) return NULL;
    
    // Binary search within node
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }
    
    // Found exact match
    if (i < node->num_keys && key == node->keys[i]) {
        return node->values[i];
    }
    
    // If leaf, key not found
    if (node->is_leaf) {
        return NULL;
    }
    
    // Recurse to appropriate child
    return btree_search(node->children[i], key);
}

// Split a full child node
void btree_split_child(btree_node_t *parent, int child_index) {
    btree_node_t *full_child = parent->children[child_index];
    btree_node_t *new_child = create_btree_node(full_child->is_leaf);
    
    int mid = MAX_KEYS / 2;
    new_child->num_keys = MAX_KEYS - mid - 1;
    
    // Copy upper half of keys to new node
    for (int i = 0; i < new_child->num_keys; i++) {
        new_child->keys[i] = full_child->keys[mid + 1 + i];
        new_child->values[i] = full_child->values[mid + 1 + i];
    }
    
    // Copy upper half of children if not leaf
    if (!full_child->is_leaf) {
        for (int i = 0; i <= new_child->num_keys; i++) {
            new_child->children[i] = full_child->children[mid + 1 + i];
            if (new_child->children[i]) {
                new_child->children[i]->parent = new_child;
            }
        }
    }
    
    full_child->num_keys = mid;
    
    // Insert middle key into parent
    for (int i = parent->num_keys; i > child_index; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[child_index + 1] = new_child;
    new_child->parent = parent;
    
    for (int i = parent->num_keys - 1; i >= child_index; i--) {
        parent->keys[i + 1] = parent->keys[i];
        parent->values[i + 1] = parent->values[i];
    }
    
    parent->keys[child_index] = full_child->keys[mid];
    parent->values[child_index] = full_child->values[mid];
    parent->num_keys++;
}

// Insert into non-full node
void btree_insert_nonfull(btree_node_t *node, uint32_t key, void *value) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        // Insert into leaf node
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        
        node->keys[i + 1] = key;
        node->values[i + 1] = value;
        node->num_keys++;
    } else {
        // Find child to recurse into
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;
        
        // Split child if full
        if (node->children[i]->num_keys == MAX_KEYS) {
            btree_split_child(node, i);
            
            if (key > node->keys[i]) {
                i++;
            }
        }
        
        btree_insert_nonfull(node->children[i], key, value);
    }
}

// Insert into B-tree
void btree_insert(btree_t *tree, uint32_t key, void *value) {
    if (tree->root == NULL) {
        tree->root = create_btree_node(1);
        tree->root->keys[0] = key;
        tree->root->values[0] = value;
        tree->root->num_keys = 1;
        tree->height = 1;
    } else {
        // If root is full, split it
        if (tree->root->num_keys == MAX_KEYS) {
            btree_node_t *new_root = create_btree_node(0);
            new_root->children[0] = tree->root;
            tree->root->parent = new_root;
            
            btree_split_child(new_root, 0);
            
            tree->root = new_root;
            tree->height++;
        }
        
        btree_insert_nonfull(tree->root, key, value);
    }
    
    tree->count++;
}

// Print B-tree structure
void btree_print_node(btree_node_t *node, int level) {
    if (node == NULL) return;
    
    printf("Level %d: [", level);
    for (int i = 0; i < node->num_keys; i++) {
        printf("%u", node->keys[i]);
        if (i < node->num_keys - 1) printf(", ");
    }
    printf("]\n");
    
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            btree_print_node(node->children[i], level + 1);
        }
    }
}

// Count disk reads for lookup (simulating disk-based B-tree)
int btree_search_disk_reads(btree_node_t *node, uint32_t key) {
    int disk_reads = 0;
    
    while (node != NULL) {
        disk_reads++;  // One disk read per node visited
        
        int i = 0;
        while (i < node->num_keys && key > node->keys[i]) {
            i++;
        }
        
        if (i < node->num_keys && key == node->keys[i]) {
            return disk_reads;  // Found
        }
        
        if (node->is_leaf) {
            return disk_reads;  // Not found
        }
        
        node = node->children[i];
    }
    
    return disk_reads;
}
```

## Database Indexes and Network Management

Network management databases store vast quantities of operational data: flow records, SNMP statistics, syslog messages, performance metrics. These databases use B-trees or B+-trees for indexing. A flow database might contain millions of records indexed by timestamp, source IP, or destination IP. Querying "show all flows from 10.0.0.0/8 to 192.168.0.0/16 in the last hour" requires scanning multiple indexes efficiently.

B+-trees, a B-tree variant, store all values in leaf nodes while internal nodes contain only keys. This modification allows leaf nodes to form a linked list, enabling efficient range scans. Finding all flows in a time range requires locating the first leaf containing the start time, then scanning the linked list until reaching the end time. B-trees would require tree traversals for each record in the range. B+-trees scan sequentially through leaves, exploiting disk prefetching and sequential access patterns.

```c
// B+ tree node (internal nodes don't store values)
typedef struct bplus_node {
    int num_keys;
    uint32_t keys[MAX_KEYS];
    union {
        struct bplus_node *children[BTREE_ORDER];  // For internal nodes
        void *values[MAX_KEYS];                     // For leaf nodes
    };
    struct bplus_node *next;                       // Leaf node linked list
    struct bplus_node *parent;
    int is_leaf;
} bplus_node_t;

typedef struct bplus_tree {
    bplus_node_t *root;
    bplus_node_t *leftmost_leaf;  // First leaf for range scans
    int count;
} bplus_tree_t;

// Flow record structure
typedef struct flow_record {
    uint64_t timestamp;
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
    uint64_t bytes;
    uint64_t packets;
} flow_record_t;

// Network management database
typedef struct netflow_db {
    bplus_tree *timestamp_index;   // Index by timestamp
    bplus_tree *src_ip_index;      // Index by source IP
    bplus_tree *dst_ip_index;      // Index by destination IP
} netflow_db_t;

// Range scan in B+ tree (efficient for time ranges)
typedef void (*range_callback_t)(uint32_t key, void *value, void *user_data);

void bplus_range_scan(bplus_node_t *start_leaf, uint32_t start_key, uint32_t end_key,
                      range_callback_t callback, void *user_data) {
    bplus_node_t *leaf = start_leaf;
    
    while (leaf != NULL) {
        for (int i = 0; i < leaf->num_keys; i++) {
            if (leaf->keys[i] >= start_key && leaf->keys[i] <= end_key) {
                callback(leaf->keys[i], leaf->values[i], user_data);
            } else if (leaf->keys[i] > end_key) {
                return;  // Past end of range
            }
        }
        
        leaf = leaf->next;  // Move to next leaf
    }
}

// Example: Query flows in time range
void query_flows_by_time(netflow_db_t *db, uint64_t start_time, uint64_t end_time) {
    printf("Querying flows from time %lu to %lu\n", start_time, end_time);
    
    // In real implementation, would do B+ tree range scan
    // This demonstrates the efficiency compared to full scan
    int records_scanned = 0;
    
    // B+ tree allows scanning only relevant leaf nodes
    printf("Scanning relevant leaf nodes...\n");
    
    // Simulate efficient range scan
    records_scanned = 100;  // Only scanned relevant records
    
    printf("Scanned %d records (vs potentially millions in full table)\n", 
           records_scanned);
}

// Bulk loading optimization for B-trees
void btree_bulk_load(btree_t *tree, uint32_t *keys, void **values, int count) {
    // Sort keys first (already sorted in this example)
    // Build tree bottom-up for optimal fill factor
    
    printf("Bulk loading %d entries into B-tree\n", count);
    
    for (int i = 0; i < count; i++) {
        btree_insert(tree, keys[i], values[i]);
    }
    
    printf("Bulk load complete. Tree height: %d\n", tree->height);
}
```

## Comparison: When to Use Each Tree Type

RB-trees belong in memory where every access costs nanoseconds and space efficiency matters. Timer systems, in-memory route caches, connection tracking tables—all benefit from RB-trees' consistent O(log n) performance with minimal per-node overhead. Each node requires only two pointers, one color bit, and the data. This compact representation maximizes cache efficiency and memory utilization.

B-trees dominate persistent storage scenarios where minimizing disk operations outweighs memory consumption. Flow databases, configuration stores, route archives—these applications pay milliseconds for each disk access, making the additional memory cost of wide nodes negligible compared to the savings from fewer accesses. A B-tree node might consume 4 kilobytes, but fetching it requires only one disk operation regardless of size.

Network devices often employ both: RB-trees for active timers and connection state in RAM, B-trees for archival flow data and historical statistics on disk. The in-memory structures must optimize for CPU cache lines and pointer chasing overhead. The on-disk structures must optimize for sequential access patterns and minimize seek operations. Understanding these trade-offs determines whether your implementation achieves line-rate packet processing or stalls on database queries.

```c
// Performance comparison demonstration
void compare_tree_performance() {
    printf("=== Tree Performance Comparison ===\n\n");
    
    // RB-tree performance (in-memory)
    printf("RB-Tree (in-memory timer queue):\n");
    rb_tree_t *rb = create_rb_tree();
    
    clock_t start = clock();
    for (int i = 0; i < 10000; i++) {
        uint64_t expiry = get_time_usec() + (rand() % 1000000);
        rb_insert(rb, expiry, NULL);
    }
    clock_t end = clock();
    
    printf("  Inserted 10,000 timers: %.2f ms\n", 
           (double)(end - start) / CLOCKS_PER_SEC * 1000);
    printf("  Tree height: ~%d\n", (int)(log(10000) / log(2)));
    printf("  Memory per node: ~%lu bytes\n", sizeof(rb_node_t));
    printf("  Total memory: ~%lu KB\n", (sizeof(rb_node_t) * 10000) / 1024);
    
    start = clock();
    for (int i = 0; i < 10000; i++) {
        rb_node_t *min = rb_minimum(rb, rb->root);
        rb_delete(rb, min);
    }
    end = clock();
    
    printf("  Removed all timers: %.2f ms\n\n", 
           (double)(end - start) / CLOCKS_PER_SEC * 1000);
    
    // B-tree performance (disk-based)
    printf("B-Tree (disk-based flow database):\n");
    btree_t *btree = create_btree();
    
    start = clock();
    for (int i = 0; i < 10000; i++) {
        btree_insert(btree, i, NULL);
    }
    end = clock();
    
    printf("  Inserted 10,000 flows: %.2f ms\n",
           (double)(end - start) / CLOCKS_PER_SEC * 1000);
    printf("  Tree height: %d\n", btree->height);
    printf("  Max keys per node: %d\n", MAX_KEYS);
    printf("  Memory per node: ~%lu bytes\n", sizeof(btree_node_t));
    printf("  Disk reads for lookup: ~%d (vs ~14 for binary tree)\n", btree->height);
    
    // Simulate disk access cost
    printf("\n  With 5ms disk seek time:\n");
    printf("    Binary tree lookup: ~70ms\n");
    printf("    B-tree lookup: ~%dms\n", btree->height * 5);
}

// Practical usage example
void practical_example() {
    printf("\n=== Practical Network Application ===\n\n");
    
    // Scenario: Router with active timers and flow archive
    printf("Router Configuration:\n");
    printf("  - 5,000 active TCP connections (RB-tree for timers)\n");
    printf("  - 10,000,000 archived flow records (B-tree on disk)\n\n");
    
    // In-memory timer management
    timer_system_t *ts = create_timer_system();
    printf("Memory timer system: Using RB-tree\n");
    printf("  Lookup time: O(log n) = ~12 comparisons for 5,000 entries\n");
    printf("  Memory overhead: ~160KB for tree structure\n\n");
    
    // Disk-based flow archive
    printf("Flow archive: Using B-tree with order 256\n");
    printf("  Tree height: ~3 levels for 10M entries\n");
    printf("  Disk reads per query: 3 (vs 24 for binary tree)\n");
    printf("  Query time: ~15ms (vs 120ms for binary tree)\n");
}
```

## Conclusion

Balanced trees provide the foundation for time-critical operations throughout network systems. RB-trees enable kernel TCP stacks to manage millions of timers efficiently, finding the next expiring timer in microseconds rather than milliseconds. B-trees allow network management systems to query massive flow databases with minimal disk operations, transforming impractical full-table scans into targeted index lookups.

The mathematical guarantees of balanced trees—logarithmic height regardless of insertion order, predictable worst-case performance, efficient sequential access—make them indispensable for production systems where performance cannot degrade unpredictably. A network device cannot afford timer systems that occasionally stall for milliseconds or databases that sometimes require seconds for queries. Balanced trees deliver consistent performance under all workloads, a requirement for systems that must forward packets at line rate while simultaneously managing operational state.

Master these structures and understand when to apply each variant, and you possess the tools to build network systems that scale from hundreds to millions of entries without sacrificing responsiveness. The choice between RB-trees and B-trees, between in-memory and on-disk storage, between space efficiency and access patterns determines whether your implementation meets production requirements or collapses under load.

# Graphs in Networking: Topology and Shortest Paths

## The Network as a Graph

Every network, from a small office LAN to the global Internet, forms a graph. Routers and switches become vertices. Physical links, wireless connections, and virtual circuits become edges. This abstraction transforms complex network topologies into mathematical structures amenable to algorithmic analysis. A three-router network with bidirectional links translates into a graph with three vertices and edges connecting them. Edge weights represent link costs—latency, bandwidth, administrative preference, or composite metrics.

This graph representation enables routing protocols to compute optimal paths mathematically rather than through trial and error. When OSPF needs to find the best route from Router A to Router Z through a network of 50 routers, it constructs a graph of the network topology and applies shortest-path algorithms. When SDN controllers reconfigure traffic flows, they manipulate graph representations of the network. Understanding graphs means understanding how networks make forwarding decisions at a fundamental level.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

// Graph representation for network topology
#define MAX_ROUTERS 100
#define INFINITY_COST 0xFFFFFFFF

// Edge structure representing a network link
typedef struct edge {
    int dest;                  // Destination router ID
    uint32_t cost;            // Link cost (metric)
    uint32_t bandwidth;       // Link bandwidth in Mbps
    uint32_t delay;           // Link delay in microseconds
    struct edge *next;        // Next edge in adjacency list
} edge_t;

// Vertex structure representing a router
typedef struct router {
    int id;                   // Router ID
    char name[32];            // Router name (e.g., "R1", "CoreRouter1")
    uint32_t router_id;       // OSPF router ID (IP address)
    edge_t *adj_list;         // Adjacency list (outgoing links)
    int degree;               // Number of adjacent routers
} router_t;

// Graph structure representing network topology
typedef struct network_graph {
    router_t routers[MAX_ROUTERS];
    int num_routers;
    int **adjacency_matrix;   // Alternative representation
} network_graph_t;

// Create empty network graph
network_graph_t* create_network_graph() {
    network_graph_t *graph = (network_graph_t*)malloc(sizeof(network_graph_t));
    graph->num_routers = 0;
    
    // Allocate adjacency matrix
    graph->adjacency_matrix = (int**)malloc(MAX_ROUTERS * sizeof(int*));
    for (int i = 0; i < MAX_ROUTERS; i++) {
        graph->adjacency_matrix[i] = (int*)malloc(MAX_ROUTERS * sizeof(int));
        for (int j = 0; j < MAX_ROUTERS; j++) {
            graph->adjacency_matrix[i][j] = (i == j) ? 0 : INFINITY_COST;
        }
    }
    
    return graph;
}

// Add router to network
int add_router(network_graph_t *graph, const char *name, uint32_t router_id) {
    if (graph->num_routers >= MAX_ROUTERS) {
        return -1;
    }
    
    int id = graph->num_routers;
    router_t *router = &graph->routers[id];
    
    router->id = id;
    strncpy(router->name, name, sizeof(router->name) - 1);
    router->router_id = router_id;
    router->adj_list = NULL;
    router->degree = 0;
    
    graph->num_routers++;
    return id;
}

// Add bidirectional link between routers
void add_link(network_graph_t *graph, int src, int dest, uint32_t cost,
              uint32_t bandwidth, uint32_t delay) {
    // Add edge src -> dest
    edge_t *edge1 = (edge_t*)malloc(sizeof(edge_t));
    edge1->dest = dest;
    edge1->cost = cost;
    edge1->bandwidth = bandwidth;
    edge1->delay = delay;
    edge1->next = graph->routers[src].adj_list;
    graph->routers[src].adj_list = edge1;
    graph->routers[src].degree++;
    
    // Add edge dest -> src (bidirectional)
    edge_t *edge2 = (edge_t*)malloc(sizeof(edge_t));
    edge2->dest = src;
    edge2->cost = cost;
    edge2->bandwidth = bandwidth;
    edge2->delay = delay;
    edge2->next = graph->routers[dest].adj_list;
    graph->routers[dest].adj_list = edge2;
    graph->routers[dest].degree++;
    
    // Update adjacency matrix
    graph->adjacency_matrix[src][dest] = cost;
    graph->adjacency_matrix[dest][src] = cost;
}

// Print network topology
void print_topology(network_graph_t *graph) {
    printf("Network Topology:\n");
    printf("Routers: %d\n\n", graph->num_routers);
    
    for (int i = 0; i < graph->num_routers; i++) {
        router_t *router = &graph->routers[i];
        printf("Router %s (ID: %d, Degree: %d)\n", 
               router->name, router->id, router->degree);
        
        edge_t *edge = router->adj_list;
        while (edge != NULL) {
            printf("  -> %s (cost: %u, bandwidth: %u Mbps, delay: %u us)\n",
                   graph->routers[edge->dest].name,
                   edge->cost, edge->bandwidth, edge->delay);
            edge = edge->next;
        }
        printf("\n");
    }
}

// Create example network topology
network_graph_t* create_example_network() {
    network_graph_t *graph = create_network_graph();
    
    // Add routers
    int r1 = add_router(graph, "R1", (10 << 24) | (0 << 16) | (0 << 8) | 1);
    int r2 = add_router(graph, "R2", (10 << 24) | (0 << 16) | (0 << 8) | 2);
    int r3 = add_router(graph, "R3", (10 << 24) | (0 << 16) | (0 << 8) | 3);
    int r4 = add_router(graph, "R4", (10 << 24) | (0 << 16) | (0 << 8) | 4);
    int r5 = add_router(graph, "R5", (10 << 24) | (0 << 16) | (0 << 8) | 5);
    
    // Add links with costs representing different scenarios
    // R1-R2: High bandwidth, low latency (10 Gbps fiber)
    add_link(graph, r1, r2, 1, 10000, 5);
    
    // R1-R3: Medium bandwidth (1 Gbps)
    add_link(graph, r1, r3, 5, 1000, 10);
    
    // R2-R4: High bandwidth link
    add_link(graph, r2, r4, 2, 10000, 8);
    
    // R3-R4: Lower bandwidth backup link
    add_link(graph, r3, r4, 10, 100, 50);
    
    // R3-R5: Direct connection
    add_link(graph, r3, r5, 3, 1000, 15);
    
    // R4-R5: High bandwidth
    add_link(graph, r4, r5, 1, 10000, 5);
    
    return graph;
}
```

## Adjacency List vs Adjacency Matrix

Representing a graph requires choosing between two fundamental structures: adjacency lists and adjacency matrices. An adjacency list stores, for each vertex, a linked list of its neighbors. Router R1 connected to R2 and R3 stores a list containing R2 and R3. This representation consumes memory proportional to the number of edges, making it space-efficient for sparse graphs—typical network topologies where each router connects to a handful of neighbors rather than every other router.

The adjacency matrix stores a two-dimensional array where entry [i][j] contains the cost of the link from router i to router j, or infinity if no link exists. This representation requires space proportional to the square of the number of vertices. For a 1000-router network, the matrix consumes 4 megabytes even if each router connects to only 4 neighbors. However, matrices allow constant-time edge lookups: checking if R1 connects to R500 requires a single array access rather than traversing a linked list.

```c
// Compare memory usage of representations
void compare_representations(network_graph_t *graph) {
    int num_edges = 0;
    
    // Count edges in adjacency list
    for (int i = 0; i < graph->num_routers; i++) {
        edge_t *edge = graph->routers[i].adj_list;
        while (edge != NULL) {
            num_edges++;
            edge = edge->next;
        }
    }
    
    // Adjacency list memory
    size_t list_memory = num_edges * sizeof(edge_t);
    list_memory += graph->num_routers * sizeof(router_t);
    
    // Adjacency matrix memory
    size_t matrix_memory = graph->num_routers * graph->num_routers * sizeof(int);
    
    printf("Representation Comparison:\n");
    printf("  Routers: %d\n", graph->num_routers);
    printf("  Edges: %d\n", num_edges);
    printf("  Adjacency List: %zu bytes\n", list_memory);
    printf("  Adjacency Matrix: %zu bytes\n", matrix_memory);
    printf("  Ratio: %.2fx\n\n", (double)matrix_memory / list_memory);
    
    // Edge lookup performance
    printf("Edge Lookup Performance:\n");
    printf("  Adjacency List: O(degree) - average %d comparisons\n", 
           num_edges / graph->num_routers);
    printf("  Adjacency Matrix: O(1) - 1 array access\n");
}

// Get link cost using adjacency list
uint32_t get_link_cost_list(network_graph_t *graph, int src, int dest) {
    edge_t *edge = graph->routers[src].adj_list;
    while (edge != NULL) {
        if (edge->dest == dest) {
            return edge->cost;
        }
        edge = edge->next;
    }
    return INFINITY_COST;
}

// Get link cost using adjacency matrix
uint32_t get_link_cost_matrix(network_graph_t *graph, int src, int dest) {
    return graph->adjacency_matrix[src][dest];
}

// Benchmark lookup performance
void benchmark_lookups(network_graph_t *graph) {
    clock_t start, end;
    int lookups = 100000;
    
    // Benchmark adjacency list
    start = clock();
    for (int i = 0; i < lookups; i++) {
        int src = rand() % graph->num_routers;
        int dest = rand() % graph->num_routers;
        uint32_t cost = get_link_cost_list(graph, src, dest);
        (void)cost;
    }
    end = clock();
    double list_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    // Benchmark adjacency matrix
    start = clock();
    for (int i = 0; i < lookups; i++) {
        int src = rand() % graph->num_routers;
        int dest = rand() % graph->num_routers;
        uint32_t cost = get_link_cost_matrix(graph, src, dest);
        (void)cost;
    }
    end = clock();
    double matrix_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Lookup Benchmark (%d operations):\n", lookups);
    printf("  Adjacency List: %.3f ms\n", list_time * 1000);
    printf("  Adjacency Matrix: %.3f ms\n", matrix_time * 1000);
    printf("  Speedup: %.2fx\n\n", list_time / matrix_time);
}
```

## Dijkstra's Algorithm: Single-Source Shortest Path

When OSPF computes routes, it uses Dijkstra's algorithm to find the shortest path from a router to all other routers in the network. The algorithm maintains a set of vertices whose shortest distance from the source is known, starting with just the source itself. It repeatedly selects the unvisited vertex with minimum distance, adds it to the known set, and updates distances to its neighbors. This greedy approach guarantees finding optimal paths when all edge weights are non-negative—true for OSPF where link costs represent physical properties, never negative values.

The algorithm's efficiency depends on how quickly it finds the minimum-distance vertex. A naive implementation scans all unvisited vertices, requiring O(V²) time for V vertices. Using a priority queue reduces this to O((V + E) log V), where E represents edges. For dense graphs where most routers connect to most others, the difference matters little. For sparse graphs typical of real networks, the priority queue optimization dramatically improves performance.

```c
#include <stdbool.h>

// Priority queue node for Dijkstra's algorithm
typedef struct pq_node {
    int router_id;
    uint32_t distance;
    struct pq_node *next;
} pq_node_t;

// Priority queue (simple implementation, not optimized)
typedef struct priority_queue {
    pq_node_t *head;
    int size;
} priority_queue_t;

// Create priority queue
priority_queue_t* create_pq() {
    priority_queue_t *pq = (priority_queue_t*)malloc(sizeof(priority_queue_t));
    pq->head = NULL;
    pq->size = 0;
    return pq;
}

// Insert into priority queue (sorted by distance)
void pq_insert(priority_queue_t *pq, int router_id, uint32_t distance) {
    pq_node_t *node = (pq_node_t*)malloc(sizeof(pq_node_t));
    node->router_id = router_id;
    node->distance = distance;
    
    // Insert in sorted order
    if (pq->head == NULL || distance < pq->head->distance) {
        node->next = pq->head;
        pq->head = node;
    } else {
        pq_node_t *current = pq->head;
        while (current->next != NULL && current->next->distance <= distance) {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
    
    pq->size++;
}

// Extract minimum from priority queue
int pq_extract_min(priority_queue_t *pq, uint32_t *distance) {
    if (pq->head == NULL) {
        return -1;
    }
    
    pq_node_t *node = pq->head;
    int router_id = node->router_id;
    *distance = node->distance;
    
    pq->head = node->next;
    free(node);
    pq->size--;
    
    return router_id;
}

// Check if priority queue is empty
bool pq_is_empty(priority_queue_t *pq) {
    return pq->size == 0;
}

// Dijkstra's shortest path algorithm
void dijkstra(network_graph_t *graph, int source, 
              uint32_t *distances, int *predecessors) {
    // Initialize distances and predecessors
    for (int i = 0; i < graph->num_routers; i++) {
        distances[i] = INFINITY_COST;
        predecessors[i] = -1;
    }
    distances[source] = 0;
    
    // Priority queue of unvisited vertices
    priority_queue_t *pq = create_pq();
    bool *visited = (bool*)calloc(graph->num_routers, sizeof(bool));
    
    pq_insert(pq, source, 0);
    
    while (!pq_is_empty(pq)) {
        uint32_t current_dist;
        int u = pq_extract_min(pq, &current_dist);
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // Relax edges from u
        edge_t *edge = graph->routers[u].adj_list;
        while (edge != NULL) {
            int v = edge->dest;
            uint32_t new_dist = distances[u] + edge->cost;
            
            if (new_dist < distances[v]) {
                distances[v] = new_dist;
                predecessors[v] = u;
                pq_insert(pq, v, new_dist);
            }
            
            edge = edge->next;
        }
    }
    
    free(visited);
    free(pq);
}

// Reconstruct path from source to destination
void print_path(network_graph_t *graph, int source, int dest, int *predecessors) {
    if (dest == source) {
        printf("%s", graph->routers[source].name);
        return;
    }
    
    if (predecessors[dest] == -1) {
        printf("No path exists");
        return;
    }
    
    // Build path recursively
    print_path(graph, source, predecessors[dest], predecessors);
    printf(" -> %s", graph->routers[dest].name);
}

// Compute and display routing table using Dijkstra
void compute_routing_table(network_graph_t *graph, int source) {
    uint32_t *distances = (uint32_t*)malloc(graph->num_routers * sizeof(uint32_t));
    int *predecessors = (int*)malloc(graph->num_routers * sizeof(int));
    
    printf("Computing routing table for %s using Dijkstra's algorithm...\n\n",
           graph->routers[source].name);
    
    dijkstra(graph, source, distances, predecessors);
    
    printf("Destination | Cost | Next Hop | Path\n");
    printf("------------|------|----------|-----\n");
    
    for (int i = 0; i < graph->num_routers; i++) {
        if (i == source) continue;
        
        printf("%-11s | ", graph->routers[i].name);
        
        if (distances[i] == INFINITY_COST) {
            printf("INF  | -        | No path\n");
        } else {
            // Find next hop (first router on path)
            int next_hop = i;
            while (predecessors[next_hop] != source && predecessors[next_hop] != -1) {
                next_hop = predecessors[next_hop];
            }
            
            printf("%-4u | %-8s | ", distances[i], graph->routers[next_hop].name);
            print_path(graph, source, i, predecessors);
            printf("\n");
        }
    }
    
    printf("\n");
    
    free(distances);
    free(predecessors);
}
```

## OSPF Link State Database

OSPF routers flood Link State Advertisements throughout the network, each router building an identical graph of the topology. The Link State Database stores this graph, with each LSA describing a vertex and its adjacent edges. When a link fails or a new router joins, updated LSAs propagate through the network, and every router recomputes routes using Dijkstra's algorithm on the updated graph.

This distributed database consistency requirement creates interesting challenges. LSAs carry sequence numbers to prevent old information from overwriting new. They include age fields that cause stale entries to expire. Routers must detect when their view of the topology diverges from neighbors, triggering database synchronization. The graph structure makes this possible—comparing two routers' databases reduces to comparing their vertex and edge sets.

```c
// OSPF Link State Advertisement
typedef struct lsa {
    uint32_t advertising_router;  // Router that originated this LSA
    uint32_t sequence_number;     // For detecting newer versions
    uint16_t age;                 // Time since originated (seconds)
    uint16_t checksum;            // Data integrity
    int num_links;                // Number of links described
    struct {
        uint32_t link_id;         // Connected router
        uint32_t link_data;       // Interface IP
        uint8_t link_type;        // Point-to-point, transit, stub
        uint8_t num_tos;          // Type of Service metrics
        uint16_t metric;          // Link cost
    } links[10];                  // Array of links
} lsa_t;

// OSPF Link State Database
typedef struct lsdb {
    lsa_t **lsas;                 // Array of LSA pointers
    int count;                    // Number of LSAs
    int capacity;                 // Array capacity
} lsdb_t;

// Create LSDB
lsdb_t* create_lsdb() {
    lsdb_t *db = (lsdb_t*)malloc(sizeof(lsdb_t));
    db->capacity = 100;
    db->count = 0;
    db->lsas = (lsa_t**)calloc(db->capacity, sizeof(lsa_t*));
    return db;
}

// Install LSA in database
void lsdb_install(lsdb_t *db, lsa_t *lsa) {
    // Check for existing LSA from same router
    for (int i = 0; i < db->count; i++) {
        if (db->lsas[i]->advertising_router == lsa->advertising_router) {
            // Check sequence number
            if (lsa->sequence_number > db->lsas[i]->sequence_number) {
                // Newer LSA, replace old one
                free(db->lsas[i]);
                db->lsas[i] = lsa;
                return;
            } else {
                // Old LSA, discard
                return;
            }
        }
    }
    
    // New LSA, add to database
    if (db->count >= db->capacity) {
        db->capacity *= 2;
        db->lsas = realloc(db->lsas, db->capacity * sizeof(lsa_t*));
    }
    
    db->lsas[db->count++] = lsa;
}

// Build graph from LSDB
network_graph_t* build_graph_from_lsdb(lsdb_t *db) {
    network_graph_t *graph = create_network_graph();
    
    // First pass: Add all routers as vertices
    for (int i = 0; i < db->count; i++) {
        lsa_t *lsa = db->lsas[i];
        char name[32];
        snprintf(name, sizeof(name), "R%u", lsa->advertising_router);
        add_router(graph, name, lsa->advertising_router);
    }
    
    // Second pass: Add all links as edges
    for (int i = 0; i < db->count; i++) {
        lsa_t *lsa = db->lsas[i];
        
        // Find source router ID in graph
        int src_id = -1;
        for (int j = 0; j < graph->num_routers; j++) {
            if (graph->routers[j].router_id == lsa->advertising_router) {
                src_id = j;
                break;
            }
        }
        
        // Add links
        for (int j = 0; j < lsa->num_links; j++) {
            // Find destination router ID
            int dest_id = -1;
            for (int k = 0; k < graph->num_routers; k++) {
                if (graph->routers[k].router_id == lsa->links[j].link_id) {
                    dest_id = k;
                    break;
                }
            }
            
            if (dest_id != -1) {
                // Add unidirectional link (LSA describes one direction)
                edge_t *edge = (edge_t*)malloc(sizeof(edge_t));
                edge->dest = dest_id;
                edge->cost = lsa->links[j].metric;
                edge->bandwidth = 1000;  // Default
                edge->delay = 10;        // Default
                edge->next = graph->routers[src_id].adj_list;
                graph->routers[src_id].adj_list = edge;
                graph->routers[src_id].degree++;
            }
        }
    }
    
    return graph;
}

// Simulate OSPF SPF calculation
void ospf_spf_calculation(lsdb_t *db, uint32_t calculating_router) {
    printf("OSPF SPF Calculation for Router %u\n", calculating_router);
    printf("========================================\n\n");
    
    printf("Step 1: Build topology graph from LSDB\n");
    network_graph_t *graph = build_graph_from_lsdb(db);
    printf("  Graph has %d routers and represents network topology\n\n", 
           graph->num_routers);
    
    // Find source router in graph
    int source = -1;
    for (int i = 0; i < graph->num_routers; i++) {
        if (graph->routers[i].router_id == calculating_router) {
            source = i;
            break;
        }
    }
    
    if (source == -1) {
        printf("  Error: Calculating router not found in LSDB\n");
        return;
    }
    
    printf("Step 2: Run Dijkstra's algorithm from source router\n");
    uint32_t *distances = (uint32_t*)malloc(graph->num_routers * sizeof(uint32_t));
    int *predecessors = (int*)malloc(graph->num_routers * sizeof(int));
    
    dijkstra(graph, source, distances, predecessors);
    printf("  Computed shortest paths to all reachable routers\n\n");
    
    printf("Step 3: Install routes in routing table\n");
    compute_routing_table(graph, source);
    
    free(distances);
    free(predecessors);
}
```

## Bellman-Ford: Distance Vector Routing

Distance vector protocols like RIP and EIGRP use the Bellman-Ford algorithm instead of Dijkstra. Rather than each router computing paths from a global topology graph, each router maintains a vector of distances to all destinations. Routers periodically exchange these vectors with neighbors. When Router A learns that Router B can reach destination D with cost 5, and the link A-B has cost 2, Router A computes that it can reach D with cost 7 through B.

The algorithm iterates, with each iteration allowing information to propagate one hop farther through the network. After V-1 iterations where V represents the number of vertices, the algorithm guarantees finding shortest paths. Unlike Dijkstra, Bellman-Ford handles negative edge weights, making it robust to certain administrative policy configurations. However, it suffers from the count-to-infinity problem: when a link fails, bad news propagates slowly as routers incrementally increase distances until reaching infinity.

```c
// Distance vector entry
typedef struct dv_entry {
    uint32_t dest;                // Destination network
    uint32_t cost;                // Cost to reach destination
    int next_hop;                 // Next hop router ID
} dv_entry_t;

// Distance vector table
typedef struct distance_vector {
    dv_entry_t entries[MAX_ROUTERS];
    int num_entries;
    int router_id;                // Owner of this DV
} distance_vector_t;

// Initialize distance vector
void init_distance_vector(distance_vector_t *dv, int router_id, 
                         network_graph_t *graph) {
    dv->router_id = router_id;
    dv->num_entries = 0;
    
    // Add directly connected neighbors
    edge_t *edge = graph->routers[router_id].adj_list;
    while (edge != NULL) {
        dv_entry_t *entry = &dv->entries[dv->num_entries++];
        entry->dest = edge->dest;
        entry->cost = edge->cost;
        entry->next_hop = edge->dest;
        edge = edge->next;
    }
    
    // Add self with cost 0
    dv_entry_t *self_entry = &dv->entries[dv->num_entries++];
    self_entry->dest = router_id;
    self_entry->cost = 0;
    self_entry->next_hop = router_id;
}

// Bellman-Ford algorithm (distance vector protocol)
bool bellman_ford_iteration(network_graph_t *graph, distance_vector_t *dvs) {
    bool changed = false;
    
    // For each router
    for (int u = 0; u < graph->num_routers; u++) {
        distance_vector_t *dv_u = &dvs[u];
        
        // For each neighbor
        edge_t *edge = graph->routers[u].adj_list;
        while (edge != NULL) {
            int v = edge->dest;
            distance_vector_t *dv_v = &dvs[v];
            
            // For each destination in neighbor's DV
            for (int i = 0; i < dv_v->num_entries; i++) {
                uint32_t dest = dv_v->entries[i].dest;
                uint32_t cost_through_v = edge->cost + dv_v->entries[i].cost;
                
                // Find dest in our DV
                int found = -1;
                for (int j = 0; j < dv_u->num_entries; j++) {
                    if (dv_u->entries[j].dest == dest) {
                        found = j;
                        break;
                    }
                }
                
                if (found == -1) {
                    // New destination, add it
                    if (dv_u->num_entries < MAX_ROUTERS) {
                        dv_entry_t *entry = &dv_u->entries[dv_u->num_entries++];
                        entry->dest = dest;
                        entry->cost = cost_through_v;
                        entry->next_hop = v;
                        changed = true;
                    }
                } else if (cost_through_v < dv_u->entries[found].cost) {
                    // Better path found
                    dv_u->entries[found].cost = cost_through_v;
                    dv_u->entries[found].next_hop = v;
                    changed = true;
                }
            }
            
            edge = edge->next;
        }
    }
    
    return changed;
}

// Run distance vector protocol until convergence
void run_distance_vector(network_graph_t *graph) {
    printf("Distance Vector Protocol (Bellman-Ford)\n");
    printf("========================================\n\n");
    
    // Initialize distance vectors for all routers
    distance_vector_t *dvs = (distance_vector_t*)calloc(graph->num_routers, 
                                                         sizeof(distance_vector_t));
    
    for (int i = 0; i < graph->num_routers; i++) {
        init_distance_vector(&dvs[i], i, graph);
    }
    
    // Iterate until convergence
    int iteration = 0;
    bool changed = true;
    
    while (changed && iteration < graph->num_routers) {
        iteration++;
        printf("Iteration %d: Exchanging distance vectors\n", iteration);
        changed = bellman_ford_iteration(graph, dvs);
    }
    
    printf("\nConverged after %d iterations\n\n", iteration);
    
    // Print final routing tables
    for (int i = 0; i < graph->num_routers; i++) {
        printf("Router %s routing table:\n", graph->routers[i].name);
        printf("  Dest | Cost | Next Hop\n");
        printf("  -----|------|----------\n");
        
        for (int j = 0; j < dvs[i].num_entries; j++) {
            dv_entry_t *entry = &dvs[i].entries[j];
            printf("  %-4s | %-4u | %s\n",
                   graph->routers[entry->dest].name,
                   entry->cost,
                   graph->routers[entry->next_hop].name);
        }
        printf("\n");
    }
    
    free(dvs);
}

// Demonstrate count-to-infinity problem
void demonstrate_count_to_infinity(network_graph_t *graph) {
    printf("Count-to-Infinity Problem Demo\n");
    printf("================================\n\n");
    
    distance_vector_t *dvs = (distance_vector_t*)calloc(graph->num_routers,
                                                         sizeof(distance_vector_t));
    
    // Initialize and converge
    for (int i = 0; i < graph->num_routers; i++) {
        init_distance_vector(&dvs[i], i, graph);
    }
    
    int iteration = 0;
    while (bellman_ford_iteration(graph, dvs) && iteration < graph->num_routers) {
        iteration++;
    }
    
    printf("Network converged. Now simulating link failure...\n\n");
    
    // Simulate link failure by setting cost to infinity
    // Assume link between routers 0 and 1 fails
    if (graph->num_routers >= 2) {
        for (int i = 0; i < dvs[0].num_entries; i++) {
            if (dvs[0].entries[i].next_hop == 1) {
                printf("Link %s -> %s failed\n", 
                       graph->routers[0].name, 
                       graph->routers[1].name);
                dvs[0].entries[i].cost = INFINITY_COST;
            }
        }
        
        // Watch slow convergence
        printf("\nWatching convergence after failure:\n");
        for (int iter = 0; iter < 10; iter++) {
            bellman_ford_iteration(graph, dvs);
            
            printf("Iteration %d: R0's cost to destinations: ", iter + 1);
            for (int i = 0; i < dvs[0].num_entries; i++) {
                if (dvs[0].entries[i].cost < INFINITY_COST) {
                    printf("%u ", dvs[0].entries[i].cost);
                }
            }
            printf("\n");
        }
        
        printf("\nNotice gradual increase - this is count-to-infinity\n");
    }
    
    free(dvs);
}
```

## Path Computation for Traffic Engineering

Modern SDN networks require computing not just shortest paths but constrained paths satisfying multiple requirements: minimum bandwidth, maximum delay, path diversity for redundancy. This transforms into constrained shortest path problems. Finding a path with minimum cost subject to delay constraints requires algorithms more sophisticated than simple Dijkstra.

MPLS Traffic Engineering uses Constrained Shortest Path First (CSPF), extending Dijkstra to prune edges that violate constraints. Before relaxing an edge, CSPF checks if the path through that edge would violate bandwidth or delay constraints. This efficiently computes feasible paths but cannot guarantee finding a solution even when one exists, because constraints might be mutually exclusive along different path segments.

```c
// Constrained path requirements
typedef struct path_constraints {
    uint32_t min_bandwidth;    // Minimum required bandwidth (Mbps)
    uint32_t max_delay;        // Maximum acceptable delay (microseconds)
    int avoid_router;          // Router to avoid (-1 for none)
} path_constraints_t;

// Check if edge satisfies constraints
bool edge_satisfies_constraints(edge_t *edge, path_constraints_t *constraints,
                                uint32_t accumulated_delay) {
    if (edge->bandwidth < constraints->min_bandwidth) {
        return false;
    }
    
    if (accumulated_delay + edge->delay > constraints->max_delay) {
        return false;
    }
    
    if (edge->dest == constraints->avoid_router) {
        return false;
    }
    
    return true;
}

// Constrained Shortest Path First (CSPF)
bool cspf(network_graph_t *graph, int source, int destination,
          path_constraints_t *constraints, 
          uint32_t *path_cost, int **path, int *path_length) {
    
    uint32_t *distances = (uint32_t*)malloc(graph->num_routers * sizeof(uint32_t));
    uint32_t *delays = (uint32_t*)malloc(graph->num_routers * sizeof(uint32_t));
    int *predecessors = (int*)malloc(graph->num_routers * sizeof(int));
    bool *visited = (bool*)calloc(graph->num_routers, sizeof(bool));
    
    // Initialize
    for (int i = 0; i < graph->num_routers; i++) {
        distances[i] = INFINITY_COST;
        delays[i] = UINT32_MAX;
        predecessors[i] = -1;
    }
    distances[source] = 0;
    delays[source] = 0;
    
    priority_queue_t *pq = create_pq();
    pq_insert(pq, source, 0);
    
    bool found = false;
    
    while (!pq_is_empty(pq)) {
        uint32_t current_dist;
        int u = pq_extract_min(pq, &current_dist);
        
        if (visited[u]) continue;
        visited[u] = true;
        
        if (u == destination) {
            found = true;
            break;
        }
        
        // Examine neighbors
        edge_t *edge = graph->routers[u].adj_list;
        while (edge != NULL) {
            int v = edge->dest;
            
            // Check constraints
            if (edge_satisfies_constraints(edge, constraints, delays[u])) {
                uint32_t new_dist = distances[u] + edge->cost;
                uint32_t new_delay = delays[u] + edge->delay;
                
                if (new_dist < distances[v]) {
                    distances[v] = new_dist;
                    delays[v] = new_delay;
                    predecessors[v] = u;
                    pq_insert(pq, v, new_dist);
                }
            }
            
            edge = edge->next;
        }
    }
    
    if (found) {
        *path_cost = distances[destination];
        
        // Reconstruct path
        int count = 0;
        int current = destination;
        while (current != -1) {
            count++;
            current = predecessors[current];
        }
        
        *path_length = count;
        *path = (int*)malloc(count * sizeof(int));
        
        current = destination;
        for (int i = count - 1; i >= 0; i--) {
            (*path)[i] = current;
            current = predecessors[current];
        }
    }
    
    free(distances);
    free(delays);
    free(predecessors);
    free(visited);
    free(pq);
    
    return found;
}

// Compute traffic engineering path
void compute_te_path(network_graph_t *graph) {
    printf("MPLS Traffic Engineering Path Computation\n");
    printf("==========================================\n\n");
    
    // Define constraints
    path_constraints_t constraints = {
        .min_bandwidth = 1000,   // Need 1 Gbps
        .max_delay = 30,         // Max 30 microseconds
        .avoid_router = 2        // Avoid R3 for diversity
    };
    
    printf("Computing path from R1 to R5 with constraints:\n");
    printf("  Minimum bandwidth: %u Mbps\n", constraints.min_bandwidth);
    printf("  Maximum delay: %u us\n", constraints.max_delay);
    printf("  Avoid router: R3\n\n");
    
    uint32_t cost;
    int *path;
    int path_len;
    
    bool found = cspf(graph, 0, 4, &constraints, &cost, &path, &path_len);
    
    if (found) {
        printf("Constrained path found:\n");
        printf("  Path: ");
        for (int i = 0; i < path_len; i++) {
            printf("%s", graph->routers[path[i]].name);
            if (i < path_len - 1) printf(" -> ");
        }
        printf("\n");
        printf("  Total cost: %u\n\n", cost);
        
        free(path);
    } else {
        printf("No path satisfying constraints exists\n\n");
    }
}
```

## Conclusion

Graphs provide the mathematical foundation for network routing. Whether representing physical topology, logical overlays, or virtual networks, graph algorithms determine how packets flow through infrastructure. Dijkstra powers OSPF and IS-IS, computing optimal paths from complete topology knowledge. Bellman-Ford enables RIP and EIGRP, converging through distributed computation without global knowledge. Constrained shortest path algorithms enable traffic engineering, computing paths that satisfy business policies alongside technical requirements.

Understanding these algorithms means understanding how networks make forwarding decisions at scale. A routing protocol choosing Dijkstra over Bellman-Ford makes specific trade-offs: faster convergence and lower overhead at the cost of requiring complete topology flooding. An SDN controller using CSPF can enforce policies that basic shortest-path routing cannot express. The graph algorithms you choose determine the capabilities and limitations of your network's control plane.

Master graph algorithms, and you understand why networks behave as they do—why OSPF converges in seconds while RIP takes minutes, why MPLS traffic engineering can split load across multiple paths, why SDN can enforce constraints that traditional routing cannot. These algorithms transform network topology from a mess of cables and routers into a structured problem amenable to mathematical solution.

# Heaps and Priority Queues in Networking

## The Priority Queue Problem

Network systems constantly juggle events ordered by priority or time. A router must transmit high-priority control packets before bulk data transfers. A TCP stack must process retransmission timeouts in chronological order. A QoS scheduler must serve traffic classes according to configured priorities. These scenarios share a common pattern: maintain a collection of elements where you repeatedly need to extract the highest-priority item and insert new items, all while maintaining efficiency.

A naive approach using an unsorted array requires O(n) time to find the minimum element—scanning every entry to locate the highest priority. Keeping a sorted array makes extraction O(1) but insertion becomes O(n) as you search for the correct position. Heaps solve this by providing O(log n) insertion and extraction while maintaining a simple array-based structure with excellent cache locality. For a router processing thousands of packets per second with different priorities, this performance difference transforms from academic curiosity to operational necessity.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Min-heap node for priority queue
typedef struct heap_node {
    uint64_t priority;        // Lower value = higher priority
    void *data;               // Pointer to actual data
} heap_node_t;

// Binary min-heap structure
typedef struct min_heap {
    heap_node_t *nodes;       // Array of nodes
    int capacity;             // Maximum size
    int size;                 // Current number of elements
} min_heap_t;

// Create min-heap
min_heap_t* create_min_heap(int capacity) {
    min_heap_t *heap = (min_heap_t*)malloc(sizeof(min_heap_t));
    heap->capacity = capacity;
    heap->size = 0;
    heap->nodes = (heap_node_t*)malloc(capacity * sizeof(heap_node_t));
    return heap;
}

// Get parent index
static inline int parent(int i) {
    return (i - 1) / 2;
}

// Get left child index
static inline int left_child(int i) {
    return 2 * i + 1;
}

// Get right child index
static inline int right_child(int i) {
    return 2 * i + 2;
}

// Swap two nodes
static inline void swap_nodes(heap_node_t *a, heap_node_t *b) {
    heap_node_t temp = *a;
    *a = *b;
    *b = temp;
}

// Heapify up (for insertion)
void heapify_up(min_heap_t *heap, int index) {
    while (index > 0 && heap->nodes[index].priority < heap->nodes[parent(index)].priority) {
        swap_nodes(&heap->nodes[index], &heap->nodes[parent(index)]);
        index = parent(index);
    }
}

// Heapify down (for extraction)
void heapify_down(min_heap_t *heap, int index) {
    int smallest = index;
    int left = left_child(index);
    int right = right_child(index);
    
    if (left < heap->size && heap->nodes[left].priority < heap->nodes[smallest].priority) {
        smallest = left;
    }
    
    if (right < heap->size && heap->nodes[right].priority < heap->nodes[smallest].priority) {
        smallest = right;
    }
    
    if (smallest != index) {
        swap_nodes(&heap->nodes[index], &heap->nodes[smallest]);
        heapify_down(heap, smallest);
    }
}

// Insert into heap
int heap_insert(min_heap_t *heap, uint64_t priority, void *data) {
    if (heap->size >= heap->capacity) {
        // Resize heap
        heap->capacity *= 2;
        heap->nodes = realloc(heap->nodes, heap->capacity * sizeof(heap_node_t));
    }
    
    // Insert at end
    int index = heap->size;
    heap->nodes[index].priority = priority;
    heap->nodes[index].data = data;
    heap->size++;
    
    // Restore heap property
    heapify_up(heap, index);
    
    return 1;
}

// Extract minimum (highest priority)
void* heap_extract_min(min_heap_t *heap, uint64_t *priority) {
    if (heap->size == 0) {
        return NULL;
    }
    
    // Save root
    void *data = heap->nodes[0].data;
    if (priority) {
        *priority = heap->nodes[0].priority;
    }
    
    // Move last element to root
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    // Restore heap property
    if (heap->size > 0) {
        heapify_down(heap, 0);
    }
    
    return data;
}

// Peek at minimum without removing
void* heap_peek_min(min_heap_t *heap, uint64_t *priority) {
    if (heap->size == 0) {
        return NULL;
    }
    
    if (priority) {
        *priority = heap->nodes[0].priority;
    }
    
    return heap->nodes[0].data;
}

// Check if heap is empty
int heap_is_empty(min_heap_t *heap) {
    return heap->size == 0;
}

// Example: Packet structure
typedef struct packet {
    int id;
    uint8_t *payload;
    size_t length;
    uint8_t priority_class;    // 0=highest, 7=lowest
    uint64_t enqueue_time;
} packet_t;

// Create example packet
packet_t* create_packet(int id, uint8_t priority_class, size_t length) {
    packet_t *pkt = (packet_t*)malloc(sizeof(packet_t));
    pkt->id = id;
    pkt->priority_class = priority_class;
    pkt->length = length;
    pkt->enqueue_time = time(NULL);
    pkt->payload = (uint8_t*)malloc(length);
    return pkt;
}

// Demonstrate basic heap operations
void demonstrate_heap_basics() {
    printf("=== Basic Heap Operations ===\n\n");
    
    min_heap_t *heap = create_min_heap(10);
    
    // Insert packets with different priorities
    printf("Inserting packets with priorities:\n");
    for (int i = 0; i < 5; i++) {
        uint8_t priority = rand() % 8;
        packet_t *pkt = create_packet(i, priority, 1500);
        
        printf("  Packet %d: priority class %u\n", i, priority);
        heap_insert(heap, priority, pkt);
    }
    
    printf("\nExtracting packets in priority order:\n");
    while (!heap_is_empty(heap)) {
        uint64_t priority;
        packet_t *pkt = (packet_t*)heap_extract_min(heap, &priority);
        printf("  Packet %d: priority class %lu\n", pkt->id, priority);
        free(pkt->payload);
        free(pkt);
    }
    
    printf("\n");
    free(heap->nodes);
    free(heap);
}
```

## The Binary Heap Structure

A binary heap stores elements in a complete binary tree where every parent node maintains the heap property relative to its children. In a min-heap, each parent's priority must be less than or equal to both children's priorities. This partial ordering—weaker than full sorting—allows efficient operations. The tree's completeness means we can store it in a simple array without pointers: the root occupies index 0, and for any node at index i, its left child sits at 2i+1 and right child at 2i+2.

Insertion places the new element at the next available position—the array's end—then bubbles it upward by repeatedly swapping with its parent until the heap property is restored. In a heap with 1024 elements, this requires at most log₂(1024) = 10 comparisons. Extraction removes the root, moves the last element to the root position, then sinks it downward by repeatedly swapping with the smaller child until settling into place. Again, at most logarithmic comparisons.

```c
// Detailed heap visualization
void print_heap_structure(min_heap_t *heap) {
    if (heap->size == 0) {
        printf("Empty heap\n");
        return;
    }
    
    printf("Heap structure (array representation):\n");
    printf("Index: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%3d ", i);
    }
    printf("\n");
    
    printf("Prior: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%3lu ", heap->nodes[i].priority);
    }
    printf("\n\n");
    
    // Print tree structure
    printf("Tree structure:\n");
    int level = 0;
    int level_size = 1;
    int printed = 0;
    
    while (printed < heap->size) {
        printf("Level %d: ", level);
        
        for (int i = 0; i < level_size && printed < heap->size; i++) {
            printf("%lu ", heap->nodes[printed].priority);
            printed++;
        }
        printf("\n");
        
        level++;
        level_size *= 2;
    }
    printf("\n");
}

// Verify heap property
int verify_heap(min_heap_t *heap) {
    for (int i = 0; i < heap->size; i++) {
        int left = left_child(i);
        int right = right_child(i);
        
        if (left < heap->size) {
            if (heap->nodes[i].priority > heap->nodes[left].priority) {
                printf("Heap property violated at index %d (left child)\n", i);
                return 0;
            }
        }
        
        if (right < heap->size) {
            if (heap->nodes[i].priority > heap->nodes[right].priority) {
                printf("Heap property violated at index %d (right child)\n", i);
                return 0;
            }
        }
    }
    
    return 1;
}

// Demonstrate heap property maintenance
void demonstrate_heap_property() {
    printf("=== Heap Property Maintenance ===\n\n");
    
    min_heap_t *heap = create_min_heap(15);
    
    printf("Inserting elements: 50, 30, 20, 15, 10, 8, 16\n\n");
    
    int values[] = {50, 30, 20, 15, 10, 8, 16};
    for (int i = 0; i < 7; i++) {
        printf("Inserting %d:\n", values[i]);
        heap_insert(heap, values[i], NULL);
        print_heap_structure(heap);
        
        if (verify_heap(heap)) {
            printf("✓ Heap property maintained\n\n");
        }
    }
    
    printf("Extracting minimum elements:\n\n");
    for (int i = 0; i < 3; i++) {
        uint64_t priority;
        heap_extract_min(heap, &priority);
        printf("Extracted: %lu\n", priority);
        print_heap_structure(heap);
        
        if (verify_heap(heap)) {
            printf("✓ Heap property maintained\n\n");
        }
    }
    
    free(heap->nodes);
    free(heap);
}
```

## QoS Packet Scheduling

Quality of Service mechanisms use priority queues to schedule packet transmission according to configured policies. A strict priority scheduler maintains separate queues for each priority class, always transmitting from the highest non-empty queue. A weighted fair queuing scheduler tracks virtual finish times for packets, using a priority queue ordered by these times to determine transmission order.

Consider a router interface with three traffic classes: voice (highest priority), video (medium priority), and data (lowest priority). Incoming packets are enqueued based on their class marking. The scheduler continuously extracts the highest-priority packet from the priority queue and transmits it. During congestion, low-priority packets accumulate while high-priority packets flow through with minimal delay. The heap structure ensures the scheduler can always find the next packet to transmit in logarithmic time regardless of queue depth.

```c
// QoS traffic class
typedef enum {
    QOS_VOICE = 0,        // Highest priority
    QOS_VIDEO = 1,
    QOS_CRITICAL_DATA = 2,
    QOS_BEST_EFFORT = 3,  // Lowest priority
    QOS_NUM_CLASSES = 4
} qos_class_t;

// Packet with QoS marking
typedef struct qos_packet {
    int id;
    qos_class_t qos_class;
    uint64_t arrival_time;
    uint64_t virtual_finish_time;  // For WFQ
    size_t length;
    uint8_t *data;
} qos_packet_t;

// QoS scheduler using priority queue
typedef struct qos_scheduler {
    min_heap_t *queue;
    uint64_t packets_sent[QOS_NUM_CLASSES];
    uint64_t bytes_sent[QOS_NUM_CLASSES];
    uint64_t packets_dropped[QOS_NUM_CLASSES];
} qos_scheduler_t;

// Create QoS scheduler
qos_scheduler_t* create_qos_scheduler(int capacity) {
    qos_scheduler_t *scheduler = (qos_scheduler_t*)malloc(sizeof(qos_scheduler_t));
    scheduler->queue = create_min_heap(capacity);
    memset(scheduler->packets_sent, 0, sizeof(scheduler->packets_sent));
    memset(scheduler->bytes_sent, 0, sizeof(scheduler->bytes_sent));
    memset(scheduler->packets_dropped, 0, sizeof(scheduler->packets_dropped));
    return scheduler;
}

// Enqueue packet (strict priority)
int qos_enqueue_strict(qos_scheduler_t *scheduler, qos_packet_t *pkt) {
    // Priority is the QoS class directly
    return heap_insert(scheduler->queue, pkt->qos_class, pkt);
}

// Enqueue packet (weighted fair queuing)
int qos_enqueue_wfq(qos_scheduler_t *scheduler, qos_packet_t *pkt, 
                    uint64_t virtual_clock) {
    // Compute virtual finish time based on packet length and class weight
    int weights[QOS_NUM_CLASSES] = {4, 3, 2, 1};  // Higher = more bandwidth
    
    pkt->virtual_finish_time = virtual_clock + (pkt->length / weights[pkt->qos_class]);
    
    return heap_insert(scheduler->queue, pkt->virtual_finish_time, pkt);
}

// Dequeue packet for transmission
qos_packet_t* qos_dequeue(qos_scheduler_t *scheduler) {
    if (heap_is_empty(scheduler->queue)) {
        return NULL;
    }
    
    uint64_t priority;
    qos_packet_t *pkt = (qos_packet_t*)heap_extract_min(scheduler->queue, &priority);
    
    if (pkt != NULL) {
        scheduler->packets_sent[pkt->qos_class]++;
        scheduler->bytes_sent[pkt->qos_class] += pkt->length;
    }
    
    return pkt;
}

// Print QoS statistics
void print_qos_stats(qos_scheduler_t *scheduler) {
    const char *class_names[] = {"Voice", "Video", "Critical", "Best-Effort"};
    
    printf("QoS Statistics:\n");
    printf("Class         | Packets Sent | Bytes Sent | Dropped\n");
    printf("--------------|--------------|------------|--------\n");
    
    for (int i = 0; i < QOS_NUM_CLASSES; i++) {
        printf("%-13s | %12lu | %10lu | %7lu\n",
               class_names[i],
               scheduler->packets_sent[i],
               scheduler->bytes_sent[i],
               scheduler->packets_dropped[i]);
    }
    printf("\n");
}

// Simulate QoS scheduling
void simulate_qos_scheduling() {
    printf("=== QoS Packet Scheduling Simulation ===\n\n");
    
    qos_scheduler_t *scheduler = create_qos_scheduler(1000);
    
    printf("Enqueueing mixed traffic:\n");
    
    // Enqueue packets with different priorities
    for (int i = 0; i < 20; i++) {
        qos_packet_t *pkt = (qos_packet_t*)malloc(sizeof(qos_packet_t));
        pkt->id = i;
        
        // Mix of traffic types
        if (i % 5 == 0) {
            pkt->qos_class = QOS_VOICE;
            pkt->length = 200;  // Small voice packet
        } else if (i % 3 == 0) {
            pkt->qos_class = QOS_VIDEO;
            pkt->length = 1500;
        } else if (i % 7 == 0) {
            pkt->qos_class = QOS_CRITICAL_DATA;
            pkt->length = 500;
        } else {
            pkt->qos_class = QOS_BEST_EFFORT;
            pkt->length = 1500;
        }
        
        pkt->arrival_time = i;
        qos_enqueue_strict(scheduler, pkt);
        
        const char *class_names[] = {"Voice", "Video", "Critical", "Best-Effort"};
        printf("  Packet %2d: %s class\n", i, class_names[pkt->qos_class]);
    }
    
    printf("\nDequeuing in priority order:\n");
    
    const char *class_names[] = {"Voice", "Video", "Critical", "Best-Effort"};
    while (!heap_is_empty(scheduler->queue)) {
        qos_packet_t *pkt = qos_dequeue(scheduler);
        printf("  Transmit packet %2d: %s class (%zu bytes)\n",
               pkt->id, class_names[pkt->qos_class], pkt->length);
        free(pkt);
    }
    
    printf("\n");
    print_qos_stats(scheduler);
    
    free(scheduler->queue->nodes);
    free(scheduler->queue);
    free(scheduler);
}
```

## Timer Wheels vs Priority Queue Timers

Network stacks need efficient timer management for TCP retransmissions, keepalives, and connection timeouts. Two approaches dominate: timer wheels and priority queue timers. Timer wheels bucket timers by expiration time modulo wheel size, providing O(1) insertion and deletion but limited precision. Priority queue timers using heaps provide arbitrary precision with O(log n) operations.

For coarse-grained timers where millisecond precision suffices, timer wheels excel. A wheel with 1024 buckets representing milliseconds handles timers up to 1024ms in the future with constant time. For fine-grained timers requiring microsecond precision, or for widely dispersed expiration times, heaps prove superior. Modern TCP stacks often combine both: a timer wheel for frequent short-term timers and a heap for infrequent long-term timers.

```c
// Timer wheel bucket
typedef struct tw_bucket {
    void **timers;            // Array of timer pointers
    int count;
    int capacity;
} tw_bucket_t;

// Timer wheel
typedef struct timer_wheel {
    tw_bucket_t *buckets;
    int num_buckets;
    uint64_t current_tick;
    uint64_t tick_duration_us;  // Microseconds per tick
} timer_wheel_t;

// Create timer wheel
timer_wheel_t* create_timer_wheel(int num_buckets, uint64_t tick_duration_us) {
    timer_wheel_t *tw = (timer_wheel_t*)malloc(sizeof(timer_wheel_t));
    tw->num_buckets = num_buckets;
    tw->tick_duration_us = tick_duration_us;
    tw->current_tick = 0;
    
    tw->buckets = (tw_bucket_t*)calloc(num_buckets, sizeof(tw_bucket_t));
    for (int i = 0; i < num_buckets; i++) {
        tw->buckets[i].capacity = 10;
        tw->buckets[i].count = 0;
        tw->buckets[i].timers = malloc(10 * sizeof(void*));
    }
    
    return tw;
}

// Insert timer into wheel
void tw_insert(timer_wheel_t *tw, uint64_t expiration_us, void *data) {
    uint64_t ticks_from_now = expiration_us / tw->tick_duration_us;
    int bucket_index = (tw->current_tick + ticks_from_now) % tw->num_buckets;
    
    tw_bucket_t *bucket = &tw->buckets[bucket_index];
    
    if (bucket->count >= bucket->capacity) {
        bucket->capacity *= 2;
        bucket->timers = realloc(bucket->timers, bucket->capacity * sizeof(void*));
    }
    
    bucket->timers[bucket->count++] = data;
}

// Advance timer wheel and process expired timers
int tw_advance(timer_wheel_t *tw) {
    int bucket_index = tw->current_tick % tw->num_buckets;
    tw_bucket_t *bucket = &tw->buckets[bucket_index];
    
    int processed = bucket->count;
    
    // Process all timers in this bucket
    for (int i = 0; i < bucket->count; i++) {
        // Timer callback would be invoked here
        // For demo, just count them
    }
    
    // Clear bucket
    bucket->count = 0;
    
    tw->current_tick++;
    return processed;
}

// Compare timer wheel vs heap performance
void compare_timer_mechanisms() {
    printf("=== Timer Wheel vs Priority Queue Comparison ===\n\n");
    
    int num_timers = 10000;
    
    // Timer wheel: 1024 buckets, 1ms per bucket
    printf("Timer Wheel (1024 buckets, 1ms resolution):\n");
    timer_wheel_t *tw = create_timer_wheel(1024, 1000);
    
    clock_t start = clock();
    for (int i = 0; i < num_timers; i++) {
        uint64_t expiration = (rand() % 1024) * 1000;  // Random 0-1024ms
        tw_insert(tw, expiration, NULL);
    }
    clock_t end = clock();
    
    printf("  Inserted %d timers: %.3f ms\n", num_timers,
           (double)(end - start) / CLOCKS_PER_SEC * 1000);
    printf("  Average insert time: %.2f ns\n",
           (double)(end - start) / CLOCKS_PER_SEC * 1000000000 / num_timers);
    
    // Priority queue (heap)
    printf("\nPriority Queue (heap, unlimited precision):\n");
    min_heap_t *heap = create_min_heap(num_timers);
    
    start = clock();
    for (int i = 0; i < num_timers; i++) {
        uint64_t expiration = rand() % 1024000;  // Random 0-1024ms (microseconds)
        heap_insert(heap, expiration, NULL);
    }
    end = clock();
    
    printf("  Inserted %d timers: %.3f ms\n", num_timers,
           (double)(end - start) / CLOCKS_PER_SEC * 1000);
    printf("  Average insert time: %.2f ns\n",
           (double)(end - start) / CLOCKS_PER_SEC * 1000000000 / num_timers);
    
    printf("\nTradeoffs:\n");
    printf("  Timer Wheel: O(1) operations, limited precision, fixed range\n");
    printf("  Priority Queue: O(log n) operations, arbitrary precision, unlimited range\n");
    printf("\n");
    
    // Cleanup
    for (int i = 0; i < tw->num_buckets; i++) {
        free(tw->buckets[i].timers);
    }
    free(tw->buckets);
    free(tw);
    free(heap->nodes);
    free(heap);
}
```

## Event-Driven Simulation

Network simulators use priority queues to manage future events ordered by simulation time. Each event—packet arrival, transmission completion, timer expiration—is inserted into the queue with its scheduled time. The simulation loop repeatedly extracts the nearest future event, advances simulation time to that event, and processes it. This discrete event simulation approach allows simulating complex networks with thousands of events without tracking wall-clock time.

The priority queue's efficiency directly determines simulation speed. A simulator tracking 100,000 future events requires logarithmic time to insert new events and extract the next event. With a heap, this means approximately 17 comparisons per operation. Linear search through an unsorted list would require 100,000 comparisons, making the simulation 5,000 times slower. For large-scale network simulations running millions of events, this difference separates feasible from infeasible.

```c
// Simulation event types
typedef enum {
    EVENT_PACKET_ARRIVAL,
    EVENT_PACKET_DEPARTURE,
    EVENT_TIMEOUT,
    EVENT_LINK_FAILURE,
    EVENT_LINK_RECOVERY
} event_type_t;

// Simulation event
typedef struct sim_event {
    event_type_t type;
    uint64_t time_us;         // Simulation time (microseconds)
    int node_id;              // Node where event occurs
    void *event_data;         // Event-specific data
} sim_event_t;

// Network simulator
typedef struct network_simulator {
    min_heap_t *event_queue;
    uint64_t current_time;
    uint64_t events_processed;
} network_simulator_t;

// Create simulator
network_simulator_t* create_simulator() {
    network_simulator_t *sim = (network_simulator_t*)malloc(sizeof(network_simulator_t));
    sim->event_queue = create_min_heap(10000);
    sim->current_time = 0;
    sim->events_processed = 0;
    return sim;
}

// Schedule event
void schedule_event(network_simulator_t *sim, event_type_t type, 
                   uint64_t time_us, int node_id, void *data) {
    sim_event_t *event = (sim_event_t*)malloc(sizeof(sim_event_t));
    event->type = type;
    event->time_us = time_us;
    event->node_id = node_id;
    event->event_data = data;
    
    heap_insert(sim->event_queue, time_us, event);
}

// Process next event
int process_next_event(network_simulator_t *sim) {
    if (heap_is_empty(sim->event_queue)) {
        return 0;
    }
    
    uint64_t event_time;
    sim_event_t *event = (sim_event_t*)heap_extract_min(sim->event_queue, &event_time);
    
    // Advance simulation time
    sim->current_time = event_time;
    
    // Process event based on type
    const char *event_names[] = {
        "Packet Arrival", "Packet Departure", "Timeout",
        "Link Failure", "Link Recovery"
    };
    
    printf("Time %10lu us: %s at node %d\n",
           event_time, event_names[event->type], event->node_id);
    
    // Event-specific processing would go here
    switch (event->type) {
        case EVENT_PACKET_ARRIVAL:
            // Schedule departure after processing delay
            schedule_event(sim, EVENT_PACKET_DEPARTURE,
                         event_time + 100, event->node_id, NULL);
            break;
            
        case EVENT_TIMEOUT:
            // Schedule retransmission
            schedule_event(sim, EVENT_PACKET_ARRIVAL,
                         event_time + 1000, event->node_id, NULL);
            break;
            
        default:
            break;
    }
    
    free(event);
    sim->events_processed++;
    
    return 1;
}

// Run simulation
void run_simulation(network_simulator_t *sim, uint64_t duration_us) {
    printf("=== Network Simulation ===\n\n");
    printf("Running simulation for %lu microseconds\n\n", duration_us);
    
    // Schedule initial events
    schedule_event(sim, EVENT_PACKET_ARRIVAL, 100, 1, NULL);
    schedule_event(sim, EVENT_PACKET_ARRIVAL, 150, 2, NULL);
    schedule_event(sim, EVENT_TIMEOUT, 500, 1, NULL);
    schedule_event(sim, EVENT_LINK_FAILURE, 1000, 3, NULL);
    schedule_event(sim, EVENT_LINK_RECOVERY, 5000, 3, NULL);
    
    // Process events until duration
    while (!heap_is_empty(sim->event_queue)) {
        uint64_t next_time;
        heap_peek_min(sim->event_queue, &next_time);
        
        if (next_time > duration_us) {
            break;
        }
        
        process_next_event(sim);
    }
    
    printf("\nSimulation complete:\n");
    printf("  Simulation time: %lu us\n", sim->current_time);
    printf("  Events processed: %lu\n", sim->events_processed);
    printf("  Events remaining: %d\n\n", sim->event_queue->size);
}
```

## D-ary Heaps for Better Cache Performance

Binary heaps use two children per node, requiring log₂(n) levels. D-ary heaps generalize this to d children per node, reducing height to log_d(n) at the cost of more comparisons per level. A 4-ary heap reaches 1 million elements in 10 levels versus 20 for binary. This shallower tree means fewer memory accesses—critical when heaps exceed CPU cache size.

The trade-off matters for different workloads. Insertion benefits from fewer levels but requires finding the minimum among d children instead of 2 during heapify-down. For d=4, this means 3 comparisons versus 1. If insertions dominate—typical for network event queues where events arrive faster than processing—4-ary heaps often win. If extractions dominate—typical for batch processing scenarios—binary heaps may perform better. Understanding your access pattern determines the optimal choice.

```c
// D-ary heap (4-ary example)
#define DARY_D 4

typedef struct dary_heap {
    heap_node_t *nodes;
    int capacity;
    int size;
} dary_heap_t;

// D-ary heap parent
static inline int dary_parent(int i) {
    return (i - 1) / DARY_D;
}

// D-ary heap child k (0 to D-1)
static inline int dary_child(int i, int k) {
    return DARY_D * i + k + 1;
}

// Create D-ary heap
dary_heap_t* create_dary_heap(int capacity) {
    dary_heap_t *heap = (dary_heap_t*)malloc(sizeof(dary_heap_t));
    heap->capacity = capacity;
    heap->size = 0;
    heap->nodes = (heap_node_t*)malloc(capacity * sizeof(heap_node_t));
    return heap;
}

// D-ary heapify up
void dary_heapify_up(dary_heap_t *heap, int index) {
    while (index > 0) {
        int parent_idx = dary_parent(index);
        
        if (heap->nodes[index].priority >= heap->nodes[parent_idx].priority) {
            break;
        }
        
        swap_nodes(&heap->nodes[index], &heap->nodes[parent_idx]);
        index = parent_idx;
    }
}

// D-ary heapify down
void dary_heapify_down(dary_heap_t *heap, int index) {
    while (1) {
        int smallest = index;
        
        // Find smallest among node and all children
        for (int k = 0; k < DARY_D; k++) {
            int child_idx = dary_child(index, k);
            
            if (child_idx < heap->size &&
                heap->nodes[child_idx].priority < heap->nodes[smallest].priority) {
                smallest = child_idx;
            }
        }
        
        if (smallest == index) {
            break;
        }
        
        swap_nodes(&heap->nodes[index], &heap->nodes[smallest]);
        index = smallest;
    }
}

// D-ary insert
int dary_insert(dary_heap_t *heap, uint64_t priority, void *data) {
    if (heap->size >= heap->capacity) {
        return 0;
    }
    
    int index = heap->size;
    heap->nodes[index].priority = priority;
    heap->nodes[index].data = data;
    heap->size++;
    
    dary_heapify_up(heap, index);
    return 1;
}

// D-ary extract min
void* dary_extract_min(dary_heap_t *heap, uint64_t *priority) {
    if (heap->size == 0) {
        return NULL;
    }
    
    void *data = heap->nodes[0].data;
    if (priority) {
        *priority = heap->nodes[0].priority;
    }
    
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    if (heap->size > 0) {
        dary_heapify_down(heap, 0);
    }
    
    return data;
}

// Compare binary vs 4-ary heap
void compare_heap_arities() {
    printf("=== Binary Heap vs 4-ary Heap ===\n\n");
    
    int num_ops = 100000;
    
    // Binary heap
    printf("Binary Heap (2 children per node):\n");
    min_heap_t *binary = create_min_heap(num_ops);
    
    clock_t start = clock();
    for (int i = 0; i < num_ops; i++) {
        heap_insert(binary, rand(), NULL);
    }
    clock_t end = clock();
    double binary_insert = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    start = clock();
    for (int i = 0; i < num_ops; i++) {
        heap_extract_min(binary, NULL);
    }
    end = clock();
    double binary_extract = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    printf("  %d insertions: %.3f ms\n", num_ops, binary_insert);
    printf("  %d extractions: %.3f ms\n", num_ops, binary_extract);
    printf("  Tree height: %d levels\n", (int)(log(num_ops) / log(2)) + 1);
    
    // 4-ary heap
    printf("\n4-ary Heap (4 children per node):\n");
    dary_heap_t *dary = create_dary_heap(num_ops);
    
    start = clock();
    for (int i = 0; i < num_ops; i++) {
        dary_insert(dary, rand(), NULL);
    }
    end = clock();
    double dary_insert = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    start = clock();
    for (int i = 0; i < num_ops; i++) {
        dary_extract_min(dary, NULL);
    }
    end = clock();
    double dary_extract = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    printf("  %d insertions: %.3f ms (%.1fx)\n", num_ops, dary_insert,
           binary_insert / dary_insert);
    printf("  %d extractions: %.3f ms (%.1fx)\n", num_ops, dary_extract,
           binary_extract / dary_extract);
    printf("  Tree height: %d levels\n", (int)(log(num_ops) / log(4)) + 1);
    
    printf("\nCache efficiency:\n");
    printf("  Binary heap: Fewer comparisons per level\n");
    printf("  4-ary heap: Fewer levels, better locality\n");
    printf("  4-ary typically wins for large heaps exceeding L1 cache\n\n");
    
    free(binary->nodes);
    free(binary);
    free(dary->nodes);
    free(dary);
}
```

## Fibonacci Heaps: Theoretical Optimum

Fibonacci heaps achieve O(1) amortized time for insertion and decrease-key operations, with O(log n) for extract-min. This theoretical advantage makes them optimal for algorithms like Dijkstra's that perform many decrease-key operations. However, the complex structure—involving circular linked lists, cascading cuts, and consolidation—introduces significant constant factors and poor cache behavior.

In practice, Fibonacci heaps rarely outperform binary or d-ary heaps for networking applications. The implementation complexity and cache-unfriendly memory layout offset the asymptotic advantages. Binary heaps with their simple array structure and excellent locality dominate real network code. Fibonacci heaps matter more for theoretical algorithm analysis than production systems. Understanding this teaches an important lesson: asymptotic complexity doesn't always predict real-world performance.

```c
// Note: Full Fibonacci heap implementation is complex (~500 lines)
// Here we show the key structures and operations conceptually

typedef struct fib_node {
    uint64_t priority;
    void *data;
    int degree;                    // Number of children
    int marked;                    // For cascading cut
    struct fib_node *parent;
    struct fib_node *child;
    struct fib_node *left;         // Circular linked list
    struct fib_node *right;
} fib_node_t;

typedef struct fib_heap {
    fib_node_t *min;
    int num_nodes;
} fib_heap_t;

// Why Fibonacci heaps matter (theoretical perspective)
void explain_fibonacci_heaps() {
    printf("=== Fibonacci Heaps: Theory vs Practice ===\n\n");
    
    printf("Theoretical Complexity:\n");
    printf("  Operation     | Binary Heap | Fibonacci Heap\n");
    printf("  --------------|-------------|---------------\n");
    printf("  Insert        | O(log n)    | O(1) amortized\n");
    printf("  Extract-min   | O(log n)    | O(log n) amortized\n");
    printf("  Decrease-key  | O(log n)    | O(1) amortized\n");
    printf("  Delete        | O(log n)    | O(log n) amortized\n");
    printf("\n");
    
    printf("Why Fibonacci heaps rarely win in practice:\n\n");
    
    printf("1. Constant Factors:\n");
    printf("   - Complex pointer manipulation\n");
    printf("   - Multiple memory allocations\n");
    printf("   - Cascading cut operations\n\n");
    
    printf("2. Cache Performance:\n");
    printf("   - Scattered memory layout\n");
    printf("   - Pointer chasing through linked lists\n");
    printf("   - Binary heap: array with perfect locality\n\n");
    
    printf("3. Implementation Complexity:\n");
    printf("   - Binary heap: ~100 lines of simple code\n");
    printf("   - Fibonacci heap: ~500 lines with many edge cases\n\n");
    
    printf("When Fibonacci heaps might matter:\n");
    printf("   - Algorithms with MANY decrease-key operations\n");
    printf("   - Theoretical algorithm analysis\n");
    printf("   - Academic interest\n\n");
    
    printf("Recommendation for networking:\n");
    printf("   Use binary or d-ary heaps. Simple, fast, cache-friendly.\n\n");
}
```

## Conclusion

Heaps provide the priority queue abstraction essential for network systems managing prioritized or time-ordered events. From QoS packet scheduling to timer management to event-driven simulation, heaps deliver logarithmic-time operations with simple array-based structures that maximize cache efficiency. The choice between binary heaps, d-ary heaps, and alternative structures like timer wheels depends on workload characteristics: insertion frequency, extraction frequency, time precision requirements, and memory access patterns.

Understanding heap implementation details matters because network systems operate at scale where constant factors determine whether you achieve line-rate packet processing or drop frames under load. A poorly chosen priority queue implementation can throttle a router's QoS scheduler or cause TCP timers to consume excessive CPU. Master heaps, understand their trade-offs, and you possess a fundamental tool for building responsive network systems that scale from hundreds to millions of prioritized events without sacrificing performance.

# Bloom Filters: Fast Membership Testing in Networks

## The Space-Time Trade-off for Set Membership

Network devices constantly face membership queries: Has this IP address been seen before? Is this flow already tracked? Did we receive this packet sequence number? Has this route been advertised? A hash table answers these questions with certainty but consumes memory proportional to the number of elements stored. For tracking millions of flows, IP addresses, or packet identifiers, even a compact hash table might require hundreds of megabytes.

Bloom filters trade absolute certainty for dramatic space savings. They answer "definitely no" or "probably yes" using a bit array and multiple hash functions. When checking if an element exists, a Bloom filter never produces false negatives—if it says no, the element is genuinely absent. However, it may produce false positives—saying yes when the element wasn't actually inserted. This probabilistic behavior enables representing a set of one million elements in just a few hundred kilobytes instead of tens of megabytes, accepting perhaps a 1% false positive rate as the cost of this compression.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

// Bloom filter structure
typedef struct bloom_filter {
    uint8_t *bit_array;           // Bit array for membership
    uint64_t size;                // Size in bits
    int num_hash_functions;       // Number of hash functions (k)
    uint64_t num_elements;        // Number of elements inserted
} bloom_filter_t;

// Create Bloom filter
// n = expected number of elements
// p = desired false positive probability
bloom_filter_t* create_bloom_filter(uint64_t n, double p) {
    bloom_filter_t *bf = (bloom_filter_t*)malloc(sizeof(bloom_filter_t));
    
    // Optimal size: m = -(n * ln(p)) / (ln(2)^2)
    bf->size = (uint64_t)(-(n * log(p)) / (log(2) * log(2)));
    
    // Optimal number of hash functions: k = (m/n) * ln(2)
    bf->num_hash_functions = (int)((double)bf->size / n * log(2));
    
    // Ensure at least 1 hash function
    if (bf->num_hash_functions < 1) {
        bf->num_hash_functions = 1;
    }
    
    // Allocate bit array (round up to bytes)
    uint64_t bytes = (bf->size + 7) / 8;
    bf->bit_array = (uint8_t*)calloc(bytes, sizeof(uint8_t));
    
    bf->num_elements = 0;
    
    printf("Bloom filter created:\n");
    printf("  Expected elements: %lu\n", n);
    printf("  Target FP rate: %.4f\n", p);
    printf("  Bit array size: %lu bits (%lu bytes)\n", bf->size, bytes);
    printf("  Hash functions: %d\n", bf->num_hash_functions);
    printf("  Bits per element: %.2f\n\n", (double)bf->size / n);
    
    return bf;
}

// Hash function using MurmurHash-inspired mixing
uint64_t bloom_hash(uint64_t key, int seed) {
    key ^= seed;
    key ^= key >> 33;
    key *= 0xff51afd7ed558ccdULL;
    key ^= key >> 33;
    key *= 0xc4ceb9fe1a85ec53ULL;
    key ^= key >> 33;
    return key;
}

// Double hashing: h_i(x) = h1(x) + i * h2(x)
uint64_t bloom_hash_i(uint64_t key, int i, uint64_t size) {
    uint64_t h1 = bloom_hash(key, 0);
    uint64_t h2 = bloom_hash(key, 1);
    return (h1 + i * h2) % size;
}

// Set bit in bit array
static inline void set_bit(uint8_t *array, uint64_t index) {
    array[index / 8] |= (1 << (index % 8));
}

// Get bit from bit array
static inline int get_bit(uint8_t *array, uint64_t index) {
    return (array[index / 8] >> (index % 8)) & 1;
}

// Insert element into Bloom filter
void bloom_insert(bloom_filter_t *bf, uint64_t key) {
    for (int i = 0; i < bf->num_hash_functions; i++) {
        uint64_t index = bloom_hash_i(key, i, bf->size);
        set_bit(bf->bit_array, index);
    }
    bf->num_elements++;
}

// Check if element might be in Bloom filter
int bloom_check(bloom_filter_t *bf, uint64_t key) {
    for (int i = 0; i < bf->num_hash_functions; i++) {
        uint64_t index = bloom_hash_i(key, i, bf->size);
        if (!get_bit(bf->bit_array, index)) {
            return 0;  // Definitely not present
        }
    }
    return 1;  // Probably present
}

// Get current false positive probability
double bloom_false_positive_rate(bloom_filter_t *bf) {
    // Approximate: (1 - e^(-kn/m))^k
    double exponent = -(double)(bf->num_hash_functions * bf->num_elements) / bf->size;
    return pow(1 - exp(exponent), bf->num_hash_functions);
}

// Calculate bits set in filter
uint64_t bloom_bits_set(bloom_filter_t *bf) {
    uint64_t count = 0;
    uint64_t bytes = (bf->size + 7) / 8;
    
    for (uint64_t i = 0; i < bytes; i++) {
        uint8_t byte = bf->bit_array[i];
        // Count bits using Brian Kernighan's algorithm
        while (byte) {
            byte &= byte - 1;
            count++;
        }
    }
    
    return count;
}

// Print Bloom filter statistics
void bloom_print_stats(bloom_filter_t *bf) {
    uint64_t bits_set = bloom_bits_set(bf);
    double fill_ratio = (double)bits_set / bf->size;
    double actual_fp_rate = bloom_false_positive_rate(bf);
    
    printf("Bloom Filter Statistics:\n");
    printf("  Elements inserted: %lu\n", bf->num_elements);
    printf("  Bits set: %lu / %lu (%.2f%%)\n", 
           bits_set, bf->size, fill_ratio * 100);
    printf("  Estimated FP rate: %.6f\n", actual_fp_rate);
    printf("  Memory usage: %lu bytes\n\n", (bf->size + 7) / 8);
}

// Demonstrate basic Bloom filter operations
void demonstrate_bloom_basics() {
    printf("=== Bloom Filter Basics ===\n\n");
    
    // Create filter for 10,000 elements with 1% FP rate
    bloom_filter_t *bf = create_bloom_filter(10000, 0.01);
    
    printf("Inserting 10,000 elements...\n");
    for (uint64_t i = 0; i < 10000; i++) {
        bloom_insert(bf, i);
    }
    
    bloom_print_stats(bf);
    
    printf("Testing membership:\n");
    
    // Test inserted elements (should all return true)
    int false_negatives = 0;
    for (uint64_t i = 0; i < 10000; i++) {
        if (!bloom_check(bf, i)) {
            false_negatives++;
        }
    }
    printf("  Inserted elements: %d / 10000 found (FN: %d)\n", 
           10000 - false_negatives, false_negatives);
    
    // Test non-inserted elements (some will return false positives)
    int false_positives = 0;
    for (uint64_t i = 10000; i < 20000; i++) {
        if (bloom_check(bf, i)) {
            false_positives++;
        }
    }
    printf("  Non-inserted elements: %d / 10000 false positives (%.2f%%)\n\n",
           false_positives, (double)false_positives / 10000 * 100);
    
    free(bf->bit_array);
    free(bf);
}
```

## Hash Functions and Optimal Parameters

A Bloom filter uses k independent hash functions to map each element to k bit positions. Implementing k truly independent hash functions wastes computation. The double hashing technique generates k hash values from two base hashes: h_i(x) = h1(x) + i * h2(x). This provides sufficient independence while requiring only two hash computations per operation.

The filter's parameters—bit array size m and number of hash functions k—determine its accuracy. For n expected elements and desired false positive probability p, optimal size is m = -(n ln p)/(ln 2)². Optimal k is (m/n) ln 2. These formulas balance filter size against false positive rate. A filter for one million elements with 1% false positive rate requires approximately 9.6 million bits (1.2 megabytes) with 7 hash functions. Accepting 5% false positives reduces this to 6.2 million bits (780 kilobytes) with 4 hash functions.

```c
// Optimal parameter calculation
typedef struct bloom_params {
    uint64_t size_bits;
    uint64_t size_bytes;
    int num_hashes;
    double bits_per_element;
} bloom_params_t;

bloom_params_t calculate_bloom_params(uint64_t n, double p) {
    bloom_params_t params;
    
    // Optimal bit array size
    params.size_bits = (uint64_t)(-(n * log(p)) / (log(2) * log(2)));
    params.size_bytes = (params.size_bits + 7) / 8;
    
    // Optimal number of hash functions
    params.num_hashes = (int)((double)params.size_bits / n * log(2));
    if (params.num_hashes < 1) params.num_hashes = 1;
    
    params.bits_per_element = (double)params.size_bits / n;
    
    return params;
}

// Compare different false positive rates
void compare_false_positive_rates() {
    printf("=== False Positive Rate Trade-offs ===\n\n");
    
    uint64_t n = 1000000;  // 1 million elements
    double rates[] = {0.001, 0.005, 0.01, 0.05, 0.10};
    
    printf("For %lu elements:\n\n", n);
    printf("FP Rate | Memory (MB) | Hashes | Bits/Elem\n");
    printf("--------|-------------|--------|----------\n");
    
    for (int i = 0; i < 5; i++) {
        bloom_params_t params = calculate_bloom_params(n, rates[i]);
        printf("%.3f   | %-11.2f | %-6d | %.2f\n",
               rates[i],
               params.size_bytes / (1024.0 * 1024.0),
               params.num_hashes,
               params.bits_per_element);
    }
    
    printf("\nKey insight: Tighter FP bounds require more memory\n");
    printf("  0.1%% FP: 14.4 bits/element\n");
    printf("  1%%   FP:  9.6 bits/element\n");
    printf("  10%%  FP:  4.8 bits/element\n\n");
}

// Demonstrate parameter tuning
void demonstrate_parameter_tuning() {
    printf("=== Parameter Tuning Example ===\n\n");
    
    uint64_t n = 100000;
    
    printf("Scenario: Track 100K flows\n\n");
    
    // Conservative: 0.1% FP rate
    printf("Conservative approach (0.1%% FP):\n");
    bloom_filter_t *bf_conservative = create_bloom_filter(n, 0.001);
    
    // Balanced: 1% FP rate
    printf("Balanced approach (1%% FP):\n");
    bloom_filter_t *bf_balanced = create_bloom_filter(n, 0.01);
    
    // Aggressive: 5% FP rate
    printf("Aggressive approach (5%% FP):\n");
    bloom_filter_t *bf_aggressive = create_bloom_filter(n, 0.05);
    
    printf("Trade-off analysis:\n");
    printf("  Conservative: Lowest FP rate, highest memory/CPU\n");
    printf("  Balanced: Good compromise for most applications\n");
    printf("  Aggressive: Minimal resources, tolerable FP rate\n\n");
    
    printf("Choice depends on:\n");
    printf("  - Cost of false positives in your application\n");
    printf("  - Available memory\n");
    printf("  - Performance requirements\n\n");
    
    free(bf_conservative->bit_array);
    free(bf_conservative);
    free(bf_balanced->bit_array);
    free(bf_balanced);
    free(bf_aggressive->bit_array);
    free(bf_aggressive);
}
```

## DDoS Detection and Flow Tracking

Network security systems use Bloom filters to track suspicious IP addresses, detect scanning attempts, or identify flows requiring deeper inspection. When a firewall sees a SYN packet, it checks whether the source IP appears in a Bloom filter of known attackers. A negative response guarantees the IP is clean, passing it through immediately. A positive response triggers additional verification—perhaps checking a more authoritative but slower hash table.

The false positive rate determines how often clean traffic undergoes unnecessary verification. With 1% false positives, 1 in 100 legitimate connections suffers a performance penalty. But the filter requires only a few megabytes to track millions of IP addresses, compared to hundreds of megabytes for a complete hash table. For DDoS mitigation devices processing millions of packets per second, this space efficiency proves essential.

```c
// IP address tracking for security
typedef struct ip_tracker {
    bloom_filter_t *suspicious_ips;
    bloom_filter_t *blocked_ips;
    uint64_t packets_checked;
    uint64_t packets_flagged;
    uint64_t packets_blocked;
} ip_tracker_t;

// Create IP tracker
ip_tracker_t* create_ip_tracker(uint64_t expected_suspicious, 
                                uint64_t expected_blocked) {
    ip_tracker_t *tracker = (ip_tracker_t*)malloc(sizeof(ip_tracker_t));
    
    // Suspicious IPs: Accept 5% FP (not critical, just causes extra checking)
    tracker->suspicious_ips = create_bloom_filter(expected_suspicious, 0.05);
    
    // Blocked IPs: Want 0.1% FP (very important to not block legitimate traffic)
    tracker->blocked_ips = create_bloom_filter(expected_blocked, 0.001);
    
    tracker->packets_checked = 0;
    tracker->packets_flagged = 0;
    tracker->packets_blocked = 0;
    
    return tracker;
}

// Mark IP as suspicious
void track_suspicious_ip(ip_tracker_t *tracker, uint32_t ip) {
    bloom_insert(tracker->suspicious_ips, ip);
}

// Mark IP as blocked
void track_blocked_ip(ip_tracker_t *tracker, uint32_t ip) {
    bloom_insert(tracker->blocked_ips, ip);
}

// Check packet and take action
typedef enum {
    ACTION_ALLOW,
    ACTION_INSPECT,
    ACTION_BLOCK
} packet_action_t;

packet_action_t check_packet(ip_tracker_t *tracker, uint32_t src_ip) {
    tracker->packets_checked++;
    
    // First check blocked list
    if (bloom_check(tracker->blocked_ips, src_ip)) {
        tracker->packets_blocked++;
        return ACTION_BLOCK;
    }
    
    // Then check suspicious list
    if (bloom_check(tracker->suspicious_ips, src_ip)) {
        tracker->packets_flagged++;
        return ACTION_INSPECT;
    }
    
    return ACTION_ALLOW;
}

// Simulate DDoS detection
void simulate_ddos_detection() {
    printf("=== DDoS Detection with Bloom Filters ===\n\n");
    
    // Create tracker for 100K suspicious, 10K blocked
    ip_tracker_t *tracker = create_ip_tracker(100000, 10000);
    
    printf("Simulating attack scenario:\n\n");
    
    // Add some blocked IPs (known attackers)
    printf("Adding 5,000 known attacker IPs to blocklist...\n");
    for (uint32_t i = 0; i < 5000; i++) {
        uint32_t attacker_ip = 0xC0000000 | i;  // 192.0.x.x range
        track_blocked_ip(tracker, attacker_ip);
    }
    
    // Add suspicious IPs (scan attempts)
    printf("Adding 50,000 suspicious IPs to watchlist...\n\n");
    for (uint32_t i = 0; i < 50000; i++) {
        uint32_t suspicious_ip = 0x0A000000 | i;  // 10.0.x.x range
        track_suspicious_ip(tracker, suspicious_ip);
    }
    
    bloom_print_stats(tracker->suspicious_ips);
    bloom_print_stats(tracker->blocked_ips);
    
    // Simulate packet processing
    printf("Processing 100,000 packets:\n");
    
    int allowed = 0, inspected = 0, blocked = 0;
    
    for (int i = 0; i < 100000; i++) {
        uint32_t src_ip = rand();
        
        packet_action_t action = check_packet(tracker, src_ip);
        switch (action) {
            case ACTION_ALLOW: allowed++; break;
            case ACTION_INSPECT: inspected++; break;
            case ACTION_BLOCK: blocked++; break;
        }
    }
    
    printf("  Allowed: %d\n", allowed);
    printf("  Flagged for inspection: %d\n", inspected);
    printf("  Blocked: %d\n\n", blocked);
    
    printf("Performance impact:\n");
    printf("  %.2f%% of packets require deep inspection\n",
           (double)inspected / 100000 * 100);
    printf("  %.2f%% of packets blocked immediately\n",
           (double)blocked / 100000 * 100);
    printf("  %.2f%% of packets fast-path allowed\n\n",
           (double)allowed / 100000 * 100);
    
    free(tracker->suspicious_ips->bit_array);
    free(tracker->suspicious_ips);
    free(tracker->blocked_ips->bit_array);
    free(tracker->blocked_ips);
    free(tracker);
}
```

## Counting Bloom Filters

Standard Bloom filters support only insertions and queries—not deletions. Once a bit is set, you cannot determine which element set it, preventing safe removal. Counting Bloom filters replace each bit with a small counter, typically 4 bits. Insertion increments counters; deletion decrements them. A counter reaching zero indicates no element maps to that position.

The counter size creates a trade-off. Four-bit counters allow values 0-15, rarely overflowing for reasonable load factors. They quadruple memory consumption compared to standard Bloom filters. Eight-bit counters eliminate overflow concerns but octuple memory usage. For network applications requiring deletion—tracking active flows that eventually close, monitoring temporary blacklists that expire—counting Bloom filters provide the necessary functionality at acceptable memory cost.

```c
// Counting Bloom filter
#define COUNTER_BITS 4
#define COUNTER_MAX ((1 << COUNTER_BITS) - 1)

typedef struct counting_bloom_filter {
    uint8_t *counters;            // Array of 4-bit counters
    uint64_t size;                // Size in counters
    int num_hash_functions;
    uint64_t num_elements;
} counting_bloom_filter_t;

// Create counting Bloom filter
counting_bloom_filter_t* create_counting_bloom(uint64_t n, double p) {
    counting_bloom_filter_t *cbf = 
        (counting_bloom_filter_t*)malloc(sizeof(counting_bloom_filter_t));
    
    // Calculate optimal size
    cbf->size = (uint64_t)(-(n * log(p)) / (log(2) * log(2)));
    cbf->num_hash_functions = (int)((double)cbf->size / n * log(2));
    if (cbf->num_hash_functions < 1) cbf->num_hash_functions = 1;
    
    // Allocate counter array (2 counters per byte)
    uint64_t bytes = (cbf->size + 1) / 2;
    cbf->counters = (uint8_t*)calloc(bytes, sizeof(uint8_t));
    
    cbf->num_elements = 0;
    
    printf("Counting Bloom filter created:\n");
    printf("  Size: %lu counters (%lu bytes)\n", cbf->size, bytes);
    printf("  Hash functions: %d\n", cbf->num_hash_functions);
    printf("  Memory overhead: 4x standard Bloom filter\n\n");
    
    return cbf;
}

// Get counter value
static inline uint8_t get_counter(counting_bloom_filter_t *cbf, uint64_t index) {
    uint64_t byte_index = index / 2;
    int is_upper = index % 2;
    
    if (is_upper) {
        return cbf->counters[byte_index] >> 4;
    } else {
        return cbf->counters[byte_index] & 0x0F;
    }
}

// Set counter value
static inline void set_counter(counting_bloom_filter_t *cbf, 
                              uint64_t index, uint8_t value) {
    uint64_t byte_index = index / 2;
    int is_upper = index % 2;
    
    if (is_upper) {
        cbf->counters[byte_index] = (cbf->counters[byte_index] & 0x0F) | (value << 4);
    } else {
        cbf->counters[byte_index] = (cbf->counters[byte_index] & 0xF0) | (value & 0x0F);
    }
}

// Insert into counting Bloom filter
void counting_bloom_insert(counting_bloom_filter_t *cbf, uint64_t key) {
    for (int i = 0; i < cbf->num_hash_functions; i++) {
        uint64_t index = bloom_hash_i(key, i, cbf->size);
        uint8_t counter = get_counter(cbf, index);
        
        if (counter < COUNTER_MAX) {
            set_counter(cbf, index, counter + 1);
        }
    }
    cbf->num_elements++;
}

// Delete from counting Bloom filter
void counting_bloom_delete(counting_bloom_filter_t *cbf, uint64_t key) {
    for (int i = 0; i < cbf->num_hash_functions; i++) {
        uint64_t index = bloom_hash_i(key, i, cbf->size);
        uint8_t counter = get_counter(cbf, index);
        
        if (counter > 0) {
            set_counter(cbf, index, counter - 1);
        }
    }
    if (cbf->num_elements > 0) {
        cbf->num_elements--;
    }
}

// Check membership in counting Bloom filter
int counting_bloom_check(counting_bloom_filter_t *cbf, uint64_t key) {
    for (int i = 0; i < cbf->num_hash_functions; i++) {
        uint64_t index = bloom_hash_i(key, i, cbf->size);
        if (get_counter(cbf, index) == 0) {
            return 0;  // Definitely not present
        }
    }
    return 1;  // Probably present
}

// Demonstrate counting Bloom filter
void demonstrate_counting_bloom() {
    printf("=== Counting Bloom Filter for Flow Tracking ===\n\n");
    
    counting_bloom_filter_t *cbf = create_counting_bloom(10000, 0.01);
    
    printf("Simulating flow lifecycle:\n\n");
    
    // Insert flows
    printf("Opening 1000 flows...\n");
    for (uint64_t i = 0; i < 1000; i++) {
        counting_bloom_insert(cbf, i);
    }
    
    printf("Active flows: %lu\n", cbf->num_elements);
    
    // Check presence
    int found = 0;
    for (uint64_t i = 0; i < 1000; i++) {
        if (counting_bloom_check(cbf, i)) {
            found++;
        }
    }
    printf("Flows found in filter: %d / 1000\n\n", found);
    
    // Delete some flows
    printf("Closing 500 flows...\n");
    for (uint64_t i = 0; i < 500; i++) {
        counting_bloom_delete(cbf, i);
    }
    
    printf("Active flows: %lu\n", cbf->num_elements);
    
    // Verify deletion
    int still_found = 0;
    for (uint64_t i = 0; i < 500; i++) {
        if (counting_bloom_check(cbf, i)) {
            still_found++;
        }
    }
    printf("Deleted flows still showing (false positives): %d / 500\n\n", 
           still_found);
    
    int remaining = 0;
    for (uint64_t i = 500; i < 1000; i++) {
        if (counting_bloom_check(cbf, i)) {
            remaining++;
        }
    }
    printf("Remaining flows found: %d / 500\n\n", remaining);
    
    free(cbf->counters);
    free(cbf);
}
```

## Bloom Filter Alternatives and Variants

Cuckoo filters provide an alternative to Bloom filters with similar space efficiency but support for deletion. They store fingerprints of elements in a cuckoo hash table, enabling removal while maintaining low false positive rates. Quotient filters offer another alternative with better cache locality through a compact representation. Each structure trades different aspects: Bloom filters optimize for simplicity and minimum memory, cuckoo filters add deletion support, quotient filters improve cache performance.

For networking applications, the choice depends on requirements. Simple presence testing without deletion: standard Bloom filter. Need deletion with acceptable memory overhead: counting Bloom filter or cuckoo filter. Require high-performance lookups with good cache behavior: quotient filter. Understanding these trade-offs allows selecting the optimal structure for each use case rather than defaulting to whatever seems familiar.

```c
// Compare Bloom filter variants
void compare_bloom_variants() {
    printf("=== Bloom Filter Variants Comparison ===\n\n");
    
    uint64_t n = 1000000;
    double p = 0.01;
    
    printf("For %lu elements, 1%% FP rate:\n\n", n);
    
    // Standard Bloom filter
    bloom_params_t standard = calculate_bloom_params(n, p);
    printf("Standard Bloom Filter:\n");
    printf("  Memory: %.2f MB\n", standard.size_bytes / (1024.0 * 1024.0));
    printf("  Features: Insert, Query\n");
    printf("  Performance: Fastest queries\n");
    printf("  Best for: Simple membership testing\n\n");
    
    // Counting Bloom filter
    uint64_t counting_bytes = standard.size_bytes * 4;  // 4x for 4-bit counters
    printf("Counting Bloom Filter:\n");
    printf("  Memory: %.2f MB\n", counting_bytes / (1024.0 * 1024.0));
    printf("  Features: Insert, Query, Delete\n");
    printf("  Performance: Same as standard\n");
    printf("  Best for: Tracking with expiration\n\n");
    
    // Cuckoo filter (approximate sizing)
    uint64_t cuckoo_bytes = n * 1;  // ~8-16 bits per element typical
    printf("Cuckoo Filter:\n");
    printf("  Memory: %.2f MB\n", cuckoo_bytes / (1024.0 * 1024.0));
    printf("  Features: Insert, Query, Delete\n");
    printf("  Performance: Good\n");
    printf("  Best for: Deletion with lower overhead\n\n");
    
    // Quotient filter
    printf("Quotient Filter:\n");
    printf("  Memory: %.2f MB\n", standard.size_bytes / (1024.0 * 1024.0));
    printf("  Features: Insert, Query, Delete\n");
    printf("  Performance: Better cache locality\n");
    printf("  Best for: High-performance lookups\n\n");
    
    printf("Selection guide:\n");
    printf("  Simple & Fast → Standard Bloom\n");
    printf("  Need Delete → Counting Bloom or Cuckoo\n");
    printf("  Cache-sensitive → Quotient\n\n");
}

// Real-world space comparison
void space_comparison_real_world() {
    printf("=== Real-World Space Comparison ===\n\n");
    
    uint64_t n = 10000000;  // 10 million flows
    
    printf("Scenario: Track 10 million active network flows\n\n");
    
    // Hash table approach
    size_t hash_entry = 48;  // 5-tuple + metadata
    size_t hash_overhead = 16;  // Pointers, etc.
    size_t hash_total = n * (hash_entry + hash_overhead);
    
    printf("Hash Table:\n");
    printf("  Memory: %.2f MB\n", hash_total / (1024.0 * 1024.0));
    printf("  Per-flow: %zu bytes\n", hash_entry + hash_overhead);
    printf("  Features: Full data storage\n\n");
    
    // Bloom filter approach
    bloom_params_t bloom = calculate_bloom_params(n, 0.01);
    
    printf("Bloom Filter (1%% FP):\n");
    printf("  Memory: %.2f MB\n", bloom.size_bytes / (1024.0 * 1024.0));
    printf("  Per-flow: %.2f bytes\n", (double)bloom.size_bytes / n);
    printf("  Features: Membership only\n");
    printf("  Space savings: %.1fx\n\n", 
           (double)hash_total / bloom.size_bytes);
    
    printf("Hybrid approach (common in practice):\n");
    printf("  - Bloom filter for fast negative lookups\n");
    printf("  - Hash table for positive hits only\n");
    printf("  - Effective memory = Bloom + (actual_flows * hash_size)\n");
    printf("  - If 90%% queries are negatives, huge savings\n\n");
}
```

## Conclusion

Bloom filters sacrifice perfect accuracy for dramatic space savings, making them indispensable for network applications dealing with massive sets where false positives are tolerable. From DDoS protection tracking millions of IP addresses to flow monitoring identifying active connections to duplicate detection preventing redundant operations, Bloom filters enable operations that would be impractical with exact data structures.

Understanding when to use Bloom filters requires assessing the cost of false positives in your specific application. A security system might tolerate 1% false positives causing unnecessary inspection, accepting this overhead for 10x memory savings. A routing system cannot tolerate false positives in reachability information and must use exact structures. Master Bloom filters, understand their probabilistic guarantees, and you possess a powerful tool for building space-efficient network systems that scale to millions of elements while maintaining performance.

# LRU and LFU Caches in Networking

## The Caching Problem in Network Systems

Network devices perform expensive lookups repeatedly: routing table searches consuming hundreds of CPU cycles, ACL evaluations requiring multiple memory accesses, MAC address resolution involving hash table lookups. When destination 8.8.8.8 appears in thousands of consecutive packets, looking up the route every time wastes resources. A cache remembers recent lookup results, trading memory for speed by storing a subset of frequently accessed entries in a fast-access structure.

The challenge lies in deciding which entries to keep when the cache fills. Least Recently Used (LRU) eviction assumes recent accesses predict future accesses—a packet to 8.8.8.8 now likely precedes more packets to 8.8.8.8 soon. Least Frequently Used (LFU) assumes access frequency predicts future need—destinations contacted thousands of times matter more than those hit once. Real network traffic exhibits both temporal locality and frequency skew, making cache replacement policy selection critical for performance.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Cache entry structure
typedef struct cache_entry {
    uint32_t key;                 // Lookup key (e.g., IP address)
    void *value;                  // Cached data (e.g., route info)
    uint64_t timestamp;           // For LRU
    uint32_t access_count;        // For LFU
    struct cache_entry *prev;     // Doubly-linked list
    struct cache_entry *next;
} cache_entry_t;

// LRU cache structure
typedef struct lru_cache {
    cache_entry_t **hash_table;   // Hash table for O(1) lookup
    cache_entry_t *head;          // Most recently used
    cache_entry_t *tail;          // Least recently used
    int capacity;
    int size;
    uint64_t hits;
    uint64_t misses;
    uint64_t evictions;
} lru_cache_t;

// Hash function for cache
uint32_t cache_hash(uint32_t key, int table_size) {
    key ^= key >> 16;
    key *= 0x85ebca6b;
    key ^= key >> 13;
    key *= 0xc2b2ae35;
    key ^= key >> 16;
    return key % table_size;
}

// Create LRU cache
lru_cache_t* create_lru_cache(int capacity) {
    lru_cache_t *cache = (lru_cache_t*)malloc(sizeof(lru_cache_t));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->hits = 0;
    cache->misses = 0;
    cache->evictions = 0;
    
    // Hash table for fast lookup (2x capacity to reduce collisions)
    cache->hash_table = (cache_entry_t**)calloc(capacity * 2, sizeof(cache_entry_t*));
    
    return cache;
}

// Move entry to head (mark as most recently used)
void lru_move_to_head(lru_cache_t *cache, cache_entry_t *entry) {
    if (entry == cache->head) {
        return;  // Already at head
    }
    
    // Remove from current position
    if (entry->prev) {
        entry->prev->next = entry->next;
    }
    if (entry->next) {
        entry->next->prev = entry->prev;
    }
    if (entry == cache->tail) {
        cache->tail = entry->prev;
    }
    
    // Insert at head
    entry->prev = NULL;
    entry->next = cache->head;
    if (cache->head) {
        cache->head->prev = entry;
    }
    cache->head = entry;
    
    if (cache->tail == NULL) {
        cache->tail = entry;
    }
}

// Remove entry from LRU list and hash table
void lru_remove_entry(lru_cache_t *cache, cache_entry_t *entry) {
    // Remove from LRU list
    if (entry->prev) {
        entry->prev->next = entry->next;
    }
    if (entry->next) {
        entry->next->prev = entry->prev;
    }
    if (entry == cache->head) {
        cache->head = entry->next;
    }
    if (entry == cache->tail) {
        cache->tail = entry->prev;
    }
    
    // Remove from hash table
    int hash_index = cache_hash(entry->key, cache->capacity * 2);
    cache_entry_t **slot = &cache->hash_table[hash_index];
    
    while (*slot && *slot != entry) {
        slot = &(*slot)->next;
    }
    
    if (*slot == entry) {
        *slot = entry->next;
    }
}

// LRU cache get operation
void* lru_get(lru_cache_t *cache, uint32_t key) {
    int hash_index = cache_hash(key, cache->capacity * 2);
    cache_entry_t *entry = cache->hash_table[hash_index];
    
    // Search in hash bucket
    while (entry) {
        if (entry->key == key) {
            // Cache hit
            cache->hits++;
            lru_move_to_head(cache, entry);
            return entry->value;
        }
        entry = entry->next;
    }
    
    // Cache miss
    cache->misses++;
    return NULL;
}

// LRU cache put operation
void lru_put(lru_cache_t *cache, uint32_t key, void *value) {
    int hash_index = cache_hash(key, cache->capacity * 2);
    
    // Check if key already exists
    cache_entry_t *entry = cache->hash_table[hash_index];
    while (entry) {
        if (entry->key == key) {
            // Update existing entry
            entry->value = value;
            lru_move_to_head(cache, entry);
            return;
        }
        entry = entry->next;
    }
    
    // Create new entry
    cache_entry_t *new_entry = (cache_entry_t*)malloc(sizeof(cache_entry_t));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->timestamp = time(NULL);
    new_entry->access_count = 1;
    new_entry->prev = NULL;
    new_entry->next = NULL;
    
    // Check if cache is full
    if (cache->size >= cache->capacity) {
        // Evict LRU entry (tail)
        cache_entry_t *lru_entry = cache->tail;
        lru_remove_entry(cache, lru_entry);
        free(lru_entry);
        cache->evictions++;
        cache->size--;
    }
    
    // Insert new entry at head
    new_entry->next = cache->hash_table[hash_index];
    cache->hash_table[hash_index] = new_entry;
    
    lru_move_to_head(cache, new_entry);
    cache->size++;
}

// Print LRU cache statistics
void lru_print_stats(lru_cache_t *cache) {
    uint64_t total = cache->hits + cache->misses;
    double hit_rate = total > 0 ? (double)cache->hits / total * 100.0 : 0.0;
    
    printf("LRU Cache Statistics:\n");
    printf("  Capacity: %d entries\n", cache->capacity);
    printf("  Current size: %d entries\n", cache->size);
    printf("  Hits: %lu\n", cache->hits);
    printf("  Misses: %lu\n", cache->misses);
    printf("  Hit rate: %.2f%%\n", hit_rate);
    printf("  Evictions: %lu\n\n", cache->evictions);
}

// Demonstrate basic LRU cache
void demonstrate_lru_basics() {
    printf("=== LRU Cache Basics ===\n\n");
    
    lru_cache_t *cache = create_lru_cache(5);
    
    printf("Cache capacity: 5 entries\n\n");
    
    printf("Inserting entries:\n");
    for (int i = 1; i <= 5; i++) {
        uint32_t key = i * 100;
        int *value = (int*)malloc(sizeof(int));
        *value = i;
        lru_put(cache, key, value);
        printf("  PUT key=%u, value=%d\n", key, i);
    }
    
    printf("\nCache is full. Accessing entry 200:\n");
    void *result = lru_get(cache, 200);
    if (result) {
        printf("  GET key=200 -> HIT (value=%d)\n", *(int*)result);
    }
    
    printf("\nInserting new entry 600 (should evict LRU entry 100):\n");
    int *value6 = (int*)malloc(sizeof(int));
    *value6 = 6;
    lru_put(cache, 600, value6);
    printf("  PUT key=600, value=6\n");
    
    printf("\nTrying to access evicted entry 100:\n");
    result = lru_get(cache, 100);
    if (!result) {
        printf("  GET key=100 -> MISS (evicted)\n");
    }
    
    printf("\n");
    lru_print_stats(cache);
    
    free(cache->hash_table);
    free(cache);
}
```

## Route Cache Implementation

Routers cache forwarding decisions to avoid expensive routing table lookups. When a packet arrives for 172.16.5.23, the router checks its route cache first. A cache hit provides the next-hop interface immediately. A cache miss triggers a full routing table lookup, with the result stored in the cache for future packets. With thousands of active flows concentrated on a few popular destinations, cache hit rates often exceed 95%, dramatically reducing average lookup time.

LRU policy works well for route caches because network traffic shows strong temporal locality. A web server receiving requests from multiple clients generates response packets to those clients in bursts. Each client's destination appears repeatedly in short succession. The LRU cache naturally maintains entries for these active destinations while aging out inactive ones. Cache size determines the working set captured—too small and hit rates plummet, too large and memory is wasted on rarely-accessed entries.

```c
// Route information structure
typedef struct route_info {
    uint32_t dest_network;
    uint8_t prefix_len;
    uint32_t next_hop;
    uint32_t interface_id;
    uint32_t metric;
} route_info_t;

// Route cache
typedef struct route_cache {
    lru_cache_t *cache;
    // Pointer to actual routing table (simulated here)
    void *routing_table;
} route_cache_t;

// Create route cache
route_cache_t* create_route_cache(int capacity) {
    route_cache_t *rc = (route_cache_t*)malloc(sizeof(route_cache_t));
    rc->cache = create_lru_cache(capacity);
    rc->routing_table = NULL;  // Would point to real routing table
    return rc;
}

// Simulate full routing table lookup (expensive operation)
route_info_t* routing_table_lookup(uint32_t dest_ip) {
    // Simulate lookup time (in real router, this is expensive)
    // Here we just create a fake result
    route_info_t *route = (route_info_t*)malloc(sizeof(route_info_t));
    route->dest_network = dest_ip & 0xFFFFFF00;  // /24 prefix
    route->prefix_len = 24;
    route->next_hop = (dest_ip & 0xFFFFFF00) | 1;  // .1 as gateway
    route->interface_id = 1;
    route->metric = 10;
    return route;
}

// Route lookup with caching
route_info_t* lookup_route_cached(route_cache_t *rc, uint32_t dest_ip) {
    // Try cache first
    route_info_t *cached = (route_info_t*)lru_get(rc->cache, dest_ip);
    if (cached) {
        return cached;  // Cache hit - fast path
    }
    
    // Cache miss - do full lookup
    route_info_t *route = routing_table_lookup(dest_ip);
    
    // Store in cache for future lookups
    lru_put(rc->cache, dest_ip, route);
    
    return route;
}

// Simulate packet forwarding with route cache
void simulate_packet_forwarding() {
    printf("=== Route Cache Simulation ===\n\n");
    
    route_cache_t *rc = create_route_cache(100);
    
    printf("Simulating packet forwarding with 100-entry cache\n\n");
    
    // Simulate 10,000 packets with realistic distribution
    // 80% of traffic goes to 20% of destinations (Pareto principle)
    int num_packets = 10000;
    int num_destinations = 1000;
    int popular_destinations = 200;  // Top 20%
    
    for (int i = 0; i < num_packets; i++) {
        uint32_t dest_ip;
        
        // 80% chance of popular destination
        if (rand() % 100 < 80) {
            dest_ip = (10 << 24) | (rand() % popular_destinations);
        } else {
            dest_ip = (10 << 24) | (rand() % num_destinations);
        }
        
        route_info_t *route = lookup_route_cached(rc, dest_ip);
        
        // In real router, would forward packet using route info
        (void)route;
    }
    
    printf("Forwarded %d packets\n", num_packets);
    lru_print_stats(rc->cache);
    
    printf("Performance impact:\n");
    uint64_t total = rc->cache->hits + rc->cache->misses;
    double hit_rate = (double)rc->cache->hits / total * 100.0;
    
    printf("  Cache hits avoid expensive routing table lookup\n");
    printf("  With %.2f%% hit rate:\n", hit_rate);
    printf("    Full lookups: %lu (%.2f%%)\n", 
           rc->cache->misses, (double)rc->cache->misses / total * 100);
    printf("    Cached lookups: %lu (%.2f%%)\n",
           rc->cache->hits, (double)rc->cache->hits / total * 100);
    
    if (rc->cache->misses > 0) {
        double speedup = (double)total / rc->cache->misses;
        printf("    Effective speedup: %.2fx\n", speedup);
    }
    
    printf("\n");
    
    free(rc->cache->hash_table);
    free(rc->cache);
    free(rc);
}
```

## Least Frequently Used (LFU) Cache

LFU eviction prioritizes frequency over recency—the entry accessed least often gets evicted. This captures a different access pattern: some destinations receive steady traffic over long periods (DNS servers, popular websites) while others spike briefly then disappear (scanning sources, short-lived flows). LFU maintains entries for high-frequency destinations even during temporary bursts of other traffic.

Implementing efficient LFU requires careful data structure design. Naive approaches scan all entries to find the minimum frequency, requiring O(n) time per eviction. Optimal implementation uses a min-heap ordered by frequency, providing O(log n) eviction at the cost of O(log n) updates when incrementing frequencies. For caches with thousands of entries and high update rates, this logarithmic overhead matters.

```c
// LFU cache entry (includes frequency)
typedef struct lfu_entry {
    uint32_t key;
    void *value;
    uint32_t frequency;
    int heap_index;               // Position in frequency heap
    struct lfu_entry *next;       // Hash table chain
} lfu_entry_t;

// LFU cache structure
typedef struct lfu_cache {
    lfu_entry_t **hash_table;
    lfu_entry_t **freq_heap;      // Min-heap ordered by frequency
    int capacity;
    int size;
    uint64_t hits;
    uint64_t misses;
    uint64_t evictions;
} lfu_cache_t;

// Create LFU cache
lfu_cache_t* create_lfu_cache(int capacity) {
    lfu_cache_t *cache = (lfu_cache_t*)malloc(sizeof(lfu_cache_t));
    cache->capacity = capacity;
    cache->size = 0;
    cache->hits = 0;
    cache->misses = 0;
    cache->evictions = 0;
    
    cache->hash_table = (lfu_entry_t**)calloc(capacity * 2, sizeof(lfu_entry_t*));
    cache->freq_heap = (lfu_entry_t**)malloc(capacity * sizeof(lfu_entry_t*));
    
    return cache;
}

// Heap operations for LFU
static inline int lfu_parent(int i) { return (i - 1) / 2; }
static inline int lfu_left(int i) { return 2 * i + 1; }
static inline int lfu_right(int i) { return 2 * i + 2; }

// Swap entries in heap
void lfu_swap(lfu_cache_t *cache, int i, int j) {
    lfu_entry_t *temp = cache->freq_heap[i];
    cache->freq_heap[i] = cache->freq_heap[j];
    cache->freq_heap[j] = temp;
    
    cache->freq_heap[i]->heap_index = i;
    cache->freq_heap[j]->heap_index = j;
}

// Heapify up (after increasing frequency)
void lfu_heapify_up(lfu_cache_t *cache, int index) {
    while (index > 0) {
        int parent_idx = lfu_parent(index);
        if (cache->freq_heap[index]->frequency >= 
            cache->freq_heap[parent_idx]->frequency) {
            break;
        }
        lfu_swap(cache, index, parent_idx);
        index = parent_idx;
    }
}

// Heapify down (after removing min)
void lfu_heapify_down(lfu_cache_t *cache, int index) {
    while (1) {
        int smallest = index;
        int left = lfu_left(index);
        int right = lfu_right(index);
        
        if (left < cache->size && 
            cache->freq_heap[left]->frequency < cache->freq_heap[smallest]->frequency) {
            smallest = left;
        }
        
        if (right < cache->size && 
            cache->freq_heap[right]->frequency < cache->freq_heap[smallest]->frequency) {
            smallest = right;
        }
        
        if (smallest == index) {
            break;
        }
        
        lfu_swap(cache, index, smallest);
        index = smallest;
    }
}

// LFU get operation
void* lfu_get(lfu_cache_t *cache, uint32_t key) {
    int hash_index = cache_hash(key, cache->capacity * 2);
    lfu_entry_t *entry = cache->hash_table[hash_index];
    
    while (entry) {
        if (entry->key == key) {
            cache->hits++;
            
            // Increment frequency and update heap position
            entry->frequency++;
            lfu_heapify_down(cache, entry->heap_index);
            
            return entry->value;
        }
        entry = entry->next;
    }
    
    cache->misses++;
    return NULL;
}

// LFU put operation
void lfu_put(lfu_cache_t *cache, uint32_t key, void *value) {
    int hash_index = cache_hash(key, cache->capacity * 2);
    
    // Check if key exists
    lfu_entry_t *entry = cache->hash_table[hash_index];
    while (entry) {
        if (entry->key == key) {
            entry->value = value;
            entry->frequency++;
            lfu_heapify_down(cache, entry->heap_index);
            return;
        }
        entry = entry->next;
    }
    
    // Check if cache is full
    if (cache->size >= cache->capacity) {
        // Evict LFU entry (heap root)
        lfu_entry_t *lfu_entry = cache->freq_heap[0];
        
        // Remove from hash table
        int lfu_hash = cache_hash(lfu_entry->key, cache->capacity * 2);
        lfu_entry_t **slot = &cache->hash_table[lfu_hash];
        while (*slot && *slot != lfu_entry) {
            slot = &(*slot)->next;
        }
        if (*slot) {
            *slot = lfu_entry->next;
        }
        
        // Remove from heap
        cache->size--;
        if (cache->size > 0) {
            cache->freq_heap[0] = cache->freq_heap[cache->size];
            cache->freq_heap[0]->heap_index = 0;
            lfu_heapify_down(cache, 0);
        }
        
        free(lfu_entry);
        cache->evictions++;
    }
    
    // Create new entry
    lfu_entry_t *new_entry = (lfu_entry_t*)malloc(sizeof(lfu_entry_t));
    new_entry->key = key;
    new_entry->value = value;
    new_entry->frequency = 1;
    new_entry->heap_index = cache->size;
    new_entry->next = NULL;
    
    // Insert into hash table
    new_entry->next = cache->hash_table[hash_index];
    cache->hash_table[hash_index] = new_entry;
    
    // Insert into heap
    cache->freq_heap[cache->size] = new_entry;
    lfu_heapify_up(cache, cache->size);
    cache->size++;
}

// Print LFU statistics
void lfu_print_stats(lfu_cache_t *cache) {
    uint64_t total = cache->hits + cache->misses;
    double hit_rate = total > 0 ? (double)cache->hits / total * 100.0 : 0.0;
    
    printf("LFU Cache Statistics:\n");
    printf("  Capacity: %d entries\n", cache->capacity);
    printf("  Current size: %d entries\n", cache->size);
    printf("  Hits: %lu\n", cache->hits);
    printf("  Misses: %lu\n", cache->misses);
    printf("  Hit rate: %.2f%%\n", hit_rate);
    printf("  Evictions: %lu\n\n", cache->evictions);
}

// Compare LRU vs LFU
void compare_lru_lfu() {
    printf("=== LRU vs LFU Comparison ===\n\n");
    
    int cache_size = 10;
    int num_accesses = 1000;
    
    lru_cache_t *lru = create_lru_cache(cache_size);
    lfu_cache_t *lfu = create_lfu_cache(cache_size);
    
    printf("Cache size: %d entries\n", cache_size);
    printf("Simulating %d accesses with mixed patterns\n\n", num_accesses);
    
    srand(42);  // Fixed seed for reproducibility
    
    // Simulate workload with both temporal and frequency characteristics
    for (int i = 0; i < num_accesses; i++) {
        uint32_t key;
        int *value = (int*)malloc(sizeof(int));
        *value = i;
        
        if (i % 100 < 70) {
            // 70% high-frequency keys (1-5)
            key = 1 + (rand() % 5);
        } else if (i % 100 < 90) {
            // 20% temporal burst (same key for a while)
            key = 100 + (i / 50);
        } else {
            // 10% random one-time keys
            key = 1000 + rand();
        }
        
        // Access in both caches
        void *lru_result = lru_get(lru, key);
        if (!lru_result) {
            lru_put(lru, key, value);
        }
        
        void *lfu_result = lfu_get(lfu, key);
        if (!lfu_result) {
            lfu_put(lfu, key, (void*)value);
        }
    }
    
    printf("Results:\n\n");
    lru_print_stats(lru);
    lfu_print_stats(lfu);
    
    printf("Analysis:\n");
    printf("  LRU performs better for temporal locality\n");
    printf("  LFU performs better for frequency-based patterns\n");
    printf("  Real workloads have both characteristics\n\n");
    
    free(lru->hash_table);
    free(lru);
    free(lfu->hash_table);
    free(lfu->freq_heap);
    free(lfu);
}
```

## Adaptive Replacement Cache (ARC)

ARC combines LRU and LFU advantages by maintaining four lists: recently used entries, frequently used entries, and ghost lists tracking recently evicted entries from each. When accessing an entry in the LRU ghost list, ARC interprets this as evidence of temporal locality, expanding the LRU list size. Access in the LFU ghost list suggests frequency matters more, expanding the LFU list. This self-tuning adapts to workload characteristics without manual configuration.

The elegance of ARC lies in learning from eviction mistakes. When a recently evicted entry is accessed again, this indicates premature eviction. ARC adjusts its internal balance to prevent similar mistakes. For network caches facing diverse traffic patterns—web browsing, video streaming, peer-to-peer transfers—this adaptability proves valuable. However, ARC's complexity and patent encumbrance limit adoption compared to simpler LRU implementations.

```c
// Simplified ARC concept (not full implementation)
typedef struct arc_cache {
    lru_cache_t *t1;              // Recently accessed once
    lru_cache_t *t2;              // Accessed more than once
    lru_cache_t *b1;              // Ghost list for T1
    lru_cache_t *b2;              // Ghost list for T2
    int target_t1_size;           // Adaptive target size for T1
    int total_capacity;
} arc_cache_t;

// Create ARC cache
arc_cache_t* create_arc_cache(int capacity) {
    arc_cache_t *arc = (arc_cache_t*)malloc(sizeof(arc_cache_t));
    arc->total_capacity = capacity;
    arc->target_t1_size = capacity / 2;  // Initially balanced
    
    arc->t1 = create_lru_cache(capacity);
    arc->t2 = create_lru_cache(capacity);
    arc->b1 = create_lru_cache(capacity);
    arc->b2 = create_lru_cache(capacity);
    
    return arc;
}

// ARC get (simplified logic)
void* arc_get(arc_cache_t *arc, uint32_t key) {
    // Check T1 (recently used once)
    void *result = lru_get(arc->t1, key);
    if (result) {
        // Move to T2 (now used more than once)
        lru_put(arc->t2, key, result);
        return result;
    }
    
    // Check T2 (frequently used)
    result = lru_get(arc->t2, key);
    if (result) {
        return result;
    }
    
    // Check ghost lists and adapt
    if (lru_get(arc->b1, key)) {
        // Was in T1, increase target for T1
        arc->target_t1_size = (arc->target_t1_size < arc->total_capacity) ?
                              arc->target_t1_size + 1 : arc->total_capacity;
    } else if (lru_get(arc->b2, key)) {
        // Was in T2, decrease target for T1 (increase T2)
        arc->target_t1_size = (arc->target_t1_size > 0) ?
                              arc->target_t1_size - 1 : 0;
    }
    
    return NULL;
}

// Explain ARC advantages
void explain_arc() {
    printf("=== Adaptive Replacement Cache (ARC) ===\n\n");
    
    printf("ARC combines LRU and LFU advantages:\n\n");
    
    printf("Structure:\n");
    printf("  T1: Recently used once (LRU behavior)\n");
    printf("  T2: Used multiple times (LFU behavior)\n");
    printf("  B1: Ghost list - tracks recently evicted from T1\n");
    printf("  B2: Ghost list - tracks recently evicted from T2\n\n");
    
    printf("Adaptation mechanism:\n");
    printf("  - Hit in B1 → Temporal locality detected\n");
    printf("    Action: Increase T1 size, decrease T2\n");
    printf("  - Hit in B2 → Frequency pattern detected\n");
    printf("    Action: Decrease T1 size, increase T2\n\n");
    
    printf("Advantages:\n");
    printf("  + Self-tuning to workload characteristics\n");
    printf("  + Good performance across diverse patterns\n");
    printf("  + No manual tuning required\n\n");
    
    printf("Disadvantages:\n");
    printf("  - More complex implementation\n");
    printf("  - Higher memory overhead (4 lists)\n");
    printf("  - Patent encumbrance (expired 2020)\n\n");
    
    printf("Practical use:\n");
    printf("  Best for: Diverse, unpredictable workloads\n");
    printf("  Overkill for: Well-understood traffic patterns\n\n");
}
```

## Cache-Conscious Data Structures

Modern CPUs fetch memory in 64-byte cache lines. A cache implementation scattering entries across memory forces multiple cache line fetches per access. Optimized implementations pack frequently accessed fields—key, value pointer, LRU pointers—into single cache lines. Hash table buckets align to cache line boundaries. This cache-aware design can double performance despite identical asymptotic complexity.

For network devices processing millions of packets per second, every nanosecond matters. A route cache access taking 50 CPU cycles instead of 100 cycles through better cache layout translates directly to higher packet forwarding rates. Understanding hardware cache behavior matters as much as algorithmic complexity for achieving maximum performance in latency-critical network code.

```c
// Cache-aligned cache entry (fits in one cache line)
typedef struct __attribute__((aligned(64))) cache_aligned_entry {
    uint32_t key;                 // 4 bytes
    uint32_t padding1;            // 4 bytes (alignment)
    void *value;                  // 8 bytes
    void *prev;                   // 8 bytes
    void *next;                   // 8 bytes
    uint64_t timestamp;           // 8 bytes
    uint32_t access_count;        // 4 bytes
    uint32_t padding2;            // 4 bytes
    uint8_t padding3[16];         // 16 bytes padding
} cache_aligned_entry_t;          // Total: 64 bytes

// Demonstrate cache alignment impact
void demonstrate_cache_alignment() {
    printf("=== Cache Line Optimization ===\n\n");
    
    printf("Standard entry size: %zu bytes\n", sizeof(cache_entry_t));
    printf("Aligned entry size: %zu bytes\n", sizeof(cache_aligned_entry_t));
    printf("CPU cache line: 64 bytes\n\n");
    
    printf("Impact analysis:\n\n");
    
    printf("Unaligned structure:\n");
    printf("  - Entry spans multiple cache lines\n");
    printf("  - Each access may trigger 2+ cache line loads\n");
    printf("  - Poor spatial locality\n");
    printf("  - More cache misses\n\n");
    
    printf("Aligned structure:\n");
    printf("  - One entry = one cache line\n");
    printf("  - Single cache line load per access\n");
    printf("  - Perfect spatial locality\n");
    printf("  - Minimal cache misses\n\n");
    
    printf("Performance impact:\n");
    printf("  Typical speedup: 1.5x - 2x\n");
    printf("  Critical for: High packet rates, tight latency budgets\n\n");
    
    printf("Trade-off:\n");
    printf("  Memory overhead: ~2x per entry\n");
    printf("  Performance gain: ~2x faster access\n");
    printf("  Decision: Depends on cache size vs performance priority\n\n");
}

// Measure cache performance
void measure_cache_performance() {
    printf("=== Cache Performance Measurement ===\n\n");
    
    int cache_sizes[] = {10, 50, 100, 500, 1000};
    int num_accesses = 100000;
    
    printf("Measuring lookup performance for %d accesses:\n\n", num_accesses);
    printf("Cache Size | Avg Lookup (ns) | Hit Rate\n");
    printf("-----------|-----------------|----------\n");
    
    for (int i = 0; i < 5; i++) {
        lru_cache_t *cache = create_lru_cache(cache_sizes[i]);
        
        // Warm up cache with popular entries
        for (int j = 0; j < cache_sizes[i]; j++) {
            int *val = (int*)malloc(sizeof(int));
            *val = j;
            lru_put(cache, j, val);
        }
        
        // Measure lookup time
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        for (int j = 0; j < num_accesses; j++) {
            uint32_t key = rand() % (cache_sizes[i] * 2);  // 50% hit rate target
            lru_get(cache, key);
        }
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        double elapsed = (end.tv_sec - start.tv_sec) * 1000000000.0 +
                        (end.tv_nsec - start.tv_nsec);
        double avg_lookup = elapsed / num_accesses;
        
        uint64_t total = cache->hits + cache->misses;
        double hit_rate = (double)cache->hits / total * 100.0;
        
        printf("%-10d | %-15.2f | %.2f%%\n", 
               cache_sizes[i], avg_lookup, hit_rate);
        
        free(cache->hash_table);
        free(cache);
    }
    
    printf("\nObservation: Larger caches → better hit rates\n");
    printf("But: Diminishing returns after working set captured\n\n");
}
```

## Conclusion

Cache structures provide the critical optimization layer between expensive lookups and packet-rate forwarding requirements. LRU caches exploit temporal locality, perfect for bursty traffic patterns. LFU caches capture frequency skew, ideal for workloads dominated by popular destinations. Adaptive caches like ARC automatically tune to workload characteristics, trading implementation complexity for versatility.

The choice of cache policy and parameters determines whether your network device forwards packets at line rate or drops frames under load. A too-small cache wastes silicon on structure overhead without capturing the working set. A too-large cache wastes precious on-chip memory that could serve other purposes. Understanding cache behavior, measuring hit rates, and selecting appropriate eviction policies separates network systems that scale from those that struggle.

Master caching data structures, understand their interaction with CPU cache hierarchies, and you possess essential tools for building high-performance network systems. Every major network device—from software routers to hardware switches—relies on caching to achieve the forwarding rates modern networks demand.

# Tree Algorithms: Spanning Tree and Minimum Spanning Tree

## The Loop Problem in Ethernet

Ethernet switches forward frames based on destination MAC addresses, flooding unknown destinations to all ports except the source. When switches connect in a loop—Switch A to Switch B, Switch B to Switch C, Switch C back to Switch A—a broadcast storm results. A frame broadcast from any host circulates endlessly, each switch receiving it, flooding it, receiving it again. Within seconds, the network saturates with duplicate frames, consuming all bandwidth.

The Spanning Tree Protocol (STP) prevents loops by logically disabling redundant links, creating a tree topology from an arbitrary mesh. Even with physical loops, STP ensures only one active path exists between any two switches. When a link fails, STP reconfigures, activating previously blocked ports to restore connectivity. This algorithm transforms the loop prevention problem into a graph theory problem: constructing a spanning tree from a connected graph.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

// Switch (bridge) structure
typedef struct bridge {
    uint64_t bridge_id;           // Priority (16 bits) + MAC address (48 bits)
    int id;                       // Internal ID for array indexing
    char name[32];
    uint64_t root_bridge_id;      // Currently believed root
    uint32_t root_path_cost;      // Cost to reach root
    int root_port;                // Port toward root (-1 if this is root)
} bridge_t;

// Port state
typedef enum {
    PORT_BLOCKING,
    PORT_LISTENING,
    PORT_LEARNING,
    PORT_FORWARDING,
    PORT_DISABLED
} port_state_t;

// Port structure
typedef struct port {
    int port_id;
    int bridge_id;                // Bridge this port belongs to
    int connected_bridge;         // Bridge on other end
    int connected_port;           // Port on other end
    uint32_t path_cost;           // Cost of this link
    port_state_t state;
    uint8_t is_designated;        // Is this a designated port?
} port_t;

// Network topology
typedef struct stp_network {
    bridge_t *bridges;
    int num_bridges;
    port_t *ports;
    int num_ports;
} stp_network_t;

// BPDU (Bridge Protocol Data Unit)
typedef struct bpdu {
    uint64_t root_bridge_id;
    uint32_t root_path_cost;
    uint64_t sender_bridge_id;
    int sender_port_id;
} bpdu_t;

// Create network topology
stp_network_t* create_stp_network(int num_bridges) {
    stp_network_t *net = (stp_network_t*)malloc(sizeof(stp_network_t));
    net->num_bridges = num_bridges;
    net->bridges = (bridge_t*)calloc(num_bridges, sizeof(bridge_t));
    net->num_ports = 0;
    net->ports = (port_t*)calloc(num_bridges * 10, sizeof(port_t));  // Max 10 ports per bridge
    return net;
}

// Initialize bridge
void init_bridge(stp_network_t *net, int id, const char *name, uint64_t bridge_id) {
    bridge_t *br = &net->bridges[id];
    br->id = id;
    br->bridge_id = bridge_id;
    strncpy(br->name, name, sizeof(br->name) - 1);
    br->root_bridge_id = bridge_id;  // Initially believes it's the root
    br->root_path_cost = 0;
    br->root_port = -1;
}

// Add link between two bridges
void add_link(stp_network_t *net, int bridge1, int bridge2, uint32_t cost) {
    // Create port on bridge1
    port_t *port1 = &net->ports[net->num_ports];
    port1->port_id = net->num_ports;
    port1->bridge_id = bridge1;
    port1->connected_bridge = bridge2;
    port1->connected_port = net->num_ports + 1;
    port1->path_cost = cost;
    port1->state = PORT_BLOCKING;
    port1->is_designated = 0;
    net->num_ports++;
    
    // Create port on bridge2
    port_t *port2 = &net->ports[net->num_ports];
    port2->port_id = net->num_ports;
    port2->bridge_id = bridge2;
    port2->connected_bridge = bridge1;
    port2->connected_port = net->num_ports - 1;
    port2->path_cost = cost;
    port2->state = PORT_BLOCKING;
    port2->is_designated = 0;
    net->num_ports++;
}

// Compare BPDUs (returns 1 if bpdu1 is better/superior)
int bpdu_is_better(bpdu_t *bpdu1, bpdu_t *bpdu2) {
    // Lower root ID is better
    if (bpdu1->root_bridge_id < bpdu2->root_bridge_id) return 1;
    if (bpdu1->root_bridge_id > bpdu2->root_bridge_id) return 0;
    
    // Same root, lower path cost is better
    if (bpdu1->root_path_cost < bpdu2->root_path_cost) return 1;
    if (bpdu1->root_path_cost > bpdu2->root_path_cost) return 0;
    
    // Same cost, lower sender bridge ID is better
    if (bpdu1->sender_bridge_id < bpdu2->sender_bridge_id) return 1;
    if (bpdu1->sender_bridge_id > bpdu2->sender_bridge_id) return 0;
    
    // Same sender, lower port ID is better
    return bpdu1->sender_port_id < bpdu2->sender_port_id;
}

// Send BPDU from bridge on port
void send_bpdu(stp_network_t *net, int bridge_id, int port_id, bpdu_t *bpdu) {
    bridge_t *br = &net->bridges[bridge_id];
    
    bpdu->root_bridge_id = br->root_bridge_id;
    bpdu->root_path_cost = br->root_path_cost;
    bpdu->sender_bridge_id = br->bridge_id;
    bpdu->sender_port_id = port_id;
}

// Receive BPDU on port
int receive_bpdu(stp_network_t *net, int port_id, bpdu_t *received_bpdu) {
    port_t *port = &net->ports[port_id];
    bridge_t *br = &net->bridges[port->bridge_id];
    int changed = 0;
    
    // Create BPDU representing current information
    bpdu_t current_bpdu;
    current_bpdu.root_bridge_id = br->root_bridge_id;
    current_bpdu.root_path_cost = br->root_path_cost + port->path_cost;
    current_bpdu.sender_bridge_id = br->bridge_id;
    current_bpdu.sender_port_id = port_id;
    
    // Check if received BPDU is better
    if (bpdu_is_better(received_bpdu, &current_bpdu)) {
        // Update root information
        br->root_bridge_id = received_bpdu->root_bridge_id;
        br->root_path_cost = received_bpdu->root_path_cost + port->path_cost;
        br->root_port = port_id;
        changed = 1;
    }
    
    return changed;
}

// Run STP algorithm
void run_stp(stp_network_t *net) {
    printf("=== Running Spanning Tree Protocol ===\n\n");
    
    int iteration = 0;
    int changed = 1;
    
    while (changed && iteration < 20) {
        iteration++;
        changed = 0;
        
        printf("Iteration %d:\n", iteration);
        
        // Each bridge sends BPDUs on all ports
        for (int i = 0; i < net->num_bridges; i++) {
            bridge_t *br = &net->bridges[i];
            
            // Send BPDU on each port
            for (int j = 0; j < net->num_ports; j++) {
                port_t *port = &net->ports[j];
                
                if (port->bridge_id == i) {
                    bpdu_t bpdu;
                    send_bpdu(net, i, j, &bpdu);
                    
                    // BPDU arrives at connected bridge
                    int connected_port = port->connected_port;
                    if (receive_bpdu(net, connected_port, &bpdu)) {
                        changed = 1;
                    }
                }
            }
        }
        
        // Print current state
        for (int i = 0; i < net->num_bridges; i++) {
            bridge_t *br = &net->bridges[i];
            printf("  %s: Root=%016lX, Cost=%u\n",
                   br->name, br->root_bridge_id, br->root_path_cost);
        }
        printf("\n");
    }
    
    printf("STP converged after %d iterations\n\n", iteration);
}

// Designate ports
void designate_ports(stp_network_t *net) {
    // Determine root bridge
    uint64_t root_id = ULLONG_MAX;
    for (int i = 0; i < net->num_bridges; i++) {
        if (net->bridges[i].bridge_id < root_id) {
            root_id = net->bridges[i].bridge_id;
        }
    }
    
    // Set port states
    for (int i = 0; i < net->num_ports; i++) {
        port_t *port = &net->ports[i];
        bridge_t *br = &net->bridges[port->bridge_id];
        
        // Root bridge: all ports forwarding
        if (br->bridge_id == root_id) {
            port->state = PORT_FORWARDING;
            port->is_designated = 1;
            continue;
        }
        
        // Root port: forwarding
        if (br->root_port == i) {
            port->state = PORT_FORWARDING;
            port->is_designated = 0;
            continue;
        }
        
        // Check if this should be designated port
        port_t *other_port = &net->ports[port->connected_port];
        bridge_t *other_br = &net->bridges[other_port->bridge_id];
        
        uint32_t our_cost = br->root_path_cost;
        uint32_t their_cost = other_br->root_path_cost;
        
        if (our_cost < their_cost ||
            (our_cost == their_cost && br->bridge_id < other_br->bridge_id)) {
            port->state = PORT_FORWARDING;
            port->is_designated = 1;
        } else {
            port->state = PORT_BLOCKING;
            port->is_designated = 0;
        }
    }
}

// Print spanning tree result
void print_spanning_tree(stp_network_t *net) {
    printf("=== Spanning Tree Result ===\n\n");
    
    // Find root bridge
    uint64_t root_id = ULLONG_MAX;
    int root_idx = -1;
    for (int i = 0; i < net->num_bridges; i++) {
        if (net->bridges[i].bridge_id < root_id) {
            root_id = net->bridges[i].bridge_id;
            root_idx = i;
        }
    }
    
    printf("Root Bridge: %s (ID: %016lX)\n\n", 
           net->bridges[root_idx].name, root_id);
    
    const char *state_names[] = {
        "BLOCKING", "LISTENING", "LEARNING", "FORWARDING", "DISABLED"
    };
    
    printf("Port States:\n");
    for (int i = 0; i < net->num_bridges; i++) {
        bridge_t *br = &net->bridges[i];
        printf("%s:\n", br->name);
        
        for (int j = 0; j < net->num_ports; j++) {
            port_t *port = &net->ports[j];
            
            if (port->bridge_id == i) {
                bridge_t *connected = &net->bridges[port->connected_bridge];
                printf("  Port %d -> %s: %s%s%s\n",
                       port->port_id,
                       connected->name,
                       state_names[port->state],
                       (br->root_port == j) ? " [ROOT PORT]" : "",
                       port->is_designated ? " [DESIGNATED]" : "");
            }
        }
        printf("\n");
    }
}

// Example network topology
void demo_spanning_tree() {
    printf("=== Spanning Tree Protocol Demo ===\n\n");
    
    // Create network with 5 switches
    stp_network_t *net = create_stp_network(5);
    
    // Initialize bridges (lower ID = higher priority)
    // Priority (16 bits) << 48 | MAC (48 bits)
    init_bridge(net, 0, "SW1", 0x8000000000000001ULL);
    init_bridge(net, 1, "SW2", 0x8000000000000002ULL);
    init_bridge(net, 2, "SW3", 0x8000000000000003ULL);
    init_bridge(net, 3, "SW4", 0x8000000000000004ULL);
    init_bridge(net, 4, "SW5", 0x8000000000000005ULL);
    
    printf("Network Topology:\n");
    printf("  SW1 has lowest bridge ID (will become root)\n\n");
    
    // Add links (creates redundant paths / loops)
    add_link(net, 0, 1, 4);   // SW1-SW2 cost 4
    add_link(net, 0, 2, 3);   // SW1-SW3 cost 3
    add_link(net, 1, 3, 2);   // SW2-SW4 cost 2
    add_link(net, 2, 3, 1);   // SW3-SW4 cost 1
    add_link(net, 2, 4, 2);   // SW3-SW5 cost 2
    add_link(net, 3, 4, 5);   // SW4-SW5 cost 5
    
    printf("Links created:\n");
    printf("  SW1-SW2 (cost 4)\n");
    printf("  SW1-SW3 (cost 3)\n");
    printf("  SW2-SW4 (cost 2)\n");
    printf("  SW3-SW4 (cost 1)\n");
    printf("  SW3-SW5 (cost 2)\n");
    printf("  SW4-SW5 (cost 5)\n\n");
    printf("Note: Multiple paths exist between switches (loops present)\n\n");
    
    // Run STP
    run_stp(net);
    
    // Designate ports
    designate_ports(net);
    
    // Print result
    print_spanning_tree(net);
    
    free(net->bridges);
    free(net->ports);
    free(net);
}
```

## Minimum Spanning Tree Algorithms

The Minimum Spanning Tree (MST) problem generalizes spanning tree construction: given a weighted graph, find a spanning tree with minimum total edge weight. Two classical algorithms solve this: Prim's algorithm and Kruskal's algorithm. Prim's grows a tree from a starting vertex, repeatedly adding the minimum-weight edge connecting the tree to an unvisited vertex. Kruskal's sorts all edges by weight and adds them in order, skipping edges that would create cycles.

For network design, MST algorithms help minimize cabling costs or latency. When planning a campus network connecting buildings, the MST represents the cheapest fiber layout providing full connectivity. When optimizing multicast tree construction, MST minimizes aggregate link costs. Both algorithms produce optimal solutions but differ in implementation complexity and performance characteristics based on graph density.

```c
// Union-Find data structure for Kruskal's algorithm
typedef struct union_find {
    int *parent;
    int *rank;
    int size;
} union_find_t;

// Create union-find
union_find_t* create_union_find(int size) {
    union_find_t *uf = (union_find_t*)malloc(sizeof(union_find_t));
    uf->size = size;
    uf->parent = (int*)malloc(size * sizeof(int));
    uf->rank = (int*)malloc(size * sizeof(int));
    
    for (int i = 0; i < size; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
    
    return uf;
}

// Find with path compression
int uf_find(union_find_t *uf, int x) {
    if (uf->parent[x] != x) {
        uf->parent[x] = uf_find(uf, uf->parent[x]);  // Path compression
    }
    return uf->parent[x];
}

// Union by rank
void uf_union(union_find_t *uf, int x, int y) {
    int root_x = uf_find(uf, x);
    int root_y = uf_find(uf, y);
    
    if (root_x == root_y) return;
    
    // Union by rank
    if (uf->rank[root_x] < uf->rank[root_y]) {
        uf->parent[root_x] = root_y;
    } else if (uf->rank[root_x] > uf->rank[root_y]) {
        uf->parent[root_y] = root_x;
    } else {
        uf->parent[root_y] = root_x;
        uf->rank[root_x]++;
    }
}

// Edge for MST algorithms
typedef struct mst_edge {
    int src;
    int dest;
    uint32_t weight;
} mst_edge_t;

// Compare function for edge sorting
int compare_edges(const void *a, const void *b) {
    mst_edge_t *edge_a = (mst_edge_t*)a;
    mst_edge_t *edge_b = (mst_edge_t*)b;
    return edge_a->weight - edge_b->weight;
}

// Kruskal's MST algorithm
void kruskal_mst(int num_vertices, mst_edge_t *edges, int num_edges) {
    printf("=== Kruskal's Minimum Spanning Tree ===\n\n");
    
    // Sort edges by weight
    qsort(edges, num_edges, sizeof(mst_edge_t), compare_edges);
    
    printf("Sorted edges:\n");
    for (int i = 0; i < num_edges; i++) {
        printf("  Edge %d-%d: weight %u\n", 
               edges[i].src, edges[i].dest, edges[i].weight);
    }
    printf("\n");
    
    // Create union-find for cycle detection
    union_find_t *uf = create_union_find(num_vertices);
    
    mst_edge_t *mst = (mst_edge_t*)malloc((num_vertices - 1) * sizeof(mst_edge_t));
    int mst_size = 0;
    uint32_t total_weight = 0;
    
    printf("Building MST:\n");
    
    // Process edges in order
    for (int i = 0; i < num_edges && mst_size < num_vertices - 1; i++) {
        mst_edge_t *edge = &edges[i];
        
        // Check if adding edge creates cycle
        int root_src = uf_find(uf, edge->src);
        int root_dest = uf_find(uf, edge->dest);
        
        if (root_src != root_dest) {
            // No cycle, add to MST
            mst[mst_size++] = *edge;
            total_weight += edge->weight;
            uf_union(uf, edge->src, edge->dest);
            
            printf("  Add edge %d-%d (weight %u)\n",
                   edge->src, edge->dest, edge->weight);
        } else {
            printf("  Skip edge %d-%d (would create cycle)\n",
                   edge->src, edge->dest);
        }
    }
    
    printf("\nMinimum Spanning Tree:\n");
    printf("  Edges: %d\n", mst_size);
    printf("  Total weight: %u\n\n", total_weight);
    
    free(uf->parent);
    free(uf->rank);
    free(uf);
    free(mst);
}

// Prim's MST algorithm
void prim_mst(int num_vertices, int **adj_matrix) {
    printf("=== Prim's Minimum Spanning Tree ===\n\n");
    
    int *parent = (int*)malloc(num_vertices * sizeof(int));
    uint32_t *key = (uint32_t*)malloc(num_vertices * sizeof(uint32_t));
    int *in_mst = (int*)calloc(num_vertices, sizeof(int));
    
    // Initialize
    for (int i = 0; i < num_vertices; i++) {
        key[i] = UINT32_MAX;
        parent[i] = -1;
    }
    
    // Start from vertex 0
    key[0] = 0;
    uint32_t total_weight = 0;
    
    printf("Building MST from vertex 0:\n");
    
    // Build MST
    for (int count = 0; count < num_vertices; count++) {
        // Find minimum key vertex not in MST
        uint32_t min_key = UINT32_MAX;
        int min_vertex = -1;
        
        for (int v = 0; v < num_vertices; v++) {
            if (!in_mst[v] && key[v] < min_key) {
                min_key = key[v];
                min_vertex = v;
            }
        }
        
        // Add to MST
        in_mst[min_vertex] = 1;
        
        if (parent[min_vertex] != -1) {
            printf("  Add edge %d-%d (weight %u)\n",
                   parent[min_vertex], min_vertex, key[min_vertex]);
            total_weight += key[min_vertex];
        }
        
        // Update keys of adjacent vertices
        for (int v = 0; v < num_vertices; v++) {
            if (adj_matrix[min_vertex][v] && 
                !in_mst[v] && 
                adj_matrix[min_vertex][v] < key[v]) {
                parent[v] = min_vertex;
                key[v] = adj_matrix[min_vertex][v];
            }
        }
    }
    
    printf("\nMinimum Spanning Tree:\n");
    printf("  Total weight: %u\n\n", total_weight);
    
    free(parent);
    free(key);
    free(in_mst);
}

// Compare MST algorithms
void compare_mst_algorithms() {
    printf("=== MST Algorithm Comparison ===\n\n");
    
    // Create example graph
    int num_vertices = 6;
    
    // Edge list for Kruskal's
    mst_edge_t edges[] = {
        {0, 1, 4}, {0, 2, 3}, {1, 2, 1}, {1, 3, 2},
        {2, 3, 4}, {3, 4, 2}, {4, 5, 6}, {2, 4, 5}
    };
    int num_edges = sizeof(edges) / sizeof(edges[0]);
    
    // Run Kruskal's
    kruskal_mst(num_vertices, edges, num_edges);
    
    // Create adjacency matrix for Prim's
    int **adj_matrix = (int**)malloc(num_vertices * sizeof(int*));
    for (int i = 0; i < num_vertices; i++) {
        adj_matrix[i] = (int*)calloc(num_vertices, sizeof(int));
    }
    
    for (int i = 0; i < num_edges; i++) {
        adj_matrix[edges[i].src][edges[i].dest] = edges[i].weight;
        adj_matrix[edges[i].dest][edges[i].src] = edges[i].weight;
    }
    
    // Run Prim's
    prim_mst(num_vertices, adj_matrix);
    
    printf("Algorithm Comparison:\n\n");
    printf("Kruskal's Algorithm:\n");
    printf("  Time: O(E log E) - dominated by sorting\n");
    printf("  Space: O(V) for union-find\n");
    printf("  Best for: Sparse graphs, edge-oriented\n");
    printf("  Implementation: Sort edges, union-find\n\n");
    
    printf("Prim's Algorithm:\n");
    printf("  Time: O(V²) or O(E log V) with heap\n");
    printf("  Space: O(V) for arrays\n");
    printf("  Best for: Dense graphs, vertex-oriented\n");
    printf("  Implementation: Priority queue, greedy\n\n");
    
    printf("Both produce identical MST (may differ in tie-breaking)\n\n");
    
    for (int i = 0; i < num_vertices; i++) {
        free(adj_matrix[i]);
    }
    free(adj_matrix);
}
```

## Rapid Spanning Tree Protocol (RSTP)

Classic STP converges slowly—30 to 50 seconds after topology changes. RSTP reduces this to seconds by eliminating timer-based state transitions. Instead of waiting in listening and learning states, RSTP uses explicit handshakes between bridges to synchronize state. Edge ports connecting to hosts transition to forwarding immediately. Point-to-point links use proposal-agreement handshakes to rapidly transition to forwarding without waiting for timers.

The algorithmic difference lies in RSTP's aggressive synchronization. When a bridge receives a superior BPDU, it immediately blocks all non-edge designated ports and proposes itself as designated bridge to downstream neighbors. This synchronization propagates through the tree rapidly, with each link transitioning as soon as both ends agree. For modern networks where seconds of downtime cost thousands of dollars, RSTP's fast convergence justifies its additional protocol complexity.

```c
// RSTP port types
typedef enum {
    PORT_TYPE_EDGE,           // Connected to end host
    PORT_TYPE_P2P,            // Point-to-point to another switch
    PORT_TYPE_SHARED          // Shared medium (rare in modern networks)
} rstp_port_type_t;

// RSTP port roles
typedef enum {
    ROLE_ROOT,
    ROLE_DESIGNATED,
    ROLE_ALTERNATE,           // Backup path to root
    ROLE_BACKUP,              // Backup designated
    ROLE_DISABLED
} rstp_port_role_t;

// RSTP port structure
typedef struct rstp_port {
    int port_id;
    rstp_port_type_t type;
    rstp_port_role_t role;
    port_state_t state;
    uint8_t proposing;        // Sending proposal
    uint8_t agreed;           // Received agreement
    uint8_t sync;             // Synchronizing
} rstp_port_t;

// RSTP rapid transition logic
void rstp_rapid_transition(rstp_port_t *port) {
    printf("RSTP Rapid Transition:\n");
    
    if (port->type == PORT_TYPE_EDGE) {
        // Edge ports go directly to forwarding
        printf("  Edge port: BLOCKING -> FORWARDING (immediate)\n");
        port->state = PORT_FORWARDING;
        return;
    }
    
    if (port->type == PORT_TYPE_P2P) {
        // Point-to-point: use proposal-agreement
        printf("  P2P port: Starting proposal-agreement\n");
        
        port->proposing = 1;
        printf("  1. Send proposal to neighbor\n");
        
        // Simulate neighbor processing
        printf("  2. Neighbor blocks non-edge ports\n");
        printf("  3. Neighbor sends agreement\n");
        
        port->agreed = 1;
        printf("  4. Received agreement\n");
        
        printf("  P2P port: BLOCKING -> FORWARDING (rapid)\n");
        port->state = PORT_FORWARDING;
    }
}

// Compare STP vs RSTP convergence
void compare_stp_rstp() {
    printf("=== STP vs RSTP Comparison ===\n\n");
    
    printf("Classic STP Convergence:\n");
    printf("  1. Detect failure: 0-2 seconds (hello timeout)\n");
    printf("  2. Blocking state: 0 seconds\n");
    printf("  3. Listening state: 15 seconds (forward delay)\n");
    printf("  4. Learning state: 15 seconds (forward delay)\n");
    printf("  5. Forwarding state: ready\n");
    printf("  Total: 30-50 seconds\n\n");
    
    printf("RSTP Convergence:\n");
    printf("  1. Detect failure: immediate (no hello timeout needed)\n");
    printf("  2. Proposal-agreement: 2-3 hello times\n");
    printf("  3. Forwarding state: ready\n");
    printf("  Total: < 1 second on edge ports, 1-3 seconds on P2P\n\n");
    
    printf("Key RSTP Improvements:\n");
    printf("  • Edge ports: immediate transition\n");
    printf("  • Proposal-agreement: eliminates timer waits\n");
    printf("  • Alternate/backup ports: pre-computed backup paths\n");
    printf("  • Explicit sync: no reliance on timers\n\n");
    
    printf("When RSTP matters:\n");
    printf("  • Mission-critical networks\n");
    printf("  • Real-time applications (VoIP, video)\n");
    printf("  • High-availability requirements\n\n");
}
```

## Multicast Tree Construction

IP multicast requires constructing distribution trees from sources to group members. The simplest approach builds a source-based tree using Dijkstra's algorithm with the source as root, pruning branches without interested receivers. Shared tree protocols like PIM-SM build a tree rooted at a rendezvous point, allowing multiple sources to share infrastructure. Core-Based Trees (CBT) extend this by carefully selecting core routers to minimize aggregate path cost.

The algorithmic challenge balances tree optimality against state overhead. Source-specific trees provide optimal paths but require per-source state at every router. Shared trees reduce state by aggregating sources but may create suboptimal paths. Practical multicast protocols choose based on group characteristics: shared trees for many-to-many applications like video conferencing, source trees for one-to-many distributions like streaming.

```c
// Multicast tree node
typedef struct mc_node {
    int router_id;
    int is_member;                // Has group members
    int *children;                // Child routers in tree
    int num_children;
    uint32_t cost_to_source;      // For source-based tree
} mc_node_t;

// Build source-based multicast tree
void build_source_tree(int num_routers, int **adj_matrix, 
                      int source, int *group_members, int num_members) {
    printf("=== Source-Based Multicast Tree ===\n\n");
    
    printf("Source: Router %d\n", source);
    printf("Group members: ");
    for (int i = 0; i < num_members; i++) {
        printf("%d ", group_members[i]);
    }
    printf("\n\n");
    
    // Run shortest path from source (simplified Dijkstra)
    uint32_t *dist = (uint32_t*)malloc(num_routers * sizeof(uint32_t));
    int *parent = (int*)malloc(num_routers * sizeof(int));
    int *visited = (int*)calloc(num_routers, sizeof(int));
    
    for (int i = 0; i < num_routers; i++) {
        dist[i] = UINT32_MAX;
        parent[i] = -1;
    }
    dist[source] = 0;
    
    // Dijkstra's algorithm
    for (int count = 0; count < num_routers; count++) {
        uint32_t min_dist = UINT32_MAX;
        int min_vertex = -1;
        
        for (int v = 0; v < num_routers; v++) {
            if (!visited[v] && dist[v] < min_dist) {
                min_dist = dist[v];
                min_vertex = v;
            }
        }
        
        if (min_vertex == -1) break;
        visited[min_vertex] = 1;
        
        for (int v = 0; v < num_routers; v++) {
            if (adj_matrix[min_vertex][v] && !visited[v]) {
                uint32_t new_dist = dist[min_vertex] + adj_matrix[min_vertex][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    parent[v] = min_vertex;
                }
            }
        }
    }
    
    printf("Multicast Tree (shortest paths from source):\n");
    for (int i = 0; i < num_members; i++) {
        int member = group_members[i];
        printf("  Path to Router %d (cost %u): ", member, dist[member]);
        
        // Print path
        int path[20];
        int path_len = 0;
        int current = member;
        
        while (current != -1) {
            path[path_len++] = current;
            current = parent[current];
        }
        
        for (int j = path_len - 1; j >= 0; j--) {
            printf("%d", path[j]);
            if (j > 0) printf(" -> ");
        }
        printf("\n");
    }
    
    printf("\n");
    
    free(dist);
    free(parent);
    free(visited);
}
```

## Conclusion

Tree algorithms solve the fundamental problem of maintaining loop-free connectivity in redundant network topologies. Spanning Tree Protocol prevents broadcast storms by constructing a logical tree from physical meshes, sacrificing some links for stability. Minimum Spanning Tree algorithms optimize network design by selecting the cheapest subset of links providing full connectivity. RSTP accelerates convergence through explicit synchronization rather than timer-based transitions.

Understanding these algorithms matters because they operate autonomously in every switched network. A misconfigured STP priority can route traffic suboptimally. A poorly chosen MST for multicast can waste bandwidth. RSTP's rapid convergence enables high-availability designs impossible with classic STP. These aren't merely theoretical constructs—they're active protocols making forwarding decisions in production networks every second. Master tree algorithms, and you understand how networks maintain connectivity despite failures and how to design optimal topologies that balance cost, performance, and resilience.

# Graph Traversal: DFS and BFS in Network Discovery

## The Topology Discovery Problem

Network management systems must discover device connectivity to visualize topology, plan capacity, and diagnose problems. A management station starts with one known device—perhaps a core router—and must find all reachable devices and their interconnections. Each discovered device exposes its neighbors through protocols like LLDP or CDP. The management system faces a graph traversal problem: systematically visit every reachable node, following edges to discover the complete topology.

Two fundamental algorithms solve this: Depth-First Search (DFS) and Breadth-First Search (BFS). DFS explores as far as possible along each branch before backtracking, while BFS explores all neighbors at the current distance before moving farther. The choice between them depends on what you need: DFS naturally detects cycles and finds articulation points, while BFS computes shortest paths and discovers topology layer by layer. Understanding their properties determines which algorithm fits your discovery requirements.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Network device structure
typedef struct device {
    int id;
    char name[32];
    char ip_address[16];
    int discovered;
    int distance;                 // Distance from source (for BFS)
    int parent;                   // Parent in traversal tree
    int discovery_time;           // DFS discovery time
    int finish_time;              // DFS finish time
} device_t;

// Network topology (adjacency list)
typedef struct network {
    device_t *devices;
    int **adjacency;              // Adjacency matrix for simplicity
    int num_devices;
    int time_counter;             // For DFS timestamps
} network_t;

// Create network
network_t* create_network(int num_devices) {
    network_t *net = (network_t*)malloc(sizeof(network_t));
    net->num_devices = num_devices;
    net->time_counter = 0;
    
    net->devices = (device_t*)calloc(num_devices, sizeof(device_t));
    net->adjacency = (int**)malloc(num_devices * sizeof(int*));
    
    for (int i = 0; i < num_devices; i++) {
        net->adjacency[i] = (int*)calloc(num_devices, sizeof(int));
    }
    
    return net;
}

// Initialize device
void init_device(network_t *net, int id, const char *name, const char *ip) {
    device_t *dev = &net->devices[id];
    dev->id = id;
    strncpy(dev->name, name, sizeof(dev->name) - 1);
    strncpy(dev->ip_address, ip, sizeof(dev->ip_address) - 1);
    dev->discovered = 0;
    dev->distance = -1;
    dev->parent = -1;
    dev->discovery_time = -1;
    dev->finish_time = -1;
}

// Add bidirectional link
void add_link(network_t *net, int device1, int device2) {
    net->adjacency[device1][device2] = 1;
    net->adjacency[device2][device1] = 1;
}

// Queue for BFS
typedef struct queue {
    int *data;
    int capacity;
    int front;
    int rear;
    int size;
} queue_t;

queue_t* create_queue(int capacity) {
    queue_t *q = (queue_t*)malloc(sizeof(queue_t));
    q->capacity = capacity;
    q->data = (int*)malloc(capacity * sizeof(int));
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    return q;
}

void enqueue(queue_t *q, int value) {
    if (q->size < q->capacity) {
        q->rear = (q->rear + 1) % q->capacity;
        q->data[q->rear] = value;
        q->size++;
    }
}

int dequeue(queue_t *q) {
    if (q->size == 0) return -1;
    
    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return value;
}

int is_queue_empty(queue_t *q) {
    return q->size == 0;
}

// Breadth-First Search
void bfs(network_t *net, int start_device) {
    printf("=== Breadth-First Search ===\n\n");
    printf("Starting BFS from %s (%s)\n\n",
           net->devices[start_device].name,
           net->devices[start_device].ip_address);
    
    // Reset discovery state
    for (int i = 0; i < net->num_devices; i++) {
        net->devices[i].discovered = 0;
        net->devices[i].distance = -1;
        net->devices[i].parent = -1;
    }
    
    queue_t *q = create_queue(net->num_devices);
    
    // Start from initial device
    net->devices[start_device].discovered = 1;
    net->devices[start_device].distance = 0;
    enqueue(q, start_device);
    
    printf("Discovery order:\n");
    int discovery_order = 1;
    
    while (!is_queue_empty(q)) {
        int current = dequeue(q);
        device_t *dev = &net->devices[current];
        
        printf("  %d. Discovered %s at distance %d",
               discovery_order++, dev->name, dev->distance);
        
        if (dev->parent != -1) {
            printf(" (via %s)", net->devices[dev->parent].name);
        }
        printf("\n");
        
        // Explore neighbors
        for (int neighbor = 0; neighbor < net->num_devices; neighbor++) {
            if (net->adjacency[current][neighbor] && 
                !net->devices[neighbor].discovered) {
                
                net->devices[neighbor].discovered = 1;
                net->devices[neighbor].distance = dev->distance + 1;
                net->devices[neighbor].parent = current;
                enqueue(q, neighbor);
            }
        }
    }
    
    printf("\n");
    free(q->data);
    free(q);
}

// Print shortest paths (BFS result)
void print_shortest_paths(network_t *net, int source) {
    printf("Shortest paths from %s:\n", net->devices[source].name);
    
    for (int i = 0; i < net->num_devices; i++) {
        if (i == source) continue;
        
        device_t *dev = &net->devices[i];
        printf("  To %s: ", dev->name);
        
        if (dev->distance == -1) {
            printf("Unreachable\n");
            continue;
        }
        
        printf("distance %d, path: ", dev->distance);
        
        // Reconstruct path
        int path[20];
        int path_len = 0;
        int current = i;
        
        while (current != -1) {
            path[path_len++] = current;
            current = net->devices[current].parent;
        }
        
        // Print path in reverse
        for (int j = path_len - 1; j >= 0; j--) {
            printf("%s", net->devices[path[j]].name);
            if (j > 0) printf(" -> ");
        }
        printf("\n");
    }
    printf("\n");
}

// Depth-First Search (recursive)
void dfs_visit(network_t *net, int device_id) {
    device_t *dev = &net->devices[device_id];
    
    // Mark as discovered
    net->time_counter++;
    dev->discovery_time = net->time_counter;
    dev->discovered = 1;
    
    printf("  Discovered %s at time %d\n", dev->name, dev->discovery_time);
    
    // Explore neighbors
    for (int neighbor = 0; neighbor < net->num_devices; neighbor++) {
        if (net->adjacency[device_id][neighbor] && 
            !net->devices[neighbor].discovered) {
            
            net->devices[neighbor].parent = device_id;
            dfs_visit(net, neighbor);
        } else if (net->adjacency[device_id][neighbor] && 
                  net->devices[neighbor].discovered &&
                  net->devices[neighbor].finish_time == -1) {
            // Back edge detected (cycle)
            printf("  [Back edge: %s -> %s indicates cycle]\n",
                   dev->name, net->devices[neighbor].name);
        }
    }
    
    // Mark as finished
    net->time_counter++;
    dev->finish_time = net->time_counter;
    
    printf("  Finished %s at time %d\n", dev->name, dev->finish_time);
}

// Depth-First Search
void dfs(network_t *net, int start_device) {
    printf("=== Depth-First Search ===\n\n");
    printf("Starting DFS from %s (%s)\n\n",
           net->devices[start_device].name,
           net->devices[start_device].ip_address);
    
    // Reset discovery state
    for (int i = 0; i < net->num_devices; i++) {
        net->devices[i].discovered = 0;
        net->devices[i].parent = -1;
        net->devices[i].discovery_time = -1;
        net->devices[i].finish_time = -1;
    }
    net->time_counter = 0;
    
    printf("Discovery process:\n");
    dfs_visit(net, start_device);
    
    // Visit any remaining unvisited nodes
    for (int i = 0; i < net->num_devices; i++) {
        if (!net->devices[i].discovered) {
            printf("\n  Starting new DFS tree from %s\n", 
                   net->devices[i].name);
            dfs_visit(net, i);
        }
    }
    
    printf("\n");
}

// Detect cycles using DFS
int detect_cycles_dfs(network_t *net) {
    printf("=== Cycle Detection with DFS ===\n\n");
    
    // Reset state
    for (int i = 0; i < net->num_devices; i++) {
        net->devices[i].discovered = 0;
        net->devices[i].finish_time = -1;
    }
    
    int cycle_found = 0;
    
    // DFS to detect back edges
    for (int i = 0; i < net->num_devices; i++) {
        if (!net->devices[i].discovered) {
            // Do DFS but check for back edges
            // Back edge = edge to ancestor in DFS tree = cycle
            
            // Simplified: just run DFS and watch for back edges
            dfs(net, i);
            
            // Check if any back edges were found
            for (int u = 0; u < net->num_devices; u++) {
                for (int v = 0; v < net->num_devices; v++) {
                    if (net->adjacency[u][v] &&
                        net->devices[u].discovered &&
                        net->devices[v].discovered &&
                        net->devices[v].parent != u &&
                        net->devices[u].parent != v &&
                        net->devices[v].discovery_time < net->devices[u].discovery_time &&
                        net->devices[v].finish_time > net->devices[u].finish_time) {
                        printf("Cycle detected: %s -> %s\n",
                               net->devices[u].name, net->devices[v].name);
                        cycle_found = 1;
                    }
                }
            }
        }
    }
    
    if (cycle_found) {
        printf("\nNetwork contains cycles (requires STP)\n\n");
    } else {
        printf("\nNo cycles detected (tree topology)\n\n");
    }
    
    return cycle_found;
}

// Find articulation points (critical devices)
void find_articulation_points(network_t *net) {
    printf("=== Finding Articulation Points (Critical Devices) ===\n\n");
    printf("Articulation points are devices whose failure disconnects the network\n\n");
    
    int *low = (int*)malloc(net->num_devices * sizeof(int));
    int *visited = (int*)calloc(net->num_devices, sizeof(int));
    int *parent = (int*)malloc(net->num_devices * sizeof(int));
    int *is_articulation = (int*)calloc(net->num_devices, sizeof(int));
    
    for (int i = 0; i < net->num_devices; i++) {
        parent[i] = -1;
        low[i] = -1;
    }
    
    // Simplified articulation point detection
    // A vertex v is an articulation point if:
    // 1. v is root of DFS tree and has 2+ children
    // 2. v is not root and has child u where low[u] >= discovery[v]
    
    // For demo, we'll mark devices with degree 1 neighbors as critical
    for (int i = 0; i < net->num_devices; i++) {
        int degree = 0;
        for (int j = 0; j < net->num_devices; j++) {
            if (net->adjacency[i][j]) degree++;
        }
        
        // If this device has neighbors with only one connection (itself),
        // it's critical
        for (int j = 0; j < net->num_devices; j++) {
            if (net->adjacency[i][j]) {
                int neighbor_degree = 0;
                for (int k = 0; k < net->num_devices; k++) {
                    if (net->adjacency[j][k]) neighbor_degree++;
                }
                
                if (neighbor_degree == 1) {
                    is_articulation[i] = 1;
                }
            }
        }
    }
    
    printf("Critical devices (articulation points):\n");
    int found_any = 0;
    for (int i = 0; i < net->num_devices; i++) {
        if (is_articulation[i]) {
            printf("  %s - failure would isolate parts of network\n",
                   net->devices[i].name);
            found_any = 1;
        }
    }
    
    if (!found_any) {
        printf("  None found - network is well-connected\n");
    }
    
    printf("\n");
    
    free(low);
    free(visited);
    free(parent);
    free(is_articulation);
}

// Compare BFS vs DFS
void compare_bfs_dfs(network_t *net, int source) {
    printf("=== BFS vs DFS Comparison ===\n\n");
    
    printf("BFS Characteristics:\n");
    printf("  • Explores level-by-level (all neighbors first)\n");
    printf("  • Uses queue data structure\n");
    printf("  • Finds shortest paths (unweighted graphs)\n");
    printf("  • Good for: Topology discovery, hop-count routing\n");
    printf("  • Time: O(V + E), Space: O(V)\n\n");
    
    printf("DFS Characteristics:\n");
    printf("  • Explores depth-first (follow path until dead end)\n");
    printf("  • Uses stack (recursion) data structure\n");
    printf("  • Detects cycles, finds articulation points\n");
    printf("  • Good for: Cycle detection, connectivity analysis\n");
    printf("  • Time: O(V + E), Space: O(V)\n\n");
    
    printf("Running both on the same network:\n\n");
    
    bfs(net, source);
    print_shortest_paths(net, source);
    
    dfs(net, source);
}

// Practical topology discovery simulation
void simulate_topology_discovery() {
    printf("=== Network Topology Discovery Simulation ===\n\n");
    
    // Create network
    network_t *net = create_network(8);
    
    // Initialize devices
    init_device(net, 0, "CoreRouter", "10.0.0.1");
    init_device(net, 1, "DistRouter1", "10.0.1.1");
    init_device(net, 2, "DistRouter2", "10.0.2.1");
    init_device(net, 3, "AccessSW1", "10.1.1.1");
    init_device(net, 4, "AccessSW2", "10.1.2.1");
    init_device(net, 5, "AccessSW3", "10.1.3.1");
    init_device(net, 6, "AccessSW4", "10.1.4.1");
    init_device(net, 7, "EdgeRouter", "192.168.1.1");
    
    // Add links (realistic campus network topology)
    add_link(net, 0, 1);  // Core -> Dist1
    add_link(net, 0, 2);  // Core -> Dist2
    add_link(net, 1, 2);  // Dist1 <-> Dist2 (redundancy)
    add_link(net, 1, 3);  // Dist1 -> Access1
    add_link(net, 1, 4);  // Dist1 -> Access2
    add_link(net, 2, 5);  // Dist2 -> Access3
    add_link(net, 2, 6);  // Dist2 -> Access4
    add_link(net, 0, 7);  // Core -> Edge
    
    printf("Campus Network Topology:\n");
    printf("  8 devices: 1 Core, 2 Distribution, 4 Access, 1 Edge\n");
    printf("  9 links with redundant paths\n\n");
    
    // Discover topology using BFS
    printf("Scenario 1: Management station discovers network\n");
    bfs(net, 0);  // Start from core router
    print_shortest_paths(net, 0);
    
    // Detect cycles
    printf("Scenario 2: Check for redundant paths (cycles)\n");
    detect_cycles_dfs(net);
    
    // Find critical points
    printf("Scenario 3: Identify single points of failure\n");
    find_articulation_points(net);
    
    // Cleanup
    for (int i = 0; i < net->num_devices; i++) {
        free(net->adjacency[i]);
    }
    free(net->adjacency);
    free(net->devices);
    free(net);
}
```

## Layer-by-Layer Discovery

Network management protocols like SNMP-based discovery use BFS naturally. Starting from a seed device, the system queries its interface table and ARP cache to find directly connected neighbors. These neighbors become the next layer to query. BFS guarantees discovering devices in order of hop distance, building an accurate topology map showing physical and logical relationships. The breadth-first approach ensures no device is queried twice, avoiding redundant polling.

The practical advantage lies in efficient resource usage. Polling thousands of devices consumes management bandwidth and CPU cycles. BFS visits each device exactly once, minimizing overhead. The layer-by-layer approach also provides progress visibility—after discovering layer N, you know all devices within N hops. For troubleshooting, this distance information helps scope problem isolation: if all devices beyond hop 3 are unreachable, the problem likely sits at hop 3.

```c
// SNMP-style topology discovery
typedef struct discovery_state {
    queue_t *to_discover;
    int *queried;
    int devices_found;
    int queries_sent;
} discovery_state_t;

void snmp_topology_discovery(network_t *net, int seed_device) {
    printf("=== SNMP Topology Discovery (BFS-based) ===\n\n");
    printf("Seed device: %s (%s)\n\n",
           net->devices[seed_device].name,
           net->devices[seed_device].ip_address);
    
    discovery_state_t state;
    state.to_discover = create_queue(net->num_devices);
    state.queried = (int*)calloc(net->num_devices, sizeof(int));
    state.devices_found = 0;
    state.queries_sent = 0;
    
    // Start with seed
    enqueue(state.to_discover, seed_device);
    
    printf("Discovery process:\n");
    
    while (!is_queue_empty(state.to_discover)) {
        int current = dequeue(state.to_discover);
        
        if (state.queried[current]) continue;
        
        // Query device (simulate SNMP GET)
        state.queries_sent++;
        state.devices_found++;
        state.queried[current] = 1;
        
        device_t *dev = &net->devices[current];
        printf("  Query %d: %s (%s)\n", 
               state.queries_sent, dev->name, dev->ip_address);
        
        // Discover neighbors (simulate parsing interface table)
        int neighbors_found = 0;
        for (int neighbor = 0; neighbor < net->num_devices; neighbor++) {
            if (net->adjacency[current][neighbor] && !state.queried[neighbor]) {
                printf("    Found neighbor: %s (%s)\n",
                       net->devices[neighbor].name,
                       net->devices[neighbor].ip_address);
                enqueue(state.to_discover, neighbor);
                neighbors_found++;
            }
        }
        
        if (neighbors_found == 0) {
            printf("    (Edge device, no new neighbors)\n");
        }
    }
    
    printf("\nDiscovery complete:\n");
    printf("  Devices found: %d\n", state.devices_found);
    printf("  SNMP queries sent: %d\n", state.queries_sent);
    printf("  Efficiency: %d queries for %d devices (optimal)\n\n",
           state.queries_sent, state.devices_found);
    
    free(state.to_discover->data);
    free(state.to_discover);
    free(state.queried);
}
```

## Cycle Detection and Network Validation

DFS excels at detecting cycles through back edge identification. During traversal, DFS classifies edges: tree edges forming the DFS tree, back edges pointing to ancestors (indicating cycles), forward edges to descendants, and cross edges between different subtrees. In network topology validation, back edges reveal redundant paths requiring loop prevention protocols. A network intended as a tree with an accidental extra link shows up immediately during DFS.

This cycle detection proves essential for verifying STP operation. After STP converges, the active topology should form a tree. Running DFS should find no back edges among forwarding ports. If back edges appear, STP has failed—perhaps due to misconfiguration or a bug. This validation can run continuously, alerting operators to loops before broadcast storms bring down the network.

```c
// Network validation with DFS
typedef struct validation_result {
    int has_cycles;
    int num_components;           // Number of disconnected components
    int *component_id;            // Which component each device belongs to
} validation_result_t;

validation_result_t validate_network_topology(network_t *net) {
    printf("=== Network Topology Validation ===\n\n");
    
    validation_result_t result;
    result.has_cycles = 0;
    result.num_components = 0;
    result.component_id = (int*)malloc(net->num_devices * sizeof(int));
    
    // Reset state
    for (int i = 0; i < net->num_devices; i++) {
        net->devices[i].discovered = 0;
        net->devices[i].finish_time = -1;
        result.component_id[i] = -1;
    }
    
    printf("Running DFS to validate topology...\n\n");
    
    // Run DFS from each unvisited component
    for (int i = 0; i < net->num_devices; i++) {
        if (!net->devices[i].discovered) {
            printf("Component %d starting from %s:\n",
                   result.num_components, net->devices[i].name);
            
            // Mark component
            int component_start = i;
            
            // DFS visit
            dfs_visit(net, i);
            
            // Mark all devices in this component
            for (int j = 0; j < net->num_devices; j++) {
                if (net->devices[j].discovered && 
                    result.component_id[j] == -1) {
                    result.component_id[j] = result.num_components;
                }
            }
            
            result.num_components++;
            printf("\n");
        }
    }
    
    // Check for back edges (cycles)
    for (int u = 0; u < net->num_devices; u++) {
        for (int v = u + 1; v < net->num_devices; v++) {
            if (net->adjacency[u][v] && 
                net->devices[u].discovered &&
                net->devices[v].discovered &&
                result.component_id[u] == result.component_id[v]) {
                
                // Check if this is a back edge
                if (net->devices[v].parent != u && 
                    net->devices[u].parent != v) {
                    printf("Redundant path detected: %s <-> %s\n",
                           net->devices[u].name, net->devices[v].name);
                    result.has_cycles = 1;
                }
            }
        }
    }
    
    printf("\nValidation summary:\n");
    printf("  Network components: %d ", result.num_components);
    if (result.num_components == 1) {
        printf("(fully connected ✓)\n");
    } else {
        printf("(DISCONNECTED - network is partitioned!)\n");
    }
    
    printf("  Redundant paths: %s\n", 
           result.has_cycles ? "Yes (requires loop prevention)" : "No");
    printf("  Topology type: %s\n",
           result.has_cycles ? "Mesh/Ring" : "Tree");
    
    printf("\n");
    
    return result;
}
```

## Connected Components and Network Partitioning

When links fail, networks may partition into disconnected components. Running DFS from a single starting point discovers only the component containing that point. To find all components, start DFS from any unvisited device after the first DFS completes. Each DFS start identifies a new component. This component detection matters for fault diagnosis—if management can reach some devices but not others, the network has partitioned.

The number of components and their sizes inform recovery strategies. Two components of similar size suggest a major link bundle failure, requiring immediate attention. Many small components might indicate a cascading failure spreading through the network. Component analysis guides troubleshooting priorities and helps operators understand failure scope before diving into detailed diagnosis.

```c
// Analyze network components in detail
void analyze_network_components(network_t *net) {
    printf("=== Network Component Analysis ===\n\n");
    
    validation_result_t result = validate_network_topology(net);
    
    if (result.num_components == 1) {
        printf("Network is fully connected - no partitioning\n\n");
        free(result.component_id);
        return;
    }
    
    printf("CRITICAL: Network is partitioned into %d components\n\n",
           result.num_components);
    
    // Analyze each component
    for (int comp = 0; comp < result.num_components; comp++) {
        printf("Component %d:\n", comp);
        printf("  Devices: ");
        
        int device_count = 0;
        for (int i = 0; i < net->num_devices; i++) {
            if (result.component_id[i] == comp) {
                printf("%s ", net->devices[i].name);
                device_count++;
            }
        }
        printf("\n");
        printf("  Size: %d devices\n", device_count);
        
        // Count internal links
        int internal_links = 0;
        for (int i = 0; i < net->num_devices; i++) {
            if (result.component_id[i] == comp) {
                for (int j = i + 1; j < net->num_devices; j++) {
                    if (result.component_id[j] == comp &&
                        net->adjacency[i][j]) {
                        internal_links++;
                    }
                }
            }
        }
        printf("  Internal links: %d\n\n", internal_links);
    }
    
    printf("Recovery actions:\n");
    printf("  1. Identify failed links between components\n");
    printf("  2. Check physical connectivity\n");
    printf("  3. Verify STP/routing protocol operation\n");
    printf("  4. Consider activating backup paths\n\n");
    
    free(result.component_id);
}
```

## Conclusion

Graph traversal algorithms provide the foundation for network discovery and validation. BFS discovers topology layer-by-layer, finding shortest paths and optimal polling orders for management systems. DFS detects cycles and articulation points, validating network connectivity and identifying critical infrastructure. Component analysis reveals network partitions, guiding fault recovery strategies.

These algorithms operate identically whether traversing a five-device test network or a fifty-thousand-device production network. Understanding their properties—BFS guarantees shortest paths, DFS detects cycles in linear time—allows building robust discovery systems that scale from campus networks to global infrastructures. Master graph traversal, and you understand how network management systems map topology, validate connectivity, and diagnose failures across networks of any size.

# Sorting and Searching Optimizations in Networking

## Why Sorting Matters in Real-Time Systems

Network devices sort constantly: ACL rules by priority, flow records by timestamp, QoS classes by precedence, route advertisements by preference. The sorting algorithm chosen determines whether configuration takes milliseconds or seconds, whether packet classification happens at line rate or drops frames. A router receiving 10,000 BGP route updates must sort them by prefix length and preference before installing in the forwarding table. An IDS must sort packets by connection for session reconstruction. The difference between quicksort and insertion sort here isn't academic—it's the difference between a working system and a failed deployment.

Beyond asymptotic complexity, real-world performance depends on data characteristics. Network data exhibits patterns: mostly sorted route tables receiving incremental updates, nearly identical ACL rules differing in one field, flow records arriving roughly in time order. Algorithms exploiting these patterns outperform general-purpose sorting despite worse worst-case complexity. Understanding when to use timsort over quicksort, when binary search beats interpolation search, separates theoretical knowledge from practical expertise.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// ACL rule structure
typedef struct acl_rule {
    uint32_t src_ip;
    uint32_t src_mask;
    uint32_t dst_ip;
    uint32_t dst_mask;
    uint16_t src_port_min;
    uint16_t src_port_max;
    uint16_t dst_port_min;
    uint16_t dst_port_max;
    uint8_t protocol;
    uint8_t action;           // PERMIT or DENY
    int priority;             // Lower number = higher priority
    int rule_id;
} acl_rule_t;

// Flow record structure
typedef struct flow_record {
    uint64_t timestamp;
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
    uint64_t bytes;
    uint32_t packets;
} flow_record_t;

// Quicksort for ACL rules by priority
void quicksort_acl(acl_rule_t *rules, int low, int high) {
    if (low < high) {
        // Partition
        int pivot_priority = rules[high].priority;
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            if (rules[j].priority < pivot_priority) {
                i++;
                acl_rule_t temp = rules[i];
                rules[i] = rules[j];
                rules[j] = temp;
            }
        }
        
        acl_rule_t temp = rules[i + 1];
        rules[i + 1] = rules[high];
        rules[high] = temp;
        
        int pi = i + 1;
        
        // Recursively sort partitions
        quicksort_acl(rules, low, pi - 1);
        quicksort_acl(rules, pi + 1, high);
    }
}

// Insertion sort for nearly-sorted data
void insertion_sort_acl(acl_rule_t *rules, int n) {
    for (int i = 1; i < n; i++) {
        acl_rule_t key = rules[i];
        int j = i - 1;
        
        while (j >= 0 && rules[j].priority > key.priority) {
            rules[j + 1] = rules[j];
            j--;
        }
        
        rules[j + 1] = key;
    }
}

// Heapsort for guaranteed O(n log n) worst case
void heapify_acl(acl_rule_t *rules, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && rules[left].priority > rules[largest].priority) {
        largest = left;
    }
    
    if (right < n && rules[right].priority > rules[largest].priority) {
        largest = right;
    }
    
    if (largest != i) {
        acl_rule_t temp = rules[i];
        rules[i] = rules[largest];
        rules[largest] = temp;
        
        heapify_acl(rules, n, largest);
    }
}

void heapsort_acl(acl_rule_t *rules, int n) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify_acl(rules, n, i);
    }
    
    // Extract elements from heap
    for (int i = n - 1; i > 0; i--) {
        acl_rule_t temp = rules[0];
        rules[0] = rules[i];
        rules[i] = temp;
        
        heapify_acl(rules, i, 0);
    }
}

// Merge sort for stable sorting (maintains relative order)
void merge_acl(acl_rule_t *rules, int left, int mid, int right, acl_rule_t *temp) {
    int i = left;
    int j = mid + 1;
    int k = left;
    
    while (i <= mid && j <= right) {
        if (rules[i].priority <= rules[j].priority) {
            temp[k++] = rules[i++];
        } else {
            temp[k++] = rules[j++];
        }
    }
    
    while (i <= mid) {
        temp[k++] = rules[i++];
    }
    
    while (j <= right) {
        temp[k++] = rules[j++];
    }
    
    for (i = left; i <= right; i++) {
        rules[i] = temp[i];
    }
}

void mergesort_acl(acl_rule_t *rules, int left, int right, acl_rule_t *temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergesort_acl(rules, left, mid, temp);
        mergesort_acl(rules, mid + 1, right, temp);
        merge_acl(rules, left, mid, right, temp);
    }
}

// Compare sorting algorithms
void compare_sorting_algorithms() {
    printf("=== Sorting Algorithm Comparison ===\n\n");
    
    int sizes[] = {100, 1000, 10000};
    
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        printf("Testing with %d ACL rules:\n\n", n);
        
        // Generate test data
        acl_rule_t *rules_orig = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
        for (int i = 0; i < n; i++) {
            rules_orig[i].priority = rand() % 1000;
            rules_orig[i].rule_id = i;
        }
        
        // Quicksort
        acl_rule_t *rules = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
        memcpy(rules, rules_orig, n * sizeof(acl_rule_t));
        
        clock_t start = clock();
        quicksort_acl(rules, 0, n - 1);
        clock_t end = clock();
        double quicksort_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        printf("Quicksort:     %.3f ms\n", quicksort_time);
        free(rules);
        
        // Heapsort
        rules = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
        memcpy(rules, rules_orig, n * sizeof(acl_rule_t));
        
        start = clock();
        heapsort_acl(rules, n);
        end = clock();
        double heapsort_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        printf("Heapsort:      %.3f ms\n", heapsort_time);
        free(rules);
        
        // Mergesort
        rules = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
        acl_rule_t *temp = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
        memcpy(rules, rules_orig, n * sizeof(acl_rule_t));
        
        start = clock();
        mergesort_acl(rules, 0, n - 1, temp);
        end = clock();
        double mergesort_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        printf("Mergesort:     %.3f ms\n", mergesort_time);
        free(rules);
        free(temp);
        
        printf("\n");
        free(rules_orig);
    }
    
    printf("Summary:\n");
    printf("  Quicksort: Fastest average case, O(n²) worst case\n");
    printf("  Heapsort:  Guaranteed O(n log n), in-place\n");
    printf("  Mergesort: Stable sort, O(n log n), needs extra space\n\n");
}

// Demonstrate adaptive sorting
void demonstrate_adaptive_sorting() {
    printf("=== Adaptive Sorting for Network Data ===\n\n");
    
    int n = 1000;
    
    // Test 1: Random data
    printf("Test 1: Random ACL rules\n");
    acl_rule_t *rules_random = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
    for (int i = 0; i < n; i++) {
        rules_random[i].priority = rand() % 1000;
    }
    
    clock_t start = clock();
    quicksort_acl(rules_random, 0, n - 1);
    clock_t end = clock();
    printf("  Quicksort: %.3f ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);
    
    // Test 2: Nearly sorted (common in incremental updates)
    printf("\nTest 2: Nearly sorted rules (90%% sorted)\n");
    acl_rule_t *rules_nearly = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
    for (int i = 0; i < n; i++) {
        rules_nearly[i].priority = i;
    }
    // Shuffle 10%
    for (int i = 0; i < n / 10; i++) {
        int idx1 = rand() % n;
        int idx2 = rand() % n;
        acl_rule_t temp = rules_nearly[idx1];
        rules_nearly[idx1] = rules_nearly[idx2];
        rules_nearly[idx2] = temp;
    }
    
    start = clock();
    insertion_sort_acl(rules_nearly, n);
    end = clock();
    double insertion_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    // Compare with quicksort
    memcpy(rules_random, rules_nearly, n * sizeof(acl_rule_t));
    start = clock();
    quicksort_acl(rules_random, 0, n - 1);
    end = clock();
    double quick_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    printf("  Insertion sort: %.3f ms\n", insertion_time);
    printf("  Quicksort:      %.3f ms\n", quick_time);
    printf("  Speedup: %.2fx\n", quick_time / insertion_time);
    
    printf("\nKey insight: For nearly-sorted data (incremental updates),\n");
    printf("insertion sort outperforms quicksort dramatically!\n\n");
    
    free(rules_random);
    free(rules_nearly);
}
```

## Radix Sort for IP Addresses

IP addresses are 32-bit integers, naturally suited for radix sort. Unlike comparison sorts requiring O(n log n) operations, radix sort achieves O(n) time by processing addresses digit by digit or byte by byte. For sorting millions of flow records by source IP, radix sort dramatically outperforms quicksort. Processing each byte requires one pass through the data, so sorting 32-bit IPs takes four passes—constant time per element regardless of input size.

The practical implementation uses counting sort for each digit position. Process the least significant byte first, counting occurrences of each value (0-255), computing positions, and redistributing elements. Repeat for each byte moving leftward. The algorithm remains stable—equal elements maintain relative order—important when sorting by multiple fields like IP address then port number. Network processors often implement radix sort in hardware for wire-speed packet classification.

```c
// Counting sort for one byte position
void counting_sort_ip(uint32_t *ips, uint32_t *output, int n, int byte_pos) {
    int count[256] = {0};
    int shift = byte_pos * 8;
    
    // Count occurrences
    for (int i = 0; i < n; i++) {
        uint8_t byte = (ips[i] >> shift) & 0xFF;
        count[byte]++;
    }
    
    // Cumulative count
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }
    
    // Build output
    for (int i = n - 1; i >= 0; i--) {
        uint8_t byte = (ips[i] >> shift) & 0xFF;
        output[count[byte] - 1] = ips[i];
        count[byte]--;
    }
}

// Radix sort for IP addresses (LSD - Least Significant Digit first)
void radix_sort_ip(uint32_t *ips, int n) {
    uint32_t *output = (uint32_t*)malloc(n * sizeof(uint32_t));
    
    // Sort by each byte (4 passes)
    for (int byte_pos = 0; byte_pos < 4; byte_pos++) {
        counting_sort_ip(ips, output, n, byte_pos);
        
        // Copy back
        for (int i = 0; i < n; i++) {
            ips[i] = output[i];
        }
    }
    
    free(output);
}

// Compare radix sort vs quicksort for IPs
void compare_ip_sorting() {
    printf("=== IP Address Sorting: Radix vs Quicksort ===\n\n");
    
    int sizes[] = {10000, 100000, 1000000};
    
    for (int s = 0; s < 3; s++) {
        int n = sizes[s];
        printf("Sorting %d IP addresses:\n", n);
        
        // Generate random IPs
        uint32_t *ips_radix = (uint32_t*)malloc(n * sizeof(uint32_t));
        uint32_t *ips_quick = (uint32_t*)malloc(n * sizeof(uint32_t));
        
        for (int i = 0; i < n; i++) {
            ips_radix[i] = rand();
            ips_quick[i] = ips_radix[i];
        }
        
        // Radix sort
        clock_t start = clock();
        radix_sort_ip(ips_radix, n);
        clock_t end = clock();
        double radix_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        // Quicksort (using stdlib qsort)
        start = clock();
        qsort(ips_quick, n, sizeof(uint32_t), 
              (int(*)(const void*, const void*))
              [](const void *a, const void *b) {
                  uint32_t ia = *(uint32_t*)a;
                  uint32_t ib = *(uint32_t*)b;
                  return (ia > ib) - (ia < ib);
              });
        end = clock();
        double quick_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
        
        printf("  Radix sort: %.2f ms\n", radix_time);
        printf("  Quicksort:  %.2f ms\n", quick_time);
        printf("  Speedup:    %.2fx\n\n", quick_time / radix_time);
        
        free(ips_radix);
        free(ips_quick);
    }
    
    printf("Key advantage: O(n) time complexity for fixed-size integers\n");
    printf("Best for: Flow sorting, route table organization, packet classification\n\n");
}
```

## Binary Search and Variants

Binary search finds elements in sorted arrays with O(log n) comparisons, essential for rule matching and route lookup. Given 1024 sorted ACL rules, binary search locates a match in at most 10 comparisons versus 512 average for linear search. However, standard binary search assumes uniform distribution. Network data often clusters—many rules in certain IP ranges, few elsewhere. Interpolation search exploits this by estimating position based on value, achieving O(log log n) for uniformly distributed data.

Exponential search combines best-of-both-worlds: start with small steps doubling in size until overshooting, then binary search the bounded range. This finds elements near the beginning faster than pure binary search—useful for time-ordered logs where recent entries are accessed most. The algorithm requires O(log n) comparisons worst-case like binary search but O(log i) where i is the target position, beneficial when searching incrementally updated sorted data.

```c
// Standard binary search
int binary_search_rule(acl_rule_t *rules, int n, int target_priority) {
    int left = 0;
    int right = n - 1;
    int comparisons = 0;
    
    while (left <= right) {
        comparisons++;
        int mid = left + (right - left) / 2;
        
        if (rules[mid].priority == target_priority) {
            printf("Binary search: found in %d comparisons\n", comparisons);
            return mid;
        }
        
        if (rules[mid].priority < target_priority) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    printf("Binary search: not found after %d comparisons\n", comparisons);
    return -1;
}

// Interpolation search (for uniformly distributed data)
int interpolation_search_rule(acl_rule_t *rules, int n, int target_priority) {
    int left = 0;
    int right = n - 1;
    int comparisons = 0;
    
    while (left <= right && 
           target_priority >= rules[left].priority &&
           target_priority <= rules[right].priority) {
        
        comparisons++;
        
        if (left == right) {
            if (rules[left].priority == target_priority) {
                printf("Interpolation search: found in %d comparisons\n", comparisons);
                return left;
            }
            break;
        }
        
        // Interpolate position
        int pos = left + ((double)(target_priority - rules[left].priority) /
                         (rules[right].priority - rules[left].priority) *
                         (right - left));
        
        if (rules[pos].priority == target_priority) {
            printf("Interpolation search: found in %d comparisons\n", comparisons);
            return pos;
        }
        
        if (rules[pos].priority < target_priority) {
            left = pos + 1;
        } else {
            right = pos - 1;
        }
    }
    
    printf("Interpolation search: not found after %d comparisons\n", comparisons);
    return -1;
}

// Exponential search (good for unbounded or unknown-size arrays)
int exponential_search_rule(acl_rule_t *rules, int n, int target_priority) {
    int comparisons = 0;
    
    if (rules[0].priority == target_priority) {
        printf("Exponential search: found in 1 comparison\n");
        return 0;
    }
    
    // Find range for binary search
    int bound = 1;
    while (bound < n && rules[bound].priority < target_priority) {
        comparisons++;
        bound *= 2;
    }
    
    // Binary search in range [bound/2, min(bound, n-1)]
    int left = bound / 2;
    int right = (bound < n) ? bound : n - 1;
    
    while (left <= right) {
        comparisons++;
        int mid = left + (right - left) / 2;
        
        if (rules[mid].priority == target_priority) {
            printf("Exponential search: found in %d comparisons\n", comparisons);
            return mid;
        }
        
        if (rules[mid].priority < target_priority) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    printf("Exponential search: not found after %d comparisons\n", comparisons);
    return -1;
}

// Compare search algorithms
void compare_search_algorithms() {
    printf("=== Search Algorithm Comparison ===\n\n");
    
    int n = 10000;
    acl_rule_t *rules = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
    
    // Create sorted array with uniform distribution
    for (int i = 0; i < n; i++) {
        rules[i].priority = i * 10;  // Uniform spacing
        rules[i].rule_id = i;
    }
    
    // Search for element near beginning
    printf("Searching for priority 100 (near beginning):\n");
    binary_search_rule(rules, n, 100);
    interpolation_search_rule(rules, n, 100);
    exponential_search_rule(rules, n, 100);
    
    printf("\nSearching for priority 50000 (middle):\n");
    binary_search_rule(rules, n, 50000);
    interpolation_search_rule(rules, n, 50000);
    exponential_search_rule(rules, n, 50000);
    
    printf("\nSearching for priority 95000 (near end):\n");
    binary_search_rule(rules, n, 95000);
    interpolation_search_rule(rules, n, 95000);
    exponential_search_rule(rules, n, 95000);
    
    printf("\n");
    free(rules);
}
```

## Partial Sorting and Selection

Often you need the top-K elements from a large dataset, not a fully sorted array. Finding the top 10 heaviest flows from one million records doesn't require sorting all million. Partial sorting algorithms like quickselect find the k-th smallest element in O(n) average time, enabling heap-based extraction of top-K in O(n + k log k) time. For k much smaller than n, this dramatically outperforms full sorting's O(n log n) cost.

Network monitoring systems use this constantly: top 10 talkers, top 20 destinations, top 5 protocols by bandwidth. A full sort wastes resources processing ranks 11 through 1,000,000 that nobody cares about. Min-heap maintaining K elements streams through data, inserting new elements and evicting the minimum when full. After processing all n elements, the heap contains the K largest, obtainable in sorted order with K extractions.

```c
// Quickselect to find k-th smallest element
int partition_flow(flow_record_t *flows, int low, int high) {
    uint64_t pivot = flows[high].bytes;
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (flows[j].bytes < pivot) {
            i++;
            flow_record_t temp = flows[i];
            flows[i] = flows[j];
            flows[j] = temp;
        }
    }
    
    flow_record_t temp = flows[i + 1];
    flows[i + 1] = flows[high];
    flows[high] = temp;
    
    return i + 1;
}

int quickselect(flow_record_t *flows, int low, int high, int k) {
    if (low == high) {
        return low;
    }
    
    int pi = partition_flow(flows, low, high);
    
    if (k == pi) {
        return k;
    } else if (k < pi) {
        return quickselect(flows, low, pi - 1, k);
    } else {
        return quickselect(flows, pi + 1, high, k);
    }
}

// Find top K heaviest flows efficiently
void find_top_k_flows(flow_record_t *flows, int n, int k) {
    printf("=== Finding Top %d Flows from %d Total ===\n\n", k, n);
    
    // Method 1: Full sort (naive)
    flow_record_t *flows_copy = (flow_record_t*)malloc(n * sizeof(flow_record_t));
    memcpy(flows_copy, flows, n * sizeof(flow_record_t));
    
    clock_t start = clock();
    qsort(flows_copy, n, sizeof(flow_record_t),
          [](const void *a, const void *b) {
              flow_record_t *fa = (flow_record_t*)a;
              flow_record_t *fb = (flow_record_t*)b;
              return (fb->bytes > fa->bytes) - (fb->bytes < fa->bytes);
          });
    clock_t end = clock();
    double full_sort_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    printf("Method 1: Full sort\n");
    printf("  Time: %.3f ms\n", full_sort_time);
    printf("  Top flow: %lu bytes\n\n", flows_copy[0].bytes);
    free(flows_copy);
    
    // Method 2: Partial sort with quickselect
    flows_copy = (flow_record_t*)malloc(n * sizeof(flow_record_t));
    memcpy(flows_copy, flows, n * sizeof(flow_record_t));
    
    start = clock();
    // Find k-th largest (which is (n-k)th smallest in ascending order)
    quickselect(flows_copy, 0, n - 1, n - k);
    
    // Now elements [n-k, n-1] are the K largest (unsorted)
    // Sort just those K elements
    qsort(&flows_copy[n - k], k, sizeof(flow_record_t),
          [](const void *a, const void *b) {
              flow_record_t *fa = (flow_record_t*)a;
              flow_record_t *fb = (flow_record_t*)b;
              return (fb->bytes > fa->bytes) - (fb->bytes < fa->bytes);
          });
    end = clock();
    double partial_sort_time = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    
    printf("Method 2: Quickselect + partial sort\n");
    printf("  Time: %.3f ms\n", partial_sort_time);
    printf("  Speedup: %.2fx\n", full_sort_time / partial_sort_time);
    printf("  Top flow: %lu bytes\n\n", flows_copy[n - k].bytes);
    
    free(flows_copy);
}

// Demonstrate top-K with heap
void demonstrate_top_k() {
    printf("=== Top-K Flow Analysis ===\n\n");
    
    int n = 1000000;
    int k = 10;
    
    flow_record_t *flows = (flow_record_t*)malloc(n * sizeof(flow_record_t));
    
    // Generate flow data (power law distribution)
    for (int i = 0; i < n; i++) {
        flows[i].bytes = rand() % 1000000;
        flows[i].src_ip = rand();
        flows[i].dst_ip = rand();
    }
    
    find_top_k_flows(flows, n, k);
    
    printf("Use case: Network monitoring\n");
    printf("  Finding top 10 talkers from 1M flows\n");
    printf("  Partial sorting is 5-10x faster than full sort\n");
    printf("  Critical for real-time dashboards\n\n");
    
    free(flows);
}
```

## Sorting Stability and Multi-Field Sorting

Network data often requires sorting by multiple criteria: first by priority, then by rule ID for deterministic ordering. Stable sorting algorithms maintain relative order of equal elements, enabling multi-pass sorting. Sort by secondary key, then by primary key using stable sort. Elements equal in the primary key retain their secondary key ordering. Merge sort and radix sort provide stability; quicksort typically does not.

The alternative, comparison functions handling multiple fields, works but couples sorting to specific field combinations. Stable multi-pass sorting decouples concerns: define comparison for each field independently, apply in reverse priority order. This flexibility matters for network policy where business logic determines field precedence, and that logic changes. Rebuilding comparison functions for each combination becomes unsustainable; stable sorting with multiple passes provides clean separation.

```c
// Demonstrate stable sorting for multi-field ACLs
void demonstrate_stable_sorting() {
    printf("=== Stable Sorting for Multi-Field ACLs ===\n\n");
    
    int n = 10;
    acl_rule_t *rules = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
    
    // Create rules with same priorities but different IDs
    rules[0] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 5};
    rules[1] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 3};
    rules[2] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 1};
    rules[3] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 7};
    rules[4] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 2};
    rules[5] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 4};
    rules[6] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 6};
    rules[7] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 9};
    rules[8] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 8};
    rules[9] = (acl_rule_t){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0};
    
    printf("Original rules (priority, ID):\n");
    for (int i = 0; i < n; i++) {
        printf("  Rule: priority=%d, id=%d\n", rules[i].priority, rules[i].rule_id);
    }
    
    // Stable sort by priority
    acl_rule_t *temp = (acl_rule_t*)malloc(n * sizeof(acl_rule_t));
    mergesort_acl(rules, 0, n - 1, temp);
    
    printf("\nAfter stable sort by priority:\n");
    for (int i = 0; i < n; i++) {
        printf("  Rule: priority=%d, id=%d\n", rules[i].priority, rules[i].rule_id);
    }
    
    printf("\nNote: Rules with same priority maintain original order\n");
    printf("  Priority 10: IDs appear as 5, 3, 7, 4, 8 (original order)\n");
    printf("  This ensures deterministic, predictable rule evaluation\n\n");
    
    free(rules);
    free(temp);
}
```

## Conclusion

Sorting and searching algorithms form the bedrock of network device performance. Choosing quicksort over insertion sort, binary search over linear scan, radix sort over comparison sorting determines whether configuration updates take seconds or minutes, whether packet classification occurs at line rate or drops frames. Understanding data characteristics—nearly sorted incremental updates, uniformly distributed IPs, clustered priority values—allows selecting algorithms that exploit these patterns for superior real-world performance.

The algorithms covered here aren't theoretical exercises. Every production router sorts BGP routes by preference before installation. Every firewall searches ACL rules for packet matches. Every flow collector sorts records for analysis. The difference between a system that scales to millions of entries and one that chokes at thousands often lies in algorithm selection. Master these sorting and searching techniques, understand their performance characteristics and data requirements, and you possess essential tools for building network systems that perform under production workloads.

# String Matching in Network Security and Policy

## Pattern Matching in Network Payloads

Intrusion Detection Systems, Deep Packet Inspection devices, and content filters must search packet payloads for attack signatures, malware patterns, and policy violations. A signature might be a specific byte sequence indicating SQL injection, a regular expression matching credit card numbers, or a string identifying prohibited file types. The device must check every packet against thousands of signatures at multi-gigabit speeds. Naive string matching—checking each pattern against each packet position—requires millions of comparisons per second, overwhelming CPUs.

Efficient string matching algorithms reduce this computational burden dramatically. The Boyer-Moore algorithm skips large portions of text by exploiting pattern characteristics, often examining only a fraction of characters. The Aho-Corasick algorithm searches for multiple patterns simultaneously, performing one pass through the packet regardless of pattern count. These algorithms transform string matching from a CPU bottleneck into a manageable operation, enabling real-time traffic analysis at line rate.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

// Naive string matching (brute force)
int naive_search(const char *text, int text_len, const char *pattern, int pattern_len) {
    int comparisons = 0;
    
    for (int i = 0; i <= text_len - pattern_len; i++) {
        int j;
        for (j = 0; j < pattern_len; j++) {
            comparisons++;
            if (text[i + j] != pattern[j]) {
                break;
            }
        }
        
        if (j == pattern_len) {
            return comparisons;  // Found at position i
        }
    }
    
    return comparisons;  // Not found
}

// Demonstrate naive search inefficiency
void demonstrate_naive_search() {
    printf("=== Naive String Matching ===\n\n");
    
    const char *text = "This packet contains malicious SQL injection attack code";
    const char *pattern = "SQL injection";
    
    printf("Text: \"%s\"\n", text);
    printf("Pattern: \"%s\"\n\n", pattern);
    
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    int comparisons = naive_search(text, text_len, pattern, pattern_len);
    
    printf("Naive search:\n");
    printf("  Comparisons: %d\n", comparisons);
    printf("  Worst case: O(n*m) where n=text length, m=pattern length\n");
    printf("  Problem: Many unnecessary comparisons\n\n");
}

// Boyer-Moore bad character rule
void compute_bad_character_table(const char *pattern, int pattern_len, int *bad_char) {
    // Initialize all characters to pattern length (not in pattern)
    for (int i = 0; i < 256; i++) {
        bad_char[i] = pattern_len;
    }
    
    // Fill table with rightmost occurrence of each character
    for (int i = 0; i < pattern_len - 1; i++) {
        bad_char[(unsigned char)pattern[i]] = pattern_len - 1 - i;
    }
}

// Boyer-Moore string matching
int boyer_moore_search(const char *text, int text_len, 
                      const char *pattern, int pattern_len) {
    int bad_char[256];
    compute_bad_character_table(pattern, pattern_len, bad_char);
    
    int comparisons = 0;
    int shifts = 0;
    int i = 0;  // Position in text
    
    while (i <= text_len - pattern_len) {
        int j = pattern_len - 1;
        
        // Match from right to left
        while (j >= 0) {
            comparisons++;
            if (text[i + j] != pattern[j]) {
                break;
            }
            j--;
        }
        
        if (j < 0) {
            // Pattern found
            printf("Boyer-Moore: found at position %d\n", i);
            printf("  Comparisons: %d\n", comparisons);
            printf("  Text positions examined: %d / %d\n", comparisons, text_len);
            return i;
        }
        
        // Shift pattern using bad character rule
        int shift = bad_char[(unsigned char)text[i + j]];
        if (shift == pattern_len) {
            shifts++;
        }
        i += shift;
    }
    
    printf("Boyer-Moore: not found\n");
    printf("  Comparisons: %d\n", comparisons);
    return -1;
}

// Compare naive vs Boyer-Moore
void compare_naive_boyer_moore() {
    printf("=== Naive vs Boyer-Moore Comparison ===\n\n");
    
    // Create text with pattern near the end
    char text[1000];
    memset(text, 'A', sizeof(text) - 1);
    text[999] = '\0';
    
    // Insert pattern near end
    const char *pattern = "MALICIOUS";
    int pattern_len = strlen(pattern);
    memcpy(&text[900], pattern, pattern_len);
    
    int text_len = strlen(text);
    
    printf("Text length: %d bytes\n", text_len);
    printf("Pattern: \"%s\"\n", pattern);
    printf("Pattern location: near byte 900\n\n");
    
    // Naive search
    printf("Naive search:\n");
    clock_t start = clock();
    int naive_comps = naive_search(text, text_len, pattern, pattern_len);
    clock_t end = clock();
    double naive_time = (double)(end - start) / CLOCKS_PER_SEC * 1000000;
    printf("  Comparisons: %d\n", naive_comps);
    printf("  Time: %.2f microseconds\n\n", naive_time);
    
    // Boyer-Moore search
    printf("Boyer-Moore search:\n");
    start = clock();
    boyer_moore_search(text, text_len, pattern, pattern_len);
    end = clock();
    double bm_time = (double)(end - start) / CLOCKS_PER_SEC * 1000000;
    printf("  Time: %.2f microseconds\n\n", bm_time);
    
    printf("Boyer-Moore skips large portions of text!\n");
    printf("Especially effective when pattern doesn't match text alphabet\n\n");
}
```

## Aho-Corasick Multi-Pattern Matching

Intrusion detection systems must check packets against thousands of signatures simultaneously. Running Boyer-Moore for each signature sequentially requires thousands of passes through the packet. The Aho-Corasick algorithm builds a finite state machine from all patterns, processing the text once and finding all pattern matches in a single pass. The state machine includes failure links that redirect to the longest proper suffix when a mismatch occurs, ensuring no potential match is missed.

Construction builds a trie of all patterns, then adds failure links using breadth-first search. Each state's failure link points to the state representing the longest suffix that is also a prefix of some pattern. During search, the algorithm follows the trie for matching characters and failure links for mismatches. This achieves O(n + m + z) time where n is text length, m is total pattern length, and z is number of matches—linear in input size regardless of pattern count.

```c
#define MAX_CHARS 256
#define MAX_STATES 10000

// AC automaton state
typedef struct ac_state {
    int next[MAX_CHARS];          // Next state for each character
    int failure;                  // Failure link
    int output[100];              // Pattern IDs that end at this state
    int output_count;
} ac_state_t;

// AC automaton
typedef struct ac_automaton {
    ac_state_t states[MAX_STATES];
    int num_states;
    char **patterns;
    int num_patterns;
} ac_automaton_t;

// Create AC automaton
ac_automaton_t* create_ac_automaton() {
    ac_automaton_t *ac = (ac_automaton_t*)malloc(sizeof(ac_automaton_t));
    ac->num_states = 1;  // Start with root state
    ac->num_patterns = 0;
    ac->patterns = NULL;
    
    // Initialize root state
    for (int i = 0; i < MAX_CHARS; i++) {
        ac->states[0].next[i] = 0;
    }
    ac->states[0].failure = 0;
    ac->states[0].output_count = 0;
    
    return ac;
}

// Add pattern to AC automaton
void ac_add_pattern(ac_automaton_t *ac, const char *pattern, int pattern_id) {
    int current_state = 0;
    
    // Follow/create path for pattern
    for (int i = 0; pattern[i] != '\0'; i++) {
        unsigned char c = pattern[i];
        
        if (ac->states[current_state].next[c] == 0) {
            // Create new state
            int new_state = ac->num_states++;
            ac->states[current_state].next[c] = new_state;
            
            // Initialize new state
            for (int j = 0; j < MAX_CHARS; j++) {
                ac->states[new_state].next[j] = 0;
            }
            ac->states[new_state].failure = 0;
            ac->states[new_state].output_count = 0;
        }
        
        current_state = ac->states[current_state].next[c];
    }
    
    // Mark pattern ending at this state
    ac->states[current_state].output[ac->states[current_state].output_count++] = pattern_id;
}

// Build failure links (BFS)
void ac_build_failure_links(ac_automaton_t *ac) {
    int queue[MAX_STATES];
    int front = 0, rear = 0;
    
    // All states at depth 1 have failure link to root
    for (int c = 0; c < MAX_CHARS; c++) {
        int next_state = ac->states[0].next[c];
        if (next_state != 0) {
            ac->states[next_state].failure = 0;
            queue[rear++] = next_state;
        }
    }
    
    // BFS to compute failure links
    while (front < rear) {
        int current = queue[front++];
        
        for (int c = 0; c < MAX_CHARS; c++) {
            int next = ac->states[current].next[c];
            if (next == 0) continue;
            
            queue[rear++] = next;
            
            // Find failure state
            int fail_state = ac->states[current].failure;
            while (fail_state != 0 && ac->states[fail_state].next[c] == 0) {
                fail_state = ac->states[fail_state].failure;
            }
            
            if (ac->states[fail_state].next[c] != 0 && 
                ac->states[fail_state].next[c] != next) {
                ac->states[next].failure = ac->states[fail_state].next[c];
            } else {
                ac->states[next].failure = 0;
            }
            
            // Inherit outputs from failure state
            int fail = ac->states[next].failure;
            for (int i = 0; i < ac->states[fail].output_count; i++) {
                ac->states[next].output[ac->states[next].output_count++] = 
                    ac->states[fail].output[i];
            }
        }
    }
}

// Search using AC automaton
void ac_search(ac_automaton_t *ac, const char *text, int text_len) {
    int current_state = 0;
    int matches = 0;
    
    printf("Aho-Corasick search:\n");
    
    for (int i = 0; i < text_len; i++) {
        unsigned char c = text[i];
        
        // Follow failure links if necessary
        while (current_state != 0 && ac->states[current_state].next[c] == 0) {
            current_state = ac->states[current_state].failure;
        }
        
        current_state = ac->states[current_state].next[c];
        
        // Check for pattern matches at this position
        if (ac->states[current_state].output_count > 0) {
            for (int j = 0; j < ac->states[current_state].output_count; j++) {
                int pattern_id = ac->states[current_state].output[j];
                printf("  Found pattern %d at position %d\n", pattern_id, i);
                matches++;
            }
        }
    }
    
    printf("  Total matches: %d\n", matches);
    printf("  Single pass through text (O(n) time)\n\n");
}

// Demonstrate AC multi-pattern matching
void demonstrate_aho_corasick() {
    printf("=== Aho-Corasick Multi-Pattern Matching ===\n\n");
    
    ac_automaton_t *ac = create_ac_automaton();
    
    // Add multiple attack signatures
    const char *patterns[] = {
        "SQL",
        "injection",
        "script",
        "alert",
        "SELECT",
        "DROP"
    };
    int num_patterns = 6;
    
    printf("Loading %d attack signatures:\n", num_patterns);
    for (int i = 0; i < num_patterns; i++) {
        printf("  Pattern %d: \"%s\"\n", i, patterns[i]);
        ac_add_pattern(ac, patterns[i], i);
    }
    printf("\n");
    
    // Build failure links
    ac_build_failure_links(ac);
    printf("Built Aho-Corasick automaton with %d states\n\n", ac->num_states);
    
    // Search for patterns in text
    const char *text = "This HTTP request contains SQL injection with SELECT and DROP commands in a script tag with alert";
    int text_len = strlen(text);
    
    printf("Searching text: \"%s\"\n\n", text);
    ac_search(ac, text, text_len);
    
    printf("Key advantage: Finds all patterns in single pass\n");
    printf("  vs. running Boyer-Moore %d times\n", num_patterns);
    printf("  Critical for IDS with thousands of signatures\n\n");
    
    free(ac);
}
```

## Regular Expression Matching for Policy

Network security policies often use regular expressions: "block URLs matching /malware.*\\.exe/", "alert on credit cards matching /\\d{16}/". These patterns require more than literal string matching—they specify character classes, repetition, alternation. Implementing regex engines efficiently matters for real-time traffic inspection. The two main approaches are backtracking (slow but space-efficient) and automaton construction (fast but potentially exponential space).

Thompson's construction converts regex to Non-deterministic Finite Automaton (NFA), which can be simulated efficiently. For pattern /a.*b/, the NFA has states for "seen a", "reading anything", "seen b". During matching, track all active states simultaneously. Each input character transitions all active states to their successors. The algorithm requires O(nm) time where n is text length and m is NFA states, acceptable for reasonable patterns and critical paths optimized in hardware.

```c
// Simple regex pattern matching (subset: . * [])
typedef struct regex_nfa {
    int num_states;
    int accepting_state;
    int **transitions;      // transitions[state][char]
    int *epsilon_closure;   // For epsilon transitions
} regex_nfa_t;

// Check if character matches pattern character
int char_matches(char text_char, char pattern_char) {
    if (pattern_char == '.') return 1;  // Dot matches anything
    return text_char == pattern_char;
}

// Simple regex matching (simplified implementation)
int simple_regex_match(const char *text, const char *pattern) {
    int text_len = strlen(text);
    int pattern_len = strlen(pattern);
    
    // Handle special cases
    if (pattern_len == 0) return text_len == 0;
    
    // Check if pattern has kleene star
    int has_star = (pattern_len >= 2 && pattern[1] == '*');
    
    if (has_star) {
        // Try matching zero or more of pattern[0]
        // First, try matching zero (skip pattern[0]*)
        if (simple_regex_match(text, pattern + 2)) {
            return 1;
        }
        
        // Try matching one or more
        for (int i = 0; i < text_len; i++) {
            if (!char_matches(text[i], pattern[0])) {
                break;
            }
            if (simple_regex_match(text + i + 1, pattern + 2)) {
                return 1;
            }
        }
        return 0;
    } else {
        // No star, match character by character
        if (text_len == 0) return 0;
        
        if (char_matches(text[0], pattern[0])) {
            return simple_regex_match(text + 1, pattern + 1);
        }
        return 0;
    }
}

// Demonstrate regex matching for policies
void demonstrate_regex_policies() {
    printf("=== Regular Expression Policy Matching ===\n\n");
    
    const char *test_cases[][2] = {
        {"malware.exe", "malware.*"},
        {"test.txt", "malware.*"},
        {"SELECT * FROM users", "SELECT.*FROM"},
        {"INSERT INTO users", "SELECT.*FROM"},
        {"192.168.1.1", "192.*"},
        {"10.0.0.1", "192.*"}
    };
    
    printf("Policy: Block patterns matching signatures\n\n");
    
    for (int i = 0; i < 6; i++) {
        const char *text = test_cases[i][0];
        const char *pattern = test_cases[i][1];
        
        int match = simple_regex_match(text, pattern);
        
        printf("Text: %-25s Pattern: %-15s %s\n",
               text, pattern, match ? "BLOCKED" : "ALLOWED");
    }
    
    printf("\nRegex enables flexible, expressive policies\n");
    printf("  But: More expensive than literal matching\n");
    printf("  Use: For complex patterns, command injection, etc.\n\n");
}
```

## DPI and Protocol Analysis

Deep Packet Inspection examines payload content, not just headers. HTTP inspection searches for malicious URLs, SQL injection attempts, or data exfiltration. The challenge lies in protocol awareness—HTTP requests span multiple packets, compressed payloads require decompression, encrypted connections need SSL/TLS inspection. String matching alone proves insufficient; the system must reconstruct protocol state before applying pattern matching.

Practical DPI systems combine techniques: specialized parsers for common protocols (HTTP, DNS, SMTP), state machines tracking connection phase, and string matching on extracted content. They buffer incomplete protocol units until assembly completes, then apply Aho-Corasick across reassembled content. This layered approach maintains line-rate performance by offloading protocol parsing to hardware while running pattern matching in optimized software or specialized processors.

```c
// HTTP request structure
typedef struct http_request {
    char method[16];
    char uri[1024];
    char host[256];
    char *body;
    int body_len;
} http_request_t;

// Simple HTTP parser (simplified)
int parse_http_request(const char *packet, int packet_len, http_request_t *request) {
    // Extract method and URI from first line
    const char *space1 = strchr(packet, ' ');
    if (!space1) return 0;
    
    int method_len = space1 - packet;
    if (method_len >= 16) return 0;
    memcpy(request->method, packet, method_len);
    request->method[method_len] = '\0';
    
    const char *space2 = strchr(space1 + 1, ' ');
    if (!space2) return 0;
    
    int uri_len = space2 - space1 - 1;
    if (uri_len >= 1024) return 0;
    memcpy(request->uri, space1 + 1, uri_len);
    request->uri[uri_len] = '\0';
    
    // Extract Host header (simplified)
    const char *host_header = strstr(packet, "Host: ");
    if (host_header) {
        const char *host_end = strchr(host_header, '\r');
        if (host_end) {
            int host_len = host_end - host_header - 6;
            if (host_len < 256) {
                memcpy(request->host, host_header + 6, host_len);
                request->host[host_len] = '\0';
            }
        }
    }
    
    // Find body
    const char *body_start = strstr(packet, "\r\n\r\n");
    if (body_start) {
        request->body = (char*)(body_start + 4);
        request->body_len = packet_len - (body_start + 4 - packet);
    } else {
        request->body = NULL;
        request->body_len = 0;
    }
    
    return 1;
}

// DPI inspection of HTTP traffic
void dpi_inspect_http(const char *packet, int packet_len, ac_automaton_t *ac) {
    printf("=== DPI HTTP Inspection ===\n\n");
    
    http_request_t request;
    if (!parse_http_request(packet, packet_len, &request)) {
        printf("Failed to parse HTTP request\n");
        return;
    }
    
    printf("Parsed HTTP Request:\n");
    printf("  Method: %s\n", request.method);
    printf("  URI: %s\n", request.uri);
    printf("  Host: %s\n", request.host);
    if (request.body) {
        printf("  Body length: %d bytes\n\n", request.body_len);
    } else {
        printf("  No body\n\n");
    }
    
    // Apply pattern matching to URI
    printf("Scanning URI for attack signatures:\n");
    ac_search(ac, request.uri, strlen(request.uri));
    
    // Apply pattern matching to body if present
    if (request.body && request.body_len > 0) {
        printf("Scanning body for attack signatures:\n");
        ac_search(ac, request.body, request.body_len);
    }
}

// Demonstrate DPI
void demonstrate_dpi() {
    printf("=== Deep Packet Inspection Demo ===\n\n");
    
    // Create AC automaton with attack signatures
    ac_automaton_t *ac = create_ac_automaton();
    
    ac_add_pattern(ac, "SELECT", 0);
    ac_add_pattern(ac, "UNION", 1);
    ac_add_pattern(ac, "../", 2);
    ac_add_pattern(ac, "<script>", 3);
    
    ac_build_failure_links(ac);
    
    // Simulate HTTP request with SQL injection
    const char *malicious_request = 
        "GET /search?q=test'+UNION+SELECT+* HTTP/1.1\r\n"
        "Host: vulnerable-site.com\r\n"
        "User-Agent: AttackerBot\r\n"
        "\r\n";
    
    dpi_inspect_http(malicious_request, strlen(malicious_request), ac);
    
    printf("DPI workflow:\n");
    printf("  1. Parse protocol (HTTP, DNS, etc.)\n");
    printf("  2. Extract relevant fields\n");
    printf("  3. Apply pattern matching to each field\n");
    printf("  4. Take action (block, alert, log)\n\n");
    
    free(ac);
}
```

## Hardware Acceleration and TCAM

Software string matching, even optimized, struggles at 100 Gbps line rates. Network processors and FPGAs implement pattern matching in hardware using techniques like parallel processing, pipeline architectures, and Ternary Content Addressable Memory (TCAM). TCAM stores patterns with don't-care bits, enabling wildcarded matching in constant time. A single TCAM lookup checks all patterns simultaneously, reporting matches in a single clock cycle.

The trade-off involves cost and power consumption. TCAM chips are expensive and power-hungry compared to SRAM. Practical systems use hybrid approaches: TCAM for critical fast-path matching (ACLs, routing), optimized software for complex pattern matching on sampled traffic, and GPU acceleration for bulk offline analysis. Understanding these hardware options allows architects to make informed decisions about where to run string matching: software, specialized processors, FPGAs, or ASICs.

```c
// Demonstrate performance scaling
void analyze_performance_scaling() {
    printf("=== String Matching Performance Analysis ===\n\n");
    
    printf("Throughput requirements for different line rates:\n\n");
    
    int line_rates[] = {1000, 10000, 40000, 100000};  // Mbps
    int avg_packet_size = 1000;  // bytes
    
    for (int i = 0; i < 4; i++) {
        int rate_mbps = line_rates[i];
        long packets_per_sec = (long)rate_mbps * 1000000 / 8 / avg_packet_size;
        int ns_per_packet = 1000000000 / packets_per_sec;
        
        printf("%d Gbps:\n", rate_mbps / 1000);
        printf("  Packets/sec: %ld\n", packets_per_sec);
        printf("  Time per packet: %d ns\n", ns_per_packet);
        printf("  Budget: ");
        
        if (ns_per_packet > 10000) {
            printf("Software matching feasible\n");
        } else if (ns_per_packet > 1000) {
            printf("Optimized software + hardware assist\n");
        } else if (ns_per_packet > 100) {
            printf("FPGA or specialized processor required\n");
        } else {
            printf("ASIC/TCAM required\n");
        }
        printf("\n");
    }
    
    printf("Hardware acceleration options:\n");
    printf("  Software (CPU): Up to 1-10 Gbps, flexible\n");
    printf("  FPGA: Up to 40 Gbps, reconfigurable\n");
    printf("  TCAM: 100+ Gbps, fixed patterns\n");
    printf("  ASIC: 400+ Gbps, highest performance\n\n");
}
```

## Conclusion

String matching algorithms form the computational core of network security systems. Boyer-Moore accelerates single-pattern searches through intelligent skipping. Aho-Corasick enables multi-pattern matching in linear time, critical for IDS processing thousands of signatures. Regular expressions provide expressive policy specification at the cost of increased computational complexity. Deep Packet Inspection combines protocol parsing with pattern matching to detect application-layer attacks.

The algorithms selected determine whether a security device processes traffic at line rate or becomes a bottleneck. Understanding when Boyer-Moore suffices versus when Aho-Corasick proves necessary, recognizing performance limitations requiring hardware acceleration, and appreciating the trade-offs between pattern expressiveness and matching speed separates functional security systems from production-grade deployments. Master these string matching techniques, and you possess essential tools for building network security systems that protect without degrading performance.