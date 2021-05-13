// client.c: Sets up, opens, prepares, and finally creates a socket to connect
//           to the server. Once the socket is ready, input file is read into
//           the socket and sent to the server. To begin the file transfer, it
//           sends the name of the input file to the server to create the file.
//           Afterwards it opens the input file and reads into the socket until
//           EOF is reached. Finalizes by closing both the socket and the input
//           file.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    // Verify the correct number of arguments
    if (argc != 5){
        printf("Usage: ./client [IP] [port] [SRC_FILE] [DST_FILE]\n");
        return -1;
    }

    int sockfd = 0, n = 0;
    int port;
    char buff[10];
    struct sockaddr_in serv_addr;
    sscanf(argv[2], "%d", &port);

    // Open read-only input file
    FILE *input_file = fopen(argv[3], "r");
    if (input_file == NULL){
        printf("Error: Could not open source file!\n");
        return -1;
    }

    // Setup
    memset(buff, '0', sizeof(buff));
    memset(&serv_addr, '0', sizeof(serv_addr));

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error: Could not create socket!\n");
        return -1;
    }

    // Set address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0){
        printf("Error: inet_pton error has occurred!\n");
        return -1;
    }

    // Connect
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("Error: Connection failed!\n");
        return -1;
    }

    // Send dst.txt name
    write(sockfd, argv[4], strlen(argv[4]) + 1);

    // Take input from src.txt, send to server
    while (feof(input_file) == 0){
        sleep(0.1);
        n = fread(buff, 1 ,sizeof(buff), input_file);
        write(sockfd, buff, n);
    }

    fclose(input_file);
    close(sockfd);
    return 0;
}
