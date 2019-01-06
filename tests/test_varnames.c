#include <complex.h>
#include <stdio.h>
#include <mpc.h>

int main()
{
    float whileABC = 1, afora = 1,  dob = 1, if1 = 0, precision, rounding, precision2, rounding2;

    printf("### Testing variables names ###\n");

    // using default pragma extensions
    #pragma MPC 
    {
        whileABC = afora + dob;
        while(if1 < whileABC*10)
        {
            if1 = if1+dob;
        }
        precision2 = dob;
        rounding2 = precision2;
    }

    printf("### result ###\n");
    printf("looped; %lf 20.000000\n", if1);
    printf("whileABC; %lf 2.000000\n", whileABC);
    printf("afora; %lf 1.000000\n", afora);
    printf("dob; %lf 1.000000\n", dob);
    printf("precision2; %lf 1.000000\n", precision2);
    printf("rounding2; %lf 1.000000\n", rounding2);

    return 0;
}