#ifndef __MotorsBS_h__
#define __MotorsBS_h__
class MotorsBS{
public:
MotorsBS(void); // constructeur
int setMotorSpeed(uint8_t num,int spd); 
int setMotorPower(uint8_t num, uint8_t pow);
int getMotorsSpeed(int16_t *spdLeft,int16_t *spdRight);
int getMotorsPower(int8_t *powLeft,int8_t *powRight);
private :
int16_t speedRight;
int16_t speedLeft;
int8_t powerLeft;
int8_t powerRight;
};

#endif
