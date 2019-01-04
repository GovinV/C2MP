#include <stdio.h>
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
    #pragma MPC precision(128) rounding(MP_PSDF)
    {
        a = x + y;
        b = x + y;
        x = y;
    	c = a + i;
    	d = b + i;
    }
}

