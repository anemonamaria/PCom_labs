#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char *argv[])
{
	msg t;
	int w, i, res, len = 0, j;

	w = (atoi(argv[1])*1000) / (sizeof(msg)*8);
	msg* win = malloc(w * sizeof(msg));
	printf("[SENDER] Starting.\n");

	init(HOST, PORT);

	printf("[SENDER]: BDP=%d\n", atoi(argv[1]));

	for (i = 0; i <= COUNT; i++) {
		/* cleanup msg */
		memset(&t, 0, sizeof(msg));

		/* gonna send an empty msg */
		t.len = MSGSIZE;
		/* send msg */
		sprintf(t.payload, "Package #%d", i+1);
		if(len < w) {
			strcpy(win[len].payload, t.payload);
			len++;

			printf("[SENDER] Sent: %s\n", t.payload);
			res = send_message(win);
			if (res < 0) {
				perror("[SENDER] Send error. Exiting.\n");
				return -1;
			}
			continue;
		}

		/* wait for ACK */
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
		printf("[SENDER] Received back: %s\n", t.payload);
		for(j = 0; j < w; j++)
			strcpy(win[j].payload, win[j+1].payload);
		len = len - 1;
	}
	printf("[SENDER] Job done, all sent.\n");

	free(win);
	return 0;
}