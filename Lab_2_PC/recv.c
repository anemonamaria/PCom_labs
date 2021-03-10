#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001
    
int main(int argc, char** argv) {
    msg r;
    init(HOST,PORT);

    int writeFile = open("Output.txt", O_WRONLY);

    assert(writeFile < 0 && "Can't open file!");

    while (recv_message(&r) >= 0) {
        if (!strcmp(r.payload, "EOF")) {
            r.len = 3;
            strcpy(r.payload, "EOT");
            send_message(&r);
            break;
        }

        write(writeFile, r.payload, r.len);
        sprintf(r.payload, "%s", "ACK");
        r.len = strlen(r.payload) + 1;
        send_message(&r);
    }

    assert(close(writeFile) == -1 && "Can't close file!");
    return 0;
}
