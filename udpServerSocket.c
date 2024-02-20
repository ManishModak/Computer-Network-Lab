#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

struct student {
    char name[20];
    int rollNo;
    char div[2];
    int age;
};

void createDatabase(struct student* data) { 
    strcpy(data[0].name, "Manish");
    data[0].rollNo = 1;
    strcpy(data[0].div, "A");
    data[0].age = 20;

    strcpy(data[1].name, "Alice");
    data[1].rollNo = 2;
    strcpy(data[1].div, "B");
    data[1].age = 21;

    strcpy(data[2].name, "Bob");
    data[2].rollNo = 3;
    strcpy(data[2].div, "C");
    data[2].age = 22;
}


int main() {
    struct student data[20];
    createDatabase(data);
    
    int sockfd;
    struct sockaddr_in thisAddr, thatAddr;
    int dataBytes, thatAddrLen;
    int studRollNo;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Cannot create socket");
        return 1;
    }

    thisAddr.sin_family = AF_INET;
    thisAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    thisAddr.sin_port = htons(50000);

    if (bind(sockfd, (struct sockaddr *)&thisAddr, sizeof(thisAddr)) < 0) {
        perror("Cannot bind");
        return 1;
    }

    thatAddrLen = sizeof(thatAddr);

    if ((dataBytes = recvfrom(sockfd, &studRollNo, sizeof(studRollNo), 0, (struct sockaddr *)&thatAddr, &thatAddrLen)) < 0) {
        perror("Cannot receive");
        close(sockfd);
        return 1;
    }

    printf("Roll number received from client: %d\n", studRollNo);

    // Search for student info based on the received roll number
    struct student* foundStudent = NULL;
    for (int i = 0; i < 3; ++i) {
        if (data[i].rollNo == studRollNo) {
            foundStudent = &data[i];
            break;
        }
    }

    // Send student information back to client
    if (foundStudent != NULL) {
    if (sendto(sockfd, foundStudent, sizeof(struct student), 0, (struct sockaddr *)&thatAddr, thatAddrLen) < 0) {
        perror("Cannot send response");
    }
}

    close(sockfd);

    return 0;
}

