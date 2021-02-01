#include "mainBS.h"

Detection::Detection()
{
}

int Detection::detect() // fonction appelée régulièrement
{
	// récupération des valeurs des sonars
	// on suppose que l'on a 2 devant et 2 derrière, mais ça peut être configurable
	
	uint8_t distFrR = Sen.getSonar(Rob.sonFrRight);
	uint8_t distFrL = Sen.getSonar(Rob.sonFrLeft);
	uint8_t distReR = Sen.getSonar(Rob.sonReRight);
	uint8_t distReL = Sen.getSonar(Rob.sonReLeft);
	
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
	return obstacle;
}
