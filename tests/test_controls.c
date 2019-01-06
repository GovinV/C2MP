#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{

    int a, b = 0, c, d, e, k, i, l;

    printf("### Playing arround with loops and conditions ###\n");

    #pragma MPC
    {
        // we do not need bracket
        for (a = 0; a < 10; a = a + 1)
            if (a == 5)
                b = b + 1;
        
        // same thing with brackets
        for (c = 0; c < 10; c = c + 1)
        {
            if (c == 5)
            {
                b = b + 1;
            }
        }

        // a and c are equal to 10 here and b = 2
    
        while (a != 15)
        {
            a = a + 1;
        }

        // a equal to 15 here
        // testing if a == 15 with useless calculation for fun ;)
        if (((a*2)/2) == 15)
        {
            d = 1;
        }
        // brackets no needed
        else
            d = 0;
        
        // testing else
        if (a != 15)
            e = 0;
        else 
            e = 1;
        
        // give a value to k
        k = 0;
        l = 0;

        // more ifs !!!
        do {
            if (k == 5)
            {
                if (i == 5)
                {
                    // very useless but for fun
                    if (k == 5)
                        l = 1;
                }
                else 
                {
                    i = i + 1;
                }
            }
            else 
            {
                k = k + 1;
            }
        } while (l != 1);

        // here, k = 5 and i = 5 and l = 1

    }
    
    printf("### Here are the results! ###\n");
    printf("for(a=0;a<=10;a=a+1);while(a!=15); %d 15\n", a);
    printf("for(c=0;c<=10;c=c+1) %d 10\n", c);
    printf("b=b+1;b=b+1; %d 2\n", b);
    printf("if(((a*2)/2)==15);d=1; %d 1\n", d);
    printf("if(a!=15);e=1; %d 1\n", e);
    printf("while(l!=1);k=1; %d 5\n", k);
    printf("while(l!=1);if(k==5);i=1; %d 5\n", i);    
    printf("while(l!=1);if(k==5);if(i==5);l=1; %d 1\n", l);


    return 0;

}