#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    printf("### Oops ! I used an undefined variable ! ###\n");

    #pragma MPC precision(2151)
    {
        undefined_variable = undefined_variable+1;
    }
    
    printf("### Here are the results! ###\n");
    printf("undefined_variable:; %6f 1.000000\n", undefined_variable);


    return 0;

}