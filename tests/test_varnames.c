#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    float whileABC = 1, afora = 1,  dob = 1, if1 = 1;

    printf("### Testing variables names ###\n");

    // using default pragma extensions
    #pragma MPC 
    {
        whileABC = afora + dob;
        while(whileABC)
        {
            if1 = dob;
        }
    }

    printf("### result ###\n");
    printf("dummy %6f 2.000000\n", whileABC);

    return 0;
}