#include<stdio.h>
#include<stdlib.h>

void swap(int *a, int i,int d)
{
	int temp;
	temp = a[i];
	a[i]=a[d];
	a[d]=temp;
}
int *selectsort(int n, int *a)
{
	int i,min,j,b;
	for(i=0;i<n;i++)
	{
		min=a[i];
		for(j=i+1;j<n;j++)
		{
			if(min>a[j])
			{
				min=a[j];
				b=j;
			}
		}
		swap(a,i,b);
	
	}
	return a;
}

int *bubblesort(int n, int *a)
{
	int i,j,b;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
		if(a[j]>a[j+1]&&j<(n-1)) 
		{
			swap(a,j,j+1);

		}
		}
	}
	return a;
}
int *quicksort(int *a,int start, int end)
{
	int i,j,k,l,m,b,c,pivot,pl;
	pl=end-1;

	pivot=a[pl];
	for(i=start;i<pl;i++)
	{
		if(a[i]>pivot)
		{
			b=i;
		}
		for(j=pl-1;j>i;j--)
		{
			if(a[j]<pivot)
			{
				c=j;
				break;
			}	
		}
		swap(a,b,j);
	}
	swap(a,c,pl);
	for(i=start;i<end;i++)
	{
		printf("%d ",a[i]);
	}


return a;
}


void main()
{
	int a[10]={9,2,5,0,1,3,8,7,6,4};
	int *sort;
	int i;
	sort=selectsort(10,a);
	//sort=bubblesort(10,a);
	//sort=quicksort(a,0,10);

	printf("main ");
	for(i=0;i<10;i++)
	{
		printf("%d ",sort[i]);
	}
	printf("\n");

}
