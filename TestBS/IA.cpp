#include "mainBS.h"

IA::IA()
{

}

int IA::launchIA()
{
	DoNothing(5000);
	//DoSquare();
	DoNothing(100000);
	
	//DoSquare();
	return 0;
}
void IA::DoSquare()
{
	Ass.goForward(1200,0,100);
	Ass.turn(90,30);
	Ass.goForward(1200,200,100);
	Ass.turn(180,30);
	Ass.goForward(1000,200,100);
	Act.OpenArms();
	Ass.turn(-90,30);
	Ass.goForward(1000,0,100);
	Ass.turn(0,30);
	return;
}
void IA::DoNothing(int t)
{
	sleepms(t);
	return;	
}
