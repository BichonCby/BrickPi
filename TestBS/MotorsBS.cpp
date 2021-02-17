#include "mainBS.h"

MotorsBS::MotorsBS()
{}

int MotorsBS::setMotorSpeed(uint8_t num,int spd)
{
	if (num == Rob.whlRight)
		speedRight = (int16_t)spd;
	if (num == Rob.whlLeft)
		speedLeft = (int16_t)spd;
		
	BP.set_motor_dps(1 << (num-1), spd); // A voir si on met un coeff
	//printf("moteur\n");
	return 0;
}
int MotorsBS::setMotorPower(uint8_t num, uint8_t pow)
{
	if (num == Rob.whlRight)
		powerRight = (int8_t)pow;
	if (num == Rob.whlLeft)
		powerLeft = (int8_t)pow;
	BP.set_motor_power(1 << (num-1),pow);
	return 0 ;
}
int MotorsBS::getMotorsSpeed(int16_t *spdLeft,int16_t *spdRight)
{
	*spdLeft = speedLeft;
	*spdRight = speedRight;
	printf("motors speed D= %d G=%d\n",speedRight,speedLeft);
	return 0;
}
int MotorsBS::getMotorsPower(int8_t *powLeft,int8_t *powRight)
{
	*powLeft = powerLeft;
	*powRight = powerRight;
	return 0;
}
