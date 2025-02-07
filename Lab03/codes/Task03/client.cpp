#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

using namespace std;

int main()
{
	int sockfd; // Socket file descriptor
	char buffer[MAXLINE]; // Buffer to store incoming and outgoing data
	struct sockaddr_in servaddr; // Structure to hold server address

								 // Create a UDP socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Initialize the server address structure to zero
	memset(&servaddr, 0, sizeof(servaddr));

	// Fill in the server's address information
	servaddr.sin_family = AF_INET; // Use IPv4
	servaddr.sin_port = htons(PORT); // Set the port number
	servaddr.sin_addr.s_addr = INADDR_ANY; // Connect to the local server

										   // Prompt the user to enter a decimal integer
	string number;
	cout << "Enter a decimal integer: ";
	cin >> number;

	socklen_t len = sizeof(servaddr); // Length of the server address structure
	int n; // Number of bytes received

		   // Send the number to the server
	sendto(sockfd, number.c_str(), number.size(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	cout << "Number sent to server." << endl;

	// Receive the server's response
	n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
	buffer[n] = '\0'; // Null-terminate the received data

					  // Display the server's response
	cout << "Server response: " << buffer << endl;

	close(sockfd);
	return 0;
}
