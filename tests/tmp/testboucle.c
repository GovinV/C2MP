#include <stdio.h>

int main()
{
    int a=1;
    int b=2;
    do
    {
        a=a+b;
        b=b+a;

        printf("a=%d, b=%d\n", a, b);

        // condition (a > 3000 && b>4000) || b>=10000
        if(a <= 3000)
        {
            goto label1;
        }
        if(b>4000)
        {
            break;
        }
        label1:
        if(b>=10000)
        {
            break;
        }
    }while(1);
}

