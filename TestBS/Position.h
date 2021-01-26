#ifndef __Position_h__
#define __Position_h__


class Position{
public:
Position(void); // constructeur
int calcPosition();// fonction récurrente
int setPosition(float x, float y, float a);
int getPosition(float *x, float *y, float *a);
// coefficients relus du fichier de config
float COEFF_SPD_FOR=1.5;// pour obtenir une vitesse en m/s
float COEFF_SPD_ROT=1;// pour obtenir une vitesse en °/s
float COEFF_ANG=1;// pour obtenir un angle à partir des pas codeurs
float COEFF_MM=1;// pour obtenir une position en mm
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
