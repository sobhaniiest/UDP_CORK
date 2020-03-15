#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/udp.h>
#include <netinet/in.h>

#define MAX_SIZE 100
#define PORT     2025

char buffer[MAX_SIZE];
char *message = "Hii from client\n";

int main (int argc, char const *argv[])
{
	int n, i, length,
		sockfd,
		opt_status;

	size_t optval;
	socklen_t optlen;
	
	struct sockaddr_in server_address;

	memset (&server_address, 0, sizeof (server_address)); 
	 
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr ("127.0.0.1");
	server_address.sin_port = htons (PORT);

	sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror ("Can not create socket");
		exit (0);
	}
	
	optval = 1;
	optlen = sizeof (optval);

	opt_status = setsockopt (sockfd, IPPROTO_IP, UDP_CORK, &optval, optlen);
	if (opt_status < 0)
	{
		perror ("setsockopt is not working ");
		exit (0);
	}

	opt_status = getsockopt (sockfd, IPPROTO_IP, UDP_CORK, &optval, &optlen);
	if (opt_status < 0)
	{
		perror ("getsockopt is not working ");
		exit (0);
	}

	printf ("optval : %ld\n", optval);

	length = sizeof (server_address);
	i = 0;

	while (1)
	{
		sendto (sockfd, (const char*)message, strlen(message), MSG_CONFIRM, (const struct sockaddr*)&server_address, length);
		n = recvfrom (sockfd, (char*)buffer, MAX_SIZE, MSG_WAITALL, (struct sockaddr*)&server_address, &length);
		buffer[n] = '\0';
		printf ("%d %s\n", i++, buffer);
	}

	return 0;
}