# APIs and Data Models

## REST APIs: A Comprehensive Guide

Covers statelessness, resource design, HTTP methods, and URI structure
Includes a complete C implementation of a REST API server handling GET, POST, and DELETE operations
Explains status codes, idempotency, content negotiation, and authentication
Focuses on practical understanding with brutal clarity about what REST actually means

## NETCONF: Network Configuration Protocol Explained

Explains the problem NETCONF solves and its four-layer architecture
Covers datastores (running, candidate, startup), RPC operations, and capabilities
Includes a full C implementation of a NETCONF client using libssh
Demonstrates complete session lifecycle: connection, hello exchange, operations, and closing
Details filtering, error handling, and the power of standardization

## RESTCONF: Bridging REST and Network Configuration

Shows how RESTCONF maps NETCONF concepts to RESTful HTTP
Explains URI structure, data formats (JSON/XML), and query parameters
Includes a complete C implementation using libcurl
Covers error handling, ETags for concurrent modification, and streaming notifications
Provides guidance on choosing between NETCONF and RESTCONF

## YANG Data Modeling: Structure and Constraints

Explains module structure, data nodes (containers, lists, leafs), and the type system
Covers constraints (must, when, mandatory), groupings for reuse, and augmentation for extension
Details RPCs, actions, notifications, and features for conditional compilation
Includes a complete, production-quality YANG module for interface management
Discusses validation tools (pyang, yanglint) and model evolution

# REST APIs: A Comprehensive Guide

## What REST Actually Means

REST stands for Representational State Transfer, but that name tells you almost nothing useful. Here's what it really means: REST is a way of building web services where you treat everything as a resource that can be identified by a URL, and you manipulate those resources using standard HTTP methods. Think of it like a library catalog system where every book has a unique identifier, and you can check books out, return them, or look them up using a standardized set of operations.

The key insight behind REST is that the web already works incredibly well for serving documents and handling requests. Instead of inventing new protocols and mechanisms for application-to-application communication, REST leverages the existing HTTP infrastructure that powers the web. This means you get all the benefits that HTTP already provides: caching, load balancing, proxies, and a well-understood request-response model.

When Roy Fielding defined REST in his doctoral dissertation in 2000, he wasn't inventing something new but rather describing constraints that, when followed, lead to scalable and maintainable web services. These constraints form the foundation of how modern web APIs work.

## The Core Constraint: Statelessness

Statelessness is the most critical constraint in REST, and understanding it will clarify many design decisions. In a stateless system, each request from a client to a server must contain all the information needed to understand and process that request. The server doesn't remember anything about previous requests from that client.

This sounds limiting at first. Why would you want to forget everything between requests? The answer lies in scalability. When a server doesn't need to remember client state, you can route any request to any server in a cluster. If one server fails, another can immediately take over without needing to reconstruct what was happening in the previous conversation. The system becomes inherently more resilient.

Consider a simple example. In a stateful system, you might log in and receive a session identifier. The server stores all your user information associated with that session ID. Every subsequent request just sends the session ID, and the server looks up your data. This works fine until that specific server crashes, taking your session data with it, or until you need to distribute load across thousands of servers, each needing to synchronize session state.

In a stateless REST system, every request carries authentication information (like a token) and any context needed to process the request. If you're requesting your user profile, the request includes both your authentication credentials and the identifier for which profile you want. The server processes this completely self-contained request without relying on memory of who you are from previous interactions.

## Resources and URI Design

Everything in REST revolves around resources. A resource is any piece of information that can be named: a user, a product, an order, a collection of blog posts, or even abstract concepts like search results or statistics. Each resource needs a unique identifier called a URI (Uniform Resource Identifier).

Good URI design makes your API intuitive and predictable. The URI should describe the resource itself, not the operation you're performing on it. Compare these two approaches:

Bad URI design uses verbs in the path: `/getUser/123`, `/deleteProduct/456`, `/updateOrder/789`. This approach fails because it mixes the "what" (the resource) with the "how" (the operation). You end up with an explosion of endpoints and no consistent pattern.

Good URI design focuses on nouns: `/users/123`, `/products/456`, `/orders/789`. The operation is expressed through the HTTP method you use (GET, POST, PUT, DELETE), not the URI itself. This separation keeps your API clean and leverages HTTP's built-in vocabulary.

Resource hierarchies appear naturally in URIs. If users write blog posts, you might have `/users/123/posts` to represent all posts by user 123, and `/users/123/posts/456` for a specific post. This hierarchical structure mirrors how you think about the relationships between resources.

## HTTP Methods: The Verbs of REST

HTTP provides a standardized set of methods that act as verbs for your resource nouns. Each method has specific semantics that you should respect to build a predictable API.

GET retrieves a representation of a resource without changing anything on the server. It's safe and idempotent, meaning you can call it repeatedly without side effects. When you GET `/users/123`, you receive information about that user. If you GET it again, you get the same information (or updated information if something else changed it, but your GET didn't cause the change).

POST creates new resources or triggers actions that change server state. It's neither safe nor idempotent because calling it multiple times creates multiple resources or triggers multiple state changes. When you POST to `/users` with user data in the request body, you create a new user. The server typically responds with the URI of the newly created resource.

PUT replaces a resource entirely or creates it if it doesn't exist at that specific URI. It's idempotent: sending the same PUT request multiple times has the same effect as sending it once. If you PUT to `/users/123` with complete user data, you're saying "make the user at this URI look exactly like this data." If there's no user at 123, create one; if there is, replace it completely.

PATCH performs partial updates to a resource. Unlike PUT, which requires the complete resource representation, PATCH only sends the changes. If you want to update just a user's email address, PATCH `/users/123` with `{"email": "newemail@example.com"}` changes only that field, leaving everything else intact.

DELETE removes a resource. It's idempotent because deleting something that's already deleted produces the same result as deleting it the first time: it's gone. Calling DELETE on `/users/123` removes that user.

## A Practical Implementation in C

Let's build a minimal REST API server in C to solidify these concepts. This implementation will handle HTTP requests, parse them, and route them to appropriate handlers. We'll focus on clarity over production-readiness.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_USERS 100

/* Simple in-memory user storage */
typedef struct {
    int id;
    char name[100];
    char email[100];
    int active;  /* 1 if slot is used, 0 if empty */
} User;

User users[MAX_USERS];
int user_count = 0;

/* Parse HTTP method from request */
void extract_method(const char *request, char *method) {
    sscanf(request, "%s", method);
}

/* Parse URI from request */
void extract_uri(const char *request, char *uri) {
    char method[16];
    sscanf(request, "%s %s", method, uri);
}

/* Parse resource ID from URI like /users/123 */
int extract_id_from_uri(const char *uri) {
    char *last_slash = strrchr(uri, '/');
    if (last_slash != NULL && *(last_slash + 1) != '\0') {
        return atoi(last_slash + 1);
    }
    return -1;
}

/* Find user by ID */
User* find_user(int id) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].active && users[i].id == id) {
            return &users[i];
        }
    }
    return NULL;
}

/* Handle GET /users - Return all users */
void handle_get_users(int client_socket) {
    char response[BUFFER_SIZE];
    char body[BUFFER_SIZE] = "[";
    
    int first = 1;
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].active) {
            char user_json[256];
            snprintf(user_json, sizeof(user_json),
                    "%s{\"id\":%d,\"name\":\"%s\",\"email\":\"%s\"}",
                    first ? "" : ",", users[i].id, users[i].name, users[i].email);
            strcat(body, user_json);
            first = 0;
        }
    }
    strcat(body, "]");
    
    /* Construct HTTP response with proper status and headers */
    snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %ld\r\n"
            "\r\n"
            "%s", strlen(body), body);
    
    send(client_socket, response, strlen(response), 0);
}

/* Handle GET /users/{id} - Return specific user */
void handle_get_user(int client_socket, int id) {
    User *user = find_user(id);
    char response[BUFFER_SIZE];
    
    if (user == NULL) {
        /* Resource not found - return 404 */
        snprintf(response, sizeof(response),
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"error\":\"User not found\"}");
    } else {
        char body[512];
        snprintf(body, sizeof(body),
                "{\"id\":%d,\"name\":\"%s\",\"email\":\"%s\"}",
                user->id, user->name, user->email);
        
        snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %ld\r\n"
                "\r\n"
                "%s", strlen(body), body);
    }
    
    send(client_socket, response, strlen(response), 0);
}

/* Handle POST /users - Create new user */
void handle_post_user(int client_socket, const char *request_body) {
    /* Find empty slot */
    int slot = -1;
    for (int i = 0; i < MAX_USERS; i++) {
        if (!users[i].active) {
            slot = i;
            break;
        }
    }
    
    if (slot == -1) {
        char response[] = "HTTP/1.1 507 Insufficient Storage\r\n\r\n";
        send(client_socket, response, strlen(response), 0);
        return;
    }
    
    /* Parse JSON manually (simplified - production code would use a JSON library) */
    char name[100] = "", email[100] = "";
    char *name_start = strstr(request_body, "\"name\":\"");
    char *email_start = strstr(request_body, "\"email\":\"");
    
    if (name_start) {
        name_start += 8;  /* Skip past "name":" */
        char *name_end = strchr(name_start, '"');
        if (name_end) {
            strncpy(name, name_start, name_end - name_start);
        }
    }
    
    if (email_start) {
        email_start += 9;  /* Skip past "email":" */
        char *email_end = strchr(email_start, '"');
        if (email_end) {
            strncpy(email, email_start, email_end - email_start);
        }
    }
    
    /* Create new user */
    users[slot].id = ++user_count;
    strcpy(users[slot].name, name);
    strcpy(users[slot].email, email);
    users[slot].active = 1;
    
    /* Return 201 Created with Location header */
    char response[BUFFER_SIZE];
    char body[256];
    snprintf(body, sizeof(body),
            "{\"id\":%d,\"name\":\"%s\",\"email\":\"%s\"}",
            users[slot].id, users[slot].name, users[slot].email);
    
    snprintf(response, sizeof(response),
            "HTTP/1.1 201 Created\r\n"
            "Content-Type: application/json\r\n"
            "Location: /users/%d\r\n"
            "Content-Length: %ld\r\n"
            "\r\n"
            "%s", users[slot].id, strlen(body), body);
    
    send(client_socket, response, strlen(response), 0);
}

