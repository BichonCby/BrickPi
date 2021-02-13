#ifndef __Sensor_h__
#define __Sensor_h__

// structure for light sensor
struct sensor_encoder_t{
  int32_t  angle;
  int16_t  rpm;
};

class Sensors{
public:
Sensors(void); // constructeur
bool getTouch(int button); // on récupè-re la valeur du contacteur en booléen
int readEncoder(void); //lance la lecture des codeurs
int getSonar(uint8_t num); // valeur du sonar
int getEncoder(int32_t *right,int32_t *left); //
private :
};

#endif
