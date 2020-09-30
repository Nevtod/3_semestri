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
long a[3] {0, };

void *mthread(void *dummy)
{
    pthread_t mthid; /* Для идентификатора нити исполнения */


/*
Заметим, что переменная mythidявляется динамической локальной переменной функции
mythread(), т. е. помещается в стеке и, следовательно, не разделяется нитями исполнения.
*/

/*
Запрашиваем идентификатор thread'а
*/

    mthid = pthread_self();

    long i = 0;
    while (i < N)
    {
        i++;
        a[0]++;
        a[2]++;
    }

    printf("Thread %d, Calculation result = [%l, %l]\n", mythid, a[0], a[2]);

    return NULL;
}


int main()
{
    pthread_t thid, mythid, thid2;
    int result;

/*
Пытаемся создать новую нить исполнения, ассоциированную с функцией mythread().
Передаем ей в качестве параметра значение NULL.
В случае удачи в переменную thid занесется идентификатор нового thread'а. Если возникнет ошибка - прекратим работу.
*/

    result = pthread_create( &thid, (pthread_attr_t *)NULL, mythread, NULL);
    if(result != 0)
    {
        printf ("Error on thread create, return value = %d\n", result);
        exit(-1);
    }

    result = pthread_create( &thid2, (pthread_attr_t *)NULL, mythread, NULL);
    if(result != 0){
        printf ("Error on thread create, return value = %d\n", result);
        exit(-1);
    }

    printf("Threads created, thread1 id = %d, thread2 id = %d \n", thid, thid2);

/*
Запрашиваем идентификатор главного thread'а
*/

    mythid = pthread_self();
/*
Ожидаем завершения порожденного thread'a, не интересуясь какое значение он нам вернет.
Если не выполнить вызов этой функции, то возможна ситуация, когда мы завершим функцию main()
до того, как выполнится порожденный thread, что автоматически повлечет его завершение,
исказив результаты.
*/

pthread_join(thid, (void **)NULL);
pthread_join(thid2, (void **)NULL);

printf("Common result:
return 0;
}
