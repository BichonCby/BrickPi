#ifndef __Detection_h__
#define __Detection_h__

#define DIST_OBSTACLE_FRONT 20

// défintion de la classe
class Detection{
public:
Detection(void); // constructeur
int detect(); // fonction appelée depuis l'extérieur pour lancer la detection
bool isObstacle();
private:
bool obstacle;

};

#endif
