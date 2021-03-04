#ifndef __Asserv_h__
#define __Asserv_h__

// * Types de déplacement
#define ASS_NUL 0
#define ASS_POLAR 1
#define ASS_POLARREV 2
#define ASS_ROTATION 3
#define ASS_MANUAL 4
#define ASS_PIVOT 6
#define ASS_CIRCLE 7
#define ASS_BLOCK 10

// défintion de la classe
class Asserv{
public:
Asserv(void); // constructeur
int calcAsserv(); // fonction appelée dans la séquence 20ms
// Fonctions de demande de déplacement
int goForward(int x, int y, int speed); // primitive polaire avec cible
int goBackward(int x, int y, int speed);  // primitive polaire à l'envers avec cible
int turn(int angle, int speed); // rotation jusqu'à l'angle défini
int pivot(uint8_t blkWhl,uint8_t dir, int angle);
int manualSpeed(int spdRight, int spdLeft);
int manualPower(int powerRight, int powerLeft);
int stopRobot(); // pour demander un déplacement nul
// accesseurs
float getSpeedForReq();
int getTarget(float *tarX,float *tarY, float *tarA, int *typ); 
int getSpeed(float *spdFor, float *spdRot);
bool isConverge(); // indique si l'on est convergé
bool isBlocked(); // indique un blocage confirmé du robot
private:
// Calibration qui seront lues dans le fichier de config
float DIST_CONVERGE = 10; // en mm
float ANGLE_CONVERGE = 10; // en degré
float KP_FOR = 1;
float KP_ROT = 1;
float DPSROB_TO_DPSWHL = 1;
float MMSROB_TO_DPSWHL = 1;
// cartographie pour asserv polaire
float facSpdForAxe[2]= {15,30}; // les angles points d'appui
float facSpdForVal[2]= {1,0}; // le coefficient de prise en compte de l'avance par rapport à la rotation
// fonctions internes à la classe
void generateVirtualSpeed(void);
void driveWheels(void);
int checkBlocked();
// varibales internes à la classe
float targetX;
float targetY;
float targetA;
int typeAss;
float speedRotReq;
float speedForReq;
float speedForMax;
float speedRotMax;
int speedRightMan;
int speedLeftMan;
bool converge;
bool blocked;
int cntBlock;
uint8_t blockedWhl;

};

#endif
