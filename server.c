#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: ./server <port> \n");
        return 0;
    }

    int listenfd = 0, connfd = 0, n = 0;
    char buff[5];
    char name[1025];
    struct sockaddr_in serv_addr;
    int i, j;
    sscanf(argv[1], "%d", &i);

    // Setup
    memset(buff, '0', sizeof(buff));
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(i);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Create socket, bind, and listen
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    // Receive dst.txt name and create file
    connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);

    FILE *output_file;
    while(1){
        n = read(connfd, name, sizeof(name));
        output_file = fopen(name, "w");

        j = 1;
        while (j > 0){
            n = read(connfd, buff, sizeof(buff));
            j = n;
            printf("%d\n", j);
            fwrite(buff, n, 1, output_file);
        }

        close(connfd);
        fclose(output_file);
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
    }

    return 0;
}
