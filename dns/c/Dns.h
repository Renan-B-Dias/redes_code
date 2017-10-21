#include <stdio.h>	// printf // socket // perror()
#include <stdlib.h>	// exit()
#include <string.h> // memset()
#include <sys/socket.h>	// socket() // pf_inet // sock_stream
#include <netinet/in.h>     /* struct sockaddr_in */
#include <arpa/inet.h>	// IPPROTO_TCP // struct sockaddr
#include <unistd.h>	// close()
#include <netdb.h>
#include <stdbool.h>

#include "Error.h"

#define BUFFSIZE 32
#define SA struct sockaddr
#define PORT 53