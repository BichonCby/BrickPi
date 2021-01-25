#include "mainBS.h"

Robot::Robot()
{
	initRobot();
}

int Robot::initRobot()
{
	color = GREEN;
	stateMatch = MATCH_PREPARE;
	score = 0;
	counterMatch = 0;
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
