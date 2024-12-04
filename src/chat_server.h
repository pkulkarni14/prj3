
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

#define DEFAULT_PORT 8080
#define MAX_CLIENTS 100
#define MAX_USERNAME 50
#define MAX_MSG_LENGTH 1024
#define LOG_FILE "chat_server.log"


typedef enum {
    CLIENT_CONNECTED,
    CLIENT_DISCONNECTED
} client_status_t;

typedef struct {
    int socket_fd;               
    int client_id;            
    char username[MAX_USERNAME]; 
    struct sockaddr_in address;  
    client_status_t status;   
    pthread_t thread_id;         
} client_t;


typedef struct {
    client_t *clients[MAX_CLIENTS]; 
    int client_count;               
    pthread_mutex_t clients_mutex; 
    int running;                  
} server_context_t;
extern server_context_t *server_context;

server_context_t* create_server_context();
int initialize_server(int port);
void run_server(int server_socket);
void cleanup_server_context(server_context_t *context);


int add_client(server_context_t *context, client_t *client);
void remove_client(server_context_t *context, int client_id);
client_t* create_client(int socket_fd, struct sockaddr_in address);

void broadcast_message(server_context_t *context, char *message, int sender_id);
void *handle_client(void *arg);

void setup_signal_handlers();
void signal_handler(int signum);

void log_event(const char *event_type, const char *message);
void log_client_event(const char *event_type, client_t *client);

#endif 