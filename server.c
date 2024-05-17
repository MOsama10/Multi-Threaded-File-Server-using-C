#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 3333
#define MAXLINE 1024
#define MAX_CLIENTS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int client_count = 0;

void *client_handler(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[MAXLINE];
    int client_id;

    pthread_mutex_lock(&mutex);
    client_count++;
    client_id = client_count;
    pthread_mutex_unlock(&mutex);

    printf("Client_%d connected\n", client_id);

    while (1) {
        ssize_t recv_len = recv(client_socket, buffer, sizeof(buffer), 0);
        if (recv_len <= 0) {
            printf("Client_%d disconnected\n", client_id);
            break;
        }

        // Handle file upload
        if (strncmp(buffer, "upload", 6) == 0) {
            char filename[MAXLINE];
            ssize_t filename_len = recv(client_socket, filename, sizeof(filename), 0);
            if (filename_len <= 0) {
                printf("Error receiving filename from client_%d\n", client_id);
                break;
            }
            filename[filename_len] = '\0';
            FILE *file = fopen(filename, "wb");
            if (file == NULL) {
                printf("Error creating file for upload\n");
                break;
            }
            while (1) {
                ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    if (feof(file)) {
                        printf("File upload complete: %s\n", filename);
                    } else {
                        printf("Error receiving file data\n");
                    }
                    break;
                }
                fwrite(buffer, 1, bytes_received, file);
            }
            fclose(file);
        }

        // Handle file rename
        else if (strncmp(buffer, "rename", 6) == 0) {
            char old_filename[MAXLINE], new_filename[MAXLINE];
            ssize_t old_len = recv(client_socket, old_filename, sizeof(old_filename), 0);
            ssize_t new_len = recv(client_socket, new_filename, sizeof(new_filename), 0);
            if (old_len <= 0 || new_len <= 0) {
                printf("Error receiving filenames from client_%d\n", client_id);
                break;
            }
            old_filename[old_len] = '\0';
            new_filename[new_len] = '\0';
            if (rename(old_filename, new_filename) == 0) {
                printf("File renamed: %s -> %s\n", old_filename, new_filename);
            } else {
                printf("Error renaming file: %s\n", old_filename);
            }
        }

        // Handle file download
        else if (strncmp(buffer, "download", 8) == 0) {
            char filename[MAXLINE];
            ssize_t filename_len = recv(client_socket, filename, sizeof(filename), 0);
            if (filename_len <= 0) {
                printf("Error receiving filename from client_%d\n", client_id);
                break;
            }
            filename[filename_len] = '\0';
            FILE *file = fopen(filename, "rb");
            if (file == NULL) {
                printf("Error opening file for download: %s\n", filename);
                break;
            }
            while (1) {
                ssize_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
                if (bytes_read > 0) {
                    send(client_socket, buffer, bytes_read, 0);
                }
                if (bytes_read < sizeof(buffer)) {
                    if (feof(file)) {
                        printf("File download complete: %s\n", filename);
                    } else {
                        printf("Error reading file: %s\n", filename);
                    }
                    break;
                }
            }
            fclose(file);
        }

        // Echo received message back to client
        else {
            printf("  Sent from client_%d: %s", client_id, buffer);
            send(client_socket, buffer, recv_len, 0);
        }
    }

    close(client_socket);
    pthread_mutex_lock(&mutex);
    client_count--;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t threads[MAX_CLIENTS];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Assign IP, PORT
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        perror("Socket listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Socket accept failed");
            continue;
        }

        // Create thread to handle client
        pthread_t thread;
        if (pthread_create(&thread, NULL, client_handler, &client_socket) != 0) {
            perror("Thread creation failed");
            close(client_socket);
            continue;
        }

        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}

