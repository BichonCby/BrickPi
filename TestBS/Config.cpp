#include "mainBS.h"

Config::Config()
{
	readConfig();
	writeConfig(); // pour le test
}

int Config::readConfig()
{
    char ligne[100];
    char calib[100];
    float val;
    int idx=0;
    FILE * pfile;
    pfile = fopen("config.ini","r");
    if (pfile == NULL) {fputs("File error",stdout); return -1;}
    while (fgets(ligne,100,pfile)!=NULL)
    {
		if (ligne[0] == '#' && idx == 0) // en tete
		{
			strcat(heading,ligne);
			printf("commentaire\n");// on peut conserver ces lignes pour la ré-écriture
		}
		else if (ligne[0] != '#' && strlen(ligne) > 5) // on supprime les lignes vides et les têtes de chapitre
		{
			sscanf(ligne,"%s = %f",elConf[idx].name, &elConf[idx].val);
			idx++;
		}
/*		else if (strncmp("COEFF_SPD_FOR",ligne,strlen("COEFF_SPD_FOR"))==0)
			sscanf(ligne,"%s = %f",calib, &Pos.COEFF_SPD_FOR);
		else if (strncmp("COEFF_SPD_ROT",ligne,strlen("COEFF_SPD_ROT"))==0)
			sscanf(ligne,"%s = %f",calib, &Pos.COEFF_SPD_ROT);
		else if (strncmp("COEFF_ANG",ligne,strlen("COEFF_ANG"))==0)
			sscanf(ligne,"%s = %f",calib, &Pos.COEFF_ANG);
		else if (strncmp("COEFF_MM",ligne,strlen("COEFF_MM"))==0)
			sscanf(ligne,"%s = %f",calib, &Pos.COEFF_MM);*/
	}
	nbConf=idx;
	printf("%s = %f\n",elConf[0].name, elConf[0].val);
    fclose(pfile);
    return 0;
}
int Config::setConfig(char *nom,float v)
{
	for (int i = 0; i<nbConf;i++)
	{
		if (strcmp(nom,elConf[i].name) == 0)
		{
			elConf[i].val = v;
			return i;
		}
	}
	return -1; // non trouvé
}
int Config::getConfig(char *nom,float *v)
{
	for (int i = 0; i<nbConf;i++)
	{
		if (strcmp(nom,elConf[i].name) == 0)
		{
			*v = elConf[i].val;
			return i;
		}
	}
	return -1; // non trouvé
}
int Config::writeConfig()
{
    FILE * pfile;
    pfile = fopen("configout.ini","w");
    if (pfile == NULL) {fputs("File error",stdout); return -1;}
	fprintf(pfile,"%s",heading);
	for (int i=0;i<nbConf;i++)
		fprintf(pfile,"%s = %f\n",elConf[i].name,elConf[i].val);
	fclose(pfile);
	return 0;
}
