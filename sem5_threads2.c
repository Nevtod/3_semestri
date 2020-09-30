/*
Программа для иллюстрации работы двух нитей исполнения
Каждая нить исполнения просто увеличивает на 1 разделяемую переменную a.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
 #include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define N 1e7


long a[3] = {0, };
int cnt = 0;

void *mthread(void *dummy)
{
    pthread_t mthid; /* Для идентификатора нити исполнения */
    mthid = pthread_self();

    long i = 0;
    while (i < N)
    {
        i++;
        a[0]++;
        a[2]++;
    }

    printf("Thread %lu, Calculation result = [%ld, %ld]\n", mthid, a[0], a[2]);

    return NULL;
}


int main()
{
    pthread_t thid, mythid, thid2;
    int result;


    result = pthread_create( &thid, (pthread_attr_t *)NULL, mthread, NULL);
    if(result != 0)
    {
        printf ("Error on thread create, return value = %d\n", result);
        exit(-1);
    }

    result = pthread_create( &thid2, (pthread_attr_t *)NULL, mthread, NULL);
    if(result != 0){
        printf ("Error on thread create, return value = %d\n", result);
        exit(-1);
    }

    printf("Threads created, thread1 id = %lu, thread2 id = %lu \n", thid, thid2);
    mythid = pthread_self();


pthread_join(thid, (void **)NULL);
pthread_join(thid2, (void **)NULL);

printf("Common result: a[0], a[1], a[0] + a[1], a[2]: \n %ld, %ld, %ld, %ld \n",
       a[0], a[1], a[0] + a[1], a[2]);

return 0;
}
