// client_handler.c
#include "chat_server.h"

// Create new client structure
client_t* create_client(int socket_fd, struct sockaddr_in address) {
    static int next_client_id = 1;

    client_t *client = malloc(sizeof(client_t));
    if (!client) {
        perror("Client memory allocation failed");
        return NULL;
    }

    // Initialize client details
    client->socket_fd = socket_fd;
    client->client_id = next_client_id++;
    client->address = address;
    client->status = CLIENT_CONNECTED;
    memset(client->username, 0, sizeof(client->username));

    // Send welcome message
    char welcome_msg[MAX_MSG_LENGTH];
    snprintf(welcome_msg, sizeof(welcome_msg), 
             "Welcome! Your client ID is %d\n", client->client_id);
    send(socket_fd, welcome_msg, strlen(welcome_msg), 0);

    return client;
}

// Add client to server context
int add_client(server_context_t *context, client_t *client) {
    pthread_mutex_lock(&context->clients_mutex);

    // Check if server is full
    if (context->client_count >= MAX_CLIENTS) {
        pthread_mutex_unlock(&context->clients_mutex);
        return -1;
    }

    // Find empty slot and add client
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!context->clients[i]) {
            context->clients[i] = client;
            context->client_count++;
            break;
        }
    }

    pthread_mutex_unlock(&context->clients_mutex);

    // Log client connection
    log_client_event("CLIENT_CONNECT", client);

    return 0;
}

// Remove client from server context
void remove_client(server_context_t *context, int client_id) {
    pthread_mutex_lock(&context->clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (context->clients[i] && context->clients[i]->client_id == client_id) {
            // Close socket and free client memory
            close(context->clients[i]->socket_fd);
            log_client_event("CLIENT_DISCONNECT", context->clients[i]);
            
            free(context->clients[i]);
            context->clients[i] = NULL;
            context->client_count--;
            break;
        }
    }

    pthread_mutex_unlock(&context->clients_mutex);
}

// Client communication handler
void *handle_client(void *arg) {
    client_t *client = (client_t*)arg;
    char buffer[MAX_MSG_LENGTH];
    int read_bytes;

    // Detach thread to auto-cleanup
    pthread_detach(pthread_self());

    // First message is username
    send(client->socket_fd, "Enter your username: ", 20, 0);
    read_bytes = recv(client->socket_fd, client->username, MAX_USERNAME, 0);
    if (read_bytes <= 0) {
        remove_client(server_context, client->client_id);
        return NULL;
    }
    client->username[read_bytes - 1] = '\0';  // Remove newline

    // Broadcast join message
    char join_msg[MAX_MSG_LENGTH];
    snprintf(join_msg, sizeof(join_msg), 
             "%s has joined the chat\n", client->username);
    broadcast_message(server_context, join_msg, client->client_id);

    // Message handling loop
    while ((read_bytes = recv(client->socket_fd, buffer, MAX_MSG_LENGTH, 0)) > 0) {
        // Null-terminate the message
        buffer[read_bytes] = '\0';

        // Check for exit command
        if (strcmp(buffer, "/quit\n") == 0) break;

        // Prepare broadcast message with username
        char full_message[MAX_MSG_LENGTH];
        snprintf(full_message, sizeof(full_message), 
                 "%s: %s", client->username, buffer);

        // Broadcast message to all clients
        broadcast_message(server_context, full_message, client->client_id);
    }

    // Client disconnection
    remove_client(server_context, client->client_id);
    return NULL;
}

// Broadcast message to all clients
void broadcast_message(server_context_t *context, char *message, int sender_id) {
    pthread_mutex_lock(&context->clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (context->clients[i] && 
            context->clients[i]->client_id != sender_id) {
            send(context->clients[i]->socket_fd, message, strlen(message), 0);
        }
    }

    pthread_mutex_unlock(&context->clients_mutex);

    // Log the broadcast
    log_event("MESSAGE_BROADCAST", message);
}