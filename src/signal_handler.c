// signal_handler.c
#include "chat_server.h"

// Global signal flag
volatile sig_atomic_t signal_received = 0;

// Signal handler
void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:
        case SIGTERM:
            signal_received = 1;
            
            // Stop server from accepting new connections
            if (server_context) {
                server_context->running = 0;
            }

            // Log server shutdown
            log_event("SERVER_SIGNAL", 
                      signum == SIGINT ? "Interrupt received" : "Termination signal received");
            break;
    }
}

// Set up signal handlers
void setup_signal_handlers() {
    struct sigaction sa;
    
    // Configure signal handler
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Register handlers for specific signals
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Unable to register SIGINT handler");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Unable to register SIGTERM handler");
        exit(EXIT_FAILURE);
    }
}