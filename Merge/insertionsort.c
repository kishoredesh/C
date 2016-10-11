#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <err.h>
#include <sysexits.h>
#include "sort.h"

void InsertionSort(void *arr, int num_elem,int elem_size,Compare_fn f)
{
    unsigned int len = (unsigned int) num_elem;
    unsigned int size = (unsigned int) elem_size;
    int *array  =(int *)arr;
    int i, j;
    unsigned int temp; 
    void *combo, *left, *right;

    if (!arr || !f)
      return;

    if (len < 2)
      return;
    
    for(i=1; i<num_elem; i++)
    {
        temp = array[i];
        for(j=i-1; j>=0 && array[j]>temp; j--)
            array[j+1] = array[j];
        array[j+1] = temp;		
    }
//    for(i=1; i<num_elem; i++)
//        printf("%d,",array[i]);
//    printf("\n\n\n");
}

/*structure to hold arguments to pass into merge sort*/
typedef struct {
  void *array;
  int len;
  int size;
  Compare_fn f;
} Arguments;

/*merges the two arrays together in sorted order*/
static void *merge(void *left_side, void *right_side, unsigned int left_len,
		   unsigned int right_len, unsigned int size, Compare_fn f) {

  char *left = left_side, *right = right_side, *combo;
  int counter = 0;

  /*allocate memory for temporary array*/
  if (!(combo = calloc((left_len + right_len), size)))
    err(EX_OSERR, "memory allocation error");

  /*until left and right are fully distributed, compare left and
    right then place element into temp array*/
  while (left_len > 0 || right_len > 0) {

    /*compare left and right if other is not empty*/
    if (left_len > 0 && right_len > 0) {
      /*compare left and right given function f*/
      /*if left <= right, put first element of left into
	temp array. otherwise put first element of right in*/
      if(f(left, right) <= 0) {
	memcpy((combo + (counter++ * size)), left, size);
	left += size;
	left_len--;
      } else {
	memcpy((combo + (counter++ * size)), right, size);
	right += size;
	right_len--;
      }

      /*if one array is empty, fill put the remaining elements
	of other array into temp array*/
    } else if(left_len > 0) {
      memcpy((combo + (counter * size)), left, (size * left_len));      
      left_len = 0;
    }
    else if(right_len > 0) {
      memcpy((combo + (counter * size)), right, (size * right_len));
      right_len = 0;
    }
  }
  return combo;
}

/*helper function to have a thread pass the needed arguments*/
static void *pass_args(void *structure) {
  Arguments *args = (Arguments *) structure;
  InsertionSort(args->array, args->len, args->size, args->f);
  return NULL;
}

/*splits array up and gives each part to a thread to sort
  then merges the sorted parts together*/
void mt_InsertionSort(void *arr, int num_elem, int elem_size, Compare_fn f,int thread_ct) {

  pthread_t *tids;
  unsigned int len = (unsigned int) num_elem, size = (unsigned int) elem_size;
  unsigned int combo_len = 0, i;
  Arguments *array_part;
  char **segments;
  void *combo, *temp;

  if (!arr || !f)
    return;

  if (len < 2)
    return;

  if (thread_ct != 1 && thread_ct != 2 && thread_ct != 4 && thread_ct != 8 && thread_ct != 16)
    return;

  /* allocates memory for tids, segments, and array_part arrays*/
  tids = (pthread_t *)calloc((int)thread_ct, sizeof(pthread_t));
  array_part = (Arguments *)calloc((int)thread_ct, sizeof(Arguments));
  segments = (char **)calloc((int)thread_ct, sizeof(char *));

  if(!tids || !array_part || !segments)
    err(EX_OSERR, "memory allocation error");

  /* distributes arr into thread_ct number of structures
     in order to split the work up among threads */
  for (i = 0; i < thread_ct; i++) {
    segments[i] = ((char *)arr) + (i * (len/thread_ct) * size);
    array_part[i].array = segments[i];
    array_part[i].len = len/thread_ct;
//    printf("Thread %d, LEN :%d\n",i,array_part[i].len);
    array_part[i].size = size;
    array_part[i].f = f;
  }

  /*if len % thread_ct != 0, add leftover elements to final thread*/
    if (len % thread_ct)
    array_part[thread_ct-1].len += len % thread_ct;

    /* spawns then joins threads */
  for (i = 0; i < thread_ct; i++)
    if (pthread_create(&tids[i], NULL, pass_args, &array_part[i]))
      err(EX_OSERR, "pthread_create() failed");
  for (i = 0; i < thread_ct; i++)
    if (pthread_join(tids[i], NULL))
      err(EX_OSERR, "pthread_join() failed");

  /* merges the work done by threads if merging is necessary */
  if (thread_ct > 1) {
    /* merges first two arrays sorted by thread 1 and 2 */
    combo = merge(segments[0], segments[1], array_part[0].len,
		  array_part[1].len, size, f);

    /* creates dummy array used for memory freeing */
    temp = combo;

    combo_len = ((int)array_part[0].len + (int)array_part[1].len);

    /* merges sorted arrays from thread 3 and 4
       into combo if thread 3 and 4 exist */
    for (i = 2; i < thread_ct; i++) {
      combo = merge(combo, segments[i], combo_len, array_part[i].len, size, f);
      free(temp);
      temp = combo;
      combo_len += (unsigned int) array_part[i].len;
    }
  } /* if only one thread, no merging necessary */
  else 
    combo = segments[0];
  
  /* replaces contents of arr with sorted contents of combo
     then frees allocated memory */
  memcpy(arr, combo, len * size);

  if (thread_ct > 1)
    free(temp);
  free(tids);
  free(array_part);
  free(segments);   
}
