#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <inttypes.h>


void mexit(char* message)
{
    printf("%s", message);
    exit(-1);
}

int cnt = 0;
void do_semop(int semid, int op)
{
    struct sembuf msembuf;

    cnt++;
    msembuf.sem_op = op;
    msembuf.sem_flg = 0;
    msembuf.sem_num = 0;
    if (semop(semid, &msembuf, 1) < 0)
    {
        printf("Can\'t perform semafor %d operation (%d)\n", cnt, op);
        exit(-1);
    }
}

int main()
{
    int fd[2];
    char keyfilename[] = "semafors_pypes.ipc";

    if(pipe(fd) < 0) mexit("Cant create pipe \n");

    key_t key;
    if ((key = ftok(keyfilename, 0)) < 0) mexit("Can\'t generate key\n");

    int semid;
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) mexit("Can\'t get semafor id\n");

    if (semctl(semid, 0, SETVAL, 0) < 0) mexit("Can\'t initialize semafor with \'0\' value \n");

    int result = fork();
    if (result < 0) mexit("Cant fork child\n");
    else if (result > 0)
    {
        while (1)
        {
            do_semop(semid, 0);

            printf("Enter 2 digits: ");
            int buf[2];
            scanf("%d %d", buf, buf + 1);
            printf("\n");

            size_t size;
            if ((size = write(fd[1], buf, sizeof(int) * 2)) != (2 * sizeof(int)))
                mexit("Pipe writing error \n");

            do_semop(semid, -1);

            int answer;
            if ((size = read(fd[0], &answer, sizeof(int))) != sizeof(int))
                mexit("Pipe reading error \n");

            printf("Answer is: %d\n", answer);

            do_semop(semid, 2);
        }
    } else
    {
        while(1)
        {

            int buf[2];
            size_t size;
            if ((size = read(fd[0], buf, 2 * sizeof(int))) != (2 * sizeof(int)))
                mexit("Pipe reading error \n");

            do_semop(semid, 1);

            int answ = buf[0] * buf[1];
            if ((size = write(fd[1], &answ, sizeof(int))) != (sizeof(int)))
               mexit("Pipe writing error \n");

            do_semop(semid, -2);
        }
    }

    return 0;
}
