#include <unistd.h> /* pentru open(), exit() */
#include <fcntl.h> /* O_RDWR */
#include <errno.h> /* perror() */
#include <stdio.h>
#include <stdlib.h>

void fatal(char * mesaj_eroare)
{
    perror(mesaj_eroare);
    exit(0);
}

int main(){
    int fisier;
    int copiat;
	char buf[1024];

	fisier = open("fisier-cat", O_RDONLY);
	if(fisier < 0)
    	fatal("Nu pot deschide fisierul");

	lseek(fisier, 0, SEEK_SET);

   	while ((copiat = read(fisier, buf, sizeof(buf)))) {
       	if (copiat < 0)
            fatal("Eroare la citire");
        	write(STDOUT_FILENO, buf, copiat);
        if (copiat < 0)
            fatal("Eroare la scriere");
    }
    close(fisier);
    return 0;
}