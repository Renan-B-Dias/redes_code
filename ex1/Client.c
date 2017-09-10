#include <stdio.h>	// printf // socket
#include <stdlib.h>	// exit()
#include <string.h> // memset()
#include <sys/socket.h>	// socket() // pf_inet // sock_stream
#include <netinet/in.h>     /* struct sockaddr_in */
#include <arpa/inet.h>	// ipproto_tcp // struct sockaddr
#include <unistd.h>	// close()

#include "Client.h"

int main(int argc, char *argv[]) {
	
	if(argc != 4) {
		error("Missing params: <Server Ip> - <Word> - <Port>");
	}

	int socketReturn = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketReturn < 0)
		error("Failled on socket creation.");

	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);	// Takes server ip
	serverAddress.sin_port = htons(atoi(argv[3]));			// Takes port number;

	int connection = connect(socketReturn, (SA *) &serverAddress, sizeof(serverAddress));
	if(connection < 0) {
		error("Failed to connect to server.");
	}

	unsigned int messageLength = strlen(argv[2]);
	int sendReturn = send(socketReturn, argv[2], messageLength, 0);
	if(sendReturn != messageLength)
		error("Failed to send whole message (bytes)");

	printf("Received: ");

	int received = 0;
	int bytes = 0;
	char messageBuffer[BUFFERSIZE];
	while(received < messageLength) {
		if((bytes = recv(socketReturn, messageBuffer, BUFFERSIZE-1, 0)) < 1) {
			error("Failed to receive from server");
		}

		received += bytes;
		messageBuffer[bytes] = '\0';
		printf("%s", messageBuffer);
	}

	printf("\n");

	close(socketReturn);
	exit(0);
}

void error(char *msg) {
	printf("%s\n", msg);
	exit(0);

	return;
}