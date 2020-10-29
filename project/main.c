#include <stddef.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
    int fd1[2], fd2[2];
    if(pipe(fd1) < 0)
    {
        printf("Cant create pipe \n");
        exit(-1);
    }

    if(pipe(fd2) < 0)
    {
        printf("Cant create pipe \n");
        exit(-1);
    }

    int result = fork();
    if (result < 0)
    {
        printf("Cant fork child\n");
        exit(-1);


    } else if (result > 0)
    {
            //we will send via fd1 numbers for calculating

        int nums[2];
        scanf("%d %d", nums, nums + 1);

        size_t size = write(fd1[1], nums, sizeof(int) * 2);
        close(fd1[1]);

        if (size != sizeof(int) * 2)
        {
            printf("Not possible to write all to fd1\n");
            exit(-1);
        }

        int answer;
        size = read(fd2[0], &answer, sizeof(int));
        close(fd2[0]);
        if (size != sizeof(int))
        {
            printf("Not possible to read all from fd2 \n");
            exit(-1);
        }

        printf("%d  my congratulations! \n", answer);





    } else
    {
            // and via fd2 we'll return the answer
        int nums[2];

        size_t size = read(fd1[0], nums, sizeof(int) * 2);
        if (size != sizeof(int) * 2)
        {
            printf("Not possible to read all from fd1 \n");
            exit(-1);
        }

        int answ = nums[0] * nums[1];
        close(fd1[0]);

        size = write(fd2[1], &answ, sizeof(int));
        if (size != sizeof(int))
        {
            printf("Not possible to write all to fd2 \n");
            exit(-1);
        }
        close(fd2[1]);
    }

    return 0;
}
