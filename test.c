#pragma MPC precision(128) rounding(MP_PSDF) 
{

    a = 0.;
    if(a < 10 || 1)
    {
        a=a+1;
    }
    else
    {
        a=a-1;
        a=a+1;
        a=a*2;
        a=a-1;
        if(a==1)
        {
            a = 0;
        }
    }

    while(a<1000)
    {
        a=a-1;
    }

    for(x=0;x<10;x=x+1)
    {
        for(y=0;y<10;y=y+1)
        {
            a=a/2;
        }
    }

    a = a&&(b||f)&&c||c&&b&&d;
}
