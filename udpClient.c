#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct student {
    char name[20]; 
    int rollNo;
    char div[2];  
    int age;
};

int main() {
    int sockfd;
    struct sockaddr_in thatAddr;
    int studRollNo;
    int dataSent;
    struct student receivedStudent;
    socklen_t thatAddrLen = sizeof(thatAddr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Cannot create socket");
        return 1;
    }

    thatAddr.sin_family = AF_INET;
    thatAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP
    thatAddr.sin_port = htons(50000);                   // Port number byte order

    printf("\nEnter roll number to be sent to server: ");
    scanf("%d", &studRollNo);

    if ((dataSent = sendto(sockfd, &studRollNo, sizeof(studRollNo), 0, (struct sockaddr *)&thatAddr, sizeof(thatAddr))) < 0) {
        perror("Cannot send");
        close(sockfd);
        return 1;
    }

    // Receive response from server
    if (recvfrom(sockfd, &receivedStudent, sizeof(receivedStudent), 0, (struct sockaddr *)&thatAddr, &thatAddrLen) < 0) {
        perror("Cannot receive response");
        close(sockfd);
        return 1;
    }

    printf("Response from server:\n");
    printf("Name: %s\n", receivedStudent.name);
    printf("Roll Number: %d\n", receivedStudent.rollNo);
    printf("Division: %s\n", receivedStudent.div);
    printf("Age: %d\n", receivedStudent.age);

    close(sockfd);

    return 0;
}

