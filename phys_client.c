/*
 * phys_client.c - Physiology Client 
 * 
 * Team AFRL LabHack - "Spotty Data Connection for Personnel Monitoring"
 * - Joe Baylor, David McAffee , Benjamin Natarian Val Red, Devin Spatzd - GNU GPLv3 license -
 * 
 * This server writes lightweight UDP datagrams and based on simulated  
 * sensor data of a human's physiological readings and sends it over Internet 
 * to a centralized server location. 
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
#include <time.h>
#include <unistd.h>

typedef struct sockaddr SA;
#define MAXLINE  8192

// Error Handling Prototype: 
void u_error(char *msg);
// Safe networking prototypes (for error handling, avoids buffer overflows, etc.) 
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
	/* We first need to read the device data. Typically an wearable device API would be 
	used, but for this implementation we will be parsing raw data string sample inputs as
	binaries translating to our 32-byte protocol. */ 
	socklen_t len; int sockfd, port = 9001;
	// UDP Port 9001 will be used for physiology communications; it's over 9000.
	struct sockaddr_in srvaddr;
	// Here we are forming a structure with all server address information.
	sockfd = Socket (AF_INET, SOCK_DGRAM, 0);
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(port);
	// 104.131.67.199 is our scenario "Ebola command server"
	inet_pton(AF_INET, "104.131.67.199", &(srvaddr.sin_addr));
	len = sizeof(srvaddr);
	// Below we assemble the UDP datagram to send
	FILE *fp; char *msg;
       	char * line = NULL;
       	size_t llen = 0;
       	ssize_t linesize;
	// ebola.str is the sample sensor data set we will distribute.
	// this should be replaced by sensor API data...
       	fp = fopen("ebola.str", "r");
       while ((linesize = getline(&line, &llen, fp)) != -1) {
           printf("Retrieved line of length %zu :\n", linesize);
	   printf("Message: \n %s \n", line);
	   /* Here we would try to condense it even further, but it is rough.*/ 
	   // unsigned int stime; double lat; double lon; double alt; 
	   // int ecg; int o2; int resp; unsigned int temp;  
	   // stime = strtok_r(line, " ", &saveptr);
	   // lat = strtok_r(NULL, " ", &saveptr);
	   // you'd only use and continue the above implementation if you 
	   // wanted it down to 32-bytes. 
	   msg = Malloc(linesize);
	   memcpy(msg, line, linesize);
	   // We treat each line in ebola.str as a new sensory update
	   Sendto(sockfd, msg, linesize, 0, (SA *) &srvaddr, len);
	   // Don't forget to free your allocs!
	   free(msg);
       }
       fclose(fp);
	return 0;
}

/* All the following are simplified, error-handling functions 
 incorporated for robustness. They simply throw errors that will 
 gracefully terminate the program. */

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

