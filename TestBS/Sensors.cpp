#include "mainBS.h"

sensor_ultrasonic_t sonar1;
sensor_color_t      Color1;
sensor_encoder_t 	coderRight, coderLeft;
i2c_struct_t myi2c;
Sensors::Sensors()
{
	// Attention, les ports de config vont de 1 à 4
	// les ports brickpi vont de 0x01 à 0x08
	if (Rob.getNbSonar(SIDE_FRONT) > 0) // le gauche en priorité
		BP.set_sensor_type((1 << (Rob.sonFrLeft-1)),SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.getNbSonar(SIDE_FRONT) > 1) // donc 2
		BP.set_sensor_type((1 << (Rob.sonFrRight-1)),SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.getNbSonar(SIDE_REAR) > 0)
		BP.set_sensor_type((1 << (Rob.sonReLeft-1)),SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.getNbSonar(SIDE_REAR) > 1)
		BP.set_sensor_type((1 << (Rob.sonReRight-1)),SENSOR_TYPE_NXT_ULTRASONIC);
	if (Rob.isExternalEncoder())
	{
		BP.set_sensor_type((1 << (Rob.encoderRight-1)),SENSOR_TYPE_I2C);
		BP.set_sensor_type((1 << (Rob.encoderRight-1)),SENSOR_TYPE_I2C);
	}
	// ajouter la config des contacteurs
	if (Rob.tiretteHW == HW_BUTTON_BP){ // si la tirette est un port BP
		if (Rob.tiretteEV3)
			BP.set_sensor_type(1 << (Rob.tirettePort-1),SENSOR_TYPE_TOUCH_EV3);
		else
			BP.set_sensor_type(1 << (Rob.tirettePort-1),SENSOR_TYPE_TOUCH);
	}
	if (Rob.bauHW == HW_BUTTON_BP){ // si la tirette est un port BP
		if (Rob.bauEV3)
			BP.set_sensor_type(1 << (Rob.bauPort-1),SENSOR_TYPE_TOUCH_EV3);
		else
			BP.set_sensor_type(1 << (Rob.bauPort-1),SENSOR_TYPE_TOUCH);
	}
	if (Rob.colorHW == HW_BUTTON_BP){ // si la couleur est un port BP
		if (Rob.colorEV3)
			BP.set_sensor_type(1 << (Rob.colorPort-1),SENSOR_TYPE_TOUCH_EV3);
		else
			BP.set_sensor_type(1 << (Rob.colorPort-1),SENSOR_TYPE_TOUCH);
	}
	setTouchType(BUTTON_TIRETTE,Rob.tiretteHW,Rob.tirettePort);
//	printf("tirette %d type %d port %d\n",BUTTON_TIRETTE,Rob.tiretteHW,Rob.tirettePort);
	setTouchType(BUTTON_BAU,Rob.bauHW,Rob.bauPort);
//	printf("BAU %d type %d port %d\n",BUTTON_BAU,Rob.bauHW,Rob.bauPort);
	setTouchType(BUTTON_COLOR,Rob.colorHW,Rob.colorPort);
//	printf("BAU %d type %d port %d\n",BUTTON_BAU,Rob.bauHW,Rob.bauPort);
}

int Sensors::setTouchType(uint8_t id, uint8_t typ, uint8_t por)
{
	button[id].type = typ;
	button[id].port = por;
	button[id].value = false;
	return 0;
}
bool Sensors::getTouch(int id)
{
	sensor_touch_t btn;
	if (button[id].type == HW_BUTTON_BP)
	{
		//printf("bouton %d\n",id);
		BP.get_sensor((1 << (button[id].port-1)),&btn);
		return btn.pressed;
	}
	if (button[id].type == HW_BUTTON_GPIO)
	{
		return (digitalRead(button[id].port) == 1);
	}
	if (button[id].type == HW_BUTTON_TCHMUX)
	{
		// TODO voir comment on fait avec le tchmux
		return button[id].value;
	}

	return false;
}
int Sensors::getSonar(uint8_t num) // valeur du sonar
{
	BP.get_sensor((1<< (num-1)),&sonar1);
	return (sonar1.cm);
}
int Sensors::readEncoder(void)
{
	if (Rob.isExternalEncoder())
	{
		BP.get_sensor(1<< (Rob.encoderRight-1),&coderRight);
		BP.get_sensor(1<< (Rob.encoderLeft-1),&coderLeft);
	}
	else // codeurs moteur
	{
		BP.get_motor_encoder(1<< (Rob.whlRight-1),coderRight.angle);
		BP.get_motor_encoder(1<< (Rob.whlLeft-1),coderLeft.angle);
	}
	return 0;
}
int Sensors::getEncoder(int32_t *right,int32_t *left)// valeur des codeurs, avec sémaphored
{
	*right = coderRight.angle;
	*left = coderLeft.angle;
	
	return 0;  
}

