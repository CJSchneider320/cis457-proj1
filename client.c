#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server; 
/*The hostent structure is used by functions to store information about a given host, such as host name, IPv4 address, and so forth*/
    char buffer[1001];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    /* gethostname() system call returns a null-terminated hostname*/

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    printf("Welcome to console messenger! You are on port %s. Type \'Quit!\' to exit.\n", argv[2]);

    while(strcmp(buffer, "Quit!") != 0) {

        //Phase 1: Client sends message to server, server recieves
        //message, and returns 'sent' to the client
        printf("Send: ");
        bzero(buffer, 1001);
        fgets(buffer, 1000, stdin);

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");

        bzero(buffer, 1001);

        n = read(sockfd, buffer, 1000);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("%s\n\n",buffer);

        bzero(buffer, 1001);

        //Phase 2: Server sends message to client, client recieves
        //message, and returns 'sent' to the server

        n = read(sockfd, buffer, 1000);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Server: %s\n",buffer);

        bzero(buffer, 1001);

        n = write(sockfd, "Sent", 18);
        if (n < 0) 
            error("ERROR writing to socket");

        bzero(buffer, 1001);

        

    }

    return 0;
}