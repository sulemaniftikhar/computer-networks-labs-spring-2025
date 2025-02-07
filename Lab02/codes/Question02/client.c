#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 1234

int main() {
    int client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        return -1;
    }

    // Connect to the server
    if (connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    printf("Connected to the server.\n");

    while (1) {
        printf("Enter command (HELLO, TIME, EXIT): ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        // Send command to server
        send(client_fd, buffer, strlen(buffer), 0);

        // Receive response from server
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (valread > 0) {
            buffer[valread] = '\0'; // Null-terminate the received string
            printf("Server: %s\n", buffer);
        }

        if (strcmp(buffer, "Goodbye!") == 0) {
            break; // Exit if the server says goodbye
        }
    }

    // Close socket
    close(client_fd);
    return 0;
}
