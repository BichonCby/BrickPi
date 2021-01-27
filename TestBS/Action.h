#ifndef __Action_h__
#define __Action_h__

#define ARMS_OFF 0
#define ARMS_OPENING 1
#define ARMS_OPEN 2
#define ARMS_CLOSING 3
#define ARMS_CLOSE 4

class Action{
public:
Action(void); // constructeur
int calcAction(); // fonction du séquenceur
int OpenArms();
int CloseArms();
int getStArms();
private:
int initAction();
int stArms;
};

#endif
