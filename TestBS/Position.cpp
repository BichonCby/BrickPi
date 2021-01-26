#include "mainBS.h"


float modulo180(float angle)
{
	if (angle <= -180.0)
		return (angle + 360.0);
	if (angle >180.0)
		return (angle - 360.0);
	return angle;
}
Position::Position()
{
		Conf.getConfig((char *)("COEFF_SPD_FOR"), &COEFF_SPD_FOR);
		Conf.getConfig((char *)("COEFF_SPD_ROT"), &COEFF_SPD_ROT);
		Conf.getConfig((char *)("COEFF_ANG"), &COEFF_ANG);
		Conf.getConfig((char *)("COEFF_MM"), &COEFF_MM);
		//printf("%d mon coeff est %f\n",i,COEFF_SPD_FOR); 
}

int Position::initPosition(void) // voir si on l'intègre dans le constructeur
{
	return 0;
}
int Position::calcPosition() // fonction récurrente
{
	// récupération des codeurs
	Sen.getEncoder(&posWhlR, &posWhlL); 
	// calcul des deltas et mémorisation
	deltaPosWhlR = posWhlR - posWhlR_prev;
	posWhlR_prev = posWhlR;
	deltaPosWhlL = posWhlL - posWhlL_prev;
	posWhlL_prev = posWhlL;
	
	// protection débordement
	deltaPosWhlR = modulo180(deltaPosWhlR);
	deltaPosWhlL = modulo180(deltaPosWhlL);
	//if (deltaPosWhlR > 180)
		//deltaPosWhlR -= 360;
	//else if (deltaPosWhlR <-180)
		//deltaPosWhlR +=360;
	//if (deltaPosWhlL > 180)
		//deltaPosWhlL -= 360;
	//else if (deltaPosWhlL <-180)
		//deltaPosWhlL +=360;
		
	// Calcul de vitesses
	speedFor = (float)(deltaPosWhlR + deltaPosWhlL)*COEFF_SPD_FOR;
	speedRot = (float)(deltaPosWhlR - deltaPosWhlL)*COEFF_SPD_ROT;
	
	// calcul de la position
	float deltaAng = (float) (deltaPosWhlR-deltaPosWhlL)*COEFF_ANG;
	posAlpha = modulo180(posAlpha + deltaAng);
	
	float deltaX = (deltaPosWhlR+deltaPosWhlL);
	deltaX *= cos(DEG2RAD*(posAlpha-deltaAng/2));
	deltaX *= COEFF_MM;
	posX += deltaX;
	float deltaY = (deltaPosWhlR+deltaPosWhlL);
	deltaY *= sin(DEG2RAD*(posAlpha-deltaAng/2));
	deltaY *= COEFF_MM;
	posY += deltaY;
	
	return 0;
}
int Position::setPosition(float x, float y, float a)
{
	posX = x;
	posY = y;
	posAlpha = a;
	return 0;
}
int Position::getPosition(float *x, float *y, float *a)
{
	
	return 0;
}
