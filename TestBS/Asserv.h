#ifndef __Asserv_h__
#define __Asserv_h__

// * Types de déplacement
#define ASS_NUL 0
#define ASS_POLAR 1
#define ASS_POLARREV 2
#define ASS_ROTATION 3
#define ASS_MANUAL 4
#define ASS_PIVOT 5
#define ASS_CIRCLE 6
#define ASS_BLOCK 10


class Asserv{
public:
Asserv(void); // constructeur
int calcAsserv(); 
bool isConverge();
bool isBlocked();
int goForward(int x, int y, int speed); 
int goBackward(int x, int y, int speed); 
int turn(int a, int speed);
private:
float DIST_CONVERGE = 10; // en mm
float ANGLE_CONVERGE = 10; // en degré
float KP_FOR = 1;
float KP_ROT = 1;
void generateVirtualSpeed(void);
void driveWheels(void);
int checkBlocked();
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
};

#endif
