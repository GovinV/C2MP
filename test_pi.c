#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    float pi=0.f, k;

    #pragma MPC precision(255) rounding(MPC_RNDZZ)
    {
        
        for(k=0;k<100;k=k+1)
        {
            pi=pi+pow(-3, -k)/(2*k+1);
        }
        pi = pi*sqrt(12);
    }

    printf("d = %d\n", pi);

    return 0;
}