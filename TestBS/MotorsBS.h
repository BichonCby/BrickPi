#ifndef __MotorsBS_h__
#define __MotorsBS_h__
class MotorsBS{
public:
MotorsBS(void); // constructeur
int setMotorSpeed(uint8_t num,int spd); 
int setMotorPower(uint8_t num, int pow);
};

#endif
