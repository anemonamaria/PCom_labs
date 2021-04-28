// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char *name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char *name)
{
	int ret;
	struct addrinfo hints, *result, *current;

	memset(&hints, 0, sizeof(hints));

	// TODO: set hints
	hints.ai_flags = AI_PASSIVE | AI_CANONNAME;
	hints.ai_family = AF_UNSPEC;

	// TODO: get addresses
	ret = getaddrinfo(name, NULL, &hints, &result);

	if (ret != 0)
	{
		printf("Error: %s\n", gai_strerror(ret));
		exit(0);
	}

	// TODO: iterate through addresses and print them
	current = result;
	while (current != NULL)
	{
		if (current->ai_family == AF_INET) // IPV4
		{
			struct sockaddr_in *address = (struct sockaddr_in *)(current->ai_addr);
			char buffer[INET_ADDRSTRLEN];
			int result = inet_ntop(AF_INET, &address->sin_addr, buffer, INET_ADDRSTRLEN); // todo
			if (result == NULL)
			{
				printf("Error\n");
				exit(0);
			}
			printf("IP: %s ; Canon name: %s; Protocol: %d; Port:%d\n", buffer, current->ai_canonname, current->ai_protocol, ntohs(address->sin_port));
		}
		else if (current->ai_family == AF_INET6) // IPV6
		{
			struct sockaddr_in6 *address = (struct sockaddr_in6 *)(current->ai_addr);
			char buffer[INET6_ADDRSTRLEN];
			int result = inet_ntop(AF_INET6, &address->sin6_port, buffer, INET6_ADDRSTRLEN); // todo
			if (result == NULL)
			{
				printf("Error\n");
				exit(0);
			}
			printf("IP: %s ; Canon name: %s; Protocol: %d; Port:%d\n", buffer, current->ai_canonname, current->ai_protocol, ntohs(address->sin6_port));
		}
		current = current->ai_next;
	}

	freeaddrinfo(result);
	// TODO: free allocated data
}

// Receives an address and prints the associated name and service
void get_name(char *ip)
{
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

	// TODO: fill in address data
	int result = inet_aton(ip, &addr.sin_addr);
	if (result == -1)
	{
		printf("Error: %s\n", gai_strerror(result));
	}
	addr.sin_family = AF_INET;

	// TODO: get name and service
	ret = getnameinfo((struct sockaddr *)&addr, sizeof(addr), host, INET_ADDRSTRLEN, service, INET_ADDRSTRLEN, 0);
	if (ret != 0)
	{
		printf("Error: %s\n", gai_strerror(ret));
		exit(0);
	}

	// TODO: print name and service
	printf("Host: %s / Service: %s\n", host, service);
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0)
	{
		get_ip(argv[2]);
	}
	else if (strncmp(argv[1], "-a", 2) == 0)
	{
		get_name(argv[2]);
	}
	else
	{
		return usage(argv[0]);
	}

	return 0;
}
