#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

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

    // check arrondis !
    printf("### Here are the results! We print 8 decimals ###");
    printf("pow(2, 3) %6f 8.00000000\n", a);
    printf("sqrt(2) %6f 1.41421356\n", b);
    printf("exp(2) %6f 7.38905609\n", d);
    printf("log(2) %6f 0.69314718\n", e);
    printf("log10(100) %6f 2.00000000\n", f);
    printf("cos(2) %6f -0.41614683\n", g);
    printf("sin(2) %6f 0.90929742\n", h);
    printf("cosh(2) %6f 3.76219569\n", i);
    printf("sinh(2) %6f 3.62686040\n", j);

    return 0;

}