#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sysexits.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <time.h>
#include "sort.h"

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

/* compare function */
static int cmp(const void * a, const void * b) {
  int *x = (int *) a;
  int *y = (int *) b;

  if ( *x > *y) return 1;
  if (*x == *y) return 0;
  return -1;
}

int main(int argc, char *argv[]) {
  int *array, i, num_ints, seed, max, *cpy;
  struct rusage start_ru, end_ru;
  long difftime;
  /* checks for correct number of arguments */
  if (argc < 2) {
    fprintf(stderr, "Usage: mergetest <# of integers> <seed> <upper bound>\n");
    exit(EX_USAGE);
  }

  /* turns arguments into integers */
  num_ints = atoi(argv[1]);
//  seed = atoi(argv[2]);
 // max = atoi(argv[3]);
  srand((unsigned)time(NULL));

  /* allocate memory for array and copy of array */
//  array = calloc(num_ints, sizeof(int));
  array = malloc(num_ints * sizeof(int));
  if (!array)
    err(EX_OSERR, "memory allocation error");
//  cpy = calloc(num_ints, sizeof(int));
  cpy = malloc(num_ints * sizeof(int));
  if (!cpy)
    err(EX_OSERR, "memory allocation error");

  /* seed rng, create num_ints sized arrays of random intengers no
     greater than the size of max */
  for (i = 0; i < num_ints; i++) {
    array[i] = rand() %2048 + 100;
    cpy[i] = array[i];
  }
  
  /* 0 threads start */
  SetTime();
  InsertionSort(cpy, num_ints, sizeof(int), cmp);
  difftime = GetTime(); 
  printf("InsertionSort : 0 threads: %06lds\n",difftime );
  
  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  SetTime();
  mt_InsertionSort(cpy, num_ints, sizeof(int), cmp,1);
  difftime = GetTime(); 
  printf("InsertionSort : 1 threads: %06lds\n",difftime );
  
  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  SetTime();
  mt_InsertionSort(cpy, num_ints, sizeof(int), cmp,2);
  difftime = GetTime(); 
  printf("InsertionSort : 2 threads: %06lds\n",difftime );
  
  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  SetTime();
  mt_InsertionSort(cpy, num_ints, sizeof(int), cmp,4);
  difftime = GetTime(); 
  printf("InsertionSort : 4 threads: %06lds\n",difftime );
  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  
  SetTime();
  mt_InsertionSort(cpy, num_ints, sizeof(int), cmp,8);
  difftime = GetTime(); 
  printf("InsertionSort : 8 threads: %06lds\n",difftime );
  
  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  SetTime();
  mt_InsertionSort(cpy, num_ints, sizeof(int), cmp,16);
  difftime = GetTime(); 
  printf("InsertionSort : 16 threads: %06lds\n",difftime );
  /*reset array*/
  for (i = 0; i < num_ints; i++)
    cpy[i] = array[i];

  
//  SetTime();
//  QuickSort(cpy,ptr,num_ints,sizeof(int),cmp);
//  difftime = GetTime(); 
//  printf("QuickSort : 0 threads: %06lds\n",difftime );
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  
//  SetTime();
//  mt_QuickSort(cpy,ptr,num_ints,sizeof(int),cmp);
//  difftime = GetTime(); 
//  printf("QuickSort : 1 threads: %06lds\n",difftime );
//  
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  SetTime();
//  mt_QuickSort(cpy,0,num_ints,sizeof(int),cmp);
//  difftime = GetTime(); 
//  printf("QuickSort : 2 threads: %06lds\n",difftime );
//  
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  SetTime();
//  mt_QuickSort(cpy,ptr,num_ints,sizeof(int),cmp);
//  difftime = GetTime(); 
//  printf("QuickSort : 4 threads: %06lds\n",difftime );
//  
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  SetTime();
//  mt_QuickSort(cpy,ptr,num_ints,sizeof(int),cmp);
//  difftime = GetTime(); 
//  printf("QuickSort : 8 threads: %06lds\n",difftime );
//  
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  SetTime();
//  mt_QuickSort(cpy,ptr,num_ints,sizeof(int),cmp);
//  difftime = GetTime(); 
//  printf("QuickSort : 16 threads: %06lds\n",difftime );
//  
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  SetTime();
//  MergeSort(cpy, num_ints, sizeof(int), cmp);
//  difftime = GetTime(); 
//  printf("MergeSort : 0 threads: %06lds\n",difftime );
//
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  /* 1 thread start */
//  SetTime();
//  mt_mergesort(cpy, num_ints, sizeof(int), cmp, 1);
//  difftime = GetTime(); 
//  
//  printf("1 threads: %06lds\n",difftime );
// 
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  /* 2 thread start */
//  
//  SetTime();
//  mt_mergesort(cpy, num_ints, sizeof(int), cmp, 2);
//  difftime = GetTime(); 
//  printf("2 threads: %06lds\n",difftime );
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  /* 4 thread start */
//  SetTime();
//  mt_mergesort(array, num_ints, sizeof(int), cmp, 4);
//  difftime = GetTime(); 
//  printf("4 threads: %06lds\n",difftime);
//  /* 4 thread end */
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//
//  /* 8 thread start */
//  SetTime();
//  mt_mergesort(array, num_ints, sizeof(int), cmp, 8);
//  difftime = GetTime(); 
//  printf("8 threads: %06lds\n",difftime);
//
//  /*reset array*/
//  for (i = 0; i < num_ints; i++)
//    cpy[i] = array[i];
//  
//  SetTime();
//  mt_mergesort(array, num_ints, sizeof(int), cmp, 16);
//  difftime = GetTime(); 
//  printf("16 threads: %06lds\n",difftime);
//
//  /* 16 thread end */

  free(array);
  free(cpy);
  

  return 0;
}
