#include "mainBS.h"

IA::IA()
{

}

int IA::launchIA()
{
	DoSquare();
	DoNothing(1000);
	DoSquare();
	return 0;
}
void IA::DoSquare()
{
	Ass.goForward(200,0,30);
	Ass.turn(90,30);
	Ass.goForward(200,200,30);
	Ass.turn(90,30);
	Ass.goForward(0,200,30);
	Ass.turn(90,30);
	Ass.goForward(0,0,30);
	Ass.turn(90,30);
	return;
}
void IA::DoNothing(int t)
{
	sleepms(t);
	return;	
}
