#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef int(*comparator)(void*, void*);

void mswap(void* a, void* b, size_t size)
{
    char* tmp = malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

void msort(void* begin, void* end, size_t size, comparator cmp)
{
    void* ptr = begin;
    void* secPtr = end;
    do
    {
        while (ptr < secPtr)
        {
            if (cmp(ptr, ptr + size) < 0)
            {
                printf("%d %d\n", *((int*)ptr), *((int*)(ptr + size)));
                mswap(ptr, ptr + size, size);
            }
            ptr += size;
        }
        secPtr -= size;
        ptr = begin;
    } while (secPtr > begin);
}

int intCmp(int *a, int *b)
{
    return *b - *a;
}

/*
int main()
{
    int arr[7] = {5, 6, 3, 2, 7, 1, 0};
    msort(arr, arr + 6, sizeof(int), (comparator)intCmp);
    for (int i = 0; i < 7; i++)
        printf("%d ", arr[i]);
}*/
