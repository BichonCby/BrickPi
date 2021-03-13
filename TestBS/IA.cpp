#include "mainBS.h"

IA::IA()
{

}

int IA::launchIA()
{
	macroStep=0;microStep=0;
	//Act.OpenArm();
	//DoNothing(5000);
	//Act.CloseArm();
	//DoNothing(5000);
	DoLineTurn();
	return 0;
	DoSquare();printf("1\n");
	DoNothing(5000);
	DoSquare();printf("2\n");
	DoNothing(5000);
	DoSquare();printf("3\n");
	DoSquare();printf("4\n");
	DoSquare();printf("5\n");
	DoSquare();printf("6\n");
	DoSquare();printf("7\n");
	DoSquare();printf("8\n");
	DoSquare();printf("9\n");
	DoSquare();printf("10\n");
	DoSquare();printf("11\n");
	DoSquare();printf("12\n");
	DoSquare();printf("13\n");
	DoSquare();printf("14\n");
	DoSquare();printf("15\n");
	DoSquare();printf("16\n");
	DoNothing(100000);
	
	//DoSquare();
	return 0;
}
void IA::DoSquare()
{
	macroStep++;
	microStep=0;
	while (Ass.goForward(1200,0,50)==-1)
		StopSec(2.0);
	microStep++;
	while (Ass.turn(90,100)==-1)
		StopSec(2.0);
	microStep++;
	while (Ass.goForward(1200,200,50)==-1)
		StopSec(2.0);
	microStep++;
	while (Ass.turn(180,100)==-1)
		StopSec(2.0);
	microStep++;
	while (Ass.goForward(1000,200,50)==-1)
		StopSec(2.0);
	//while (Act.OpenArms();
	microStep++;
	while (Ass.turn(-90,100)==-1)
		StopSec(2.0);
	microStep++;
	while (Ass.goForward(1000,0,50)==-1)
		StopSec(2.0);
	microStep++;
	while (Ass.turn(0,100)==-1)
		StopSec(2.0);
	microStep++;
	return;
}
void IA::DoLineTurn()
{
	macroStep++;
	microStep=0;
	while (Ass.goForward(300,1100,50)==-1)
		StopSec(2.0);
	microStep++;
	while (Ass.turn(90,100)==-1)
		StopSec(2.0);
}
void IA::DoNothing(int t)
{
	sleepms(t);
	return;	
}
void IA::StopSec(float ts)
{
	printf("stop2\n");
	Ass.stopRobot();
	sleeps (ts);
	return;
}
char IA::getMacroStep()
{
	return macroStep;
}
char IA::getMicroStep()
{
	return microStep;
}
