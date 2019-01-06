#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    double a, b = 4.2, c, d, e, f;

    printf("### Testing basic arithmetic operations ###\n");

    #pragma MPC
    {
        a = 3.2 + b;
        c = b * 2;
        d = a / 2;
        e = b - 0.5;
        f = 2 * ((a+b)-(0.4/2)) + 1;
    }
    
    printf("### Here are the results! We print 6 decimals ###\n");
    printf("a=3.2+4.2; %6f 7.400000\n", a);
    printf("c=4.2*2; %6f 8.400000\n", c);
    printf("d=7.4/2; %6f 3.700000\n", d);
    printf("e=4.2-0.5; %6f 3.700000\n");
    printf("f=2*((7.4+4.2)-(0.4/2))+1; %6f 23.800000\n", f);

    return 0;

}