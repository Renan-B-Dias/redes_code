#include <stdio.h>	// printf // socket
#include <stdlib.h>	// exit
#include <sys/socket.h>	// socket() // pf_inet // sock_stream
#include <arpa/inet.h>	// ipproto_tcp // struct sockaddr
#include <string.h>	// memset()
#include <stdbool.h> // use Boolean as "true" or "false"
#include <unistd.h>	// close()

#include "Server.h"

// Main args must be argc - Which I think it's stands of "arguments count" and argv - Which I think it stands for arguments values.
int main(int argc, char *argv[]) {

	if(argc != 2) {
		error("Missing params: <port number>");
	}

	// Listen = return of socket system call
	// Socket returns 0 if everything is ok or -1 if there was an error
	// returns a file descriptor
	int socketReturn = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketReturn < 0)
		error("Socket error (syscall error)");

	// struct sockaddr_in is the structure used with IPv4 addresses (e.g. "192.0.2.10"). It holds an address family (AF_INET), a port in sin_port, and an IPv4 address in sin_addr.
	// struct mus be before it's type if it's type is a struct (will always be)
	struct sockaddr_in serverAddress;
	// bzero(&serverAddress, sizeof(serverAddress)); - deprecated
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;	// Server family - AF_INET/AF_UNIX/AF_ISO/AF_NS
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// INADDR_ANY = 0 sets ip acceptable - htonl = hostLong - converts unsigned long interger from host byte order to network byte order
	serverAddress.sin_port = htons(atoi(argv[1])); // atoi - integral number to integer // htons hostShort - converts unsigned short integer from host byte order to network byte order - in this case the number of the port

	int bindReturn = bind(socketReturn, (SA *) &serverAddress, sizeof(serverAddress)); // retruns 0 if succes or -1 if error ocurred
	if(bindReturn < 0) {
		error("Error on bind");
	}

	int listenReturn = listen(socketReturn, MAXPENDING);
	if(listenReturn < 0) {
		error("Error on listen");
	}

	int messageLength, clientLength;
	struct sockaddr_in client;
	char messageBuffer[BUFFERSIZE];

	while(true) {
		messageLength = -1;
		clientLength = sizeof(client);

		int connection = accept(socketReturn, (SA *) &client, &clientLength);
		if(connection < 0)
			error("Fail to establish connection with client.");

		printf("Client connected: %s:%d\n", inet_ntoa(client.sin_addr), client.sin_port);

		// change here
		// recv() - socket - buffer (where the msg will be stored) - lenght (of the buffer) - flags
		// recv() - onSuccess returns the length of the message (probably smaller than bufferSize) - return -1 onError and 0 if no message has been sent.
		messageLength = recv(connection, messageBuffer, BUFFERSIZE, 0);
		if(messageLength < 0)
			error("Fail on receiving message from client");

		while(messageLength > 0) {
			// send() - socket, buffer (containing the message to be sent), length (in bytes of message), flags

			// printf("Here\n");
			// printf("%d\n", messageLength);
			// printf("%s\n", messageBuffer);
			// int sendResponse = send(connection, buffer, messageLength, 0)
			// if(sendResponse != messageLength) // did not send all
			// Sends what it received - messageLenght is 1 for each char on the message
			// then it tries to receive more if there is more
			// if there isn't messageLength will be 0.

			if(send(connection, messageBuffer, messageLength, 0) != messageLength) // did not send all
				error("Failed to send whole message");

			if((messageLength = recv(connection, messageBuffer, BUFFERSIZE, 0)) < 0)
				error("Failed to receive data from client");
		}

		close(connection);
	}

	exit(0);
}

void error(char *message) {
	printf("%s\n", message);
	exit(0);
	return;
}