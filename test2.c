#pragma MPC precision(128) rounding(MP_PSDF) 
{

    a = x + y;
    b = x + y;
    x = y;
    c = a + i;
    d = b + i;

}