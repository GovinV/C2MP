#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    int a = 1, b = 2, c = 3, d, e, x = 1, y = 1;

    #pragma MPC 
    {
        d = pow(-a, -b);
        d = pow(pow(a, sqrt(a)), c);
        d = pow(a+b, a);
        d = lol("lol", y+x);
        d = lol(a*3, b);
        d = lol(a, b, c, d, e);
    }

    printf("d = %d\n", d);

    return 0;
}