/* Handle DELETE /users/{id} - Delete user */
void handle_delete_user(int client_socket, int id) {
    User *user = find_user(id);
    char response[BUFFER_SIZE];
    
    if (user == NULL) {
        snprintf(response, sizeof(response),
                "HTTP/1.1 404 Not Found\r\n\r\n");
    } else {
        user->active = 0;  /* Mark as deleted */
        snprintf(response, sizeof(response),
                "HTTP/1.1 204 No Content\r\n\r\n");
    }
    
    send(client_socket, response, strlen(response), 0);
}

/* Main request router */
void handle_request(int client_socket, const char *request) {
    char method[16], uri[256];
    extract_method(request, method);
    extract_uri(request, uri);
    
    printf("Received: %s %s\n", method, uri);
    
    /* Route based on method and URI pattern */
    if (strcmp(method, "GET") == 0) {
        if (strcmp(uri, "/users") == 0) {
            handle_get_users(client_socket);
        } else if (strncmp(uri, "/users/", 7) == 0) {
            int id = extract_id_from_uri(uri);
            if (id > 0) {
                handle_get_user(client_socket, id);
            }
        }
    } else if (strcmp(method, "POST") == 0 && strcmp(uri, "/users") == 0) {
        /* Extract body from request (simplified) */
        char *body = strstr(request, "\r\n\r\n");
        if (body) {
            body += 4;  /* Skip past delimiter */
            handle_post_user(client_socket, body);
        }
    } else if (strcmp(method, "DELETE") == 0 && strncmp(uri, "/users/", 7) == 0) {
        int id = extract_id_from_uri(uri);
        if (id > 0) {
            handle_delete_user(client_socket, id);
        }
    } else {
        /* Method or route not implemented */
        char response[] = "HTTP/1.1 501 Not Implemented\r\n\r\n";
        send(client_socket, response, strlen(response), 0);
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    /* Initialize user storage */
    memset(users, 0, sizeof(users));
    
    /* Create socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    /* Allow port reuse */
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    /* Configure server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    /* Bind socket to address */
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }
    
    /* Listen for connections */
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }
    
    printf("REST API server listening on port %d\n", PORT);
    printf("Try: curl http://localhost:%d/users\n", PORT);
    
    /* Main server loop */
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        /* Read request */
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
        
        if (bytes_read > 0) {
            handle_request(client_socket, buffer);
        }
        
        close(client_socket);
    }
    
    close(server_socket);
    return 0;
}
```

This implementation demonstrates the core concepts of REST. The server is stateless because it doesn't maintain any session information between requests. Each request contains everything needed to process it: the method (GET, POST, DELETE), the URI identifying the resource, and any necessary data in the request body. Resources are identified by clean URIs like `/users` and `/users/123`. The HTTP methods determine what operation to perform, not the URI itself.

## Status Codes: Communication Through Numbers

HTTP status codes provide a standardized way for servers to communicate the outcome of a request. Understanding these codes helps you build robust APIs that clearly indicate what happened.

Status codes in the 200 range indicate success. The specific code provides nuance about the type of success. Code 200 OK means the request succeeded and the response contains the requested data. Code 201 Created indicates that a POST request successfully created a new resource, and the response should include a Location header pointing to the new resource's URI. Code 204 No Content signals successful processing but no data to return, commonly used after DELETE operations.

The 400 range indicates client errors where the request was malformed or cannot be fulfilled. Code 400 Bad Request means the server couldn't understand the request due to invalid syntax. Code 401 Unauthorized signals that authentication is required or has failed. Code 403 Forbidden means the server understood the request but refuses to fulfill it due to insufficient permissions. Code 404 Not Found indicates the requested resource doesn't exist. Code 409 Conflict arises when the request conflicts with the current state, such as trying to create a user with an email that already exists.

The 500 range indicates server errors where the request was valid but the server failed to fulfill it. Code 500 Internal Server Error is a generic catch-all for unexpected server failures. Code 503 Service Unavailable means the server is temporarily unable to handle requests, perhaps due to maintenance or overload.

## Idempotency and Safety: Guarantees About Operations

Understanding idempotency and safety helps you design reliable APIs and write robust client code. A safe method doesn't modify resources on the server. GET and HEAD are safe methods. You can call them repeatedly without fear of changing data. This property enables caching and allows intermediaries to retry requests without side effects.

An idempotent method produces the same result no matter how many times you call it with the same input. GET, PUT, and DELETE are idempotent. If you GET a resource twice, you receive the same data (or updated data if something else changed it between requests, but your GET operations themselves didn't cause changes). If you PUT the same data twice, the resource ends up in the same state as if you'd PUT it once. If you DELETE a resource twice, it's still deleted after the second call just as it was after the first.

POST is neither safe nor idempotent. Each POST typically creates a new resource or triggers a state change. Calling POST twice creates two resources or triggers the action twice. This lack of idempotency requires more care when handling POST requests, especially regarding retries and error handling.

These properties matter immensely in distributed systems where network failures are common. If a client sends a PUT request and doesn't receive a response due to a network timeout, it can safely retry the PUT because idempotency guarantees the same outcome. With POST, retrying might create duplicate resources unless you implement additional mechanisms like idempotency keys.

## Content Negotiation: Speaking Multiple Languages

REST APIs often need to support multiple data formats. A client might want JSON while another prefers XML. Content negotiation allows clients to specify their preferred format and servers to respond appropriately.

The client indicates its preferred format using the Accept header in the request. A request with `Accept: application/json` tells the server the client wants JSON. The server responds with the Content-Type header indicating the actual format of the response body, such as `Content-Type: application/json`. If the server cannot provide any format the client accepts, it returns a 406 Not Acceptable status code.

Similarly, when a client sends data to the server in a POST or PUT request, it uses the Content-Type header to indicate the format of the request body. The server examines this header to determine how to parse the incoming data.

## Authentication and Security: Protecting Resources

REST APIs need mechanisms to verify client identity and ensure secure communication. Since REST is stateless, authentication information must accompany each request rather than being established once per session.

The simplest approach uses API keys, where the client includes a secret token in each request, typically in an Authorization header like `Authorization: Bearer YOUR_API_KEY_HERE`. The server validates this key against its database of authorized keys. This works well for simple scenarios but provides limited flexibility for different permission levels or user-specific access.

OAuth provides a more sophisticated framework for authentication and authorization, especially when dealing with third-party applications accessing user data. The OAuth flow allows users to grant limited access to their resources without sharing passwords. The client receives a token that can be revoked and has specific scopes defining what it can access.

JSON Web Tokens (JWT) offer a self-contained authentication mechanism. A JWT contains encoded claims about the user and is cryptographically signed. The server can verify the token's authenticity without maintaining session state or querying a database on every request. The token itself carries the necessary information about user identity and permissions.

Regardless of authentication mechanism, REST APIs must use HTTPS rather than plain HTTP. HTTPS encrypts all communication between client and server, preventing eavesdropping and man-in-the-middle attacks. Without HTTPS, authentication credentials and sensitive data travel across the network in plain text, visible to anyone who can intercept the traffic.

## The Reality of REST

Most APIs claiming to be RESTful don't strictly follow all REST constraints. They might maintain some server-side state, not implement HATEOAS, or deviate from pure REST principles in other ways. This isn't necessarily wrong. REST provides guidelines for building scalable web services, but pragmatism often requires trade-offs.

What matters most is understanding these principles so you can make informed decisions about when to follow them strictly and when to bend them for practical reasons. The key insights remain valuable: use HTTP as it was designed, keep your API stateless when possible, design around resources rather than actions, and leverage existing web infrastructure instead of building custom protocols.

# NETCONF: Network Configuration Protocol Explained

## The Problem NETCONF Solves

Before NETCONF existed, configuring network devices was a nightmare of inconsistency. Each vendor had proprietary command-line interfaces with different syntax, different capabilities, and different quirks. An engineer working with routers from Cisco, Juniper, and Arista needed to remember three completely different configuration languages. Automation was nearly impossible because scripts written for one vendor's equipment wouldn't work with another's.

Even worse, command-line interfaces were designed for humans, not programs. They relied on text parsing, which is inherently fragile. Small changes in output format could break automation scripts. There was no reliable way to validate configurations before applying them, no standard method to roll back changes, and no consistent error reporting across vendors.

NETCONF emerged as the solution to this chaos. It's a standardized protocol defined by the IETF that provides a programmatic interface to network device configuration. Instead of sending text commands and parsing text output, applications exchange structured XML messages. Instead of each vendor inventing their own protocol, everyone implements the same standard. This makes automation feasible and enables tools that work across multiple vendors.

## The Architecture: Layers Upon Layers

NETCONF is structured in four distinct layers, each with specific responsibilities. Understanding these layers clarifies how the protocol works and where different features fit.

At the bottom sits the Secure Transport layer. NETCONF mandates secure communication, with SSH being the default and most common transport. This layer establishes the encrypted channel between client and server, handles authentication, and provides the reliable byte stream that the upper layers use. Other transports like TLS are possible, but SSH dominates real-world deployments because it's ubiquitous in network management environments.

Above the transport layer is the Messages layer. This layer frames NETCONF messages so the receiver knows where one message ends and the next begins. In NETCONF 1.0, messages are separated by a special end-of-message marker. In NETCONF 1.1, chunked framing provides more efficient handling of large messages. The messages themselves are XML documents, and this layer simply handles the mechanics of sending complete XML documents over the transport.

The Operations layer defines the actual actions you can perform: retrieving configuration, modifying it, copying between datastores, locking resources, and so on. Each operation is expressed as an XML remote procedure call with specific parameters and return values. This is where the real work happens, but the operations are generic and work the same way regardless of what data you're manipulating.

At the top is the Content layer, which defines the actual configuration data structure. NETCONF itself doesn't dictate what configuration options exist on a device or how they're organized. That's defined separately using data modeling languages like YANG. This separation is crucial because it allows NETCONF to work with any type of network device while still providing consistent operations.

## Datastores: Multiple Views of Configuration

NETCONF introduces the concept of datastores, which are distinct locations where configuration data lives. This is more sophisticated than the typical "current configuration" model because it enables safer, more controlled configuration changes.

The running datastore contains the device's current active configuration. This is what's actually controlling the device's behavior right now. When you query the device's current state, you're reading from the running datastore. When you make a configuration change that takes effect immediately, you're modifying the running datastore.

The candidate datastore provides a working area where you can prepare configuration changes without affecting the running device. Think of it as a draft workspace. You can make multiple changes to the candidate, validate that they form a coherent configuration, and then commit them all at once to the running datastore. If the changes cause problems, you can discard them without ever impacting the running device. Not all devices support the candidate datastore, but it's immensely valuable when available.

The startup datastore holds the configuration that will be loaded when the device boots. In many systems, this differs from the running configuration because changes made to the running config aren't automatically saved to startup. This separation gives you an escape hatch: if you make changes that break network connectivity, rebooting the device will restore the last saved configuration from startup.

Some implementations include additional datastores for specific purposes. An intended datastore might hold the configuration that should exist according to your network management system, while an operational datastore provides read-only access to the device's actual operational state including statistics and dynamic information.

## The RPC Model: Remote Procedure Calls

NETCONF communication follows a remote procedure call model. The client sends an RPC request to the device, and the device responds with an RPC reply. Each request-reply pair is independent and self-contained, embodying the stateless principle similar to REST (though NETCONF sessions themselves are stateful).

An RPC request looks like this:

```xml
<rpc message-id="101" xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
  <get-config>
    <source>
      <running/>
    </source>
  </get-config>
