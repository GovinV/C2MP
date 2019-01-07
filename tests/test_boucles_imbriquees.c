#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    double a, b, c, d, e, f, g, h;

    printf("### Testing imbricated loops ###\n");

    #pragma MPC precision(2000) 
    {
        for(a=0;a<10;a=a+1)
        {
            for(b=0;b<10;b=b+1)
            {
                for(c=0;c<10;c=c+1)
                {
                    for(e=0;e<10;e=e+1)
                    {
                        d=4;
                        g = b;
                        f = a;
                        h = g+f;
                    }
                }
            }
        }
    }
    
    printf("### Here are the results! ###\n");
    printf("a:; %6f 10.000000\n", a);
    printf("b:; %6f 10.000000\n", b);
    printf("c:; %6f 10.000000\n", c);
    printf("d:; %6f 4.000000\n", d);
    printf("e:; %6f 10.000000\n", e);
    printf("f:; %6f 9.000000\n", f);
    printf("g:; %6f 9.000000\n", f);
    printf("h:; %6f 9.000000\n", f);

    return 0;

}