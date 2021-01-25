#include "mainBS.h"

sensor_ultrasonic_t sonar1;
Sensors::Sensors()
{
	BP.set_sensor_type(PORT_1,SENSOR_TYPE_NXT_ULTRASONIC);
}

bool Sensors::getTouch(int button)
{
	return 0;
}
float Sensors::getSonar(uint8_t num) // valeur du sonar
{
	BP.get_sensor(num,&sonar1);
	
	return sonar1.cm;
}
int Sensors::readEncoder(void)
{
	return 0;
}
int Sensors::getEncoder(int *right,int *left)// valeur des codeurs, avec sémaphored
{
	return 0;
}