</rpc>
```

The outer `<rpc>` element wraps every request. The `message-id` attribute uniquely identifies this request within the session, allowing the client to match replies to requests. The namespace declaration specifies that we're using NETCONF base protocol version 1.0. Inside the RPC wrapper is the actual operation, in this case `<get-config>`, with its parameters.

The device responds with an RPC reply:

```xml
<rpc-reply message-id="101" xmlns="urn:ietf:params:xml:ns:netconf:base:1.0">
  <data>
    <top xmlns="http://example.com/schema/1.2/config">
      <interface>
        <name>eth0</name>
        <mtu>1500</mtu>
      </interface>
    </top>
  </data>
</rpc-reply>
```

The reply echoes the message-id from the request. Successful operations return the requested data or confirmation. Errors return an `<rpc-error>` element with detailed information about what went wrong.

## Core Operations: The Standard Toolbox

NETCONF defines a set of standard operations that all compliant implementations must or may support. These operations provide the building blocks for network configuration management.

The `<get-config>` operation retrieves configuration data from a specified datastore. You specify which datastore to read from (running, candidate, or startup) and optionally provide filters to retrieve only specific portions of the configuration. This is the safest read operation because it only returns configuration, not operational state data.

The `<get>` operation is more powerful but potentially more expensive. It retrieves both configuration and operational state data from the running datastore. Operational state includes things like interface statistics, ARP tables, or routing table contents. While useful, this operation can be slower because it must gather dynamic information from the device.

The `<edit-config>` operation modifies a datastore. This is where configuration changes happen. You specify the target datastore and provide the configuration changes in the request. The operation supports several merge strategies: merge (combine new data with existing), replace (replace specific elements), create (add new elements but fail if they exist), and delete (remove elements). This fine-grained control allows precise configuration modifications without needing to send the entire configuration.

The `<copy-config>` operation copies an entire datastore to another datastore or to a URL. This is useful for backing up configurations or promoting a validated candidate configuration to running. Unlike edit-config which merges changes, copy-config replaces the target entirely with the source.

The `<lock>` and `<unlock>` operations control concurrent access to datastores. When you lock a datastore, you prevent other clients from modifying it until you unlock it. This prevents conflicts when multiple management systems might be trying to configure the device simultaneously. Proper locking discipline is essential in environments with multiple administrators or automated systems.

The `<commit>` operation (if the candidate capability is supported) promotes the candidate datastore to running. After making a series of changes to candidate and validating them, commit applies them all atomically. If any part of the commit fails, the entire operation rolls back, maintaining consistency.

The `<validate>` operation checks whether a configuration is valid without applying it. This lets you catch errors before committing changes to the running device. Validation checks syntax, constraint violations, and semantic correctness according to the device's data models.

## A Practical NETCONF Client in C

Let's build a NETCONF client that connects to a device, retrieves configuration, and makes changes. This implementation demonstrates the protocol mechanics without getting lost in production complexities.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libssh/libssh.h>

#define NETCONF_PORT 830
#define BUFFER_SIZE 8192
#define NETCONF_10_DELIMITER "]]>]]>"

/* NETCONF session structure */
typedef struct {
    ssh_session ssh;
    ssh_channel channel;
    int message_id;
} netconf_session_t;

/* Initialize SSH and create NETCONF session */
netconf_session_t* netconf_connect(const char *host, const char *username, 
                                    const char *password) {
    netconf_session_t *session = malloc(sizeof(netconf_session_t));
    if (!session) return NULL;
    
    session->message_id = 1;
    
    /* Create SSH session */
    session->ssh = ssh_new();
    if (session->ssh == NULL) {
        free(session);
        return NULL;
    }
    
    /* Set connection parameters */
    ssh_options_set(session->ssh, SSH_OPTIONS_HOST, host);
    ssh_options_set(session->ssh, SSH_OPTIONS_PORT, &(int){NETCONF_PORT});
    ssh_options_set(session->ssh, SSH_OPTIONS_USER, username);
    
    /* Connect to server */
    if (ssh_connect(session->ssh) != SSH_OK) {
        fprintf(stderr, "Error connecting to %s: %s\n", 
                host, ssh_get_error(session->ssh));
        ssh_free(session->ssh);
        free(session);
        return NULL;
    }
    
    /* Authenticate */
    if (ssh_userauth_password(session->ssh, NULL, password) != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Authentication failed: %s\n", 
                ssh_get_error(session->ssh));
        ssh_disconnect(session->ssh);
        ssh_free(session->ssh);
        free(session);
        return NULL;
    }
    
    /* Open NETCONF subsystem channel */
    session->channel = ssh_channel_new(session->ssh);
    if (session->channel == NULL) {
        ssh_disconnect(session->ssh);
        ssh_free(session->ssh);
        free(session);
        return NULL;
    }
    
    if (ssh_channel_open_session(session->channel) != SSH_OK) {
        ssh_channel_free(session->channel);
        ssh_disconnect(session->ssh);
        ssh_free(session->ssh);
        free(session);
        return NULL;
    }
    
    /* Request NETCONF subsystem */
    if (ssh_channel_request_subsystem(session->channel, "netconf") != SSH_OK) {
        fprintf(stderr, "Failed to request NETCONF subsystem\n");
        ssh_channel_close(session->channel);
        ssh_channel_free(session->channel);
        ssh_disconnect(session->ssh);
        ssh_free(session->ssh);
        free(session);
        return NULL;
    }
    
    printf("NETCONF session established\n");
    return session;
}

/* Read NETCONF message from channel */
char* netconf_receive(netconf_session_t *session) {
    static char buffer[BUFFER_SIZE];
    int total_bytes = 0;
    int bytes_read;
    
    memset(buffer, 0, BUFFER_SIZE);
    
    /* Read until we find the delimiter or buffer is full */
    while (total_bytes < BUFFER_SIZE - 1) {
        bytes_read = ssh_channel_read(session->channel, 
                                      buffer + total_bytes, 
                                      BUFFER_SIZE - total_bytes - 1, 
                                      0);
        
        if (bytes_read <= 0) break;
        
        total_bytes += bytes_read;
        buffer[total_bytes] = '\0';
        
        /* Check if we received the complete message (ending with delimiter) */
        if (strstr(buffer, NETCONF_10_DELIMITER) != NULL) {
            /* Remove the delimiter from the end */
            char *delim = strstr(buffer, NETCONF_10_DELIMITER);
            *delim = '\0';
            break;
        }
    }
    
    return buffer;
}

/* Send NETCONF message */
int netconf_send(netconf_session_t *session, const char *message) {
    /* Send message with delimiter */
    ssh_channel_write(session->channel, message, strlen(message));
    ssh_channel_write(session->channel, NETCONF_10_DELIMITER, 
                     strlen(NETCONF_10_DELIMITER));
    return 0;
}

/* Exchange hello messages to establish session */
int netconf_hello(netconf_session_t *session) {
    /* Receive server's hello */
    char *server_hello = netconf_receive(session);
    printf("Server hello:\n%s\n\n", server_hello);
    
    /* Send client hello */
    const char *client_hello = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<hello xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n"
        "  <capabilities>\n"
        "    <capability>urn:ietf:params:netconf:base:1.0</capability>\n"
        "  </capabilities>\n"
        "</hello>";
    
    printf("Sending client hello...\n");
    netconf_send(session, client_hello);
    
    return 0;
}

/* Get running configuration */
char* netconf_get_config(netconf_session_t *session) {
    char rpc[1024];
    
    /* Build get-config RPC */
    snprintf(rpc, sizeof(rpc),
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rpc message-id=\"%d\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n"
        "  <get-config>\n"
        "    <source>\n"
        "      <running/>\n"
        "    </source>\n"
        "  </get-config>\n"
        "</rpc>",
        session->message_id++);
    
    /* Send request */
    netconf_send(session, rpc);
    
    /* Receive reply */
    char *reply = netconf_receive(session);
    return reply;
}

/* Edit configuration - add interface */
int netconf_edit_config(netconf_session_t *session, const char *interface_name, 
                        int mtu) {
    char rpc[2048];
    
    /* Build edit-config RPC with merge operation */
    snprintf(rpc, sizeof(rpc),
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rpc message-id=\"%d\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n"
        "  <edit-config>\n"
        "    <target>\n"
        "      <running/>\n"
        "    </target>\n"
        "    <default-operation>merge</default-operation>\n"
        "    <config>\n"
        "      <top xmlns=\"http://example.com/schema/1.2/config\">\n"
        "        <interface>\n"
        "          <name>%s</name>\n"
        "          <mtu>%d</mtu>\n"
        "        </interface>\n"
        "      </top>\n"
        "    </config>\n"
        "  </edit-config>\n"
        "</rpc>",
        session->message_id++, interface_name, mtu);
    
    /* Send request */
    netconf_send(session, rpc);
    
    /* Receive reply */
    char *reply = netconf_receive(session);
    printf("Edit-config reply:\n%s\n\n", reply);
    
    /* Check if reply contains <ok/> indicating success */
    return (strstr(reply, "<ok/>") != NULL) ? 0 : -1;
}

/* Lock a datastore */
int netconf_lock(netconf_session_t *session, const char *datastore) {
    char rpc[1024];
    
    snprintf(rpc, sizeof(rpc),
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rpc message-id=\"%d\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n"
        "  <lock>\n"
        "    <target>\n"
        "      <%s/>\n"
        "    </target>\n"
        "  </lock>\n"
        "</rpc>",
        session->message_id++, datastore);
    
    netconf_send(session, rpc);
    char *reply = netconf_receive(session);
    
    return (strstr(reply, "<ok/>") != NULL) ? 0 : -1;
}

/* Unlock a datastore */
int netconf_unlock(netconf_session_t *session, const char *datastore) {
    char rpc[1024];
    
    snprintf(rpc, sizeof(rpc),
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rpc message-id=\"%d\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n"
        "  <unlock>\n"
        "    <target>\n"
        "      <%s/>\n"
        "    </target>\n"
        "  </unlock>\n"
        "</rpc>",
        session->message_id++, datastore);
    
    netconf_send(session, rpc);
    char *reply = netconf_receive(session);
    
    return (strstr(reply, "<ok/>") != NULL) ? 0 : -1;
}

/* Close NETCONF session */
void netconf_close(netconf_session_t *session) {
    char rpc[512];
    
    /* Send close-session RPC */
    snprintf(rpc, sizeof(rpc),
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<rpc message-id=\"%d\" xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">\n"
        "  <close-session/>\n"
        "</rpc>",
        session->message_id++);
    
    netconf_send(session, rpc);
    
    /* Clean up SSH resources */
    ssh_channel_close(session->channel);
    ssh_channel_free(session->channel);
    ssh_disconnect(session->ssh);
    ssh_free(session->ssh);
    free(session);
}

/* Main program demonstrating NETCONF operations */
int main() {
    /* Connect to NETCONF server */
    netconf_session_t *session = netconf_connect(
        "192.168.1.1",  /* Device IP */
        "admin",        /* Username */
        "password"      /* Password */
    );
    
    if (!session) {
        fprintf(stderr, "Failed to establish NETCONF session\n");
        return 1;
    }
    
    /* Exchange hello messages */
    netconf_hello(session);
    
    /* Lock the running datastore to prevent conflicts */
    printf("Locking running datastore...\n");
    if (netconf_lock(session, "running") == 0) {
        printf("Datastore locked successfully\n\n");
    }
    
    /* Get current configuration */
    printf("Retrieving configuration...\n");
    char *config = netconf_get_config(session);
    printf("Current config:\n%s\n\n", config);
    
    /* Edit configuration */
    printf("Modifying configuration...\n");
    if (netconf_edit_config(session, "eth1", 9000) == 0) {
        printf("Configuration modified successfully\n\n");
    }
    
    /* Get updated configuration */
    printf("Retrieving updated configuration...\n");
    config = netconf_get_config(session);
    printf("Updated config:\n%s\n\n", config);
    
    /* Unlock the datastore */
    printf("Unlocking datastore...\n");
    if (netconf_unlock(session, "running") == 0) {
        printf("Datastore unlocked successfully\n\n");
    }
    
    /* Close session */
    printf("Closing session...\n");
    netconf_close(session);
    
    return 0;
}
```

