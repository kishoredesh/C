#include <stdio.h>
#include <stdlib.h>

#define numberBeam 3
int main(int argc,char* argv[]){

  int i,j;
  int L1,L2;
  float R1,R2;
  int FD = 0;
  int sumF = 0;
  int numberLoad;
  for (j =0;j < numberBeam;j++){
    printf("Enter number of Loads on beam %d,\n",j+1);
    scanf("%d",&numberLoad);
    int *F = malloc(numberLoad*sizeof(int));
    int *D = malloc(numberLoad*sizeof(int));
    FD = 0;
    sumF = 0;
    printf("Enter the Loads applied on various distance for Beam %d\n", j+1);
    for (i=0;i<numberLoad;i++){
        scanf("%d",&F[i]);
    }

    printf("Enter the distance where various load applied on  Beam %d\n", j+1);
    for (i=0;i<numberLoad;i++){
        scanf("%d",&D[i]);
    }
    
    printf("Enter the distance of Reaction 1\n");
    scanf("%d",&L1);
    
    printf("Enter the distance of Reaction 2\n");
    scanf("%d",&L2);
    
    if (L1 == 0){
      for(i=0;i<numberLoad;i++){
        FD = FD + F[i]*D[i];
        sumF +=  F[i];
      }
      R2 = sumF / L2;
      R1 = sumF - R2;
      printf("Beam %d, Reaction1 = %f, Reaction2 = %f\n",j+1,R1,R2);
    }
    else if( L2 == 0){
      for(i=0;i<numberLoad;i++){
        FD = FD + F[i]*D[i];
        sumF +=  F[i];
      }
      R1 = sumF / L1;
      R2 = sumF - R1;
      printf("Beam %d, Reaction1 = %f, Reaction2 = %f\n",j+1,R1,R2);
    }
    else{
      for(i=0;i<numberLoad;i++){
        FD = FD + F[i]*D[i];
        sumF +=  F[i];
      }
      R1 = ( sumF - FD) / (L2- L1);
      R2 = sumF - R1;
      printf("Beam %d, Reaction1 = %f, Reaction2 = %f\n",j+1,R1,R2);
    }
    free(F);
    free(D);
  }
}
