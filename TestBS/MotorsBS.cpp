#include "mainBS.h"

MotorsBS::MotorsBS()
{}

int MotorsBS::setMotorSpeed(uint8_t num,int spd)
{
	BP.set_motor_dps(num, spd); // A voir si on met un coeff
	//printf("moteur\n");
	return 0;
}
int MotorsBS::setMotorPower(uint8_t num, int pow)
{
	BP.set_motor_power(num,pow);
	return 0 ;
}
