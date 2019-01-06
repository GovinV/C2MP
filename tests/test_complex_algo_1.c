#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    double a=1, b=1, c=2, d=0, e=2, f=1;

    printf("### Testing complex algorithms ###\n");

    #pragma MPC precision(2000) 
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
        
        do
        {
            c = sqrt(c);
            d=d+1;
        }while(d<324);
        
        b = a+e;
        c = b*e;
        f = a*b*c*d*e;
        f =  pow(f, e);
        c = 5.0;
        
        printf("%f\n", c);

        while(c < 10.0)
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