#include "mainBS.h"

sensor_ultrasonic_t sonar1;
sensor_color_t      Color1;
sensor_encoder_t 	coderRight, coderLeft;
i2c_struct_t myi2c;
Sensors::Sensors()
{
	BP.set_sensor_type(Rob.sonFrLeft,SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(Rob.sonFrRight,SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(Rob.sonReLeft,SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(Rob.sonReRight,SENSOR_TYPE_NXT_ULTRASONIC);
}

bool Sensors::getTouch(int button)
{
	return 0;
}
int Sensors::getSonar(uint8_t num) // valeur du sonar
{
	BP.get_sensor(num,&sonar1);
	return (sonar1.cm);
	//myi2c.address = 0x02;
	//myi2c.length_write = 1;
	//myi2c.length_read = 1;
	//printf("result = %d\n",BP.transact_i2c(num, &myi2c));
	
	return 0;//(int) (myi2c.buffer_read[0]);//+myi2c.buffer_read[1]+myi2c.buffer_read[2]);
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

