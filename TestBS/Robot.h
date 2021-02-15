#ifndef __Robot_h__
#define __Robot_h__
#define DURATION_MATCH 5000 // en 20 ms

#define GREEN 0
#define BLUE 1
#define RED 2
#define YELLOW 3
#define PURPLE 4
#define BLACK 5
#define WHITE 6

#define MATCH_PREPARE 1
#define MATCH_IN_PROGRESS 2
#define MATCH_FUNNY 3
#define MATCH_DISPLAY 4
#define MATCH_END 5

#define TYPE_NORMAL 0
#define TYPE_REMOTE 1
#define TYPE_HOMOLO 2

#define TIRETTE Sen.getTouch(1)
#define BAU Sen.getTouch(2)

class Robot{
public:
Robot(void); // constructeur
int getColor();// 
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
uint8_t whlRight; // le numéro du moteur droit
uint8_t whlLeft; // le numéro du moteur gauche
uint8_t encoderLeft; // le port du codeur gauche
uint8_t encoderRight; // le port du codeur droit
uint8_t sonFrRight; // le numéro du sonar avant droit
uint8_t sonFrLeft; // le numéro du sonar avant gauche
uint8_t sonReRight;// le numéro du sonar arrière droit
uint8_t sonReLeft; // le numéro du sonar arrière gauche
private:
int color;
int stateMatch;
int counterMatch;
char versionRobot;
int typeMatch;
uint8_t score;
int initRobot();
bool externalEncoder;
};

#endif
