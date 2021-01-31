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
	Conf.getConfig((char *)"SCORE_INIT",&val);score = (int)val;
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
int Robot::getScore()
{
	return score;
}
int Robot::setScore(int s)
{
	score = s;
	return score;
}
int Robot::addScore(int s)
{
	score += s;
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
