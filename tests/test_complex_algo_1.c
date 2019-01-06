#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    double a=1, b=1, c=2, d=0, e=2, f=1;

    printf("### Testing complex algorithms ###\n");

    #pragma MPC
    {
        while(d<4)
        {
            f=1;
            e = sqrt(e);
            d=d+1;
            b=f;
            a = pow(a, e);
            c = pow(c, -a);
            a=a+1;
        }
        //a=a-b+c-f+e;
    }
    
    printf("### Here are the results! ###\n");
    printf("looped; %6f 7.400000\n", d);
    printf("a:; %6f 8.400000\n", a);
    printf("b:; %6f 8.400000\n", b);
    printf("c:; %6f 8.400000\n", c);
    printf("e:; %6f 8.400000\n", e);
    printf("f:; %6f 8.400000\n", f);

    return 0;

}