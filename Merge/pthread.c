//In this assignment, you will experiment with the performance impact of multithreading using real time measurements. You will use the POSIX thread library on a UNIX system. Your job is to write a program that sorts an array of random integers first sequentially and then using multi-threading. Your program should take the following three command-line arguments:
//The array size (a positive integer between 1 and 100 000 000)
//The number of threads  (a positive integer between 1 and 16)
//The sorting algorithm (I for InsertionSort, Q for QuickSort)
//
//Your program will perform the following steps:
//Fill the array with random numbers
//Based on the number of threads (T), compute the indices for dividing the array into T equal parts. For example, if the array size (N) is 1000 and T is 2, the indices will be 0, 499, 999. 
//Sort sequentially by applying the sorting algorithm to each part of the array, and then combining the sorted parts into one sorted array using an O(n) algorithm.
//Apply an O(n) algorithm to check if the array has been sorted correctly, and print a message indicating correct/incorrect sorting.
//Refill the array with random numbers.
//Sort using multi-threading. This step should be done the same way you did sequential sorting with the only difference that the sorting of each part is done in a separate thread. Combining the sorted parts into one sorted array should be done in the main (parent) thread after all child threads have completed. So, the parent thread must wait for all child threads.     
//Apply an O(n) algorithm to check if the array has been sorted correctly, and print a message indicating correct/incorrect sorting.
//
//To measure the time, use the code provided below. All what you need to do is calling SetTime() to start the timing and GetTime() to get the time elapsed since the last call to SetTime(). So, you will first call SetTime right before Step 3 and GetTime right after its completion and print that time as the sequential sorting time. Then, do the same before and after Step 6 and print that time as the multithreaded sorting time. 
//
//Use the following command to compile your code:
//g++ -O3 tsort.c -lpthread -o tsort
//Note that we are using the –O3 option to enable a high-level of compiler optimizations. For your own benefit, you may want to try compiling it without the –O3 option and measuring the difference in speed when you run it.
//
//When your program is working correctly, run the following tests:
//Run InsertionSort using two threads with array sizes 10K, 100K and 300K.
//Run InsertionSort using four threads with an array size of 100K. 
//Run QuickSort using two threads with array sizes 1M, 10M and 100M.
//Run QuickSort using four threads with an array size of 10M. 
//
#include <sys/timeb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

long gRefTime;

long GetMilliSecondTime(struct timeb timeBuf)
{
    long mliScndTime;

    mliScndTime = timeBuf.time;
    mliScndTime *= 1000;
    mliScndTime += timeBuf.millitm;
    return mliScndTime;
}

long GetCurrentTime(void)
{
    long crntTime=0;

    struct timeb timeBuf;
    ftime(&timeBuf);
    crntTime = GetMilliSecondTime(timeBuf);

    return crntTime;
}

void SetTime(void)
{
    gRefTime = GetCurrentTime();
}

long GetTime(void)
{
    long crntTime = GetCurrentTime();

    return (crntTime - gRefTime);
}

void InsertionSort(int data[], int size)
{
    int i, j, temp;

    for(i=1; i<size; i++)
    {
        temp = data[i];
        for(j=i-1; j>=0 && data[j]>temp; j--)
            data[j+1] = data[j];
        data[j+1] = temp;		
    }		
}

void Swap(int x, int y)
{
    int temp = x;
    x = y;
    y = temp;
}

int Rand(int x, int y)
{
    int range = y-x+1;
    int r = rand() % range;
    r += x;
    return r;
}


int Partition(int data[], int p, int r)
{
    int i, j, x, pi;

    pi = Rand(p, r);
    Swap(data[r], data[pi]);

    x = data[r];
    i = p-1;
    for(j=p; j<r; j++)
    {
        if(data[j] < x)
        {
            i++;
            Swap(data[i], data[j]);
        }
    }
    Swap(data[i+1], data[r]);
    return i+1;
}

void QuickSort(int data[], int p, int r)
{
    int q;
    if(p >= r) return;
    q = Partition(data, p, r);
    QuickSort(data, p, q-1);
    QuickSort(data, q+1, r);
}

struct Params
{
    int *start;
    size_t len;
    int depth;
};
// only used for synchronizing stdout from overlap.
//pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
// forward declare our thread proc
void *merge_sort_thread(void *pv);

void merge(int *start, int *mid, int *end)
{
    int *res = malloc((end - start)*sizeof(*res));
    int *lhs = start, *rhs = mid, *dst = res;
    while (lhs != mid && rhs != end)
        *dst++ = (*lhs <= *rhs) ? *lhs++ : *rhs++;
    while (lhs != mid)
        *dst++ = *lhs++;
    while (rhs != end)
        *dst++ = *rhs++;
    // copy results
    memcpy(start, res, (end - start)*sizeof(*res));
    free(res);
}
// our multi-threaded entry point.
void merge_sort_mt(int *start, size_t len, int depth)
{
    if (len < 2)
        return;
    if (depth <= 0 || len < 4)
    {
        merge_sort_mt(start, len/2, 0);
        merge_sort_mt(start+len/2, len-len/2, 0);
    }
    else {
        struct Params params = { start, len/2, depth/2 };
        pthread_t thrd;
//        pthread_mutex_lock(&mtx);
        printf("Starting subthread...\n");
//        pthread_mutex_unlock(&mtx);
        // create our thread
        pthread_create(&thrd, NULL, merge_sort_thread, &params);
        // recurse into our top-end parition
        merge_sort_mt(start+len/2, len-len/2, depth/2);
        // join on the launched thread
        pthread_join(thrd, NULL);
//        pthread_mutex_lock(&mtx);
        printf("Finished subthread.\n");
//        pthread_mutex_unlock(&mtx);
    }
    // merge the paritions
    merge(start, start+len/2, start+len);
}
// our thread-proc that invokes merge_sort. this just passes the
// given parameters off to our merge_sort algorithm
void *merge_sort_thread(void *pv)
{
    struct Params *params = pv;
    merge_sort_mt(params->start, params->len, params->depth);
    return pv;
}
// public-facing api
void merge_sort(int *start, size_t len)
{
    merge_sort_mt(start, len, 4); // 4 is a nice number, will use 7 threads.
}
int main()
{
    static const unsigned int N = 2048;
    int *data = malloc(N * sizeof(int ));
    unsigned int i;
    srand((unsigned)time(0));
    for (i=0; i<N; ++i)
    {
        data[i] = rand() % 1024;
        printf("%4d ", data[i]);
        if ((i+1)%8 == 0)
            printf("\n");
    }
    printf("\n");
    // invoke our multi-threaded merge-sort
    merge_sort(data, N);
    for (i=0; i<N; ++i)
    {
        printf("%4d ", data[i]);
        if ((i+1)%8 == 0)
            printf("\n");
    }
    printf("\n");
    free(data);
    return 0;
}

