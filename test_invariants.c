#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    float a=1., b=0., c=1.;
    #pragma MPC 
    {
        while(b < 10.)
        {
            a = c;
            b = b+a;
        }
    }
}