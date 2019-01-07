#include <complex.h>
#include <stdio.h>
#include <mpc.h>
#include <string.h>

int main()
{
    double resultat;
    double NMAX = 2;
    double pc = 3;

    #pragma MPC
    {
        resultat = -(sqrt(-NMAX*(8*pc-4*pow(NMAX,3)-4*pow(NMAX,2)-NMAX-8))-2*pow(NMAX,2)-NMAX)/(2*NMAX);

    }

    printf("resultat= %lf 0.43844718\n", resultat);

    return 0;
}