#include <complex.h>
#include <mpc.h>
#include <stdio.h>

int main()
{
    double a, b;

    #pragma MPC
    {
        a = pow(2,2);
        #pragma MPC
        {
            b = sqrt(a);
        }
    }

    printf("a=%lf; b=%lf;\n", a, b);

    return 0;
}