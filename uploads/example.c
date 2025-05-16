#include <stdio.h>
#include <stdlib.h>
int main()
{
    int n1, n2, max, lcm;
    int arr[] = {1, 2, 3, 4, 5};
    printf("Enter two positive integers: ");
    scanf("%d %d", &n1, &n2);
    max = (n1 > n2) ? n1 : n2;
    lcm = max;
    while ((lcm % n1 != 0) || (lcm % n2 != 0))
    {
        lcm += max;
    }
    printf("The LCM of %d and %d is %d.", n1, n2, lcm);
    int i;
    for (i = 0; i < 3; i++)
    {
        i = i + 1;
    }
    return 0;
}
