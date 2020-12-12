#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int check(char *str)
{
    int z1 = 0, z2 = 0;

    while(*str)
    {
        if (*str == 'a') z1 = 1;
        else z2 = 1;
        str++;
    }

    return z1 & z2;
}

int main()
{
     char logsfile[] = "2printf.log"; // logs file


    char str[1000];
    FILE* fin = fopen(logsfile, "r");
    while (fscanf(fin, "%s", str) > 0)
    {
        if (check(str))
        {
            printf("ERRRRRROOOOOOORRRRR\n");
            return 0;
        }
    }
    printf("BOOOOORING");
}
