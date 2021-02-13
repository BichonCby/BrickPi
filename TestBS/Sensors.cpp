#include "mainBS.h"

sensor_ultrasonic_t sonar1;
sensor_color_t      Color1;
sensor_encoder_t 	coderRight, coderLeft;
i2c_struct_t myi2c;
Sensors::Sensors()
{
	//BP.set_sensor_type(PORT_1,SENSOR_TYPE_NXT_ULTRASONIC);
	myi2c.address = 0x02;
	myi2c.length_write = 3;
	myi2c.length_read = 1;
	myi2c.buffer_write[0] = 0x02;
	myi2c.buffer_write[1] = 0x12;
	myi2c.buffer_write[2] = 0x05;
	
	//myi2c.speed = 100;
	printf("type = %d\n",BP.set_sensor_type(PORT_1,SENSOR_TYPE_I2C,0,&myi2c));
}

bool Sensors::getTouch(int button)
{
	return 0;
}
int Sensors::getSonar(uint8_t num) // valeur du sonar
{
	//BP.get_sensor(num,&sonar1);
	//myi2c.address = 0x02;
	//myi2c.length_write = 1;
	//myi2c.length_read = 1;
	printf("result = %d\n",BP.transact_i2c(num, &myi2c));
	
	return (int) (myi2c.buffer_read[0]);//+myi2c.buffer_read[1]+myi2c.buffer_read[2]);
	//return sonar1.cm;
}
int Sensors::readEncoder(void)
{
	BP.get_sensor(Rob.encoderRight,&coderRight);
	BP.get_sensor(Rob.encoderLeft,&coderLeft);
	return 0;
}
int Sensors::getEncoder(int32_t *right,int32_t *left)// valeur des codeurs, avec sémaphored
{
	*right = coderRight.angle;
	*left = coderLeft.angle;
	
	return 0;  
}

