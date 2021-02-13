
#include "Emul.h"
#include "mainBS.h"

Emul::Emul(uint8_t addr){

  Address = addr;
  for (int i=0;i<4;i++)
  {
    motEmul[i].position = 0;
    motEmul[i].power = 0;
    motEmul[i].speed = 0;
    motEmul[i].state = 0; //ah bon?
    sonEmul[i] = 255; // pas de détection
  }
  codEmul[1] = 0;
  codEmul[2] = 0;
  touchEmul = 0;
  
}

#define COEFF_MOT_COD 0.02 // a vérifier
int Emul::runEmul() // fonction appelée régulièmerement
{
  // pour la propulsion, on va prendre les moteurs et calculer les codeurs
  // TODO plus tard, provoquer des blocages en bordure
  // pour les autres actionneurs, on va juste calculer une position au cas où
  // pour les sonars et autres capteurs, faut voir
  
  for (int i=0;i<4;i++) // attention, les ports commencent à 1
  {
    motEmul[i].position += (float)(motEmul[i].speed) * COEFF_MOT_COD;
  }
  // recalculer les valeurs des codeurs, chercher quel codeur va avec quel moteur
  codEmul[1] = (int32_t)(motEmul[1].position); // c'est faux, il faut recalculer
  codEmul[2] = (int32_t)(motEmul[2].position); // c'est faux, il faut recalculer
  
  // pour les sonars, on verra après
  
  // pour le tchmux aussi
  
  return 0;
}
int Emul::detect(bool critical){
  
  return 0;
}

int Emul::get_manufacturer(char *str){
  return 0;
}

int Emul::get_board(char *str){
  return 1;
}

int Emul::get_version_hardware(char *str){
  return 0;
}

int Emul::get_version_firmware(char *str){
  return 0;
}

int Emul::get_id(char *str){
  // A voir si on complète ou pas
  return 0;
}

int  Emul::set_led(uint8_t value){
  return 0;
}

float Emul::get_voltage_3v3(){
  return 3.3;
}

int Emul::get_voltage_3v3(float &voltage){
  voltage = 3.3;
  return 0;
}

float Emul::get_voltage_5v(){
  
  return 5;
}

int Emul::get_voltage_5v(float &voltage){
  voltage = 5;
  return 0;
}

float Emul::get_voltage_9v(){
  return 9;
}

int Emul::get_voltage_9v(float &voltage){
  voltage = 9.0;
  return 0;
}

float Emul::get_voltage_battery(){
  return 12;
}

int Emul::get_voltage_battery(float &voltage){
  voltage = 12;
  return 0;
}

int Emul::set_sensor_type(uint8_t port, uint8_t type, uint16_t flags, i2c_struct_t *i2c_struct){
   
  return 0;
}

int Emul::transact_i2c(uint8_t port, i2c_struct_t *i2c_struct){

  return 0;
}

