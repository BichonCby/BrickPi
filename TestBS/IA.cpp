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
	doPath();
	//DoLineTurn();
	//doPath();
	printf("fin IA\n");
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
void IA::doPath()
{
	int myx[50];// = {300,700,800};
	int myy[50];// = {1100,1000,1200};

	AStar::Generator generator;
    generator.setWorldSize({30, 21}); // tous les 10cm pour l'instant
    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(true);
    generator.addCollision({8,11});
    generator.addCollision({8,11});
    generator.addCollision({8,11});
    //generator.addCollision({1,2});
    //generator.addCollision({3,2});
    //generator.addCollision({1,3});
    //generator.addCollision({2,3});
    //generator.addCollision({3,3});
    
    std::cout << "Generate path ... \n";
    auto path = generator.findPath({10, 11}, {2, 11});// la fin puis le début, à inverser plus tard
	int i=0;
    for(auto& coordinate : path) {
		myx[i] = 100*coordinate.x;
		myy[i] = 100*coordinate.y;
		i++;
        std::cout << coordinate.x << " " << coordinate.y << "\n";
    }

	macroStep++;
	microStep=0;
	Ass.setPath(myx,myy,i);
	while (Ass.followPathForward(50)==-1)
		StopSec(2.0);
	return;
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
