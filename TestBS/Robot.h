#ifndef __Robot_h__
#define __Robot_h__
#define DURATION_MATCH 1000 // en 100 ms

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

#define TIRETTE Sen.getTouch(1)
#define BAU Sen.getTouch(2)

class Robot{
public:
Robot(void); // constructeur
int getColor();// 
int setColor(int color);
int getScore();
int setScore(int s);
int addScore(int s);
int setStateMatch(int st);
int getStateMatch();
int getCounter();
int setCounter(int c);
int incCounter();
private:
int color;
int stateMatch;
int counterMatch;
int versionRobot;
int typeMatch;
int score;
int initRobot();

};

#endif
