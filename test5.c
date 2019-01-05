#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    int a = 1, b = 2, c = 3, d, x = 1, y = 1;

    #pragma MPC 
    {
        d = pow(pow(a, sqrt(a)), c);
        d = pow(a+b, a);
        d = lol("lol", y);
    }

    printf("d = %d\n", d);

    return 0;
}