// client.c: Sets up, opens, prepares, and finally creates a socket to connect
//           to the server. Once the socket is ready, input file is read into
//           the socket and sent to the server. To begin the file transfer, it
//           sends the name of the input file to the server to create the file.
//           Afterwards it opens the input file and reads into the socket until
//           EOF is reached. Finalizes by closing both the socket and the input
//           file.

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
    // Verify the correct number of arguments
    if (argc != 5){
        printf("Usage: ./client <port> <IP> src.txt dst.txt\n");
        return 0;
    }

    int sockfd = 0, n = 0;
    int port;
    char buff[10];
    struct sockaddr_in serv_addr;
    sscanf(argv[1], "%d", &port);

    // Open read-only input file
    FILE *input_file = fopen(argv[3], "r");
    if (input_file == NULL){
        printf("Error: Could not open source file!\n");
        return 0;
    }

    // Setup
    memset(buff, '0', sizeof(buff));
    memset(&serv_addr, '0', sizeof(serv_addr));

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error: Could not create socket!\n");
        return 0;
    }

    // Set address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) <= 0){
        printf("Error: inet_pton error has occurred!\n");
        return 0;
    }

    // Connect
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("Error: Connection failed!\n");
        return 0;
    }

    // Send dst.txt name
    write(sockfd, argv[4], strlen(argv[4]) + 1);

    // Take input from src.txt, send to server
    while (feof(input_file) == 0){
        n = fread(buff, 1 ,sizeof(buff), input_file);
        printf("%s\n", buff);
        write(sockfd, buff, n);
    }

    fclose(input_file);
    close(sockfd);
    return 0;
}
