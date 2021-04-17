#ifndef __IA_h__
#define __IA_h__


class IA{
public:
IA(void); // constructeur
int launchIA(); 
char getMicroStep();
char getMacroStep();
private:
char microStep;
char macroStep;
void DoSquare();
void doPath();
void DoLineTurn();
void DoNothing(int t);
void StopSec(float ts);
};

#endif
