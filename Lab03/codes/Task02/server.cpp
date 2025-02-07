#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fstream>

#define PORT 8080
#define MAXLINE 1024

using namespace std;

// Function to copy content from one file to another
bool copyFileContent(const string &srcFile, const string &destFile)
{
	// Open the source file in binary mode for reading
	ifstream src(srcFile, ios::binary);
	if (!src.is_open())
	{
		// If the source file cannot be opened, return false
		cerr << "Error: Unable to open source file." << endl;
		return false;
	}

	// Open the destination file in binary mode for writing (truncate if it exists)
	ofstream dest(destFile, ios::binary | ios::trunc);
	if (!dest.is_open())
	{
		// If the destination file cannot be opened, return false
		cerr << "Error: Unable to open destination file." << endl;
		return false;
	}

	// Copy the content of the source file to the destination file
	dest << src.rdbuf();
	return true; // Return true if the copy was successful
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

		   // Receive the file names from the client
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
	buffer[n] = '\0'; // Null-terminate the received data

					  // Parse the received file names (format: "source.txt,dest.txt")
	string input(buffer);
	size_t pos = input.find(",");
	if (pos == string::npos)
	{
		// If the input format is invalid, log an error and exit
		cerr << "Invalid input format from client." << endl;
		close(sockfd);
		return -1;
	}

	string srcFile = input.substr(0, pos); // Extract the source file name
	string destFile = input.substr(pos + 1); // Extract the destination file name

											 // Attempt to copy the content of the source file to the destination file
	bool success = copyFileContent(srcFile, destFile);

	// Prepare the response message based on the success or failure of the copy operation
	const char *response;
	if (success)
	{
		response = "File copied successfully.";
	}
	else
	{
		response = "Failed to copy file.";
	}

	// Send the response back to the client
	sendto(sockfd, response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
	cout << "Response sent to client: " << response << endl;

	// Close the socket and exit
	close(sockfd);
	return 0;
}
