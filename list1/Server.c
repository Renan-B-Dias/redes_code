#include <stdio.h>	// printf // socket
#include <stdlib.h>	// exit
#include <sys/socket.h>	// socket() // pf_inet // sock_stream
#include <arpa/inet.h>	// ipproto_tcp // struct sockaddr
#include <string.h>	// memset()
#include <stdbool.h> // use Boolean as "true" or "false"
#include <unistd.h>	// close()
#include <time.h>

#include "Server.h"

int main(int argc, char *argv[]) {

	if(argc != 2)
		error("Messing params: <port number>");

	int socketReturn = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketReturn < 0)
		error("Socket error");

	struct sockaddr_in serverAddres;
	memset(&serverAddres, 0, sizeof(serverAddres));
	serverAddres.sin_family = AF_INET;
	serverAddres.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddres.sin_port = htons(atoi(argv[1]));

	int bindReturn = bind(socketReturn, (SA *) &serverAddres, sizeof(serverAddres));
	if(bindReturn < 0)
		error("Error on bind");

	int listenReturn = listen(socketReturn, MAXPENDING);
	if(listenReturn < 0)
		error("Error on listen");

	int messageLength = 0, clientLength;
	struct sockaddr_in client;
	char messageBuffer[BUFFERSIZE];

	while(true) {
		// messageLength = -1;
		clientLength = sizeof(client);

		int connection = accept(socketReturn, (SA *) &client, &clientLength);
		if(connection < 0)
			error("Fail to establish connection with client.");

		printf("Client connected: %s:%d\n", inet_ntoa(client.sin_addr), client.sin_port);
		messageLength = recv(connection, messageBuffer, BUFFERSIZE, 0);
		if(messageLength < 0)
			error("Fail to receive message from client");

		messageBuffer[messageLength] = '\0';
		// printf("%s\n", messageBuffer);

		time_t currentTime = time(NULL);
		char *timeString = ctime(&currentTime);
		// printf("%s", timeString);
		// int timeLength = sizeof(timeString);
		int timeLength = (int) strlen(timeString);
		// printf("%d\n", timeLength);
		// printf(ctime(&currentTime));
		if(send(connection, timeString, timeLength, 0) != timeLength)
			error("Error sending date");

		close(connection);
	}

	exit(0);
}

void error(char *message) {
	printf("%s\n", message);
	exit(0);

	return;
}
