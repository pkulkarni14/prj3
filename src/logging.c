// logging.c
#include "chat_server.h"
#include <time.h>

// Log an event to file
void log_event(const char *event_type, const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    // Get current timestamp
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0;  // Remove newline

    // Write log entry
    fprintf(log_file, "[%s] %s: %s\n", timestamp, event_type, message);
    fclose(log_file);
}

// Log client-specific events
void log_client_event(const char *event_type, client_t *client) {
    if (!client) return;

    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    // Get current timestamp
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0;  // Remove newline

    // Prepare client information
    char client_info[256];
    snprintf(client_info, sizeof(client_info), 
             "Client ID: %d, Username: %s, IP: %s", 
             client->client_id, 
             client->username[0] ? client->username : "Unnamed",
             inet_ntoa(client->address.sin_addr));

    // Write log entry
    fprintf(log_file, "[%s] %s: %s\n", timestamp, event_type, client_info);
    fclose(log_file);
}

// Debug logging function (optional, more verbose)
void debug_log(const char *message) {
    #ifdef DEBUG
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    // Get current timestamp
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0;  // Remove newline

    // Write debug log entry
    fprintf(log_file, "[%s] DEBUG: %s\n", timestamp, message);
    fclose(log_file);
    #endif
}

// Log system errors
void error_log(const char *error_type, const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    // Get current timestamp
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0;  // Remove newline

    // Write error log entry
    fprintf(log_file, "[%s] ERROR (%s): %s\n", timestamp, error_type, message);
    fclose(log_file);
}