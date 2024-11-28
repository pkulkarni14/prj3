// main.c
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "chat_server.h"

// Declare server_context as external to avoid global definition issues
extern server_context_t *server_context;

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    
    // Parse command line arguments for port
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    // Initialize server context
    server_context = create_server_context();
    if (!server_context) {
        fprintf(stderr, "Failed to create server context\n");
        return EXIT_FAILURE;
    }

    // Set up signal handlers
    setup_signal_handlers();

    // Initialize server socket
    int server_socket = initialize_server(port);
    if (server_socket < 0) {
        fprintf(stderr, "Failed to initialize server\n");
        cleanup_server_context(server_context);
        return EXIT_FAILURE;
    }

    log_event("SERVER_START", "Chat server initialized successfully");

    // Main server loop
    run_server(server_socket);

    // Cleanup
    cleanup_server_context(server_context);
    close(server_socket);

    return EXIT_SUCCESS;
}