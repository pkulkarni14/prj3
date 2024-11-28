#include "chat_server.h"

// Global server context
server_context_t *server_context = NULL;

// Create server context
server_context_t* create_server_context() {
    server_context_t *context = malloc(sizeof(server_context_t));
    if (!context) {
        perror("Failed to allocate server context");
        return NULL;
    }

    // Initialize context fields
    memset(context->clients, 0, sizeof(context->clients));
    context->client_count = 0;
    context->running = 1;

    // Initialize mutex
    if (pthread_mutex_init(&context->clients_mutex, NULL) != 0) {
        perror("Failed to initialize clients mutex");
        free(context);
        return NULL;
    }

    return context;
}

// Clean up server context
void cleanup_server_context(server_context_t *context) {
    if (!context) return;

    // Close all client connections
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (context->clients[i]) {
            close(context->clients[i]->socket_fd);
            free(context->clients[i]);
            context->clients[i] = NULL;
        }
    }

    // Destroy mutex
    pthread_mutex_destroy(&context->clients_mutex);

    // Free context
    free(context);
}

// Initialize server socket
int initialize_server(int port) {
    int server_socket;
    struct sockaddr_in server_address;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Allow socket reuse
    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                   &optval, sizeof(optval)) < 0) {
        perror("Socket option setting failed");
        close(server_socket);
        return -1;
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&server_address, 
             sizeof(server_address)) < 0) {
        perror("Socket bind failed");
        close(server_socket);
        return -1;
    }

    // Start listening
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_socket);
        return -1;
    }

    return server_socket;
}