#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "BrickPi3.h"

i2c_struct_t myi2c;
BrickPi3 BP;

int main(int argc, char **argv)
{
	printf("detect = %d\n",BP.detect()); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

	myi2c.address = 0x02;
	myi2c.length_write = 1;
	myi2c.length_read = 8;
	myi2c.buffer_write[0] = 0x00;
	myi2c.buffer_write[1] = 0x00;
	myi2c.buffer_write[2] = 0x03;
	printf("type = %d\n",BP.set_sensor_type(PORT_1,SENSOR_TYPE_I2C,0,&myi2c));
	for (int i=1;i<20;i++)
	{
		printf("result = %d\n",BP.transact_i2c(PORT_1, &myi2c));
		printf("value = %d %d %d %d %d %d\n\n",(int)myi2c.buffer_read[0],(int)myi2c.buffer_read[1],(int)myi2c.buffer_read[2],(int)myi2c.buffer_read[3],(int)myi2c.buffer_read[4],(int)myi2c.buffer_read[5]);
		printf("value = %s\n\n",myi2c.buffer_read[0]);
		sleep(1);
	}
}
