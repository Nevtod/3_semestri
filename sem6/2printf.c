#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
 #include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int cnt;
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
    
    // Реализуем в данной программе обе
    cnt = 0;
    int fd; //file descriptor
    int progId; // first or second prog
    
    printf("Введите номер программы (1 или 2): ");
    scanf("%d", &progId);
    
    char keyfileName[] = "2printfIPC";   // filename for semafors key
    char logsfile[] = "2printf.log"; // logs file
    int semid;   //semafor id
    key_t key;  // exactly mentioned IPC key
    
    if ((key = ftok(keyfileName, 0)) < 0)
    {
        printf("Not possible to generate key\n");
        exit(-1);
    }
    
    if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        printf("where am i\n");
        
        if (errno != EEXIST)
        {
            printf("Can\'t get semid\n");
            exit(-1);
        } else if ((semid = semget(key, 1, 0666)) < 0)
        {
            printf("Can\'t get id of existed semafor\n");
            exit(-1);
        } else if (semctl(semid, 0, SETVAL, 1) < 0)
        {
            printf("Can\'t change existed semafor\n");
            exit(-1);
        }
    } else do_semop(semid, 1);
    
        
    if((fd = open(logsfile, O_WRONLY | O_CREAT, 0666)) < 0)
    {
        printf("Can\'t open logs file\n");
        exit(-1);
    }
        
    while (progId == 1)
    {
        do_semop(semid, -1);
        dprintf(fd, "aaaaaaaaaaaa\n");
        do_semop(semid, 1);
    }
    
    while (progId == 2)
    {
        do_semop(semid, -1);
        dprintf(fd, "bbbbbbbbbbbb\n");
        do_semop(semid, 1);
    }
    if (close(fd) < 0) printf("Cant close file");
        
    return 0;
}
