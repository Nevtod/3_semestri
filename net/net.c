#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "constants.h"

void* handler(void* _cliaddr);


int main()
{
    int sockfd;

    unsigned clilen;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Not able to make server socket\n");
        perror(NULL);
        exit(1);
    }
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_ADDR, &servaddr.sin_addr);

    char *maddr = inet_ntoa(servaddr.sin_addr);
    printf("server adress: %s", maddr);
    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 10) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    unsigned cnt = 0;
    int newsockfd[MAX_CLIENT_AMOUNT];
    while(1)
    {
        clilen = sizeof(cliaddr);

        if ((newsockfd[cnt] = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
        {
            printf("Cant accept connection");
            perror(NULL);
            close(sockfd);
            exit(1);
        }

        pthread_t threadID;
            int result = pthread_create( &threadID,
                                         (pthread_attr_t *)NULL,
                                         handler, (void*)(newsockfd + cnt));
            cnt++;
            if(result != 0)
            {
                printf ("Error on thread create, return value = %d\n", result);
                exit(-1);
            } else
            {
                printf("Started thread #%u \n", cnt);
            }
    }
}



void* handler(void* _sockfd)
{
    char buf[1001];
    int sockfd = *((int*)_sockfd);
    int res;
    if ((res = read(sockfd, buf, 1000)) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    int a, b;
    sscanf(buf, "%d %d", &a, &b);
    a = a * b;
    if ((res = write(sockfd, &a, sizeof(int))) < 0)
    {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    close(sockfd);
    printf("thread terminated, result = %d \n", a);
    return NULL;
}
