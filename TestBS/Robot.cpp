#include "mainBS.h"

Robot::Robot()
{
	initRobot();
}

int Robot::initRobot()
{
	float val;
	Conf.getConfig((char *)"VERSION",&val);versionRobot = (char)val;
	Conf.getConfig((char *)"WHEEL_RIGHT",&val);whlRight = (uint8_t)val;
	Conf.getConfig((char *)"WHEEL_LEFT",&val);whlLeft = (uint8_t)val;
	Conf.getConfig((char *)"ENCODER_LEFT",&val);encoderLeft = (uint8_t)val;
	Conf.getConfig((char *)"ENCODER_RIGHT",&val);encoderRight = (uint8_t)val;
	Conf.getConfig((char *)"SCORE_INIT",&val);score = (int)val;
	Conf.getConfig((char *)"EXTERNAL_ENCODER",&val);externalEncoder = (bool)val;
	Conf.getConfig((char *)"NUMBER_FRONT_SONAR",&val);nbFrSonar = (uint8_t)val;
	Conf.getConfig((char *)"NUMBER_REAR_SONAR",&val);nbReSonar = (uint8_t)val;
	Conf.getConfig((char *)"SONAR_FRONT_LEFT",&val);sonFrLeft = (uint8_t)val;
	Conf.getConfig((char *)"SONAR_FRONT_RIGHT",&val);sonFrRight = (uint8_t)val;
	Conf.getConfig((char *)"SONAR_REAR_LEFT",&val);sonReLeft = (uint8_t)val;
	Conf.getConfig((char *)"SONAR_REAR_RIGHT",&val);sonReRight = (uint8_t)val;

	Conf.getConfig((char *)"TIRETTE_HW",&val);tiretteHW = (uint8_t)val;
	Conf.getConfig((char *)"TIRETTE_PORT",&val);tirettePort = (uint8_t)val;
	Conf.getConfig((char *)"TIRETTE_EV3",&val);tiretteEV3 = (uint8_t)val;
	Conf.getConfig((char *)"BAU_HW",&val);bauHW = (uint8_t)val;
	Conf.getConfig((char *)"BAU_PORT",&val);bauPort = (uint8_t)val;
	Conf.getConfig((char *)"BAU_EV3",&val);bauEV3 = (uint8_t)val;
	Conf.getConfig((char *)"COLOR_HW",&val);colorHW = (uint8_t)val;
	Conf.getConfig((char *)"COLOR_PORT",&val);colorPort = (uint8_t)val;
	Conf.getConfig((char *)"COLOR_EV3",&val);colorEV3 = (uint8_t)val;

	Conf.getConfig((char *)"ARM_PORT",&val);motArm = (uint8_t)val;

	Conf.getConfig((char *)"INIT_POS_X",&val);initPosX = (int)val;
	Conf.getConfig((char *)"INIT_POS_Y",&val);initPosY = (int)val;
	Conf.getConfig((char *)"INIT_POS_A",&val);initPosA = (int)val;
	
	color = BLUE; // sera à lire sur le contacteur
	color = YELLOW;
	stateMatch = MATCH_PREPARE;
	typeMatch = TYPE_NORMAL;
	//score = 0;
	counterMatch = 0;
	return 0;
}
char Robot::getVersion()
{
	return versionRobot;
}
int Robot::getColor()
{
	return color;
}
int Robot::setColor(int c)
{
	color = c;
	return 0;
}
bool Robot::isOpposite()
{
	return (getColor() == YELLOW);
}
uint8_t Robot::getScore()
{
	//printf("score = %d\n",score);
	return (uint8_t)score;
}
uint8_t Robot::setScore(int s)
{
	score = (uint8_t)s;
	return score;
}
uint8_t Robot::addScore(int s)
{
	score += (uint8_t)s;
	return score;
}
int Robot::setStateMatch(int st)
{
	stateMatch = st;
	return 0;
}
int Robot::getStateMatch()
{
	return stateMatch;
}
int Robot::getCounter()
{
	return counterMatch;
}
int Robot::setCounter(int c)
{
	return(counterMatch = c);
	//return 0;
}
int Robot::incCounter()
{
	return counterMatch++;
}
int Robot::getTypeMatch()
{
	return typeMatch;
}	
int Robot::setTypeMatch(int t)
{
	typeMatch = t;
	return 0;
}	
bool Robot::isExternalEncoder()
{
	return externalEncoder;
}
uint8_t Robot::getNbSonar(int side)
{
	if (side == SIDE_FRONT)
		return nbFrSonar;
	if (side == SIDE_REAR)
		return nbReSonar;
	return 0;
}

bool Robot::getSeqRun() { return seqRun;}
int Robot::setSeqRun() { seqRun = true;return 0;}
int Robot::resetSeqRun()  { seqRun = false;return 0;}
