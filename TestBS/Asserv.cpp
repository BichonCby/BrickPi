#include "mainBS.h"

Asserv::Asserv()
{
	speedForMax = 500; // en mm/s
	speedRotMax = 90; // en °rob par seconde
	Conf.getConfig((char *)("KP_FOR"), &KP_FOR);
	Conf.getConfig((char *)("KP_ROT"), &KP_ROT);
	Conf.getConfig((char *)("ANGLE_CONVERGE"), &ANGLE_CONVERGE);
	Conf.getConfig((char *)("DIST_CONVERGE"), &DIST_CONVERGE);
	Conf.getConfig((char *)("DPSROB_TO_DPSWHL"), &DPSROB_TO_DPSWHL);
	Conf.getConfig((char *)("MMSROB_TO_DPSWHL"), &MMSROB_TO_DPSWHL);
	
	
}

int Asserv::calcAsserv()
{
	generateVirtualSpeed();
	//printf("type asserv %d\n",typeAss);
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
			deltaAngle = 0;
			break;
		case ASS_POLAR :
			if (abs(distance) > DIST_CONVERGE)
			{
				absAngle = modulo180(RAD2DEG*asinf(deltay/distance));
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
				absAngle = modulo180(RAD2DEG*asinf(deltay/distance));
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
		case ASS_PIVOT :
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
	
	// cas particulier du manuel
	if (typeAss == ASS_MANUAL)
	{
		speedForReq = (float)(speedRightMan+speedRightMan)/2;
		speedRotReq = (float)(speedRightMan-speedLeftMan);
	}
	// saturation des vitesses de consigne
	speedForReq = fmaxf(fminf(speedForReq,speedForMax),-speedForMax);
	speedRotReq = fmaxf(fminf(speedRotReq,speedRotMax),-speedRotMax);
	//return;
	
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
		//printf(".");
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
			Mot.setMotorSpeed(Rob.whlLeft,0);
			Mot.setMotorSpeed(Rob.whlRight,0);
			Mot.setMotorPower(Rob.whlLeft,0);
			Mot.setMotorPower(Rob.whlRight,0);
			break;
		case ASS_BLOCK :
			Mot.setMotorSpeed(Rob.whlLeft,0);
			Mot.setMotorSpeed(Rob.whlRight,0);
			break;
		case ASS_MANUAL :
			Mot.setMotorSpeed(Rob.whlRight,speedRightMan*MMSROB_TO_DPSWHL);
			Mot.setMotorSpeed(Rob.whlLeft,speedLeftMan*MMSROB_TO_DPSWHL);
			break;
		case ASS_POLAR:
		case ASS_POLARREV:
		case ASS_ROTATION:
		case ASS_CIRCLE:
			// roue droite
			tmp = (speedForReq*MMSROB_TO_DPSWHL+speedRotReq*DPSROB_TO_DPSWHL); // en dps roue
			// à voir pour saturer
			// à voir pour pratiquer une rampe
			Mot.setMotorSpeed(Rob.whlRight,(int)tmp);
			// roue gauche
			tmp = speedForReq*MMSROB_TO_DPSWHL-speedRotReq*DPSROB_TO_DPSWHL;
			// à voir pour saturer
			// à voir pour pratiquer une rampe
			Mot.setMotorSpeed(Rob.whlLeft,(int)tmp);
			break;
		case ASS_PIVOT:
			if (blockedWhl == Rob.whlLeft)
			{
				Mot.setMotorSpeed(Rob.whlLeft,0);
				Mot.setMotorSpeed(Rob.whlRight,speedRotReq*DPSROB_TO_DPSWHL);
			}
			else
			{
				Mot.setMotorSpeed(Rob.whlRight,0);
				Mot.setMotorSpeed(Rob.whlLeft,speedRotReq*DPSROB_TO_DPSWHL);
			}
			break;
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
	speedRotMax = 90; // il faudra mettre une calibration
	sleepms(20); // pour être sûr que c'est pris en compte
	while (!isConverge() && !Det.isObstacle() && Rob.getTypeMatch() != TYPE_REMOTE)
		sleepms(20);
	if (Det.isObstacle())
		return -1; // détection
	return 0;
}
int Asserv::goBackward(int x, int y, int speed)
{
	// mettre un sémaphore
	targetX = x;
	targetY = y;
	typeAss = ASS_POLARREV;
	speedForMax = speed;
	speedRotMax = 90; // il faudra mettre une calibration
	sleepms(20); // pour être sûr que c'est pris en compte
	while (!isConverge() && !Det.isObstacle() && Rob.getTypeMatch() != TYPE_REMOTE)
		sleepms(20);
	if (Det.isObstacle())
		return -1; // détection
	return 0;
}
int Asserv::turn(int a, int speed)
{
	// mettre un sémaphore
	printf("rotation\n");
	targetA = (float)a;
	typeAss = ASS_ROTATION;
	speedRotMax = (float)speed;
	speedForMax = 30; // il faudra mettre une calibration
	sleepms(20); // pour être sûr que c'est pris en compte
	//printf("tarA =%d vmax = %f\n",(int)targetA,speedRotMax);
	while (!isConverge() && !Det.isObstacle() && Rob.getTypeMatch() != TYPE_REMOTE)
		sleepms(20);
	if (Det.isObstacle())
		return -1; // détection
	return 0;
}
int Asserv::pivot(uint8_t blkWhl,uint8_t dir, int angle)
{
	blockedWhl = blkWhl;
	typeAss = ASS_PIVOT;
	sleepms(20);
	while (!isConverge() && !Det.isObstacle() && Rob.getTypeMatch() != TYPE_REMOTE)
		sleepms(20);
	if (Det.isObstacle())
		return -1; // détection
	return 0;
}
int Asserv::manualSpeed(int spdRight, int spdLeft)
{
	typeAss = ASS_MANUAL;
	speedLeftMan = spdLeft;
	speedRightMan = spdRight;
}
int Asserv::manualPower(int powerRight, int powerLeft)
{
	Mot.setMotorPower(Rob.whlLeft,powerLeft);
	Mot.setMotorPower(Rob.whlRight,powerRight);
}

int Asserv::stopRobot()
{
	typeAss = ASS_NUL;
	speedLeftMan =0; // utile?
	speedRightMan = 0; // utile?
}
int Asserv::checkBlocked() // détection de blocage, appel régulier
{
	float sf,sr;
	//Pos.getSpeed(&sf,&sr);
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
float Asserv::getSpeedForReq()
{
	return speedForReq;
}
int Asserv::getTarget(float *tarX,float *tarY, float *tarA, int *typ) 
{
	*tarX=targetX;
	*tarY=targetY;
	*tarA=targetA;
	*typ=typeAss;
	//printf("targetA = %d\n",(int)targetA);
	return 0;
}
int Asserv::getSpeed(float *spdFor, float *spdRot)
{
	*spdFor=speedForReq;
	*spdRot=speedRotReq;
	//printf("spdRot = %d\n",(int)(speedRotReq));
	return 0;
}
