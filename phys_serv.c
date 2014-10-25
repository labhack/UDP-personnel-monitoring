/*
 * phys_serv.c - Physiology Server
 * 
 * Team AFRL LabHack - "Spotty Data Connection for Personnel Monitoring"
 * - C by Val A. Red - GNU GPLv3 license -
 * 
 * This server handles lightweight datagrams and parses it for processing by 
 * other JS scripts.
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

// truncated sockaddr definition and MAXLINE defines:
typedef struct sockaddr SA;
#define MAXLINE  8192

// Error Handling Prototype: 
void u_error(char *msg);
// Safe networking prototypes (avoids buffer overflows, etc.) 
int Socket(int domain, int type, int protocol);
void Bind(int sockfd, struct sockaddr *my_addr, int addrlen);
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                                struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
                                const struct sockaddr *dest_addr, socklen_t addrlen);
// Safe Malloc prototype:
void *Malloc(size_t size);

// Main daemon function. 
int main() {
	int port = 9001;
	// The first byte is an opcode. We can strncpy with an offset of 1.
	struct sockaddr_in srvaddr, clntaddr;
	sockfd = Socket (AF_INET, SOCK_DGRAM, 0);
	// Zero the memory for the server address.
	bzero(&srvaddr, sizeof(srvaddr));
	// Populate the server address socket
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s=addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(port);
	// Finally, bind the UDP Socket!
	Bind(sockfd, (SA* ) &srvaddr, sizeof(srvaddr)); 	
	int n;
	socklen_t len;
	char msg[MAXLINE];
	// Start the physiology datagram listening daemon!
	while (1) {
		len = sizeof(clntaddr);
		uint8_t opcode;
		strncpy(opcode, msg, 1);
		fprintf(stdout, "Hi! Your opcode is!"); 		
		// If not 1, it is a check-in. Treat 1 as an emergency!
		if (opcode != 1) {
			; // TO BE CONTINUED!
		}
		
	}
	return 0;
}

int Socket(int domain, int type, int protocol)
{
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0)
                u_error("Socket error");
    return rc;
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen)
{
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
        u_error("Bind error");
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                                struct sockaddr *src_addr, socklen_t *addrlen) {
        ssize_t rc;
        if ((rc = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0)
                u_error("Recvfrom error");
        return rc;

}
ssize_t Sendto(int sockfd, const void *buf, size_t len, int flags,
                                const struct sockaddr *dest_addr, socklen_t addrlen) {
        ssize_t rc;
        if ((rc = sendto(sockfd, buf, len, flags, dest_addr, addrlen)) < 0)
                u_error("Sendto error");
        return rc;
}

void *Malloc(size_t size)
{
    void *p;

    if ((p  = malloc(size)) == NULL)
        u_error("Malloc error");
    return p;
}
