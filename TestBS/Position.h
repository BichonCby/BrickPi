#ifndef __Position_h__
#define __Position_h__


class Position{
public:
Position(void); // constructeur
int calcPosition();// fonction récurrente
int setPosition(float x, float y, float a);
int getPosition(float *x, float *y, float *a);
int getSpeed(float *f, float *r);
// coefficients relus du fichier de config
private:
float COEFF_SPD_FOR=1.5;// pour obtenir une vitesse en m/s
float COEFF_SPD_ROT=1.0;// pour obtenir une vitesse en °/s
float COEFF_ANG=1;// pour obtenir un angle à partir des pas codeurs
float COEFF_MM=1;// pour obtenir une position en mm
int initPosition();
int32_t posWhlR;
int32_t posWhlL;
int32_t deltaPosWhlR;
int32_t deltaPosWhlL;
int32_t posWhlR_prev;
int32_t posWhlL_prev;
float speedFor;
float speedRot;
float posAlpha;
float posX;
float posY;

};

#endif
