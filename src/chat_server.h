
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

// Configuration Constants
#define DEFAULT_PORT 8080
#define MAX_CLIENTS 100
#define MAX_USERNAME 50
#define MAX_MSG_LENGTH 1024
#define LOG_FILE "chat_server.log"
// At the top of chat_server.h, after includes

// Client Connection Status
typedef enum {
    CLIENT_CONNECTED,
    CLIENT_DISCONNECTED
} client_status_t;

// Client Structure
typedef struct {
    int socket_fd;               // Client socket file descriptor
    int client_id;               // Unique client identifier
    char username[MAX_USERNAME]; // Client username
    struct sockaddr_in address;  // Client network address
    client_status_t status;      // Connection status
    pthread_t thread_id;         // Client handling thread
} client_t;

// Server Context Structure
typedef struct {
    client_t *clients[MAX_CLIENTS]; // Array of connected clients
    int client_count;               // Current number of connected clients
    pthread_mutex_t clients_mutex;  // Mutex for thread-safe client management
    int running;                    // Server running status
} server_context_t;
extern server_context_t *server_context;
// Function Prototypes
// Server Initialization
server_context_t* create_server_context();
int initialize_server(int port);
void run_server(int server_socket);
void cleanup_server_context(server_context_t *context);

// Client Management
int add_client(server_context_t *context, client_t *client);
void remove_client(server_context_t *context, int client_id);
client_t* create_client(int socket_fd, struct sockaddr_in address);

// Message Handling
void broadcast_message(server_context_t *context, char *message, int sender_id);
void *handle_client(void *arg);

// Signal Handling
void setup_signal_handlers();
void signal_handler(int signum);

// Logging
void log_event(const char *event_type, const char *message);
void log_client_event(const char *event_type, client_t *client);

#endif // CHAT_SERVER_H