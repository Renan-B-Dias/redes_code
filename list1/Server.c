#include <stdio.h>	// printf // socket // perror()
#include <stdlib.h>	// exit
#include <sys/socket.h>	// socket() // pf_inet // sock_stream
#include <arpa/inet.h>	// ipproto_tcp // struct sockaddr
#include <string.h>	// memset()
#include <stdbool.h> // use Boolean as "true" or "false"
#include <unistd.h>	// close()
#include <time.h>

#include "Server.h"

/*
	Note about socket "Address already in use":
	Address already in use." What does that mean?
	Well, a little bit of a socket that was connected is
	still hanging around in the kernel, and it's hogging the port.
	You can either wait for it to clear (a minute or so),
	or add code to your program allowing it to reuse the port, like this:
	*Check Header file...*
*/

int main(int argc, char *argv[]) {

	if(argc != 2)
		error("Missing params: <port number>");

	int socketReturn = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socketReturn < 0)
		error("Socket error");

	struct sockaddr_in serverAddres;
	memset(&serverAddres, 0, sizeof(serverAddres));
	serverAddres.sin_family = AF_INET;
	serverAddres.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddres.sin_port = htons(atoi(argv[1]));

	// Not sure if doind this way has consequences. (Ask teacher later...)
	if(setsockopt(socketReturn, SOL_SOCKET, SO_REUSEADDR, &REUSESOCKET, sizeof REUSESOCKET) == -1)
		error("Error on reuse socket");

	int bindReturn = bind(socketReturn, (SA *) &serverAddres, sizeof(serverAddres));
	if(bindReturn < 0)
		error("Error on bind");

	int listenReturn = listen(socketReturn, MAXPENDING);
	if(listenReturn < 0)
		error("Error on listen");

	printf(CYAN "Server is Up\n" RESET);

	int messageLength = 0, clientLength;
	struct sockaddr_in client;
	char messageBuffer[BUFFERSIZE];
	while(true) {
		// messageLength = -1;
		clientLength = sizeof(client);

		int connection = accept(socketReturn, (SA *) &client, &clientLength);
		if(connection < 0)
			error("Fail to establish connection with client.");

		printf(GRN "Client connected: %s:%d\n" RESET, inet_ntoa(client.sin_addr), client.sin_port);
		// messageLength = recv(connection, messageBuffer, BUFFERSIZE, 0);
		// if(messageLength < 0)
		// 	error("Fail to receive message from client");

		// messageBuffer[messageLength] = '\0';
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
	printf(RED "%s\n", message);
	perror(YEL "C: error");
	exit(1);

	return;
}
