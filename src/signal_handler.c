
#include "chat_server.h"

volatile sig_atomic_t signal_received = 0;

void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:
        case SIGTERM:
            signal_received = 1;
            
            if (server_context) {
                server_context->running = 0;
            }

            log_event("SERVER_SIGNAL", 
                      signum == SIGINT ? "Interrupt received" : "Termination signal received");
            break;
    }
}

void setup_signal_handlers() {
    struct sigaction sa;

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Unable to register SIGINT handler");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Unable to register SIGTERM handler");
        exit(EXIT_FAILURE);
    }
}