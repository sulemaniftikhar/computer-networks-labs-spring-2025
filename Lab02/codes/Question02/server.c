#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <ctime>

using namespace std;

int main() {
    char buffer[1024];
    int conn_sock, comm_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    conn_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_sock < 0) {
        cerr << "Socket creation failed." << endl;
        return -1;
    }

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    // Bind the socket
    if (bind(conn_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Bind failed." << endl;
        close(conn_sock);
        return -1;
    }

    // Listen for incoming connections
    listen(conn_sock, 5);
    cout << "Server is listening on port 1234..." << endl;

    // Accept a connection
    client_addr_len = sizeof(client_addr);
    comm_sock = accept(conn_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    if (comm_sock < 0) {
        cerr << "Accept failed." << endl;
        close(conn_sock);
        return -1;
    }
    cout << "Connection established with client." << endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int n = recv(comm_sock, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) {
            cout << "Client disconnected." << endl;
            break;
        }

        buffer[n] = '\0'; // Null-terminate the received string
        cout << "Received: " << buffer << endl;

        if (strcmp(buffer, "HELLO") == 0) {
            const char* response = "Hello, Client!";
            send(comm_sock, response, strlen(response), 0);
        } else if (strcmp(buffer, "TIME") == 0) {
            time_t now = time(0);
            char* dt = ctime(&now);
            send(comm_sock, dt, strlen(dt), 0);
        } else if (strcmp(buffer, "EXIT") == 0) {
            const char* response = "Goodbye!";
            send(comm_sock, response, strlen(response), 0);
            break;
        } else {
            const char* response = "Unknown command.";
            send(comm_sock, response, strlen(response), 0);
        }
    }

    // Close sockets
    close(comm_sock);
    close(conn_sock);
    return 0;
}
