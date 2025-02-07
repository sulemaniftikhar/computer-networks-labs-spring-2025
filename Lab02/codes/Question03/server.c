#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>

using namespace std;

int main()
{
    char filename[100];
    int block_size;
    int conn_sock, comm_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    conn_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket
    int bind_result = bind(conn_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_result < 0) {
        perror("Bind failed");
        return -1;
    }

    // Listen for incoming connections
    listen(conn_sock, 10);

    // Accept a connection
    client_addr_len = sizeof(client_addr);
    comm_sock = accept(conn_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    
    if (comm_sock < 0) {
        printf("Accept failed.\n");
        return -1;
    } else {
        printf("Connection established with client.\n");
    }

    // Receive filename and block size from the client
    recv(comm_sock, filename, sizeof(filename), 0);
    recv(comm_sock, &block_size, sizeof(block_size), 0);

    // Print the requested filename
    cout << "Requested filename: " << filename << endl;

    // Open the file
    ifstream file(filename);
    if (!file) {
        perror("File not found or cannot be opened");
        const char* msg = "File not found.\n";
        send(comm_sock, msg, strlen(msg), 0);
    } else {
        char *buffer = new char[block_size];
        while (file.read(buffer, block_size) || file.gcount() > 0) {
            int bytes_sent = file.gcount();
            send(comm_sock, buffer, bytes_sent, 0);
            cout << "Sending block of size: " << bytes_sent << endl; // Print the size of data sent
        }
        delete[] buffer;
        file.close();
        const char* msg = "\nFile transfer complete.\n";
        send(comm_sock, msg, strlen(msg), 0);
    }

    // Close sockets
    close(comm_sock);
    close(conn_sock);
    return 0;
}
