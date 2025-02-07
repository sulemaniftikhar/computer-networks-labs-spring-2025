#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 1234

int main(int argc, char const* argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    int addr_result = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    

    status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (status < 0)
    {
    	printf("Connection failed.\n");
    }
    else
    {
    	printf("Connection successfull.\n");
    }

    close(client_fd);
    return 0;
}
