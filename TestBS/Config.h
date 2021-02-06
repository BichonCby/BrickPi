#ifndef __Config_h__
#define __Config_h__

struct conf_element {
	char name[50];
	float val;
	char comment[50];
	char category[15];
};
class Config{
public:
Config(void); // constructeur
int readConfig();// fonction 
int getConfig(char *nom,float *v);
int setConfig(char *nom,float v);
int writeConfig();
private:
struct conf_element elConf[50];
int nbConf;
char heading[1000];
};

#endif
