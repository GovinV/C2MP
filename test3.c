#include <stdio.h>
#include <complex.h>
#include <mpc.h>


int main()
{
    mpc_t T0;
    mpc_init2(T0,128);
    if(1)
    {

    }
    else
    {
        int a=1;
    }
    #pragma MPC precision(128) rounding(MP_RNDZZ)
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

    #pragma MPC precision(128) rounding(MP_RNDZZ)
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

