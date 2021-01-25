#include "mainBS.h"

MotorsBS::MotorsBS()
{}

int MotorsBS::setMotorSpeed(int num,int spd)
{
	return 0;
}
int MotorsBS::setMotorPower(int num, int pow)
{
	BP.set_motor_power(num,pow);
	return 0 ;
}
