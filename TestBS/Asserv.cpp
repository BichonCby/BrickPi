#include "mainBS.h"

Asserv::Asserv()
{
	speedForMax = 30;
	speedRotMax = 30;
	Conf.getConfig((char *)("KP_FOR"), &KP_FOR);
	Conf.getConfig((char *)("KP_ROT"), &KP_ROT);
	Conf.getConfig((char *)("ANGLE_CONVERGE"), &ANGLE_CONVERGE);
	Conf.getConfig((char *)("DIST_CONVERGE"), &DIST_CONVERGE);
}

int Asserv::calcAsserv()
{
	generateVirtualSpeed();
	driveWheels();
	checkBlocked();
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
	float tmp;
	switch (typeAss)
	{
		case ASS_NUL :
		default :
			Mot.setMotorPower(Rob.whlLeft,0);
			Mot.setMotorPower(Rob.whlRight,0);
			break;
		case ASS_BLOCK :
			Mot.setMotorSpeed(Rob.whlLeft,0);
			Mot.setMotorSpeed(Rob.whlRight,0);
			break;
		case ASS_MANUAL :
			Mot.setMotorSpeed(Rob.whlRight,speedRightMan);
			Mot.setMotorSpeed(Rob.whlLeft,speedLeftMan);
			break;
		case ASS_POLAR:
		case ASS_POLARREV:
		case ASS_ROTATION:
		case ASS_CIRCLE:
		case ASS_PIVOT:
			// roue droite
			tmp = speedForReq+speedRotReq;
			// à voir pour saturer
			// à voir pour pratiquer une rampe
			Mot.setMotorSpeed(Rob.whlRight,(int)tmp);
			// roue gauche
			tmp = speedForReq-speedRotReq;
			// à voir pour saturer
			// à voir pour pratiquer une rampe
			Mot.setMotorSpeed(Rob.whlLeft,(int)tmp);
	}
	return;
	
}

bool Asserv::isConverge(void)
{
	return converge;
}

int Asserv::goForward(int x, int y, int speed)
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
	return 0;
}
int Asserv::goBackward(int x, int y, int speed)
{
	// mettre un sémaphore
	targetX = x;
	targetY = y;
	typeAss = ASS_POLARREV;
	speedForMax = speed;
	speedRotMax = 30; // il faudra mettre une calibration
	sleepms(20); // pour être sûr que c'est pris en compte
	while (isConverge() == false)
		sleepms(20);
	return 0;
}
int Asserv::turn(int a, int speed)
{
	// mettre un sémaphore
	targetA = a;
	typeAss = ASS_ROTATION;
	speedRotMax = speed;
	speedForMax = 30; // il faudra mettre une calibration
	usleep(20000); // pour être sûr que c'est pris en compte
	while (isConverge() == false)
		usleep(20000);
	return 0;
}
int Asserv::checkBlocked() // détection de blocage, appel régulier
{
	float sf,sr;
	Pos.getSpeed(&sf,&sr);
	// il faudra y mettre des calibrations de config
	if (	(speedForReq > 30 && sf <20)
		||	(speedForReq < -30 && sr >-20))
		cntBlock +=1;
	else
		cntBlock -=2;
	if (cntBlock > 14)
	{
		blocked = true;
		cntBlock = 14;
	}
	if (cntBlock < 0)
	{
		blocked = false;
		cntBlock = 0;
	}
	return 0;
}
bool Asserv::isBlocked()
{
	return blocked;
}
	
