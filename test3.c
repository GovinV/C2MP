#include <stdio.h>
#include <complex.h>
#include <mpc.h>


int main()
{
    mpc_t T0;
    mpc_init2(T0,128);
    int a = 1, b = 2, c = 3, d, x = 1, y = 1, i = 3;
    if(1)
    {

    }
    else
    {
        int a=1;
    }
    #pragma MPC precision(128) rounding(MPC_RNDZZ)
    {
        a = x + y;
        b = x + y;
        x = y;
    	c = a + i;
    	d = b + i;
        a = fabs(a);
        d = exp(a);
        x = log(x);
        y = log10(y);
        c = cos(c);
        i = sin(i);
        c = cosh(c);
        i = sinh(i);
    }
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("c = %d\n", c);
    printf("d = %d\n", d);
    printf("i = %d\n", i);
    printf("x = %d\n", x);
    printf("y = %d\n", y);

    if(5)
    {
        int z = 5;
    }

    #pragma MPC precision(128) rounding(MPC_RNDZZ)
    {
        a = x + y;
        b = x + y;
        x = y;
    	c = a + i;
    	d = b + i;
        a = fabs(a);
        d = exp(a);
        x = log(x);
        y = log10(y);
        c = cos(c);
        i = sin(i);
        c = cosh(c);
        i = sinh(i);
    }

    if(5)
    {
        int z = 5;
    }
}

