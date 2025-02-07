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

// Function to calculate the sum of even-positioned digits
int calculateEvenPositionedSum(const string &number)
{
	int sum = 0;
	// Iterate through the digits, considering 1-based positions
	for (size_t i = 0; i < number.length(); ++i)
	{
		if ((i + 1) % 2 == 0)
		{ // Check if the position is even
			sum += number[i] - '0'; // Convert character to integer
		}
	}
	return sum;
}

int main()
{
	int sockfd; // Socket file descriptor
	char buffer[MAXLINE]; // Buffer to store incoming and outgoing data
	struct sockaddr_in servaddr, cliaddr; // Structures to hold server and client addresses

										  // Create a UDP socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Initialize server and client address structures to zero
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Fill in the server's address information
	servaddr.sin_family = AF_INET; // Use IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
	servaddr.sin_port = htons(PORT); // Set the port number

									 // Bind the socket to the server's address
	if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	socklen_t len = sizeof(cliaddr); // Length of the client address structure
	int n; // Number of bytes received

	cout << "Server is running and waiting for client..." << endl;

	while (true)
	{
		// Receive the number from the client
		n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
		buffer[n] = '\0'; // Null-terminate the received data

		cout << "Received number from client: " << buffer << endl;

		// Calculate the sum of even-positioned digits
		string number(buffer);
		int sum = calculateEvenPositionedSum(number);

		// Prepare the response message
		string response = "Sum of even-positioned digits: " + to_string(sum);

		// Send the response back to the client
		sendto(sockfd, response.c_str(), response.size(), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
		cout << "Response sent to client: " << response << endl;
	}

	close(sockfd);
	return 0;
}
