#include "mainBS.h"

sensor_ultrasonic_t sonar1;
sensor_color_t      Color1;
sensor_encoder_t 	coderRight, coderLeft;
i2c_struct_t myi2c;
Sensors::Sensors()
{
	if (Rob.getNbSonar(SIDE_FRONT) > 0) // le gauche en priorité
		BP.set_sensor_type(Rob.sonFrLeft,SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.getNbSonar(SIDE_FRONT) > 1) // donc 2
		BP.set_sensor_type(Rob.sonFrRight,SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.getNbSonar(SIDE_REAR) > 0)
		BP.set_sensor_type(Rob.sonReLeft,SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.getNbSonar(SIDE_REAR) > 1)
		BP.set_sensor_type(Rob.sonReRight,SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.isExternalEncoder())
	{
		BP.set_sensor_type(Rob.encoderRight,SENSOR_TYPE_I2C);
		BP.set_sensor_type(Rob.encoderRight,SENSOR_TYPE_I2C);
	}
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
	int32_t val;
	if (Rob.isExternalEncoder())
	{
		BP.get_sensor(Rob.encoderRight,&coderRight);
		BP.get_sensor(Rob.encoderLeft,&coderLeft);
	}
	else // codeurs moteur
	{
		BP.get_motor_encoder(Rob.whlRight,coderRight.angle);
		//coderRight.angle=val;
		BP.get_motor_encoder(Rob.whlLeft,coderLeft.angle);
		//coderLeft.angle=val;
	}
	return 0;
}
int Sensors::getEncoder(int32_t *right,int32_t *left)// valeur des codeurs, avec sémaphored
{
	*right = coderRight.angle;
	*left = coderLeft.angle;
	
	return 0;  
}

