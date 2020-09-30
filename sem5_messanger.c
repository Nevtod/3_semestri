#include <stdio.h>
#include <stdlib.h>
 #include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define STRING_SIZE 1000

void m_read(int fileDescriptor, char* data,
            size_t bytes, char*  errMessage);
void m_write(int fileDescriptor, char* data,
             size_t bytes, char*  errMessage);
int m_open(char* name, int mode, char* errMessage);
int get_friend_num(int myNum);


int main()
{
    char* names[] = {"aaa.fifo", "bbb.fifo"};
    int messNum;
    char buf[1001];

    printf("Введите номер мессенджера(1 или 2): \n");
    scanf("%d", &messNum);
    messNum--;

    umask(0);

    int fifoRes = mknod(names[messNum], S_IFIFO | 0666, 0);
    if (errno == EEXIST)
        printf("FIFO %s already exist \n", names[messNum]);
    else if (fifoRes < 0) {
        printf("Can\'t create FIFO %s, maybe it already exist\n", names[messNum]);
    exit(-1);
    }

    // Проектируем мессенджер
    // В родителе будем писать
    // А в ребёнке читать

    int forkRes = -1;
    if ((forkRes = fork()) < 0) {
        printf("Can\'t fork child\n");
        exit(-1);

    } else if (forkRes > 0) {   // Переходим в родителя
        int fd = m_open(names[messNum], O_WRONLY, "Not possible to open for write FIFO file");

        while (1)
        {
            char* res = fgets(buf, STRING_SIZE, stdin);
            if (res != NULL)
                m_write(fd, buf, sizeof (char) * (strlen(buf)),
                        "Not possible to write in FIFO ");
        }
    } else { // Переходим в дочерний процесс для считывания
        int fd = m_open(names[get_friend_num(messNum)],
                O_RDONLY,"Not possible to open for read ");

        while (1)
        {

            m_read(fd, buf, STRING_SIZE, "");
            printf("%s", buf);
        }
    }

    return 0;
}


void m_read(int fileDescriptor, char* data,
            size_t bytes, char*  errMessage)
{
    size_t size = read(fileDescriptor, data, bytes);
    if (size != bytes)
        printf("%s", errMessage);
}

void m_write(int fileDescriptor, char* data,
             size_t bytes, char*  errMessage)
{
    size_t size = write(fileDescriptor, data, bytes);
    if (size != bytes)
        printf("%s \n", errMessage);
}

int m_open(char* name, int mode, char* errMessage)
{
    int fd = open(name, mode);
    if (fd < 0) {
        printf("%s \n", errMessage);
        exit(-1);
    }

    return fd;
}

int get_friend_num(int myNum)
{
    return 1 - myNum;
}
