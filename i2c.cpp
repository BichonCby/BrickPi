#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "BrickPi3.h"

i2c_struct_t myi2c;
sensor_ultrasonic_t sonar1;

BrickPi3 BP;

int main(int argc, char **argv)
{
	printf("detect = %d\n",BP.detect()); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

	BP.reset_all(); // hyper important !!!
	
	sleep(1);
	myi2c.address = 0x02;
	myi2c.delay = 10000;
	myi2c.speed = 0;
	myi2c.length_write = 1;
	myi2c.length_read = 3;
	myi2c.buffer_write[0] = 0x42;
	myi2c.buffer_write[1] = 0x03;
	myi2c.buffer_write[2] = 0x00;
	printf("type sonar = %d\n",BP.set_sensor_type(PORT_1,SENSOR_TYPE_NXT_ULTRASONIC));
	sleep(1);
	for (int i=1;i<2;i++)
	{
		
		printf("result = %d\n",BP.get_sensor(PORT_1,&sonar1));
		//printf("presence = %d\n",sonar1.presence);
		printf("value = %f cm\n",sonar1.cm);
		usleep(100000);
	}
	printf("\n\ntransaction %d\n",BP.transact_i2c(PORT_1,&myi2c));
//	printf("\nretour = %x %x %x %x %x %x\n\n",(int)myi2c.buffer_read[0],(int)myi2c.buffer_read[1],(int)myi2c.buffer_read[2],(int)myi2c.buffer_read[3],(int)myi2c.buffer_read[4],(int)myi2c.buffer_read[5]);

	BP.reset_all(); // hyper important !!!
	printf("reset \n");
	sleep(1);

	printf("type i2c= %d\n",BP.set_sensor_type(PORT_1,SENSOR_TYPE_I2C,0,&myi2c));
	for (int i=1;i<2;i++)
	{
		
		printf("result = %d\n",BP.get_sensor(PORT_1,&myi2c));
		usleep(100000);
	}
}
