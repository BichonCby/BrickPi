#ifndef __Sensor_h__
#define __Sensor_h__
class Sensors{
public:
Sensors(void); // constructeur
bool getTouch(int button); // on récupè-re la valeur du contacteur en booléen
int readEncoder(void); //lance la lecture des codeurs
float getSonar(uint8_t num); // valeur du sonar
int getEncoder(int *right,int *left); //
};

#endif
