#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc,char *argv[]){

    if (argc < 2){
        printf("Usage: %s number( record need to enter for paired samples)\n",argv[0]);
        exit(-1);
    }
    int i;
    float  a,b;
    int iteration = atoi(argv[1]);
    int *arrayX = malloc(iteration*sizeof(int));
    int *arrayY = malloc(iteration*sizeof(int));
    int count =0;
    int sumX = 0;
    int sumY = 0;
    long int sumXY = 0;
    long int sumXX = 0;
    long int sumYY = 0;
    float meanX = 0.0;
    float meanY = 0.0;
    float stddevX = 0.0;
    float stddevY = 0.0;
    
    printf("Enter the Value of Variable X for six samples\n");
    for (i=0;i<iteration;i++){
        scanf("%d",&arrayX[i]);
    }

    printf("Enter the Value of Variable Y for six samples\n");
    for (i=0;i<iteration;i++){
        scanf("%d",&arrayY[i]);
    }

//*********************************STEP : 1******************************
//------Calculateing value of sum of X

    for (i=0;i<iteration;i++){
        sumX += arrayX[i];
    }
//    printf("Sum of X = %d\n",sumX);
//------Calculateing value of Sum of Square of X
    for (i=0;i<iteration;i++){
        sumXX += arrayX[i]*arrayX[i];
    }
//    printf("Sum of Square of X = %d\n",sumXX);

//---Calculating value of sum of Y
    for (i=0;i<iteration;i++){
        sumY += arrayY[i];
    }

 //   printf("Sum of Y = %d\n",sumY);

//--Calculating value of Sum of Square of Y
    for (i=0;i<iteration;i++){
        sumYY += arrayY[i]*arrayY[i];
    }
  //  printf("Sum of Square of Y = %d\n",sumYY);

//--Calculating value of Sum of X*Y
    for (i=0;i<iteration;i++){
        sumXY += arrayX[i]*arrayY[i];
    }
   // printf("Sum of X*Y = %d\n",sumXY);



//*********************************STEP: 2 ******************************
    meanX = sumX / iteration;
    meanY = sumY / iteration ;

    //printf("Mean of X = %f \n",meanX);
    //printf("Mean of Y = %f \n",meanY);
//*********************************STEP: 3 ******************************

    stddevX = sqrt((sumXX /iteration) -(meanX*meanX)); 
    //printf("Standard Deviation of X = %f \n",stddevX);
    
    stddevY = sqrt((sumYY /iteration) -(meanY*meanY)) ;
  //  printf("Standard Deviation of Y = %f \n",stddevY);

//*********************************STEP: 4 ******************************
    a = (sumXY - iteration*meanX*meanY) /(sumXX - iteration*meanX*meanX);
   // printf("Value of A = %f \n",a);
    b = meanY - a*meanX;

    //printf("Value of B = %f \n",b);
    printf("Y = %f*X  + %f\n",a,b);

}


