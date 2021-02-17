#ifndef __Sensor_h__
#define __Sensor_h__

#define HW_BUTTON_BP 0
#define HW_BUTTON_GPIO 1
#define HW_BUTTON_TCHMUX 2

// structure for light sensor
struct sensor_encoder_t{
  int32_t  angle;
  int16_t  rpm;
};

struct def_button_t{
	uint8_t type; // Brcikpi, GPIO, tchmux
	uint8_t port; // le port sur le HW sélectionné
	bool value; // la valeur du bouto (pas sûr d'en avoir besoin)
	bool ev3; // si c'est un capteur i2c
};
 
class Sensors{
public:
Sensors(void); // constructeur
int setTouchType(uint8_t id, uint8_t typ, uint8_t por);
bool getTouch(int id); // on récupè-re la valeur du contacteur en booléen
int readEncoder(void); //lance la lecture des codeurs
int getSonar(uint8_t num); // valeur du sonar
int getEncoder(int32_t *right,int32_t *left); //
private :
def_button_t button[8]; // on prévoit large
};

#endif
