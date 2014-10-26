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
	int sockfd, port = 9001;
	// The first byte is an opcode indicating status severity.
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
	char msg[MAXLINE];
	// Start the physiology datagram listening daemon
	while (1) {
		len = sizeof(clntaddr);
		n = Recvfrom (sockfd, msg, MAXLINE, 0, (SA *) &clntaddr, &len);
		if (n>0) {
			// To account for individual client identities, get the IP address!
			char ipaddr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET,&(clntaddr.sin_addr),ipaddr,INET_ADDRSTRLEN);
			// strncat a space delimiter and the clean IP addess
			strncat(msg, " ", 1);
			strncat(msg, ipaddr, sizeof(ipaddr));
			// Print for debugging! 
			fprintf(stderr, "%s\n", msg);
			// We'll use a portable 255-character limited filename
			char filename [255] = {0};
			// IMPORTANT !!!!
			
			// below directory should line up with where the 
			// server_script.py file is drawing data from!

			// IMPORTANT !!!!
			snprintf(filename, sizeof(filename), "/home/devinws/Data/%d.ebola", y);
			FILE *fp;
			fp = fopen(filename,"wb");
			fwrite(msg, strlen(msg), 1, fp);
			fclose(fp);
			y++;
		}
		memset(msg,0,sizeof(msg));
		// Let's zero for security! Always sanitize memory before re-use!	
	}
	return 0;
}

/* Everything beyond
 * this point is very trivial error-checking for severe error conditions.
 * Will throw appropriate error notice before terminating..
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

