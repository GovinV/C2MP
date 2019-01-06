#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <string.h>

mpc_t myfunction1(int a, int b)
{
    double r;
    #pragma MPC 
    {
        r = pow(a, b);
    }
    mpc_pow(r, a, b, MPC_RNDZZ);
    return r;
}

mpc_t myfunction2(char *string, int a, int b, float f)
{
    float x;
    if (strlen(string) == 3)
    {
        x = f + 2;
    }
    return a + b;
}

mpc_t myfunction3(mpc_t a, mpc_t b)
{
    int e;
    mpc_t r;
    mpc_pow(r, a, b, MPC_RNDZZ);
    e = (int)mpc_get_ldc(r, MPC_RNDZZ);
    return e;
}

int main()
{

    int a, b, c, x = 2, y = 3;

    printf("### Testing supported mathematic functions ###\n");

    // using default pragma extensions
    #pragma MPC 
    {
        // we need to assign them because our grammar don't parse it if not
        // variables as arguments will not change (can't guess return type)
        a = myfunction1(a, c);
        // strings are parsed and will be printed as they are
        // of course, it is not possible to use a symbol representing a string
        a = myfunction2("string", a, 4, 1.2);
        // if expressions are used as arguments, they are converted in MPC!
        a = myfunction3(x+y, sqrt(4));
    }

    printf("### Here are the results! ###\n");
    printf("myfunction3(2+3,sqrt(4),\"MPC_RNDZZ\") %d 25", a);

    return 0;

}