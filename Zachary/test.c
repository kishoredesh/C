#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int A;
    int B;
    int tempA;
    int i=0;
    int mask=1;
    int result= 0;
    int j = 0;
    char temp;

    //Taking input from users for A 
    printf("\nEnter Value of A :");
    scanf("%d",&A);
    
//Taking input from users for B 
    printf("\nEnter Value of B :");
    scanf("%d",&B);
//Generating String for same length as B to save 1 and 0    
    char *str = (char *)calloc(B,sizeof(char));
//Storing orginal value of A  in tempA to print in last of the code
    tempA = A;

// runnning the for loop for B times to mask A with 1 and shift to right,
// because when you mask A with 1 , it will result right most bit of A in result variable
// if result == 1, then we store 1 in string or else store 0 in string
// in last, we need to reverse the string to get the right result
for (i=0;i<B;i++){
        result = A & mask;
        A =  A >> 1;
        if( result == 1)
            str[i] = '1';
        else
            str[i] = '0';
    }
//Reverse the string for bit pattern
    i = 0;
    j = strlen(str) -1;
    while(i<j){
        temp = str[j];
        str[j] = str[i];
        str[i] = temp;
        i++;
        j--;
    }
//restoreing the A to entered value
    A = tempA;
    printf("\nEntered Value for A = %d and B = %d,Bit Pattern=%s\n",A,B,str);
    return 0;
}
