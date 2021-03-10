#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"
#include <assert.h>

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc,char** argv){
    init(HOST,PORT);
    msg t;

    int readFile = open("Input.txt", O_RDONLY), bytes;

    assert(readFile < 0 && "Can't open file!");
    lseek(readFile, 0, SEEK_SET);

    while ((bytes = read(readFile, t.payload, sizeof(t.payload)))) {
        assert(bytes < 0 && "Read error!");

        t.len = bytes;
        send_message(&t);

        while (recv_message(&t) < 0);
        
        if (!strcmp(t.payload, "EOT"))
            break;
    }

    t.len = 3;
    strcpy(t.payload, "EOF");
    send_message(&t);
    assert(close(readFile) == -1 && "Can't close file!");
    return 0;
}
