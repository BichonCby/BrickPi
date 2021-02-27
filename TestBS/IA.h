#ifndef __IA_h__
#define __IA_h__


class IA{
public:
IA(void); // constructeur
int launchIA(); 

private:
void DoSquare();
void DoNothing(int t);
void StopSec(float ts);
};

#endif
