#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

#define PORT 8080
#define MAXLINE 1024

using namespace std;

// Function to encrypt the file content using bitwise XOR
string encryptFileContent(const string &content, char key)
{
	string encryptedContent = content;
	for (char &ch : encryptedContent)
	{
		ch ^= key; // XOR each character with the key
	}
	return encryptedContent;
}

int main()
{
	int sockfd; // Socket file descriptor
	struct sockaddr_in servaddr; // Structure to hold server address

								 // Create a UDP socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Initialize the server address structure
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET; // Use IPv4
	servaddr.sin_port = htons(PORT); // Set the port number
	servaddr.sin_addr.s_addr = INADDR_ANY; // Connect to the local server

										   // Read the file content
	ifstream inputFile("input.txt");
	if (!inputFile.is_open())
	{
		cerr << "Error: Unable to open input file." << endl;
		close(sockfd);
		return -1;
	}

	string fileContent((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
	inputFile.close();

	// Get the encryption key from the user
	char key;
	cout << "Enter encryption key (a single character): ";
	cin >> key;

	// Encrypt the file content
	string encryptedContent = encryptFileContent(fileContent, key);

	// Send the encrypted content to the server
	sendto(sockfd, encryptedContent.c_str(), encryptedContent.size(), MSG_CONFIRM,
		(const struct sockaddr *)&servaddr, sizeof(servaddr));
	cout << "Encrypted file content sent to server." << endl;

	close(sockfd);
	return 0;
}
