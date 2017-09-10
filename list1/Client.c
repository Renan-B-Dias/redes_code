#include <stdio.h>	// printf // socket
#include <stdlib.h>	// exit()
#include <string.h> // memset()
#include <sys/socket.h>	// socket() // pf_inet // sock_stream
#include <netinet/in.h>     /* struct sockaddr_in */
#include <arpa/inet.h>	// IPPROTO_TCP // struct sockaddr
#include <unistd.h>	// close()

#include "Client.h"

int main(int argc, char *argv[]) {

	if(argc != 3) {
		error("Missing params: <Server Ip> - <Port>");
	}

	int socketReturn = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketReturn < 0)
		error("Failed on socket connection");

	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddress.sin_port = htons(atoi(argv[2]));

	int connection = connect(socketReturn, (SA *) &serverAddress, sizeof(serverAddress));
	if(connection < 0)
		error("Failed to connect to server");

	char message[] = "Get Date";
	unsigned int messageLength = strlen(message);
	int sendReturn = send(socketReturn, message, messageLength, 0);
	if(sendReturn != messageLength)
		error("Failed to send whole message (bytes)");

	printf("The date is: ");

	int received = 0;
	int bytes = 1;
	char returnBuffer[BUFFERSIZE];
	while(bytes != 0) {
		if((bytes = recv(socketReturn, returnBuffer, BUFFERSIZE-1, 0)) < 0) {
			error("Failed to receive from server");
		}

		if(bytes == 0)
			break;

		received += bytes;
		returnBuffer[bytes] = '\0';
		printf("%s", returnBuffer);
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