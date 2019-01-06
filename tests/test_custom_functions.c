#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <string.h>

double myfunction1(int a, int b)
{
    double r;
    #pragma MPC 
    {
        r = pow(a, b);
    }
    return r;
}

int myfunction2(char *string, int a, int b, float f)
{
    float x;
    if (strlen(string) == 3)
    {
        x = f + 2;
    }
    return a + b;
}

double myfunction3(mpc_t a, mpc_t b)
{
    mpc_t r;
    mpc_init2(r, 128);
    double e;
    mpc_pow(r, a, b, MPC_RNDZZ);
    e = mpc_get_ldc(r, MPC_RNDZZ);
    return e;
}

int main()
{

    int a, x = 2, y = 3;
    double k, l;

    printf("### Testing supported mathematic functions ###\n");

    // using default pragma extensions
    #pragma MPC 
    {
        // we need to assign them because our grammar don't parse it if not
        // variables as arguments will not change (can't guess return type)
        l = myfunction1(x, y);
        // strings are parsed and will be printed as they are
        // of course, it is not possible to use a symbol representing a string
        a = myfunction2("string", a, 4, 1.2);
        // if expressions are used as arguments, they are converted in MPC!
        k = myfunction3(x+y, sqrt(4));
    }

    printf("### Here are the results! ###\n");
    printf("myfunction1(2, 3); %lf 8.000000\n", l);
    printf("myfunction3(2+3,sqrt(4),\"MPC_RNDZZ\") %lf 25.000000\n", k);

    return 0;

}