This implementation shows the complete lifecycle of a NETCONF session: establishing the SSH connection, opening the NETCONF subsystem channel, exchanging hello messages to advertise capabilities, performing operations with proper message framing, and cleanly closing the session. The lock-operate-unlock pattern demonstrates safe configuration management practices.

## Capabilities: Negotiating Features

NETCONF devices advertise their capabilities during the hello exchange. Capabilities indicate which optional features the device supports beyond the base protocol. This negotiation allows clients to discover what operations they can perform without prior knowledge of the specific device.

The base capability is mandatory and indicates which version of the NETCONF base protocol the device supports. Base 1.0 and base 1.1 differ primarily in their message framing mechanisms. Base 1.1 uses chunked framing which handles large messages more efficiently.

The writable-running capability indicates that the device allows direct modification of the running datastore. Without this capability, you must use the candidate datastore workflow even for simple changes.

The candidate capability signals support for the candidate datastore. This enables the prepare-validate-commit workflow that provides safer configuration changes. Not all devices support candidate configuration.

The rollback-on-error capability means that if any part of an edit-config operation fails, the entire operation rolls back atomically. Without this capability, partial changes might be applied even if later changes fail.

The validate capability indicates that the device supports explicit validation of configurations before applying them. This lets you check that proposed changes are valid without committing them.

The notification capability enables event notifications, allowing the device to push updates to clients when significant events occur rather than requiring constant polling.

## Filtering: Retrieving Subsets of Data

Network device configurations can be enormous. Retrieving the entire configuration just to check one interface's status is wasteful. NETCONF provides filtering mechanisms to retrieve only the data you need.

Subtree filtering uses XML structures to specify what data to retrieve. You provide an XML template that matches the structure of the data you want, and the device returns only matching elements. For example:

```xml
<filter type="subtree">
  <top xmlns="http://example.com/schema/1.2/config">
    <interface>
      <name>eth0</name>
    </interface>
  </top>
</filter>
```

This filter retrieves only the interface named eth0, ignoring all other interfaces and configuration elements. The filter acts as a template: elements you include in the filter determine which parts of the configuration tree to return.

XPath filtering provides more powerful selection capabilities using XPath expressions. You can select based on attribute values, apply conditions, and express complex selection logic. However, XPath filtering is an optional capability that not all devices support.

## Error Handling: When Things Go Wrong

NETCONF provides detailed error reporting through structured rpc-error elements. When an operation fails, the reply contains one or more error elements describing what went wrong.

Each error includes an error-type that categorizes the error source: transport (communication layer problems), rpc (invalid RPC structure), protocol (NETCONF protocol violations), or application (problems with the operation itself). The error-tag provides a machine-readable error identifier like operation-failed, invalid-value, or data-missing. The error-severity indicates whether this is an error or just a warning. The error-info element provides additional details specific to this error, such as which element caused validation to fail.

This structured error reporting allows clients to programmatically handle errors rather than parsing text messages. You can determine whether an error is retryable, identify the specific configuration element that caused problems, and provide meaningful feedback to users.

## The Power of Standardization

NETCONF's greatest strength is standardization. Once you've written a NETCONF client, it works with devices from different vendors that implement NETCONF. The operations are the same, the error handling is the same, and the session management is the same. Only the actual configuration data structure varies, and even that is standardized through YANG models.

This standardization enables network automation at scale. You can write orchestration systems that manage thousands of heterogeneous devices using the same code. You can validate configurations before deploying them. You can implement atomic transactions that either completely succeed or completely fail, maintaining network consistency. These capabilities were impossible with command-line interface automation.

# RESTCONF: Bridging REST and Network Configuration

## Why RESTCONF Exists

NETCONF solved the problem of inconsistent network device configuration by providing a standardized protocol over SSH with structured XML operations. It works well for traditional network management systems and purpose-built automation tools. However, it has a significant limitation: NETCONF doesn't fit naturally into modern web-based architectures.

Web developers understand HTTP, JSON, and REST APIs. These technologies power modern applications, cloud services, and microservices architectures. But NETCONF requires SSH connections, XML parsing, and a completely different programming model. If you're building a web application that needs to configure network devices, you face a steep learning curve with NETCONF. You need specialized libraries, must manage persistent SSH sessions, and work with XML in a world that has largely moved to JSON.

RESTCONF bridges this gap by providing HTTP-based access to the same configuration datastores that NETCONF uses. It's not a replacement for NETCONF but rather a complementary interface that makes network configuration accessible to web applications and REST-familiar developers. The device maintains the same underlying data models, the same datastores, and the same validation logic. RESTCONF simply exposes them through a RESTful HTTP API instead of SSH-based XML-RPC.

This architectural choice has profound implications. A web application can configure network devices using the same tools it uses to call any other REST API. Standard HTTP libraries work. Load balancers and reverse proxies work. OAuth and JWT authentication work. The entire web infrastructure ecosystem becomes available for network management. At the same time, traditional management systems can continue using NETCONF for the same devices. Both interfaces coexist, accessing the same underlying configuration data.

## The Mapping to NETCONF Concepts

RESTCONF doesn't invent new configuration concepts but rather maps NETCONF's operations and datastores onto REST principles. Understanding this mapping clarifies how RESTCONF works and when to use which operation.

NETCONF's running, candidate, and startup datastores become REST resources accessible at specific URIs. The running datastore appears at `/restconf/data` by default. When you perform a GET request to `/restconf/data/interfaces/interface=eth0`, you're retrieving that interface's configuration from the running datastore, equivalent to a NETCONF get-config operation with appropriate filtering.

HTTP methods map naturally to NETCONF operations. GET corresponds to NETCONF's get or get-config, retrieving data without modification. POST creates new resources or invokes operations, similar to NETCONF's edit-config with create operation or RPC invocations. PUT replaces resources entirely, matching NETCONF's edit-config with replace. PATCH performs partial updates like NETCONF's edit-config with merge. DELETE removes resources just as NETCONF's edit-config with delete operation does.

This mapping isn't perfect because REST and NETCONF have different philosophies. REST emphasizes stateless resources while NETCONF builds on persistent sessions with explicit locking and unlocking. RESTCONF resolves these differences through careful protocol design. For example, RESTCONF doesn't require explicit locking because HTTP's stateless nature and proper use of ETags handle concurrent modification detection.

## URI Structure and Resource Addressing

