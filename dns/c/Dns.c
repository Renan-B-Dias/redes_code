#include <stdio.h>	// printf // socket // perror()
#include <stdlib.h>	// exit()
#include <string.h> // memset()
#include <sys/socket.h>	// socket() // pf_inet // sock_stream
#include <netinet/in.h>     /* struct sockaddr_in */
#include <arpa/inet.h>	// IPPROTO_TCP // struct sockaddr
#include <unistd.h>	// close()
#include <netdb.h>
#include <stdbool.h>

#define BUFFSIZE 32
#define SA struct sockaddr
#define PORT 53

void error(char *msg);

int main(int argc, char *argv[]) {

	if(argc < 2) {
		error("Missing params: <domain> <ip?>\n");
	}

	char *domain = argv[1];
	char *ip = "127.0.0.1\0";

	if(argc > 2) {
		ip = argv[2];
	}

	struct sockaddr_in serverStruct;
	serverStruct.sin_family = AF_INET;
	if(!inet_aton(ip, &(serverStruct.sin_addr)))
		error("Invalid server IP");

	serverStruct.sin_port = htons(PORT);

	int length = strlen(domain);
	if(domain[length - 1] == '.')
		domain[length - 1] = '\0';

	printf("%s\n", domain);
	printf("%s\n", ip);

	struct hostent *host;
	struct in_addr hostAddress;

	host = gethostbyname(domain);
	if(host == NULL)
		error("getHostByName failed");

	hostAddress.s_addr = *((unsigned long*) host->h_addr_list[0]);
	char *ipAddress = inet_ntoa(hostAddress);
	printf("%s\n", ipAddress);

	// for(int i = 0; i < 3; i++) {

		int socketReturn = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(socketReturn < 0)
			error("Failed on socket connection");

		// Set time of socket timeout
		struct timeval timeValue;
		timeValue.tv_sec = 5;
		setsockopt(socketReturn, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *) &timeValue,sizeof(struct timeval));

		struct sockaddr_in serverAddress;
		memset(&serverAddress, 0, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = inet_addr(ipAddress);
		serverAddress.sin_port = htons(atoi("80"));

		int ret = connect(socketReturn, (SA *) &serverAddress, sizeof(serverAddress));
		if(ret < 0)
			error("Failed to connect");

		unsigned int messageLength = strlen(domain);
		int sendReturn = send(socketReturn, domain, messageLength, 0);
		if(sendReturn != messageLength)
			error("Failed to send");

		printf("Received: ");

		int received = 0;
		int bytes = 0;
		char messageBuffer[BUFFSIZE];
		while(true) {
			if((bytes = recv(socketReturn, messageBuffer, BUFFSIZE-1, 0)) < 1) {
				printf(bytes == 0 ? "" : "<none>");
				break;
			}

			received += bytes;
			messageBuffer[bytes] = '\0';
			printf("%s\n", messageBuffer);
		}

		printf("\n");

		close(socketReturn);

	// }
}

void error(char *msg) {
  printf("%s\n", msg);
  perror("C error: ");
  exit(0);

  return;
}