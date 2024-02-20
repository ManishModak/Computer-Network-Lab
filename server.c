// server.c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int sockfd, newsockfd; /* Socket descriptors */
    int clilen;
    struct sockaddr_in cli_addr, serv_addr;

    int i;
    char buf[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available interface
    serv_addr.sin_port = htons(6000);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Unable to bind local address");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);
    printf("\nWaiting for client connection...\n");

    while (1) {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0) {
            perror("Accept error");
            exit(EXIT_FAILURE);
        }

        if (fork() == 0) {
            close(sockfd);

            strcpy(buf, "Message from server :: CONNECTION ACCEPTED: Hello from server");
            send(newsockfd, buf, strlen(buf) + 1, 0);

            for (i = 0; i < 100; i++)
                buf[i] = '\0';
            recv(newsockfd, buf, 100, 0);
            printf("Client %d sent data: %s\n", newsockfd, buf);

            close(newsockfd);
            exit(EXIT_SUCCESS);
        }

        close(newsockfd);
    }

    return 0;
}

