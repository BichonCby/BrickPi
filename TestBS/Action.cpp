#include "mainBS.h"

Action::Action()
{
	initAction();
}

int Action::initAction()
{
	stArms = ARMS_OFF;
}

int Action::calcAction()
{
	switch (stArms)
	{
		case ARMS_OFF :
			// pas d'activation
			break;
		case ARMS_OPENING:
			// action
			break;
		default :
			// on retourne à OFF
			stArms = ARMS_OFF;
			break;
	}
	return 0;
}

int Action::OpenArms()
{
	// test de l'état du bras déjà
	// puis on active
	stArms = ARMS_OPENING;
	return 0;
}
