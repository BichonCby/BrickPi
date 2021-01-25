#include "mainBS.h"

Asserv::Asserv()
{
	speedForMax = 30;
	speedRotMax = 30;
	}

int Asserv::calcAsserv()
{
	generateVirtualSpeed();
	driveWheels();
	return 0;
}

void Asserv::generateVirtualSpeed(void)
{
	float curX, curY, curA;
	Pos.getPosition(&curX, &curY, &curA);
	float deltax = targetX - curX;
	float deltay = targetY - curY;
	float distance = sqrtf(deltax*deltax + deltay*deltay);
	float absAngle, deltaAngle;
	// calcul des deltas en distance et en angle selon le type
	switch (typeAss)
	{
		case ASS_NUL :
			distance = 0;
			break;
		case ASS_POLAR :
			if (abs(distance) > DIST_CONVERGE)
			{
				absAngle = modulo180(RAD2DEG*asinf(-deltay/distance));
				if (targetX < curX)
					absAngle = 180-absAngle;
				deltaAngle = modulo180(absAngle-curA);
			}
			else // on est trop près
			{
				absAngle = curA;
				deltaAngle = 0;
			}
			// changement de repère
			if (abs(deltaAngle) > 90 && abs(distance) < 200)
			{
				distance = -distance;
				deltaAngle = modulo180(deltaAngle+180);
			}
			break;
		case ASS_POLARREV :
			if (abs(distance) > DIST_CONVERGE)
			{
				absAngle = modulo180(RAD2DEG*asinf(-deltay/distance));
				if (targetX < curX)
					absAngle = 180-absAngle;
				deltaAngle = modulo180(absAngle-curA);
			}
			else // on est trop près
			{
				absAngle = curA+180;
				deltaAngle = 180;
			}
			distance = -distance; // marche arrière
			deltaAngle = deltaAngle + 180; // a voir pourquoi !!!
			
			// changement de repère A SIMPLIFIER
			if (abs(deltaAngle) > 90 && abs(distance) < 200)
			{
				distance = -distance;
				deltaAngle = modulo180(deltaAngle+180);
			}
			
			break;
		case ASS_ROTATION :
			deltaAngle = modulo180(targetA-curA);
			distance = 0;
			break;
		default :
			break;
	}
	// calcul des vitesses de consigne
	// en attendant la carto, on fait à l'ancienne
	if (abs(deltaAngle)>30.0)
		speedForReq = 0;
	else if (abs(deltaAngle) > 15.0)
		speedForReq = KP_FOR*distance*(2-2*deltaAngle/300);
	else
		speedForReq = KP_FOR*distance;
		
	// gestion de l'arrive vite (A faire)
	
	// rotation
	speedRotReq = KP_ROT*deltaAngle;
	
	// saturation des vitesses de consigne
	speedForReq = fmaxf(fminf(speedForReq,speedForMax),-speedForMax);
	speedRotReq = fmaxf(fminf(speedRotReq,speedRotMax),-speedRotMax);
	return;
	
	// détermination de la convergence
	if (  (typeAss == ASS_NUL)
		||(typeAss == ASS_POLAR && abs(distance) < DIST_CONVERGE)
		||(typeAss == ASS_POLARREV && abs(distance) < DIST_CONVERGE)
		||(typeAss == ASS_ROTATION && abs(deltaAngle) < ANGLE_CONVERGE)
		)
	{
		converge = true;
		speedForReq = 0;
		speedRotReq = 0;
	}
	else
		converge = false;

}

void Asserv::driveWheels(void)
{
	switch (typeAss)
	{
		case ASS_NUL :
		default :
			Mot.setMotorPower(1,0);
			break;
		case ASS_MANUAL :
			Mot.setMotorSpeed(1,speedRightMan);
			Mot.setMotorSpeed(2,speedLeftMan);
			break;
	}
	return;
	
}

bool Asserv::isConverge(void)
{
	return converge;
}

void Asserv::goForward(int x, int y, int speed)
{
	// mettre un sémaphore
	targetX = x;
	targetY = y;
	typeAss = ASS_POLAR;
	speedForMax = speed;
	speedRotMax = 30; // il faudra mettre une calibration
	sleepms(20); // pour être sûr que c'est pris en compte
	while (isConverge() == false)
		sleepms(20);
	return;
}
void Asserv::turn(int a, int speed)
{
	// mettre un sémaphore
	targetA = a;
	typeAss = ASS_ROTATION;
	speedRotMax = speed;
	speedForMax = 30; // il faudra mettre une calibration
	usleep(20000); // pour être sûr que c'est pris en compte
	while (isConverge() == false)
		usleep(20000);
	return;
}
	
