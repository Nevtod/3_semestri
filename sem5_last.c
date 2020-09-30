#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE 10000

long **matrxA, **matrxB, **matrx;
int linesNum[MAX_SIZE];

int n;

void* mthread(void* thereSometimesIsArgument);

int main()
{
    //initialization
    printf("Please fill in the N constant\n");
    scanf("%d", &n);
    
    matrxA = (long**)malloc(sizeof(long*) * n);
    matrxB = (long**)malloc(sizeof(long*) * n);
    matrx = (long**)malloc(sizeof(long*) * n);
    for (int i = 0; i < n; i++)
    {
        matrxA[i] = (long*)malloc(sizeof(long) * n);
        matrxB[i] = (long*)malloc(sizeof(long) * n);
        matrx[i] = (long*)malloc(sizeof(long) * n);
    }
    
    //reading values
    
    long cnt = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrxA[i][j] = matrxB[i][j] = cnt++;
    
    
    //Threads will work out with one own line of result matix
    int threadMakingRes;
    pthread_t *threadIds = (pthread_t*)malloc(sizeof(pthread_t) * n);
    for (int i = 0; i < n; i++)
    {    
        linesNum[i] = i;
        if((threadMakingRes = pthread_create(threadIds + i,
                (pthread_attr_t *)NULL,
                mthread,
                (void*)(linesNum + i))) != 0)
        {
            printf ("Error on thread create, return value = %d\n", threadMakingRes);
            exit(-1);
        }
    }       
            
    
    for (int i = 0; i < n; i++)
        pthread_join(threadIds[i], (void**) NULL);
            
    
    //some check
    for (int i = 0; i < n; i++)
    {
        printf("\n");
        for (int j = 0; j < n; j++)
            printf("%ld ", matrx[i][j]);
    }
    
    // free
    for (int i = 0; i < n; i++)
    {
        free(matrxA[i]);
        free(matrxB[i]);
    }
    free(matrxA);
    free(matrxB);
    free(threadIds);
    
    
    return 0;
}


void* mthread(void* lineNum)
{
    pthread_t threadId = pthread_self();
    printf("%d\n", *((int*)lineNum));
    
    for (int k = 0; k < n; k++)
    {
        matrx[*((int*)lineNum)][k] = 0;
        for (int j = 0; j < n; j++)
        {
            matrx[*((int*)lineNum)][k] += matrxA[*((int*)lineNum)][j] 
                                            * matrxB[k][j];
        }                                    //We multiply matrices like as matrxB was
                                            //transposed before
        
    }
    
    printf("Thread #%lu ended calculations \n", threadId);
    
    return NULL;
}