int Emul::get_sensor(uint8_t port, void *value_ptr){
// TODO : gérer si les codeurs sont indépendants
  if (port == Rob.encoderRight)
  {
      sensor_encoder_t *Value = (sensor_encoder_t*)value_ptr;
      Value->angle = codEmul[1];
  }
  if (port == Rob.encoderLeft)
  {
      sensor_encoder_t *Value = (sensor_encoder_t*)value_ptr;
      Value->angle = codEmul[2];
  }
    
  // Get some commonly used values
/*  uint8_t  raw_value_8 = spi_array_in[6];
  uint16_t raw_value_16 = ((spi_array_in[6] << 8) | spi_array_in[7]);
  uint16_t raw_value_16_2 = ((spi_array_in[8] << 8) | spi_array_in[9]);

  // For each sensor type, copy the value(s) into the corresponding structure value(s)
  if(SensorType[port_index] == SENSOR_TYPE_TOUCH ||
     SensorType[port_index] == SENSOR_TYPE_TOUCH_NXT ||
     SensorType[port_index] == SENSOR_TYPE_TOUCH_EV3){
    sensor_touch_t *Value = (sensor_touch_t*)value_ptr;
    Value->pressed = raw_value_8;
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_ULTRASONIC){
    sensor_ultrasonic_t *Value = (sensor_ultrasonic_t*)value_ptr;
    Value->cm = raw_value_8;
    Value->inch = raw_value_8 / 2.54;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_COLOR_REFLECTED){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->reflected_red = raw_value_8;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_COLOR_AMBIENT){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->ambient = raw_value_8;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_COLOR_COLOR){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->color = raw_value_8;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_ULTRASONIC_LISTEN){
    sensor_ultrasonic_t *Value = (sensor_ultrasonic_t*)value_ptr;
    Value->presence = raw_value_8;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_INFRARED_PROXIMITY){
    sensor_infrared_t *Value = (sensor_infrared_t*)value_ptr;
    Value->proximity = raw_value_8;
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_LIGHT_ON){
    sensor_light_t *Value = (sensor_light_t*)value_ptr;
    Value->reflected = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_LIGHT_OFF){
    sensor_light_t *Value = (sensor_light_t*)value_ptr;
    Value->ambient = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_COLOR_RED){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->reflected_red = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_COLOR_GREEN){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->reflected_green = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_COLOR_BLUE){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->reflected_blue = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_COLOR_OFF){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->ambient = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_GYRO_ABS){
    sensor_gyro_t *Value = (sensor_gyro_t*)value_ptr;
    Value->abs = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_GYRO_DPS){
    sensor_gyro_t *Value = (sensor_gyro_t*)value_ptr;
    Value->dps = raw_value_16;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_ULTRASONIC_CM){
    sensor_ultrasonic_t *Value = (sensor_ultrasonic_t*)value_ptr;
    Value->cm = raw_value_16 / 10.0;
    Value->inch = raw_value_16 / 25.4;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_ULTRASONIC_INCHES){
    sensor_ultrasonic_t *Value = (sensor_ultrasonic_t*)value_ptr;
    Value->cm = raw_value_16 * 0.254;
    Value->inch = raw_value_16 / 10.0;
  }else if(SensorType[port_index] == SENSOR_TYPE_CUSTOM){
    sensor_custom_t *Value = (sensor_custom_t*)value_ptr;
    Value->adc1 = (((spi_array_in[8] & 0x0F) << 8) | spi_array_in[9]);
    Value->adc6 = (((spi_array_in[8] >> 4) & 0x0F) | (spi_array_in[7] << 4));
    Value->pin5 = (spi_array_in[6] & 0x01);
    Value->pin6 = ((spi_array_in[6] >> 1) & 0x01);
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_COLOR_RAW_REFLECTED){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->reflected_red = raw_value_16;
    //Value-> = raw_value_16_2; not sure what this value is
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_GYRO_ABS_DPS){
    sensor_gyro_t *Value = (sensor_gyro_t*)value_ptr;
    Value->abs = raw_value_16;
    Value->dps = raw_value_16_2;
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_INFRARED_REMOTE){
    sensor_infrared_t *Value = (sensor_infrared_t*)value_ptr;
    for(uint8_t v = 0; v < 4; v++){
      switch(spi_array_in[6 + v]){
        case 1:
          Value->remote[v] = REMOTE_BIT_RED_UP;
        break;
        case 2:
          Value->remote[v] = REMOTE_BIT_RED_DOWN;
        break;
        case 3:
          Value->remote[v] = REMOTE_BIT_BLUE_UP;
        break;
        case 4:
          Value->remote[v] = REMOTE_BIT_BLUE_DOWN;
        break;
        case 5:
          Value->remote[v] = REMOTE_BIT_RED_UP | REMOTE_BIT_BLUE_UP;
        break;
        case 6:
          Value->remote[v] = REMOTE_BIT_RED_UP | REMOTE_BIT_BLUE_DOWN;
        break;
        case 7:
          Value->remote[v] = REMOTE_BIT_RED_DOWN | REMOTE_BIT_BLUE_UP;
        break;
        case 8:
          Value->remote[v] = REMOTE_BIT_RED_DOWN | REMOTE_BIT_BLUE_DOWN;
        break;
        case 9:
          Value->remote[v] = REMOTE_BIT_BROADCAST;
        break;
        case 10:
          Value->remote[v] = REMOTE_BIT_RED_UP | REMOTE_BIT_RED_DOWN;
        break;
        case 11:
          Value->remote[v] = REMOTE_BIT_BLUE_UP | REMOTE_BIT_BLUE_DOWN;
        break;
        default:
          Value->remote[v] = 0;
        break;
      }
    }
  }else if(SensorType[port_index] == SENSOR_TYPE_NXT_COLOR_FULL){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->color           = spi_array_in[6];
    Value->reflected_red   = ((spi_array_in[ 7] << 2) | ((spi_array_in[11] >> 6) & 0x03));
    Value->reflected_green = ((spi_array_in[ 8] << 2) | ((spi_array_in[11] >> 4) & 0x03));
    Value->reflected_blue  = ((spi_array_in[ 9] << 2) | ((spi_array_in[11] >> 2) & 0x03));
    Value->ambient         = ((spi_array_in[10] << 2) | ( spi_array_in[11]       & 0x03));
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_COLOR_COLOR_COMPONENTS){
    sensor_color_t *Value = (sensor_color_t*)value_ptr;
    Value->reflected_red   = ((spi_array_in[ 6] << 8) | spi_array_in[ 7]);
    Value->reflected_green = ((spi_array_in[ 8] << 8) | spi_array_in[ 9]);
    Value->reflected_blue  = ((spi_array_in[10] << 8) | spi_array_in[11]);
    //Value-> = ((spi_array_in[12] << 8) | spi_array_in[13]); not sure what this value is
  }else if(SensorType[port_index] == SENSOR_TYPE_EV3_INFRARED_SEEK){
    sensor_infrared_t *Value = (sensor_infrared_t*)value_ptr;
    for(uint8_t v = 0; v < 4; v++){
      Value->heading [v] = spi_array_in[6 + (v * 2)];
      Value->distance[v] = spi_array_in[7 + (v * 2)];
    }
  }else if(SensorType[port_index] == SENSOR_TYPE_I2C){
    i2c_struct_t *Value = (i2c_struct_t*)value_ptr;
    for(uint8_t b = 0; b < I2CInBytes[port_index]; b++){
      Value->buffer_read[b] = spi_array_in[6 + b];
    }
  }*/
  return SENSOR_STATE_VALID_DATA;
}

