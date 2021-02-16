#include "mainBS.h"

Detection::Detection()
{
}

int Detection::detect() // fonction appelée régulièrement
{
	uint8_t distFrR=255;
	uint8_t distFrL=255;
	uint8_t distReR=255;
	uint8_t distReL=255;
	// récupération des valeurs des sonars
	// on suppose que l'on a 2 devant et 2 derrière, mais ça peut être configurable
	if (Rob.getNbSonar(SIDE_FRONT)>0)
		distFrL = Sen.getSonar(Rob.sonFrLeft); // le gauche d'abord
	if (Rob.getNbSonar(SIDE_FRONT)>1)
		distFrR = Sen.getSonar(Rob.sonFrRight);
	if (Rob.getNbSonar(SIDE_REAR)>0)
		distReL = Sen.getSonar(Rob.sonReLeft);
	if (Rob.getNbSonar(SIDE_REAR)>1)
		distReR= Sen.getSonar(Rob.sonReRight);
//	printf("sonars %d %d %d %d\n",Rob.sonFrLeft,distFrL,Rob.sonReLeft,distReL);
	// il faut vérifier sonar par sonar que l'on ne détecte pas en dehors de la table
	
	// ensuite on renvoie le booléen de détection
	obstacle = false;
	if (Ass.getSpeedForReq() > 0 
		&& (distFrR < DIST_OBSTACLE_FRONT || distFrL < DIST_OBSTACLE_FRONT))
		obstacle = true;
	if (Ass.getSpeedForReq() < 0 
		&& (distReR < DIST_OBSTACLE_FRONT || distReL < DIST_OBSTACLE_FRONT))
		obstacle = true;
	return 0;
}

bool Detection::isObstacle()
{
	//return false;
	return obstacle;
}
