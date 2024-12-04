#include "chat_server.h"
#include <time.h>

void log_event(const char *event_type, const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0;  

    fprintf(log_file, "[%s] %s: %s\n", timestamp, event_type, message);
    fclose(log_file);
}

void log_client_event(const char *event_type, client_t *client) {
    if (!client) return;

    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0;  

    char client_info[256];
    snprintf(client_info, sizeof(client_info), 
             "Client ID: %d, Username: %s, IP: %s", 
             client->client_id, 
             client->username[0] ? client->username : "Unnamed",
             inet_ntoa(client->address.sin_addr));

    fprintf(log_file, "[%s] %s: %s\n", timestamp, event_type, client_info);
    fclose(log_file);
}

void error_log(const char *error_type, const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0;  

    fprintf(log_file, "[%s] ERROR (%s): %s\n", timestamp, error_type, message);
    fclose(log_file);
}