RESTCONF organizes resources hierarchically through URI paths that mirror the YANG data model structure. The URI directly reflects the path through the configuration tree to reach a specific element. This intuitive mapping lets developers who understand the data model construct URIs without consulting extensive documentation.

The root RESTCONF endpoint is `/restconf`. Querying this endpoint returns information about the server's capabilities and supported operations. It acts as the API's entry point for capability discovery.

Configuration and state data live under `/restconf/data`. This is where the actual network configuration resides. Everything below this path corresponds to elements in the device's YANG models. The path structure follows the YANG hierarchy: modules, containers, lists, and leafs.

For example, consider a YANG model defining interfaces. The path `/restconf/data/interfaces` accesses the entire interfaces container. To access a specific interface, you append the list key: `/restconf/data/interfaces/interface=eth0`. Here, `interface` is the list name and `eth0` is the key value that uniquely identifies this particular interface. Multiple keys are separated by commas: `/restconf/data/example/list=key1,key2`.

Operations and RPCs defined in YANG models appear under `/restconf/operations`. These aren't configuration resources but rather actions you can invoke. To restart an interface, you might POST to `/restconf/operations/interfaces/interface=eth0/restart`. The POST body contains any input parameters the operation requires.

Event streams for notifications live under `/restconf/streams`. These provide server-sent events that push updates to clients when significant changes occur. Getting a stream establishes a long-lived HTTP connection over which the server sends events as they happen.

## Working with Data Formats

RESTCONF primarily supports two data formats: JSON and XML. While NETCONF exclusively uses XML, RESTCONF embraces JSON as the preferred format for modern applications. The device's YANG models define the data structure, and RESTCONF translates between YANG and either JSON or XML representations.

The client specifies its preferred format using the Accept header. Setting `Accept: application/yang-data+json` requests JSON format, while `Accept: application/yang-data+xml` requests XML. When sending data to the server in POST, PUT, or PATCH requests, the client uses the Content-Type header to indicate the format of the request body.

JSON representations follow specific encoding rules defined in RFC 7951. YANG leaf values map to JSON strings, numbers, or booleans based on their type. YANG containers become JSON objects. YANG lists become JSON arrays of objects. YANG leaf-lists become JSON arrays of values. Module names appear as prefixes on keys to handle namespace disambiguation when multiple YANG modules define elements with the same name.

Here's an example of an interface configuration in JSON:

```json
{
  "ietf-interfaces:interface": {
    "name": "eth0",
    "type": "iana-if-type:ethernetCsmacd",
    "enabled": true,
    "ietf-ip:ipv4": {
      "address": [
        {
          "ip": "192.168.1.1",
          "prefix-length": 24
        }
      ]
    }
  }
}
```

The same configuration in XML:

```xml
<interface xmlns="urn:ietf:params:xml:ns:yang:ietf-interfaces">
  <name>eth0</name>
  <type>iana-if-type:ethernetCsmacd</type>
  <enabled>true</enabled>
  <ipv4 xmlns="urn:ietf:params:xml:ns:yang:ietf-ip">
    <address>
      <ip>192.168.1.1</ip>
      <prefix-length>24</prefix-length>
    </address>
  </ipv4>
</interface>
```

Both representations convey identical information, structured according to the same YANG model. The choice between them depends on what's more convenient for your application.

## Query Parameters: Controlling Responses

RESTCONF provides query parameters that modify how the server processes requests and formats responses. These parameters give you fine-grained control over what data you receive without requiring custom API endpoints.

The `depth` parameter limits how deep the server traverses the resource hierarchy. Setting `depth=1` returns only immediate children of the requested resource, not descendants further down the tree. This prevents overwhelming responses when you only need high-level information. Unlimited depth is the default, returning the complete subtree.

The `fields` parameter selects specific elements within a resource, similar to SQL's SELECT clause. Instead of receiving all fields of an interface, you can request just the fields you care about: `fields=name;enabled;mtu`. This reduces bandwidth and processing overhead when you only need a subset of the data.

The `filter` parameter applies XPath or subtree filtering to select which resources to return. While more complex than other parameters, it provides powerful selection capabilities for sophisticated queries that need to match specific criteria across multiple elements.

The `insert` and `point` parameters control where new list entries are placed when creating ordered lists. You can specify whether to insert at the beginning, end, before a specific entry, or after a specific entry. This matters for configurations where order is significant, like access control lists or routing policies.

The `content` parameter determines whether to return configuration data, operational state data, or both. Setting `content=config` returns only configuration elements, while `content=nonconfig` returns only state data. The default, `content=all`, returns everything. This distinction matters because operational state can be extensive and slow to gather, so filtering it out improves performance when you only need configuration.

## A RESTCONF Client Implementation in C

Building a RESTCONF client requires HTTP handling and JSON or XML parsing capabilities. This implementation uses libcurl for HTTP and a simple JSON parsing approach for clarity.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define RESTCONF_ROOT "https://192.168.1.1:443/restconf"
#define USERNAME "admin"
#define PASSWORD "password"

/* Structure to hold HTTP response */
typedef struct {
    char *data;
    size_t size;
} response_t;

/* Callback for libcurl to store response data */
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    response_t *response = (response_t *)userp;
    
    char *new_data = realloc(response->data, response->size + total_size + 1);
    if (new_data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }
    
    response->data = new_data;
    memcpy(&(response->data[response->size]), contents, total_size);
    response->size += total_size;
    response->data[response->size] = '\0';
    
    return total_size;
}

/* Initialize CURL with common settings */
CURL* restconf_init(const char *username, const char *password) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return NULL;
    }
    
    /* Set authentication */
    curl_easy_setopt(curl, CURLOPT_USERNAME, username);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
    
    /* Accept self-signed certificates (for testing only) */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    /* Set write callback */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    
    return curl;
}

/* Perform GET request */
response_t* restconf_get(CURL *curl, const char *path) {
    char url[512];
    snprintf(url, sizeof(url), "%s%s", RESTCONF_ROOT, path);
    
    response_t *response = malloc(sizeof(response_t));
    response->data = NULL;
    response->size = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    
    /* Set headers for JSON */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/yang-data+json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "GET failed: %s\n", curl_easy_strerror(res));
        free(response->data);
        free(response);
        return NULL;
    }
    
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("HTTP Status: %ld\n", http_code);
    
    return response;
}

/* Perform POST request */
response_t* restconf_post(CURL *curl, const char *path, const char *data) {
    char url[512];
    snprintf(url, sizeof(url), "%s%s", RESTCONF_ROOT, path);
    
    response_t *response = malloc(sizeof(response_t));
    response->data = NULL;
    response->size = 0;
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    
    /* Set headers */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/yang-data+json");
    headers = curl_slist_append(headers, "Accept: application/yang-data+json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "POST failed: %s\n", curl_easy_strerror(res));
        free(response->data);
        free(response);
        return NULL;
    }
    
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("HTTP Status: %ld\n", http_code);
    
    return response;
}

/* Perform PUT request */
int restconf_put(CURL *curl, const char *path, const char *data) {
    char url[512];
    snprintf(url, sizeof(url), "%s%s", RESTCONF_ROOT, path);
    
    response_t response = {NULL, 0};
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/yang-data+json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    free(response.data);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "PUT failed: %s\n", curl_easy_strerror(res));
        return -1;
    }
    
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("HTTP Status: %ld\n", http_code);
    
    return (http_code >= 200 && http_code < 300) ? 0 : -1;
}

/* Perform PATCH request */
int restconf_patch(CURL *curl, const char *path, const char *data) {
    char url[512];
    snprintf(url, sizeof(url), "%s%s", RESTCONF_ROOT, path);
    
    response_t response = {NULL, 0};
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/yang-data+json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    free(response.data);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "PATCH failed: %s\n", curl_easy_strerror(res));
        return -1;
    }
    
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("HTTP Status: %ld\n", http_code);
    
    return (http_code >= 200 && http_code < 300) ? 0 : -1;
}

/* Perform DELETE request */
int restconf_delete(CURL *curl, const char *path) {
    char url[512];
    snprintf(url, sizeof(url), "%s%s", RESTCONF_ROOT, path);
    
    response_t response = {NULL, 0};
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    CURLcode res = curl_easy_perform(curl);
    
    free(response.data);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "DELETE failed: %s\n", curl_easy_strerror(res));
        return -1;
    }
    
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    printf("HTTP Status: %ld\n", http_code);
    
    return (http_code >= 200 && http_code < 300) ? 0 : -1;
}

