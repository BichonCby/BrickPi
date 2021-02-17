#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <wiringPi.h>

#define   inv(port) (port==1)?0:((port==2)?1:((port==4)?2:3))


int main(int argc, char **argv)
{
	wiringPiSetup();
	pinMode(0,INPUT);
	printf("0x01 donne %d\n",inv(0x01));
	printf("0x02 donne %d\n",inv(0x02));
	printf("0x04 donne %d\n",inv(0x04));
	printf("0x08 donne %d\n",inv(0x08));
	delay(5000);
	while(1)
	{
		if (digitalRead(0) == 1)
			printf("1\n");
		else
			printf("0\n");
		delay(500);
	}
	return 0;
}
