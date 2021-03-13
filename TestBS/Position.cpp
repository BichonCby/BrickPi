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
	Conf.getConfig((char *)("COEFF_SPD_FOR"), &COEFF_SPD_FOR);
	Conf.getConfig((char *)("COEFF_SPD_ROT"), &COEFF_SPD_ROT);
	Conf.getConfig((char *)("COEFF_ANG"), &COEFF_ANG);
	Conf.getConfig((char *)("COEFF_MM"), &COEFF_MM);
	// récupération des codeurs
	Sen.getEncoder(&posWhlR, &posWhlL);  
	// calcul des deltas et mémorisation
	deltaPosWhlR = posWhlR - posWhlR_prev;
	posWhlR_prev = posWhlR;
	deltaPosWhlL = posWhlL - posWhlL_prev;
	posWhlL_prev = posWhlL; 
	
	// protection débordement
	if (Rob.isExternalEncoder()) // pour les codeurs externes, on boucle à 360
	{
		deltaPosWhlR = modulo180(deltaPosWhlR);
		deltaPosWhlL = modulo180(deltaPosWhlL);
	}
		
	// Calcul de vitesses
	speedFor = (float)(deltaPosWhlR + deltaPosWhlL)*COEFF_SPD_FOR; // en mm/s
	speedRot = (float)(deltaPosWhlR - deltaPosWhlL)*COEFF_SPD_ROT; // en °/s
	
	// calcul de la position
	float deltaAng = (float) (deltaPosWhlR-deltaPosWhlL)*COEFF_ANG;
	if (Rob.isOpposite()) // on est de l'autre côté
		deltaAng = -deltaAng;
	posAlpha = modulo180(posAlpha + deltaAng);
	
	float deltaX = (float)(deltaPosWhlR+deltaPosWhlL);
	deltaX *= cos(DEG2RAD*(posAlpha-deltaAng/2));
	deltaX *= COEFF_MM;
	posX += deltaX;
	float deltaY = (float)(deltaPosWhlR+deltaPosWhlL);
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
	*x=posX;
	*y=posY;
	*a=posAlpha;
	//printf("position (%d,%d,%d)\n",(int)posX,int(posY),(int)posAlpha);
	return 0;
}
int Position::getSpeed(float *f, float *r)
{
	*f=speedFor;
	*r=speedRot;
//	printf("vit Av= %d Rot=%f\n",(int)speedFor,speedRot);
	return 0;
}
