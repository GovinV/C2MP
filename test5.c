#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    int a = 1, b = 2, c = 3, d, x = 1, y = 1;

    #pragma MPC 
    {
        d = pow(pow(a, sqrt(a)), c);
        c = lol(x, y);
    }

    printf("d = %d\n", d);

    return 0;
}