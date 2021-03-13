#include "mainBS.h"

Action::Action()
{
	// il va falloir configurer les moteurs
	initAction();
}

int Action::initAction()
{
	stArm = ARM_OFF;
	return 0;
}

int Action::calcAction()
{
	switch (stArm)
	{
		case ARM_OFF :
			// pas d'activation
			break;
		case ARM_OPEN :
			BP.set_motor_power(Rob.motArm,30);
			ctArm = 50;
			stArm = ARM_OPENING;
		case ARM_OPENING:
//			BP.setMotorPower(Rob.motArm,30);
			if (ctArm-- <=0)
			{
				BP.set_motor_power(Rob.motArm,0);
				stArm = ARM_OFF;
			}
			// action
			break;
		case ARM_CLOSE :
			BP.set_motor_power(Rob.motArm,-30);
			ctArm = 50;
			stArm = ARM_CLOSING;
		case ARM_CLOSING:
			if (ctArm-- <=0)
			{
				BP.set_motor_power(Rob.motArm,0);
				stArm = ARM_OFF;
			}
			break;
		default :
			// on retourne à OFF
			stArm = ARM_OFF;
			break;
	}
	return 0;
}

int Action::OpenArm()
{
	// test de l'état du bras déjà
	// puis on active
	stArm = ARM_OPEN;
	return 0;
}
int Action::CloseArm()
{
	// test de l'état du bras déjà
	// puis on active
	stArm = ARM_CLOSE;
	return 0;
}
