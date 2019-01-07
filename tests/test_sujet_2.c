#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <string.h>

int main()
{

    double a = 5, b = 300, c = 4;

    #pragma MPC
    {
        while ( (pow(a,3) -pow(c, 2) + 12 > 0) && (a * 46 - pow(b,2) < 3.14) ) {
            a = (pow(a,3)+pow(a,2))/2 ;
            printf("#a=%lf;\n", a);
        }
    }

    printf("a= %lf 213750.000000\n", a);

    return 0;

}