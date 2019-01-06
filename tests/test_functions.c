#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    // using float for fun ;)
    float a, b, d, e, f, g, h, i, j, x = 2, y = 3, z = 3.14;

    printf("### Testing supported mathematic functions ###\n");

    // using default pragma extensions
    #pragma MPC 
    {
        a = pow(x, y);
        b = sqrt(2);
        d = exp(x);
        e = log(x);
        f = log10(100);
        g = cos(x);
        h = sin(x);
        i = cosh(2);
        j = sinh(x);
    }

    printf("### Here are the results! We print 6 decimals ###\n");
    printf("pow(2,3) %6f 8.000000\n", a);
    printf("sqrt(2) %6f 1.414214\n", b);
    printf("exp(2) %6f 7.389056\n", d);
    printf("log(2) %6f 0.693147\n", e);
    printf("log10(100) %6f 2.000000\n", f);
    printf("cos(2) %6f -0.416147\n", g);
    printf("sin(2) %6f 0.909297\n", h);
    printf("cosh(2) %6f 3.762196\n", i);
    printf("sinh(2) %6f 3.626860\n", j);

    return 0;

}