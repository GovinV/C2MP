#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    float a=1., b=0., c=1.;
    #pragma MPC 
    {
        do
        {
            a = b + c;
            d = b + c;
            while(a < 10.)
            {
            	c = a + d;
            	b = a + d;
            	a = a + b;
            }
            b = b+a;
        }while(b < 10.);
        w=w;
    }
}