/*
*  	Protocoale de comunicatii:
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);

	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];
	socklen_t socklength;

	/*Deschidere socket*/
	int s = socket(PF_INET, SOCK_DGRAM, 0);
	if (s < 0) {
		perror("Socket creation failed");
	}

	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_port = htons(atoi(argv[1]));
	my_sockaddr.sin_addr.s_addr = INADDR_ANY;
	// printf("Am setat sockaddr_in\n");

	/* Legare proprietati de socket */
	bind(s, (struct sockaddr*)&my_sockaddr, sizeof(struct sockaddr_in));
	// printf("Am facut bind\n");

	/* Deschidere fisier pentru scriere */
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	// printf("Am deschis fisierul\n");



	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/
	socklength = sizeof(from_station);
	int recvResult;
	// int i = 0;
	memset(buf, 0, BUFLEN);
	while ((recvResult = recvfrom(s, buf, BUFLEN, 0,
								(struct sockaddr *) &from_station,
								&socklength)
						) > 0) {
		// printf("%d Salvez buffer de lungime %d\n", i++, recvResult);
        write(fd, buf, recvResult);
		memset(buf, 0, BUFLEN);
    }
	printf("RecvResult %d \n", recvResult);

	/*Inchidere socket*/
	close(s);

	/*Inchidere fisier*/
    close(fd);

	return 0;
}