#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include "constants.h"

void mexit(char* message)
{
    printf("%s", message);
    exit(-1);
}

int main()
{

    int N;
    printf("Enter N (you will have about N clients): ");
    scanf("%d", &N);
    printf("\n");

    int sockfd[MAX_CLIENT_AMOUNT];
    char sendline[1000], recvline[1000];
    struct sockaddr_in servaddr;

    bzero(recvline, STRING_SIZE);
    bzero(sendline, STRING_SIZE);
    bzero(&servaddr, sizeof (servaddr));


    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    if (inet_aton(SERVER_ADDR, &servaddr.sin_addr) == 0)
    {
        printf("Invalid IP adress\n");
        exit(1);
    }

    int data[1000][2];
    for (int i = 0; i < N; i++)
    {
        data[i][0] = i * 2;
        data[i][1] = i * 2 - 1;

        printf("Numbers for client #%d: %d  %d \n",
               i + 1, data[i][0], data[i][1]);
        if ((sockfd[i] = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("NULL");
            printf("Not able to make client socket #%d \n", i + 1);
            exit(1);
        }
    }


    printf("Start connecting clients to server\n");

    for (int i = 0; i < N; i++)
    {
        if (connect(sockfd[i], (struct sockaddr *) &servaddr,
                    sizeof (servaddr)) < 0)
        {
            printf("Not able to connect client #%d \n", i + 1);
            perror(NULL);
            close(sockfd[i]);
            exit(1);
        }
    }


    printf("Start of calculations\n");
    for (int i = 0; i < N; i++)
    {
        int iores;
        sprintf(sendline, "%d %d", data[i][0], data[i][1]);
        if ((iores = write(sockfd[i], sendline, strlen(sendline) + 1)) < 0)
        {
            perror("Cant write\n");
            close(sockfd[i]);
            exit(1);
        }

        int answer;
        if ((iores = read(sockfd[i], &answer, sizeof (int)) < 0))
        {
            perror("Cant read\n");
            close(sockfd[i]);
            exit(1);
        }

        printf("Multiplication is: %d\n", answer);
        close(sockfd[i]);
    }
}
