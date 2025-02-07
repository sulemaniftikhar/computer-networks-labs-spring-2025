#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> // Include this header for atoi
#define PORT 1234

int main(int argc, char const* argv[])
{
    int status, client_fd;
    struct sockaddr_in serv_addr;

    if (argc != 3) {
        printf("Usage: %s <filename> <block_size>\n", argv[0]);
        return -1;
    }

    const char* filename = argv[1];
    int block_size = atoi(argv[2]); // Convert block size from string to int

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (status < 0) {
        printf("Connection failed.\n");
        return -1;
    } else {
        printf("Connection successful.\n");
    }

    // Send filename and block size to the server
    send(client_fd, filename, strlen(filename), 0);
    send(client_fd, &block_size, sizeof(block_size), 0);

    // Receive file content in blocks
    char buffer[block_size + 1];
    int bytes_received;
    
    printf("Receiving file content:\n");
    while ((bytes_received = recv(client_fd, buffer, block_size, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the buffer
        printf("%s", buffer); // Print the received data
    }

    if (bytes_received < 0) {
        perror("Receive failed");
    } else if (bytes_received == 0) {
        printf("No more data received from the server.\n");
    }

    printf("\nFile transfer complete.\n"); // Indicate completion

    close(client_fd);
    return 0;
}
