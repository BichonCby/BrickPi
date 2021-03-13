#ifndef __Robot_h__
#define __Robot_h__
#define DURATION_MATCH 5000 // en 20 ms

#define BLUE 0
#define YELLOW 1
#define RED 2
#define GREEN 3
#define PURPLE 4
#define BLACK 5
#define WHITE 6

#define MATCH_PREPARE 0
#define MATCH_WAIT 1
#define MATCH_IN_PROGRESS 2
#define MATCH_FUNNY 3
#define MATCH_DISPLAY 4
#define MATCH_END 5

#define TYPE_NORMAL 0
#define TYPE_REMOTE 1
#define TYPE_HOMOLO 2

#define SIDE_FRONT 1
#define SIDE_REAR 2
#define SIDE_LEFT 3
#define SIDE_RIGHT 4

#define TIRETTE Sen.getTouch(BUTTON_TIRETTE)
#define BAU Sen.getTouch(BUTTON_BAU)
#define COLOR Sen.getTouch(BUTTON_COLOR)
//#define BAU false

#define BUTTON_TIRETTE 0
#define BUTTON_BAU 1
#define BUTTON_COLOR 2

class Robot{
public:
Robot(void); // constructeur
int getColor();// 
bool isOpposite();
int setColor(int color);
uint8_t getScore();
uint8_t setScore(int s);
uint8_t addScore(int s);
int setStateMatch(int st);
int getStateMatch();
int getCounter();
int setCounter(int c);
int incCounter();
char getVersion();
int setTypeMatch(int t);
int getTypeMatch();
bool isExternalEncoder();
bool getSeqRun();
int setSeqRun();
int resetSeqRun();
uint8_t getNbSonar(int side);
uint8_t whlRight; // le numéro du moteur droit
uint8_t whlLeft; // le numéro du moteur gauche
uint8_t encoderLeft; // le port du codeur gauche
uint8_t encoderRight; // le port du codeur droit
uint8_t sonFrRight; // le numéro du sonar avant droit
uint8_t sonFrLeft; // le numéro du sonar avant gauche
uint8_t sonReRight;// le numéro du sonar arrière droit
uint8_t sonReLeft; // le numéro du sonar arrière gauche
uint8_t tiretteHW;
uint8_t tirettePort;
uint8_t tiretteEV3;
uint8_t bauHW;
uint8_t bauPort;
uint8_t bauEV3;
uint8_t colorHW;
uint8_t colorPort;
uint8_t colorEV3;
uint8_t motArm; // le port du moteur du bras
int initPosX;
int initPosY;
int initPosA;

private:
int color;
int stateMatch;
int counterMatch;
char versionRobot;
int typeMatch;
bool seqRun; 
uint8_t score;
int initRobot();
bool externalEncoder;
uint8_t nbFrSonar; // nombre de sonars à l'avant
uint8_t nbReSonar; // nombre de sonars à l'avant

};

#endif
