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

        while(c < 50.0)
        {
            b = sin(cos(sinh(c*d)+f-a)*c+d);
            c = c+1.0;
        }
        
        f = (f-c)/pow(d, 3.5);
    }
    
    printf("### Here are the results! ###\n");
    printf("a:; %6f 2.396271\n", a);
    printf("b:; %6f -0.622225\n", b);
    printf("c:; %6f 50.127622\n", c);
    printf("d:; %6f 305.000000\n", d);
    printf("e:; %6f 2.618034\n", e);
    printf("f:; %6f 45443.148086\n", f);

    return 0;

}