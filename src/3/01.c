#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define VEC_SIZE 1024
#define ITERATIONS 10000

int kr_binsearch(const int x, const int v[], const int n)
{
    int low = 0;
    int high = n - 1;

    while(low <= high)
    {
        const int mid = (low + high) >> 1;
        if(v[mid] > x)
        {
            high = mid - 1;
        } 
        else if (v[mid] < x)
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }

    return -1;
}

int my_binsearch(const int x, const int v[], const int n)
{
    int low = 0;
    int high = n - 1;
    int mid;

    while(low <= high)
    {
        mid = (low + high) >> 1;

        if(v[mid] > x)
        {
            high = mid - 1;
        }
        else 
        {
            low = mid + 1;
        }
    }


    if(high >= 0 && v[high] == x) return mid;

    return -1;
}

int comp_int(const void *a, const void *b)
{
    const int *this = a;
    const int *that = b;
    return *this - *that;
}

void init_vec(int v[], const int n)
{
    for(int i = 0; i < n; ++i)
    {
        v[i] = rand();
    }

    qsort(v, n, sizeof(int), comp_int);
}


int main(void)
{
    srand(42);
    
    int vec[VEC_SIZE];
    init_vec(vec, VEC_SIZE);    

    const clock_t kr_start = clock();
    for(size_t i = 0; i < ITERATIONS; ++i)
    {
        kr_binsearch(42, vec, sizeof(vec)/sizeof(vec[0]));
    }
    printf("kr: %f\n", (double) (clock() - kr_start) / CLOCKS_PER_SEC);
    
    const clock_t my_start = clock();
    for(size_t i = 0; i < ITERATIONS; ++i)
    {
        my_binsearch(42, vec, sizeof(vec)/sizeof(vec[0]));
    }
    printf("my: %f\n", (double) (clock() - my_start) / CLOCKS_PER_SEC);


    return 0;
}

