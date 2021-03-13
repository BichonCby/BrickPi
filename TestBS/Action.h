#ifndef __Action_h__
#define __Action_h__

#define ARM_OFF 0
#define ARM_OPENING 1
#define ARM_OPEN 2
#define ARM_CLOSING 3
#define ARM_CLOSE 4

class Action{
public:
Action(void); // constructeur
int calcAction(); // fonction du séquenceur
int OpenArm();
int CloseArm();
int getStArm();
private:
int initAction();
int stArm;
int ctArm;
};

#endif
