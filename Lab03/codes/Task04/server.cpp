#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

#define PORT 8080
#define MAXLINE 1024

using namespace std;

// Function to decrypt the file content using bitwise XOR
string decryptFileContent(const string &content, char key)
{
	string decryptedContent = content;
	for (char &ch : decryptedContent)
	{
		ch ^= key; // XOR each character with the key
	}
	return decryptedContent;
}

int main()
{
	int sockfd; // Socket file descriptor
	char buffer[MAXLINE]; // Buffer to store incoming data
	struct sockaddr_in servaddr, cliaddr; // Structures to hold server and client addresses

										  // Create a UDP socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Initialize server and client address structures
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

	// Receive the encrypted file content from the client
	n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
	buffer[n] = '\0'; // Null-terminate the received data

	cout << "Received encrypted file content from client." << endl;

	// Get the decryption key from the user
	char key;
	cout << "Enter decryption key (a single character): ";
	cin >> key;

	// Decrypt the file content
	string encryptedContent(buffer);
	string decryptedContent = decryptFileContent(encryptedContent, key);

	// Write the decrypted content to a new file
	ofstream outputFile("output.txt");
	if (!outputFile.is_open())
	{
		cerr << "Error: Unable to open output file." << endl;
		close(sockfd);
		return -1;
	}

	outputFile << decryptedContent;
	outputFile.close();

	cout << "Decrypted file content written to output.txt." << endl;

	close(sockfd);
	return 0;
}
