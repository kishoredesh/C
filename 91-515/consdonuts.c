#include "donuts.h"

 int shmid, semid[3];


int main(int argc, char *argv[])
{
  int out [NUMFLAVORS][12];

  int i,j,donutType,noOfDonut;
  struct donut_ring *shared_ring;
  struct timeval randtime, now;

  /* Start Getting shared memory */
  if((shmid = shmget (SHMKEY, sizeof(struct donut_ring), IPC_CREAT | 0600)) == -1){
  	perror("shared get failed: ");
  	exit(1);
  }

  /* Now its Start Attaching shared memory */
  if((shared_ring = shmat (shmid, NULL, 0)) == (void *)-1){
    perror("shared attach failed: ");
    exit(2);
  }

  /* Initiating Semophore allocation*/
  for(i = 0; i < NUMSEMIDS; i++){
    if ((semid[i] = semget (SEMKEY+i, NUMFLAVORS, IPC_CREAT | 0600)) == -1){
    	perror("semaphore allocation failed: ");
    	exit(3);
    }
  }


  /* Now we are Getting Random number */
  gettimeofday (&randtime, (struct timezone *)0);
  unsigned short xsub1[3];
  xsub1[0] = (ushort) randtime.tv_usec;
  xsub1[1] = (ushort)(randtime.tv_usec >> 16);
  xsub1[2] = (ushort)(getpid());

	/* Generating Random Number */
  for( i = 0; i < no_of_dozens; i++ ) {

    for(donutType= 0; donutType< NUMFLAVORS; donutType++){
      for(noOfDonut = 0; noOfDonut < 12; noOfDonut++){
        out[donutType][noOfDonut] = 0;
      }
    }

    /* Consumer to consume the donuts using P and V operation*/

    for( noOfDonut = 0; noOfDonut < 12; noOfDonut++ ) {
      j=nrand48(xsub1) & 3;

      p(semid[CONSUMER], j);
      p(semid[OUTPTR], j);

      out[j][noOfDonut] = shared_ring->flavor[j][shared_ring->outptr[j]];
      shared_ring->outptr[j] = (shared_ring->outptr[j] + 1) % NUMSLOTS;

      v(semid[OUTPTR], j);
      v(semid[PROD], j);
    }
    gettimeofday (&now, (struct timezone *)0);
    printf("Consumer Process ID : %d, Time : %d , Dozen #  %d\n", getpid(), (ushort)now.tv_usec, i+1);
    printf("\n   plain   jelly   coconut   honey-dip\n");
    for(noOfDonut = 0; noOfDonut < 12; noOfDonut++){
      for(donutType= 0; donutType< NUMFLAVORS; donutType++){
        if(out[donutType][noOfDonut] != 0)
          printf("   %3d", out[donutType][noOfDonut]);
        else
          printf("         ");
      }
     printf("\n");
    }
  	printf("-------------------------------------------");
    printf("\n");
  }
}
