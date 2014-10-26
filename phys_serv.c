/*
 * phys_serv.c - Physiology Server
 * 
 * Team AFRL LabHack - "Spotty Data Connection for Personnel Monitoring"
 * - Joe Baylor, David McAffee , Benjamin Natarian Val Red, Devin Spatz - GNU GPLv3 license -
 * 
 * This server handles lightweight UDP datagrams and parses it for use by 
 * python and PHP scripts.
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
#include <inttypes.h>
#include <time.h>

// truncated sockaddr definition and MAXLINE defines:
typedef struct sockaddr SA;
#define MAXLINE  8192

// Error Handling Prototype: 
void u_error(char *msg);
// Safe networking prototypes (for error handling, etc.) 
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
	// Our port number is over 9000!
	int sockfd, port = 9001;
	// We're using a basic UDP header structure
	struct sockaddr_in srvaddr, clntaddr;
	sockfd = Socket (AF_INET, SOCK_DGRAM, 0);
	// Zero the memory for the server address.
	bzero(&srvaddr, sizeof(srvaddr));
	// Populate the server address socket
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(port);
	// Finally, bind the UDP Socket!
	Bind(sockfd, (SA* ) &srvaddr, sizeof(srvaddr)); 	
	int n; int y = 1;
	socklen_t len;
	// Maxline is an arbitrary max length. 
	char msg[MAXLINE];
	// Start the physiology datagram listening daemon
	while (1) {
		len = sizeof(clntaddr);
		// We invoke a UDP receive and poll it to check for new dgrams
		n = Recvfrom (sockfd, msg, MAXLINE, 0, (SA *) &clntaddr, &len);
		if (n>0) {
			fprintf(stderr, "%s\n", msg); 
			// for portability, we use a basic 255 filename minimum
			char filename [255] = {0};
			// We use snprintf to assemble a filename string for 
			// what we generate a file for. 
			snprintf(filename, sizeof(filename), "%d.ebola", y);
			FILE *fp;
			fp = fopen(filename,"wb");
			// Write a new file for Python/PHP use based on data. 
			fwrite(msg, sizeof(msg[0]), sizeof(msg)/sizeof(msg[0]), fp);
			fclose(fp);
			y++;
		}
		// zero the message string for safety!
		memset(msg,0,sizeof(msg));
		
	}
	return 0;
}

/* 
ALL THE BELOW FUNCTIONS are for more robust 
error-handling. They are very basic in how they throw 
exceptions for basic errors that would silently be tripped 
in a bad execution. 
*/

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

void u_error(char *msg) /* unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

