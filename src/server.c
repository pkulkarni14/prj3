#include "chat_server.h"

void run_server(int server_socket) {
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    while (server_context && server_context->running) {
        int client_socket = accept(server_socket,
                                   (struct sockaddr *)&client_address,
                                   &address_length);

        if (client_socket < 0) {
            if (errno == EINTR) continue;
            perror("Accept failed");
            break;
        }

        client_t *new_client = create_client(client_socket, client_address);
        if (!new_client) {
            close(client_socket);
            continue;
        }

        if (add_client(server_context, new_client) != 0) {
            free(new_client);
            close(client_socket);
            continue;
        }

        if (pthread_create(&new_client->thread_id, NULL,
                           handle_client, (void*)new_client) != 0) {
            perror("Thread creation failed");
            remove_client(server_context, new_client->client_id);
            free(new_client);
            close(client_socket);
        }
    }
}