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
    char categ[50]="* no categ";
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
//			printf("commentaire\n");// on peut conserver ces lignes pour la ré-écriture
		}
		else if (ligne[0] == '*') // category
		{
			sscanf(ligne,"%s",categ);
		}
		else if (ligne[0] != '#' && strlen(ligne) > 5) // on supprime les lignes vides et les têtes de chapitre
		{
			sscanf(ligne,"%s = %f",elConf[idx].name, &elConf[idx].val);
			if (strstr(ligne,"//")!=NULL)
				strcpy(elConf[idx].comment,strstr(ligne,"//"));
			else
				strcpy(elConf[idx].comment,"//");
			//char *tmp=ligne+strstr(ligne,"//");
			//strcpy(elConf[idx].comment,((char*)ligne)+strstr(ligne,"//"));
			strcpy(elConf[idx].category,categ);
			idx++;
		}
	}
	nbConf=idx;
//	printf("%s = %f %s\n",elConf[0].name, elConf[0].val, elConf[0].comment);
//	printf("%s = %f %s\n",elConf[1].name, elConf[1].val, elConf[1].comment);
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
    char cat[50]=" nul";
    if (pfile == NULL) {fputs("File error",stdout); return -1;}
	fprintf(pfile,"%s\n",heading);
	for (int i=0;i<nbConf;i++)
	{
		if (strcmp(elConf[i].category,cat)) // c'est different
			fprintf( pfile,"\n%s\n\n",elConf[i].category);
		strcpy(cat,elConf[i].category);
		fprintf(pfile,"%s = %f\n",elConf[i].name,elConf[i].val);
	}
	fclose(pfile);
	return 0;
}