/* Main program demonstrating RESTCONF operations */
int main() {
    /* Initialize CURL */
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = restconf_init(USERNAME, PASSWORD);
    
    if (!curl) {
        curl_global_cleanup();
        return 1;
    }
    
    /* Discover server capabilities */
    printf("=== Getting RESTCONF Root ===\n");
    response_t *root = restconf_get(curl, "/");
    if (root && root->data) {
        printf("Response:\n%s\n\n", root->data);
        free(root->data);
        free(root);
    }
    
    /* Get all interfaces */
    printf("=== Getting All Interfaces ===\n");
    response_t *interfaces = restconf_get(curl, "/data/ietf-interfaces:interfaces");
    if (interfaces && interfaces->data) {
        printf("Response:\n%s\n\n", interfaces->data);
        free(interfaces->data);
        free(interfaces);
    }
    
    /* Get specific interface with depth limit */
    printf("=== Getting Specific Interface (depth=1) ===\n");
    response_t *eth0 = restconf_get(curl, 
        "/data/ietf-interfaces:interfaces/interface=eth0?depth=1");
    if (eth0 && eth0->data) {
        printf("Response:\n%s\n\n", eth0->data);
        free(eth0->data);
        free(eth0);
    }
    
    /* Create new interface using POST */
    printf("=== Creating New Interface ===\n");
    const char *new_interface = 
        "{"
        "  \"ietf-interfaces:interface\": {"
        "    \"name\": \"eth2\","
        "    \"type\": \"iana-if-type:ethernetCsmacd\","
        "    \"enabled\": true,"
        "    \"description\": \"Created via RESTCONF\""
        "  }"
        "}";
    
    response_t *create_result = restconf_post(curl, 
        "/data/ietf-interfaces:interfaces", new_interface);
    if (create_result) {
        if (create_result->data) {
            printf("Response:\n%s\n\n", create_result->data);
            free(create_result->data);
        }
        free(create_result);
    }
    
    /* Update interface using PATCH (partial update) */
    printf("=== Updating Interface MTU ===\n");
    const char *update_mtu = 
        "{"
        "  \"ietf-interfaces:interface\": {"
        "    \"mtu\": 9000"
        "  }"
        "}";
    
    restconf_patch(curl, 
        "/data/ietf-interfaces:interfaces/interface=eth2", update_mtu);
    
    /* Replace interface using PUT (complete replacement) */
    printf("=== Replacing Interface Configuration ===\n");
    const char *replace_interface = 
        "{"
        "  \"ietf-interfaces:interface\": {"
        "    \"name\": \"eth2\","
        "    \"type\": \"iana-if-type:ethernetCsmacd\","
        "    \"enabled\": false,"
        "    \"description\": \"Replaced via RESTCONF\","
        "    \"mtu\": 1500"
        "  }"
        "}";
    
    restconf_put(curl, 
        "/data/ietf-interfaces:interfaces/interface=eth2", replace_interface);
    
    /* Invoke an RPC operation */
    printf("=== Invoking RPC Operation ===\n");
    const char *rpc_input = 
        "{"
        "  \"input\": {"
        "    \"delay\": 5"
        "  }"
        "}";
    
    response_t *rpc_result = restconf_post(curl, 
        "/operations/example:restart-interface", rpc_input);
    if (rpc_result) {
        if (rpc_result->data) {
            printf("RPC Result:\n%s\n\n", rpc_result->data);
            free(rpc_result->data);
        }
        free(rpc_result);
    }
    
    /* Delete the interface */
    printf("=== Deleting Interface ===\n");
    restconf_delete(curl, 
        "/data/ietf-interfaces:interfaces/interface=eth2");
    
    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}
```

This implementation demonstrates the complete RESTCONF operation set. Notice how familiar HTTP operations feel compared to NETCONF's XML-RPC approach. You use standard HTTP methods and JSON data, making integration with web applications straightforward. The code could run in a browser using JavaScript's fetch API with only minor modifications.

## Error Handling and Status Codes

RESTCONF uses HTTP status codes to indicate operation outcomes, supplemented by detailed error information in the response body when failures occur. This combination provides both machine-readable codes and human-readable descriptions.

Success codes include 200 OK for successful GET requests, 201 Created for successful resource creation with POST, 204 No Content for successful operations that don't return data like DELETE or PATCH, and 304 Not Modified when the resource hasn't changed based on conditional request headers.

Client error codes indicate problems with the request. 400 Bad Request means malformed syntax, 401 Unauthorized indicates missing or invalid authentication, 403 Forbidden means the authenticated user lacks permission, 404 Not Found signals that the requested resource doesn't exist, 409 Conflict arises when the request conflicts with current state like trying to create a resource that already exists, and 412 Precondition Failed occurs when conditional headers like If-Match don't match the current resource state.

Server error codes like 500 Internal Server Error indicate unexpected server failures, while 503 Service Unavailable suggests temporary problems.

Beyond HTTP status codes, RESTCONF returns detailed error information in the response body using the `ietf-restconf:errors` container. This structure includes the error type, error tag identifying the specific error condition, error application tag for application-specific errors, error message providing human-readable description, and error info containing additional details about the failure. This structured error reporting enables sophisticated error handling in client applications.

## Concurrent Modification and ETags

Because RESTCONF is stateless and doesn't use NETCONF's explicit locking mechanism, it needs another way to handle concurrent modifications. HTTP's Entity Tags (ETags) and conditional requests solve this problem elegantly.

When you GET a resource, the server includes an ETag header containing a token that uniquely identifies this version of the resource. Any change to the resource generates a new ETag. Before modifying the resource, you include an If-Match header with the ETag value you received. The server only processes your modification if the current resource still has that ETag, meaning nobody else modified it since you retrieved it. If someone else changed it, the ETag differs, and the server returns 412 Precondition Failed, preventing your potentially conflicting change from applying.

This optimistic concurrency control scales better than locking because clients don't hold locks for extended periods. Multiple clients can read simultaneously without blocking. Conflicts are detected at modification time rather than preventing all concurrent access.

## Notifications and Streaming

RESTCONF provides event notifications through Server-Sent Events (SSE), an HTTP-based streaming protocol. Unlike polling where clients repeatedly request updates, SSE establishes a long-lived HTTP connection over which the server pushes notifications as events occur.

To receive notifications, you perform a GET request on a stream endpoint like `/restconf/streams/interface-events`. The server responds with `Content-Type: text/event-stream` and keeps the connection open. When events occur, the server sends them over this connection in a standardized format. Each event includes a timestamp and the notification data structured according to the relevant YANG notification model.

This push-based approach is far more efficient than polling for applications that need real-time updates about configuration changes or operational state transitions. The client doesn't waste resources repeatedly asking "anything new?" and receives updates immediately when they occur.

## Choosing Between NETCONF and RESTCONF

Both protocols access the same underlying configuration datastores and enforce the same YANG-modeled constraints. The choice between them depends on your application's requirements and environment.

Use NETCONF when you need persistent session context, explicit configuration locking across multiple operations, direct access to all NETCONF capabilities like candidate configuration and explicit commit operations, or when your application already exists in a SSH-based management ecosystem. NETCONF's session model provides more control over complex multi-step configuration transactions.

Use RESTCONF when building web applications or microservices that naturally use HTTP and JSON, when you need to leverage web infrastructure like load balancers and caching proxies, when stateless operations align better with your architecture, or when you want simpler integration that doesn't require SSH libraries and XML parsing. RESTCONF's RESTful nature makes it more approachable for web developers and easier to integrate into modern cloud-native applications.

In practice, network devices often support both protocols simultaneously, allowing different applications to use whichever fits their needs best. A web dashboard might use RESTCONF while an automation system uses NETCONF, both managing the same device through their preferred interface.

# YANG Data Modeling: Structure and Constraints

## The Need for Data Models

Configuration data is complex and interconnected. An interface configuration might reference ACLs, routing policies, and VLANs. These relationships need validation: you can't apply an ACL that doesn't exist, and you can't configure mutually exclusive options. Without a formal data model, every application must implement its own validation logic, leading to inconsistencies and errors.

YANG (Yet Another Next Generation) solves this by providing a formal language to define the structure, relationships, and constraints of configuration data. It's like a schema for network configuration, similar to how database schemas define table structures and relationships. Once you define your configuration in YANG, tools can automatically validate data against the model, generate documentation, and create language bindings for programming languages.

Most importantly, YANG decouples the data model from the protocol. NETCONF and RESTCONF are protocols for transmitting and manipulating data, but they don't define what that data looks like. YANG fills this gap. The same YANG model works with both NETCONF and RESTCONF, and even with other protocols. This separation gives you protocol flexibility while maintaining a single authoritative definition of your configuration structure.

YANG itself is human-readable and hierarchical. Unlike XML Schema or JSON Schema which are verbose and difficult to read, YANG looks almost like pseudocode. Network engineers can understand YANG models without deep programming knowledge, making them accessible to the people who best understand the configuration requirements.

## Module Structure: The Foundation

A YANG module is a self-contained unit that defines data structures and their semantics. Every YANG file starts with a module statement that names the module. This name becomes the prefix used to reference elements from this module in XML or JSON representations.

After declaring the module name, you define the module's namespace. This is a URI that uniquely identifies this module across the entire world. Even if two organizations create modules with similar names, their namespaces ensure no confusion. The namespace doesn't need to be a working URL; it's just a unique identifier, though many organizations use their domain name as the namespace base.

The prefix statement defines the short string that will represent this module in XML and JSON encodings. When you see `if:interface` in an instance document, `if` is the prefix pointing to the interfaces module. Keeping prefixes short and memorable improves readability.

The import statement brings in other modules you need to reference. If your module uses types or groupings defined elsewhere, you import those modules and assign them local prefixes. This creates clean module boundaries while enabling reuse of common definitions.

The organization, contact, and description statements provide human-readable metadata. These aren't just formalities; good documentation in the model helps engineers understand the intended usage without reverse-engineering from examples.

The revision statement tracks the module's evolution. Each time you modify the module, you add a new revision with the date and a description of changes. This creates a clear history and helps tools understand compatibility between versions.

Here's a complete example of module structure:

```yang
module example-interfaces {
    yang-version 1.1;
    namespace "http://example.com/ns/interfaces";
    prefix "exif";
    
    import ietf-inet-types {
        prefix inet;
    }
    
    organization "Example Corporation";
    contact "support@example.com";
    description
        "This module defines interface configuration and state data.
         It provides structures for configuring network interfaces
         and monitoring their operational status.";
    
    revision 2024-01-15 {
        description "Added support for interface groups";
    }
    
    revision 2023-06-01 {
        description "Initial revision";
    }
    