int Emul::set_motor_power(uint8_t port, int8_t power){
  // définir une courbe de vitesse en fonction de la puissance
  
  return 0;
}

int Emul::set_motor_position(uint8_t port, int32_t position){
  // on va juste mettre une tempo avant de l'atteindre, non?
  return 0;
}

int Emul::set_motor_position_relative(uint8_t port, int32_t position){
 // on va juste mettre une tempo avant de l'atteindre
  return ERROR_NONE;
}

int Emul::set_motor_dps(uint8_t port, int16_t dps){
  motEmul[port].speed = (int16_t)dps; // avec une rampe?
  //printf("consigne moteur %d = %d\n",port, dps);
  return 0;
}

int Emul::set_motor_limits(uint8_t port, uint8_t power, uint16_t dps){
  // A voir si on utilise ou pas
  return 0;
}

int Emul::get_motor_status(uint8_t port, uint8_t &state, int8_t &power, int32_t &position, int16_t &dps){
  state = motEmul[port].state;
  power = motEmul[port].power;
  dps = motEmul[port].speed;
  position = (int32_t)(motEmul[port].position);

  return 0;
}

int Emul::offset_motor_encoder(uint8_t port, int32_t position){
  motEmul[port].position += position;
  return 0;
}

int Emul::reset_motor_encoder(uint8_t port){
  motEmul[port].position = 0.0;
  
  return 0;
}

int Emul::reset_motor_encoder(uint8_t port, int32_t &value){
  motEmul[port].position = 0.0;
  value = 0;
  return 0;
}

int Emul::set_motor_encoder(uint8_t port, int32_t value){
  // A verfifier ce que doit faire la fonction !!
  motEmul[port].position = value;
  return 0;
}

int32_t Emul::get_motor_encoder(uint8_t port){
  return (int32_t)(motEmul[port].position);
}

int Emul::get_motor_encoder(uint8_t port, int32_t &value){

  value = (int32_t)(motEmul[port].position);
  return 0;
}

int Emul::reset_all(){
  for (int i=0;i<4;i++)
    motEmul[i].speed = 0;
  return 0;
}
