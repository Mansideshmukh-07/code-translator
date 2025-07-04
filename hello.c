#include<stdio.h>
void main()
{
int n,i;
    printf("enter limit:");
    scanf("%d",&n);
    int a[n];
    for(i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    printf("array:\n");
    for(i=0;i<n;i++)
    {
        printf("%d\n",a[i]);
    }
}