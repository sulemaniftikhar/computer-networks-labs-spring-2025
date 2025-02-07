#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    char msg[100];
    int conn_sock, comm_sock, n;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    conn_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234); // Use port 1234
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket
    int bind_result = bind(conn_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen for incoming connections
    listen(conn_sock, 10);

    // Accept a connection
    client_addr_len = sizeof(client_addr);
    comm_sock = accept(conn_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    
    if (comm_sock < 0)
    {
    	printf("Accept failed.\n");
    }
    else
    {
    	printf("Connection established with client.\n");	
    }

    // Close sockets
    close(comm_sock);
    close(conn_sock);
    return 0;
}
