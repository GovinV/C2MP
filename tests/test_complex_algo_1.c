#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    double a=1, b=1, c=2, d=0, e=2, f=1;

    printf("### Testing complex algorithms ###\n");

    #pragma MPC precision(2000) 
    {
        while(d<300)
        {
            f=1.0;
            e = sqrt(e)+1;
            d=d+1.0;
            b=cos(f)+1;
            a = pow(a, 1/e);
            c = pow(c, 1/a)+2.3242;
            a=a+1.0;
        }
        
        do
        {
            c = sqrt(c);
            d=d+1;
        }while(d<305);
        
        b = a+e;
        c = b*e;
        f = a*b*c*d*e;
        f =  pow(f, e);
        
        printf("%f\n", c);

        while(c < 100.0)
        {
            c = c+1.0;
            printf("%f\n", c);
        }
    }
    
    printf("### Here are the results! ###\n");
    printf("a:; %6f 8.400000\n", a);
    printf("b:; %6f 8.400000\n", b);
    printf("c:; %6f 8.400000\n", c);
    printf("d:; %6f 8.400000\n", d);
    printf("e:; %6f 8.400000\n", e);
    printf("f:; %6f 8.400000\n", f);

    return 0;

}