    // Data model definitions follow
}
```

## Data Nodes: Building the Hierarchy

YANG organizes data into a tree structure composed of different node types. Understanding these node types is essential because they determine how data can be structured and accessed.

A container groups related configuration items without being repeated. Think of it as a folder in a file system. An interface container might hold all configuration specific to interfaces. Containers don't have keys because they appear exactly once in their parent context. You can't have multiple "interface" containers at the same level; there's just one that contains all interface-related data.

```yang
container interfaces {
    description "Interface configuration and state";
    
    // Contains lists and leafs defining interface data
}
```

A list represents repeatable elements, like multiple interfaces on a device. Each list entry must be uniquely identifiable through one or more key leafs. The key makes each entry retrievable independently. When you request `/interfaces/interface=eth0`, the name "eth0" is the key value identifying which interface you want.

```yang
list interface {
    key "name";
    description "Configuration for a single interface";
    
    leaf name {
        type string;
        description "Interface name, e.g., eth0";
    }
    
    leaf enabled {
        type boolean;
        default true;
        description "Enable or disable this interface";
    }
    
    leaf mtu {
        type uint16 {
            range "68..65535";
        }
        description "Maximum transmission unit in bytes";
    }
}
```

A leaf is a simple data value: a string, number, boolean, or other primitive type. Leafs are the actual configuration values that get set. In the example above, name, enabled, and mtu are all leafs. Each leaf has a type that constrains what values are valid.

A leaf-list is an ordered or unordered collection of simple values without keys. It's like an array of strings or numbers. DNS server addresses might be a leaf-list because you can have multiple servers, order might matter, but there's no meaningful key to identify each one individually.

```yang
leaf-list dns-server {
    type inet:ip-address;
    ordered-by user;
    description "List of DNS server addresses";
}
```

The ordered-by statement controls whether the list order is significant. When set to "user", the order is preserved as configured. When set to "system", the implementation determines the order.

## Type System: Constraining Values

YANG provides a rich set of built-in types and mechanisms to derive custom types with specific constraints. This type system is where much of the validation logic lives.

Numeric types include int8 through int64 for signed integers and uint8 through uint64 for unsigned integers. Each has a specific range that can be further constrained. The decimal64 type handles fixed-point decimal numbers with configurable precision.

String types can have length constraints and pattern matching using regular expressions. This lets you enforce format requirements like MAC addresses or hostnames.

```yang
typedef mac-address {
    type string {
        pattern '[0-9a-fA-F]{2}(:[0-9a-fA-F]{2}){5}';
    }
    description "MAC address in colon-separated hexadecimal notation";
}

typedef hostname {
    type string {
        length "1..253";
        pattern '[a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?(\.[a-zA-Z0-9]([a-zA-Z0-9-]*[a-zA-Z0-9])?)*';
    }
    description "Valid hostname according to RFC 1123";
}
```

Enumeration types define a fixed set of named values. This is cleaner than string types with artificial constraints and provides better validation.

```yang
leaf speed {
    type enumeration {
        enum "10M" {
            value 1;
            description "10 Megabit per second";
        }
        enum "100M" {
            value 2;
            description "100 Megabit per second";
        }
        enum "1G" {
            value 3;
            description "1 Gigabit per second";
        }
        enum "10G" {
            value 4;
            description "10 Gigabit per second";
        }
    }
    description "Interface speed";
}
```

Boolean types represent true or false values. The empty type represents existence rather than a value; the leaf is either present or absent, with no value needed. This is useful for flags.

Union types allow a leaf to accept multiple different types. For example, a timeout might accept either a number of seconds or the string "infinite".

```yang
typedef timeout {
    type union {
        type uint32 {
            range "1..max";
        }
        type enumeration {
            enum "infinite" {
                description "No timeout";
            }
        }
    }
    description "Timeout value in seconds or infinite";
}
```

The identityref type provides extensible enumerations. Unlike regular enums, identities can be extended in other modules. This is crucial for values that different vendors might want to augment.

```yang
identity interface-type {
    description "Base identity for interface types";
}

identity ethernet {
    base interface-type;
    description "Ethernet interface";
}

identity serial {
    base interface-type;
    description "Serial interface";
}

leaf type {
    type identityref {
        base interface-type;
    }
    description "Interface type";
}
```

Other modules can define additional interface types by creating new identities with the interface-type base, extending the type system without modifying your original module.

## Constraints: Enforcing Rules

Types provide basic validation, but complex configuration often requires more sophisticated constraints. YANG provides several mechanisms to express these rules.

The must statement defines XPath conditions that must evaluate to true for the configuration to be valid. This allows expressing constraints that involve multiple elements or complex logic.

```yang
container address-pool {
    leaf start-address {
        type inet:ipv4-address;
    }
    
    leaf end-address {
        type inet:ipv4-address;
    }
    
    must "start-address < end-address" {
        error-message "Start address must be less than end address";
        description "The address pool range must be valid";
    }
}
```

The when statement controls conditional presence. A node is only valid if the when condition evaluates to true. This handles situations where certain configuration only makes sense in specific contexts.

```yang
leaf vlan-id {
    when "../type = 'vlan-interface'";
    type uint16 {
        range "1..4094";
    }
    description "VLAN ID, only applicable for VLAN interfaces";
}
```

The mandatory statement forces a leaf to be present in valid configurations. Without this, leafs are optional by default.

```yang
leaf ip-address {
    type inet:ipv4-address;
    mandatory true;
    description "IP address must be configured";
}
```

The unique statement ensures that specific leafs have unique values across all list entries. This prevents duplicate configurations that would cause conflicts.

```yang
list user {
    key "username";
    
    leaf username {
        type string;
    }
    
    leaf email {
        type string;
    }
    
    unique "email";  // Email addresses must be unique across users
}
```

Range and length constraints restrict numeric and string values to specific boundaries. These appear within type definitions and provide immediate validation.

## Grouping and Reuse

As models grow, duplication becomes a problem. Multiple parts of your model might need similar structures. YANG's grouping statement enables defining reusable chunks of schema.

A grouping defines a set of nodes without placing them in the data tree. It's like a template or a function that you can instantiate wherever needed. The uses statement instantiates a grouping at a specific point in the hierarchy.

```yang
grouping address-info {
    description "Common address information";
    
    leaf street {
        type string;
    }
    
    leaf city {
        type string;
    }
    
    leaf postal-code {
        type string {
            pattern '[0-9]{5}(-[0-9]{4})?';
        }
    }
    
    leaf country {
        type string;
    }
}

container company {
    container billing-address {
        uses address-info;
    }
    
    container shipping-address {
        uses address-info;
    }
}
```

The refine statement modifies how a grouping is instantiated at a particular location. You might make a usually-optional leaf mandatory in one context, or tighten the constraints on a type.

```yang
container critical-address {
    uses address-info {
        refine "street" {
            mandatory true;
        }
        refine "city" {
            mandatory true;
        }
    }
}
```

This lets you maintain a single definition while adapting it to different requirements in different parts of your model.

## Augmentation: Extending Models

Network vendors often need to add proprietary features to standard models. Rather than copying and modifying the entire model, YANG's augment statement allows adding new nodes to existing structures.

```yang
module example-interface-extensions {
    namespace "http://example.com/extensions";
    prefix exext;
    
    import example-interfaces {
        prefix exif;
    }
    
    augment "/exif:interfaces/exif:interface" {
        description "Add vendor-specific features to interfaces";
        
        leaf hardware-offload {
            type boolean;
            default false;
            description "Enable hardware acceleration";
        }
        
        container vendor-stats {
            leaf packet-drops-hw {
                type uint64;
                description "Packets dropped by hardware";
            }
        }
    }
}
```

The augment path identifies where to insert the new nodes. When data is encoded, augmented elements appear as if they were part of the original model but use their own namespace prefix. This makes the extension explicit while maintaining clean separation between standard and proprietary features.

## RPCs and Actions

Configuration data isn't everything. Network devices need operations like restarting interfaces, clearing counters, or running diagnostics. YANG models operations using RPC (remote procedure call) and action statements.

An RPC defines a top-level operation with optional input and output parameters.

```yang
rpc restart-device {
    description "Restart the device";
    
    input {
        leaf delay {
            type uint32;
            units "seconds";
            default 0;
            description "Delay before restart";
        }
        
        leaf reason {
            type string;
            description "Reason for restart";
        }
    }
    
    output {
        leaf restart-time {
            type uint32;
            units "seconds";
            description "Estimated time until device comes back online";
        }
    }
}
```

An action is like an RPC but tied to a specific data node. Actions operate on particular configuration elements rather than the device as a whole.

```yang
list interface {
    key "name";
    
    leaf name {
        type string;
    }
    
    action reset-statistics {
        description "Reset counters for this interface";
        
        output {
            leaf reset-time {
                type yang:date-and-time;
                description "When the reset occurred";
            }
        }
    }
}
```

When invoked through RESTCONF, actions appear as POST operations on specific resources, while RPCs appear under the operations endpoint.

## Notifications: Modeling Events

Configuration changes and operational state transitions generate events. YANG notification statements define the structure of event data.

```yang
notification interface-state-change {
    description "Interface operational state changed";
    
    leaf interface-name {
        type leafref {
            path "/interfaces/interface/name";
        }
        description "Interface that changed";
    }
    
    leaf old-state {
        type enumeration {
            enum up;
            enum down;
            enum testing;
        }
    }
    
    leaf new-state {
        type enumeration {
            enum up;
            enum down;
            enum testing;
        }
    }
    
    leaf reason {
        type string;
        description "Human-readable reason for state change";
    }
}
```

When this notification fires, the device sends a message containing these fields, structured according to the notification definition. Management systems receiving the notification can parse it reliably because they have the YANG model.

## Features and Conditional Compilation

Not all devices support all features. A low-end switch might lack certain capabilities that a high-end router provides. YANG's feature statement handles this variance.

```yang
feature advanced-qos {
    description "Advanced quality-of-service features";
}

feature multicast-routing {
    description "Multicast routing support";
}

container qos {
    if-feature "advanced-qos";
    
    description "QoS configuration, only present if device supports it";
    
    // QoS-specific configuration
}

container multicast {
    if-feature "multicast-routing";
    
    description "Multicast routing configuration";
    
    // Multicast configuration
}
```

A device advertises which features it supports. Data nodes marked with if-feature only exist on devices that support that feature. This provides clean conditional inclusion without maintaining multiple model variants.

## A Complete YANG Example

Let's build a complete YANG module for managing network interfaces, demonstrating all major concepts.

```yang
module advanced-interfaces {
    yang-version 1.1;
    namespace "http://example.com/yang/interfaces";
    prefix "ai";
    
    import ietf-inet-types {
        prefix inet;
    }
    
    import ietf-yang-types {
        prefix yang;
    }
    
    organization "Example Network Corporation";
    contact "engineering@example.com";
    description
        "This module defines a comprehensive interface management model
         including configuration, operational state, statistics, and
         operations for network interfaces.";
    
    revision 2024-03-15 {
        description "Added support for link aggregation and VLAN subinterfaces";
    }
    
    revision 2024-01-10 {
        description "Initial release";
    }
    
    /*
     * Features
     */
    
