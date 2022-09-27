/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[1001];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr)); 
	/* erases the data in the n bytes of the memory*/

     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
	/* Host to net byte order for short int*/

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd,5);
	/* 5 outstanding connections*/

     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

     if (newsockfd < 0) 
          error("ERROR on accept");

    bzero(buffer,1001);

    printf("Welcome to console messenger! You are on port %s. Type \'Quit!\' to exit.\n", argv[1]);

    while(strcmp(buffer, "Quit!") != 0) {

        //Phase 1: Client sends message to server, server recieves
        //message, and returns 'sent' to the client

        n = read(newsockfd, buffer, 1000);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Client: %s\n",buffer);

        bzero(buffer,1001);

        n = write(newsockfd, "Sent", 18);
        if (n < 0)
            error("ERROR writing to socket");

        bzero(buffer,1001);

        //Phase 2: Server sends message to client, client recieves
        //message, and returns 'sent' to the server

        printf("Send: ");
        bzero(buffer, 1001);
        fgets(buffer, 1000, stdin);

        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) 
            error("ERROR writing to socket");

        bzero(buffer,1001);

        n = read(newsockfd, buffer, 1000);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("%s\n\n",buffer);

        bzero(buffer,1001);
    }

     return 0; 
}