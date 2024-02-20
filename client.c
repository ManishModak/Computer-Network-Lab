// client.c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
    int sockfd;
    struct sockaddr_in serv_addr;

    int i;
    char buf[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(6000);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Unable to connect to server");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < 100; i++)
        buf[i] = '\0';
    recv(sockfd, buf, 100, 0);
    printf("Server response: %s\n", buf);

    printf("Enter data to be sent to server: ");
    fgets(buf, sizeof(buf), stdin);
    send(sockfd, buf, strlen(buf) + 1, 0);

    close(sockfd);
    return 0;
}

