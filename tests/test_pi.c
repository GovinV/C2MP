#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    float pi=0.f, k=0;

    printf("### Computing decimals of pi ###\n");
    
    #pragma MPC precision(255) rounding(MPC_RNDZZ)
    {
        for(k=0;k<1000;k=k+1)
        {
            pi=pi+pow(-3, -k)/(2*k+1);
        }
        pi = pi*sqrt(12);
    }

    printf("### Result ###\n");
    printf("iteration_num:; %6f 1000.000000\n", k);
    printf("pi:; %6f 3.141593\n", pi);

    return 0;
}