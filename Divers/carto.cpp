#include <stdio.h>

//float carto(float x, const float axe[], const float val[]);
float carto(float x, float *axe, float *val)
{
	int n = sizeof(axe);
	int m=1;
	if (x<=axe[0])
		return val[0];
	if (x>=axe[n-1])
		return val[n-1];
	while (m<=n)
	{
		if (x>axe[m])
			m++;
		else
		{
			return (val[m-1]+(x-axe[m-1])*(val[m]-val[m-1])/(axe[m]-axe[m-1]));
		}
	}
	return 0;//impossible
}

int main(int argv, char**argc)
{
	float mon_axe[]={0, 15 ,30 ,200};
	float mes_val[]={1 ,2 ,3 ,4};
	float input = 2;
	while (1)
	{
		scanf("%f",&input);
		printf("pour x = %f, on a f(x) = %f\n",input,carto(input,mon_axe,mes_val));
	}
	return 0;
}


	
