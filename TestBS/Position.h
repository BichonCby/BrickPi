#ifndef __Position_h__
#define __Position_h__

#define COEFF_SPD_FOR 1 // pour obtenir une vitesse en m/s
#define COEFF_SPD_ROT 1 // pour obtenir une vitesse en °/s
#define COEFF_ANG 1 // pour obtenir un angle à partir des pas codeurs
#define COEFF_MM 1 // pour obtenir une position en mm
class Position{
public:
Position(void); // constructeur
int calcPosition();// fonction récurrente
int setPosition(float x, float y, float a);
int getPosition(float *x, float *y, float *a);
private:
int initPosition();
int posWhlR;
int posWhlL;
int deltaPosWhlR;
int deltaPosWhlL;
int posWhlR_prev;
int posWhlL_prev;
float speedFor;
float speedRot;
float posAlpha;
float posX;
float posY;

};

#endif
