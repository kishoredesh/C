#include <stdio.h>

int main(){
    int i,j;
    int iarray[10];
    int *iptr;
    iptr = iarray;
    
    char string[10];
    char *sptr;
    sptr = string ;
    
    float farray[10];
    float *fptr;
    fptr = farray;
        
    double darray[10];
    double *dptr;
    dptr = darray;
    
    for(i=0;i<10;i++){
        printf("string[%d] -> %x\n", i,fptr+i);
    }
    printf("\n");
    for(i=0;i<10;i++){
        printf("iarray[%d] -> %x\n", i,iptr+i);
    }
    printf("\n");
    for(i=0;i<10;i++){
        printf("farray[%d] -> %x\n", i,fptr+i);
    }
    printf("\n");
    for(i=0;i<10;i++){
        printf("darray[%d] -> %x\n", i,dptr+i);
    }
    printf("\n");

    return 0;
}
