#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];

	if (argc < 3)
	{
		usage(argv[0]);
	}

	fd_set read_set;
	fd_set read_set_copy;

	int numfd;

	FD_ZERO(&read_set);
	FD_ZERO(&read_set_copy);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	FD_SET(sockfd, &read_set);
	FD_SET(STDIN_FILENO, &read_set);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	ret = inet_aton(argv[1], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	numfd = sockfd > STDIN_FILENO ? sockfd : STDIN_FILENO;

	while (1)
	{
		read_set_copy = read_set;

		ret = select(numfd + 1, &read_set_copy, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &read_set_copy))
		{
			memset(buffer, 0, BUFLEN);
			recv(sockfd,buffer,BUFLEN,0);
		}

		// citesc de la tastatura
		if (FD_ISSET(STDIN_FILENO, &read_set_copy))
		{
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

			if (strncmp(buffer, "exit", 4) == 0)
			{
				break;
			}

			// se trimite mesaj la server
			n = send(sockfd, buffer, strlen(buffer), 0);
			DIE(n < 0, "send");
		}
	}

	close(sockfd);

	return 0;
}
