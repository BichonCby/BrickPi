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
	
	color = GREEN;
	stateMatch = MATCH_PREPARE;
	score = 0;
	counterMatch = 0;
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
	counterMatch = c;
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
