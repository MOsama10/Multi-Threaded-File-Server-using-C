#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 3333
#define MAXLINE 1024

void upload_file(int sockfd, const char *file_path) {
    char buffer[MAXLINE];
    ssize_t bytes_read;

    // Send upload command to server
    send(sockfd, "upload", 6, 0);

    // Send filename to server
    char *filename = strrchr(file_path, '/');
    if (filename == NULL) {
        filename = (char *)file_path;
    } else {
        filename++; // Move past the '/'
    }
    send(sockfd, filename, strlen(filename), 0);

    // Send file contents to server
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error opening file %s.\n", file_path);
        return;
    }
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(sockfd, buffer, bytes_read, 0);
    }
    fclose(file);
}

void rename_file(int sockfd, const char *old_filename, const char *new_filename) {
    // Send rename command to server
    send(sockfd, "rename", 7, 0);

    // Send old and new filenames to server
    send(sockfd, old_filename, strlen(old_filename), 0);
    send(sockfd, new_filename, strlen(new_filename), 0);
}

void download_file(int sockfd, const char *filename) {
    // Send download command to server
    send(sockfd, "download", 9, 0);

    // Send filename to server
    send(sockfd, filename, strlen(filename), 0);

    // Receive file contents from server
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error creating file for download.\n");
        return;
    }
    char buffer[MAXLINE];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);
    }
    fclose(file);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    servaddr.sin_port = htons(PORT);

    // Connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection with the server failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");
    printf("Type 'exit' to quit.\n");

    char command[MAXLINE];
    char arg1[MAXLINE], arg2[MAXLINE];

    while (1) {
        printf("Enter command (upload, rename, download): ");
        fgets(command, sizeof(command), stdin);

        if (strncmp(command, "exit", 4) == 0) {
            printf("Client exiting...\n");
            break;
        }

        // Upload file
        else if (strncmp(command, "upload", 6) == 0) {
            printf("Enter file path to upload: ");
            fgets(arg1, sizeof(arg1), stdin);
            arg1[strcspn(arg1, "\n")] = 0; // remove newline character
            upload_file(sockfd, arg1);
        }

        // Rename file
        else if (strncmp(command, "rename", 6) == 0) {
            printf("Enter old filename: ");
            fgets(arg1, sizeof(arg1), stdin);
            arg1[strcspn(arg1, "\n")] = 0; // remove newline character
            printf("Enter new filename: ");
            fgets(arg2, sizeof(arg2), stdin);
            arg2[strcspn(arg2, "\n")] = 0; // remove newline character
            rename_file(sockfd, arg1, arg2);
        }

        // Download file
        else if (strncmp(command, "download", 8) == 0) {
            printf("Enter filename to download: ");
            fgets(arg1, sizeof(arg1), stdin);
            arg1[strcspn(arg1, "\n")] = 0; // remove newline character
            download_file(sockfd, arg1);
        }

        else {
            printf("Invalid command\n");
        }
    }

    // Close the socket
    close(sockfd);

    return 0;
}