    feature link-aggregation {
        description "Device supports link aggregation (LAG)";
    }
    
    feature vlan-subinterfaces {
        description "Device supports VLAN subinterfaces";
    }
    
    feature hardware-offload {
        description "Device supports hardware offload for specific operations";
    }
    
    /*
     * Typedefs
     */
    
    typedef interface-state {
        type enumeration {
            enum up {
                description "Interface is operational";
            }
            enum down {
                description "Interface is not operational";
            }
            enum testing {
                description "Interface is in testing mode";
            }
            enum unknown {
                description "Interface state cannot be determined";
            }
        }
        description "Operational state of an interface";
    }
    
    typedef mtu-size {
        type uint16 {
            range "68..65535";
        }
        description "Maximum transmission unit size in bytes";
    }
    
    typedef vlan-id {
        type uint16 {
            range "1..4094";
        }
        description "VLAN identifier";
    }
    
    /*
     * Identities
     */
    
    identity interface-type {
        description "Base identity for interface types";
    }
    
    identity ethernet {
        base interface-type;
        description "Ethernet interface";
    }
    
    identity serial {
        base interface-type;
        description "Serial interface";
    }
    
    identity loopback {
        base interface-type;
        description "Loopback interface";
    }
    
    identity lag {
        base interface-type;
        if-feature "link-aggregation";
        description "Link aggregation group interface";
    }
    
    /*
     * Groupings
     */
    
    grouping interface-counters {
        description "Common interface statistics counters";
        
        leaf in-octets {
            type yang:counter64;
            description "Total bytes received";
        }
        
        leaf in-packets {
            type yang:counter64;
            description "Total packets received";
        }
        
        leaf in-errors {
            type yang:counter64;
            description "Inbound packets with errors";
        }
        
        leaf in-discards {
            type yang:counter64;
            description "Inbound packets discarded";
        }
        
        leaf out-octets {
            type yang:counter64;
            description "Total bytes transmitted";
        }
        
        leaf out-packets {
            type yang:counter64;
            description "Total packets transmitted";
        }
        
        leaf out-errors {
            type yang:counter64;
            description "Outbound packets with errors";
        }
        
        leaf out-discards {
            type yang:counter64;
            description "Outbound packets discarded";
        }
    }
    
    grouping ipv4-address-config {
        description "IPv4 address configuration";
        
        list address {
            key "ip";
            description "List of IPv4 addresses on this interface";
            
            leaf ip {
                type inet:ipv4-address;
                description "IPv4 address";
            }
            
            leaf prefix-length {
                type uint8 {
                    range "0..32";
                }
                mandatory true;
                description "Length of the subnet prefix";
            }
        }
    }
    
    /*
     * Configuration data
     */
    
    container interfaces {
        description "Interface configuration and state";
        
        list interface {
            key "name";
            description "The list of interfaces on the device";
            
            leaf name {
                type string {
                    length "1..64";
                    pattern '[a-zA-Z][a-zA-Z0-9_.-]*';
                }
                description "Interface name";
            }
            
            leaf description {
                type string {
                    length "0..255";
                }
                description "Human-readable description of the interface";
            }
            
            leaf type {
                type identityref {
                    base interface-type;
                }
                mandatory true;
                description "Type of the interface";
            }
            
            leaf enabled {
                type boolean;
                default true;
                description "Enable or disable the interface administratively";
            }
            
            leaf mtu {
                type mtu-size;
                default 1500;
                description "Maximum transmission unit";
            }
            
            leaf mac-address {
                type yang:mac-address;
                description "MAC address of the interface";
            }
            
            container ipv4 {
                description "IPv4 configuration";
                
                leaf enabled {
                    type boolean;
                    default true;
                    description "Enable IPv4 on this interface";
                }
                
                uses ipv4-address-config;
            }
            
            container vlan {
                when "../type = 'ai:ethernet' or ../type = 'ai:lag'";
                if-feature "vlan-subinterfaces";
                description "VLAN configuration";
                
                leaf vlan-id {
                    type vlan-id;
                    description "VLAN identifier for subinterface";
                }
            }
            
            container lag {
                when "../type = 'ai:lag'";
                if-feature "link-aggregation";
                description "Link aggregation configuration";
                
                leaf-list member {
                    type leafref {
                        path "/interfaces/interface/name";
                    }
                    must "../../type != 'ai:lag'" {
                        error-message "Cannot add a LAG interface to another LAG";
                    }
                    description "Member interfaces of this LAG";
                }
                
                leaf mode {
                    type enumeration {
                        enum active {
                            description "Active LACP mode";
                        }
                        enum passive {
                            description "Passive LACP mode";
                        }
                        enum on {
                            description "LAG without LACP";
                        }
                    }
                    default active;
                    description "LAG mode";
                }
            }
            
            container offload {
                if-feature "hardware-offload";
                description "Hardware offload settings";
                
                leaf tcp-checksum {
                    type boolean;
                    default true;
                    description "Offload TCP checksum calculation";
                }
                
                leaf rx-checksum {
                    type boolean;
                    default true;
                    description "Offload receive checksum verification";
                }
            }
            
            /*
             * Operational state (config false)
             */
            
            container state {
                config false;
                description "Operational state data";
                
                leaf oper-status {
                    type interface-state;
                    description "Current operational state";
                }
                
                leaf last-change {
                    type yang:date-and-time;
                    description "Time of last operational state change";
                }
                
                leaf speed {
                    type uint64;
                    units "bits/second";
                    description "Current interface speed";
                }
                
                container statistics {
                    description "Interface statistics";
                    uses interface-counters;
                    
                    leaf discontinuity-time {
                        type yang:date-and-time;
                        description "Time of last counter discontinuity";
                    }
                }
            }
            
            /*
             * Actions
             */
            
            action reset-counters {
                description "Reset all statistics counters for this interface";
                
                output {
                    leaf reset-time {
                        type yang:date-and-time;
                        description "Time when counters were reset";
                    }
                }
            }
            
            action bring-up {
                description "Bring the interface up operationally";
                
                output {
                    leaf success {
                        type boolean;
                        description "Whether the operation succeeded";
                    }
                    
                    leaf message {
                        type string;
                        description "Status message";
                    }
                }
            }
            
            action bring-down {
                description "Bring the interface down operationally";
                
                input {
                    leaf reason {
                        type string;
                        description "Reason for bringing interface down";
                    }
                }
                
                output {
                    leaf success {
                        type boolean;
                        description "Whether the operation succeeded";
                    }
                }
            }
        }
    }
    
    /*
     * RPCs
     */
    
    rpc clear-all-counters {
        description "Clear statistics counters on all interfaces";
        
        output {
            leaf interfaces-cleared {
                type uint32;
                description "Number of interfaces whose counters were cleared";
            }
            
            leaf-list failed-interfaces {
                type string;
                description "List of interfaces where clearing failed";
            }
        }
    }
    
    /*
     * Notifications
     */
    
    notification interface-state-change {
        description "Generated when an interface changes operational state";
        
        leaf interface-name {
            type leafref {
                path "/interfaces/interface/name";
            }
            mandatory true;
            description "Interface that changed state";
        }
        
        leaf old-state {
            type interface-state;
            description "Previous operational state";
        }
        
        leaf new-state {
            type interface-state;
            mandatory true;
            description "New operational state";
        }
        
        leaf reason {
            type string;
            description "Reason for the state change";
        }
        
        leaf timestamp {
            type yang:date-and-time;
            mandatory true;
            description "Time of the state change";
        }
    }
    
    notification interface-error {
        description "Generated when significant errors occur on an interface";
        
        leaf interface-name {
            type leafref {
                path "/interfaces/interface/name";
            }
            mandatory true;
            description "Interface experiencing errors";
        }
        
        leaf error-type {
            type enumeration {
                enum crc-error {
                    description "CRC errors detected";
                }
                enum frame-error {
                    description "Framing errors detected";
                }
                enum overflow {
                    description "Buffer overflow";
                }
                enum carrier-lost {
                    description "Carrier signal lost";
                }
            }
            mandatory true;
            description "Type of error";
        }
        
        leaf error-count {
            type uint64;
            description "Number of errors detected";
        }
    }
}
```

This comprehensive model demonstrates configuration data with the interfaces container, operational state that cannot be modified with config false, constraints using must statements and when conditions, reusable patterns through groupings, extensibility via features and identities, operations through actions and RPCs, and event definitions with notifications.

## Validation and Tools

Writing YANG models is only the first step. You need tools to validate models, check data instances against models, and generate code or documentation.

The pyang tool is the reference YANG validator and can check syntax, resolve imports, verify constraints, generate tree representations showing the data hierarchy, convert between YANG and other formats, and create language bindings for Python, Java, and other languages.

To validate a YANG module, run pyang against it. The tool will report syntax errors, unresolved imports, constraint violations, and warnings about questionable modeling choices. Running pyang with the tree format generates a readable view of the data hierarchy that helps visualize the model structure.

The yanglint tool from the libyang library provides similar validation capabilities plus the ability to validate XML or JSON data instances against YANG models. This lets you check whether actual configuration data conforms to the model before sending it to a device.

These tools catch errors early in the development process rather than discovering them during device configuration, significantly improving reliability.

## The Living Model

YANG models aren't static artifacts but living documents that evolve with your network's needs. As requirements change, you update the model by adding new revisions. The revision history provides a clear change log showing exactly when each modification occurred and why.

Backward compatibility matters immensely. You can add new optional elements without breaking existing configurations, but removing elements or changing their semantics breaks compatibility and requires careful version management. Good modeling practices include designing for extensibility from the start, using features for optional capabilities, and thinking carefully before marking anything as mandatory.

When multiple organizations contribute to models, standard YANG modules emerge. The IETF maintains standard models for common networking concepts like interfaces, routing, and access control. Vendors implement these standard models while augmenting them with proprietary extensions. This creates a foundation of interoperability while allowing product differentiation.

The combination of standard models, vendor extensions, and custom organizational models creates a flexible ecosystem where automation tools can work across different devices while still accessing vendor-specific features when needed. This balance between standardization and flexibility makes YANG successful in the real world.