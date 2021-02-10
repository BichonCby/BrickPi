#ifndef __Sensor_h__
#define __Sensor_h__

// structure for light sensor
struct sensor_encoder_t{
  int16_t  angle;
  int16_t  rpm;
};

class Sensors{
public:
Sensors(void); // constructeur
bool getTouch(int button); // on récupè-re la valeur du contacteur en booléen
int readEncoder(void); //lance la lecture des codeurs
int getSonar(uint8_t num); // valeur du sonar
int getEncoder(int *right,int *left); //
private :
};

#endif
