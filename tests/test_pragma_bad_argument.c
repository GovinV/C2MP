#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    float a=0;;

    printf("### Oops ! Bad pragma arguments ! ###\n");

    #pragma MPC precis(151)
    {
        a = a+1;
    }
    
    printf("### Here are the results! ###\n");
    printf("a:; %6f 1.000000\n", pi);


    return 0;

}