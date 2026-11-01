#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 2. Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Iterative Server listening on port %d...\n", PORT);

    // 4. Iterative Loop: Process one client at a time sequentially
    while (1) {
        printf("\nWaiting for a connection...\n");
        
        // Accept a connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected: %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Process client request
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            printf("Received from client: %s\n", buffer);

            // Echo response back
            char *response = "Hello from Iterative Server!";
            send(client_fd, response, strlen(response), 0);
            printf("Response sent to client.\n");
        }

        // Close connection with the current client before handling the next
        close(client_fd);
        printf("Client disconnected.\n");
    }

    close(server_fd);
    return 0;
}
