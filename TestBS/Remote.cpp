#include "mainBS.h"
//#include <fcntl.h>
#include <sys/stat.h>
//#include <sys/types.h>
Remote::Remote()
{}

void Remote::threadRemote()
{
	
	int fd;
	char input;
	const char * fifowrite = "/tmp/RobBSmap"; // du robot vers l'outil
	const char * fiforead = "/tmp/BSmapRob"; // de l'outil vers le robot
	
	// création des fifo, au cas où elles n'existeraient pas
	if (mkfifo(fiforead,0777) != 0 || mkfifo(fifowrite,0777) != 0)
	{
		// si on est là, c'est peut être parce que le fichier existe, ce n'est pas grave en soi
		//printf("imposible de créer le pipe, communication impossible \n");
		//return;
	}
	printf("pipe OK\n");
	stRemote = STREMOTE_WAIT_RECEIVE; // init
	// on va lire et attendre de recevoir quelque chose
	// début de la machine à état pour la com remote
	// pour l'instant on boucle en lecture écriture
	Pos.setPosition(1000,0,0);
	while (1)
	{
		switch (stRemote)
		{
			case STREMOTE_WAIT_RECEIVE :
				fd=open(fiforead,O_RDONLY);
				if (fd != -1) // la lecture s'est bien passée
				{
					//printf("Reçu par le lecteur :\n");
					// traitement
					sizeRead=0;
					while(read(fd,&input,1)>0)
					{
						strRead[sizeRead++] = input;
						//decodeByte(input);
					}
					decodeFrame();
					//printf("La lecture se termine!\n");
				}
				else
					printf("pipe HS\n"); // oups!
				close (fd);
				stRemote = STREMOTE_WAIT_SEND;
				break;
			case STREMOTE_WAIT_SEND :
				fd=open(fifowrite,O_WRONLY);
				//printf("écriture de la réponse\n");
				if (fd != -1)
				{
					write(fd,strWrite, sizeWrite);
					//sprintf(message, "bonjour du writer %d [%d]\n",compteur++,getpid());
				}
				else
					printf("pipe HS\n");
		
				close (fd);
				stRemote = STREMOTE_WAIT_RECEIVE;
				break;
			default :
				break;
		}
	}
	return;
}

// fonction de décodage de la trame reçue. Pour l'instant on écrit
int Remote::decodeFrame()
{
	float ftmp1;
	//float ftmp2;
	int itmp1, itmp2;
	char ctmp1,ctmp2,ctmp3;
	char name[50];
	//for (int j=0;j<sizeRead;j++)
		//printf("%x ",strRead[j]);
	//printf("\n");
	if (sizeRead <3 || strRead[2] != Rob.getVersion())
	{
		printf("mauvaise taille %d ou version\n",sizeRead);
		encodeFrame(ID_NACK,ERR_VERSION);
		return -1;
	}
	// vérification du CS et de la longueur
	if ((strRead[1]+4) !=sizeRead)
	{
		printf("mauvaise longueur\n");
		encodeFrame(ID_NACK,ERR_SIZE_FRAME);
		return -2;
	} // a compléter par le CS
	// décodage
	switch(strRead[0])
	{
		case ID_INFO : // on demande a lire une trame
			// on peut tout vérifier
			//printf("demande de la trame ");
			encodeFrame(strRead[3]);
			break;
		case ID_ORDERMOVE : // on donne un ordre de déplacement 
			// on va traiter l'ordre, une fois qu'on saura comment faire
			printf("ordre reçu\n");
			Rob.setTypeMatch(TYPE_REMOTE); // si on donne un ordre, on prend la main
			switch (strRead[3]) // le type de déplacement
			{
				case 'P':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					itmp2 = (int16_t)((int)strRead[6] + ((int)(strRead[7]))*256);
					Ass.goForward(itmp1,itmp2,100); // voir comment on récupère la vitesse
					break;
				case 'I':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					itmp2 = (int16_t)((int)strRead[6] + ((int)(strRead[7]))*256);
					Ass.goBackward(itmp1,itmp2,100); // voir comment on récupère la vitesse
					break;
				case 'R':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					Ass.turn(itmp1,100); // voir comment on récupère la vitesse
					break;
				case 'M':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					itmp2 = (int16_t)((int)strRead[6] + ((int)(strRead[7]))*256);
					printf("manuel d=%d et g=%d\n",itmp1, itmp2);
					Ass.manualSpeed(itmp1,itmp2); // voir comment on récupère la vitesse
					break;
				case 'N':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					itmp2 = (int16_t)((int)strRead[6] + ((int)(strRead[7]))*256);
					Ass.manualPower(itmp1,itmp2); // voir comment on récupère la vitesse
					break;
				case 'S': // stop robot
					printf("stop1\n");
					Ass.stopRobot();
					break;
				default :
					encodeFrame(ID_NACK,ERR_BAD_MOVE);
					return -1*(int)ERR_BAD_MOVE;
					break;
			}
			encodeFrame(ID_ACK);
			break;
		case ID_PARAM : // on va demander un truc sur la calibration
			printf("param %c\n",strRead[4]);
			if (strRead[4] == 'S') // on impose une nouvelle valeur
			{
				sscanf(strRead,"%c%c%cCS %s %f",&ctmp1,&ctmp2,&ctmp3,name,&ftmp1);
				printf ("demande de passer %s à la valeur %f\n",name,ftmp1);
				if (Conf.setConfig(name,ftmp1)==-1)
				{
					printf("Mauvais paramètre demandé\n");
					encodeFrame(ID_NACK,ERR_PARAM_NAME);
					return -1*(int)ERR_PARAM_NAME;
				}
			}
			else if (strRead[4] == 'G') // on récupère une valeur
			{
				sscanf(strRead,"%c%c%cCG %s",&ctmp1,&ctmp2,&ctmp3,name);
				printf ("demande de récupérer %s \n",name);
				if(Conf.getConfig(name,&calibf) >= 0)
				{ // récupération dans calibf
					encodeFrame(ID_CALGET);
					return 0;
				}
				encodeFrame(ID_NACK,ERR_PARAM_NAME);
				return -1*(int)ERR_PARAM_NAME;
			}
			else if (strRead[4] == 'W') // on écrit le fichier
			{
				sscanf(strRead,"CS %s %f",name,&ftmp1);
				printf ("demande d'écriture du fichier\n");
				Conf.writeConfig();
			}
			else
			{
				encodeFrame(ID_NACK,ERR_PARAM_REQ);
				return -1*(int)ERR_PARAM_REQ;
			}
			encodeFrame(ID_ACK);
			break;
		case ID_BUTTON : // n'a de sens qu'en emulation
#ifdef EMULATOR
			printf("bouton %c\n",strRead[3]);
			BP.changeButton(strRead[3]);
#endif
			encodeFrame(ID_ACK);
			break;
		case ID_OBSTACLE : // n'a de sens qu'en emulation
#ifdef EMULATOR
			printf("obstacle change\n");
			BP.changeObstacle();
#endif
			encodeFrame(ID_ACK);
			break;
		default : // on ne connait pas la trame
			encodeFrame(ID_NACK,ERR_BAD_ID);
			printf("Trame reçue inconnue\n");
			return -1*(int)ERR_BAD_ID;
			break;
	}
	return 0;
}

int Remote::encodeFrame(char id, char err)
{
	// l'identifiant est la question
	// si c'est un ordre, on va répondre OK avec le numéro de l'ordre
	// si c'est un requete de trame, on renvoie la trame
	float valf1, valf2, valf3;
	int vali1;
	uint16_t i;
	int16_t vali10,vali11;
	int32_t vali20,vali21;
	int8_t valc1,valc2;
	char calstr[20];
	switch (id)
	{
		case ID_ACK :
			strWrite[0] = ID_ACK; // identifiant
			strWrite[1] = 2;// taille utile hors version
			strWrite[2] = Rob.getVersion();
			strWrite[3] = strRead[3]; // l'odre donné
			
			strWrite[4] = ACK_OK; // pour l'instant tout est OK
			strWrite[5] = checkSum();
			sizeWrite = strWrite[1]+4;
			break;
		case ID_NACK :
		default :
			printf("oups\n");
			strWrite[0] = ID_NACK; // identifiant
			strWrite[1] = 2;// taille utile hors version
			strWrite[2] = Rob.getVersion();
			strWrite[3] = strRead[3]; // l'odre donné
			
			strWrite[4] = err; // pour l'instant tout est OK
			strWrite[5] = checkSum();
			sizeWrite = strWrite[1]+4;
			break;
		case ID_POSITION :
			//printf("Position.\n");
			strWrite[0] = ID_POSITION;
			strWrite[1] = 10;// taille utile
			strWrite[2] = Rob.getVersion();
			Pos.getPosition(&valf1,&valf2,&valf3);
			strWrite[3] = (char) ((int)valf1 & 0x00FF); // pos X poids faible
			strWrite[4] = (char) (((int)valf1)>>8 & 0x00FF); // pos X poids fort
			strWrite[5] = (char) ((int)valf2 & 0x00FF); // pos Y poids faible
			strWrite[6] = (char) (((int)valf2)>>8 & 0x00FF); // pos Y poids fort
			strWrite[7] = (char) ((int)valf3 & 0x00FF); // pos A poids faible
			strWrite[8] = (char) (((int)valf3)>>8 & 0x00FF); // pos A poids fort
			Pos.getSpeed(&valf1,&valf2);
			strWrite[9] = (char) ((int)valf1 & 0x00FF); // vit longi  poids faible;
			strWrite[10] = (char) (((int)valf1)>>8 & 0x00FF); // vit longi PF
			strWrite[11] = (char) ((int)valf2 & 0x00FF); // vit rot pf
			strWrite[12] = (char) (((int)valf2)>>8 & 0x00FF); // vit rot PF
			strWrite[13] = checkSum();
			// ...
			sizeWrite = strWrite[1]+4;
			Pos.getPosition(&valf1,&valf2,&valf3);
			//printf("pos (X,Y,A) = %d %d %d\n",(int)valf1, (int)valf2, (int)valf3);
			//printf("posX pf %d\n",strWrite[3]);
			//printf("posX pF %d\n",strWrite[4]);
			break;
		case ID_ASSERV :
			strWrite[0] = ID_ASSERV;
			strWrite[1] = 13;// taille utile
			strWrite[2] = Rob.getVersion();
			Ass.getTarget(&valf1,&valf2,&valf3,&vali1);
			//printf("asser %d x = %d y = %d a = %d ",(int)vali1,(int)valf1,(int)valf2,(int)valf3);
			strWrite[3] = (char) (vali1); // type de déplacement
			strWrite[4] = (char) ((int)valf1 & 0x00FF); // tar X poids faible
			strWrite[5] = (char) (((int)valf1)>>8 & 0x00FF); // tar X poids fort
			strWrite[6] = (char) ((int)valf2 & 0x00FF); // tar Y poids faible
			strWrite[7] = (char) (((int)valf2)>>8 & 0x00FF); // tar Y poids fort
			strWrite[8] = (char) ((int)valf3 & 0x00FF); // tar A poids faible
			strWrite[9] = (char) (((int)valf3)>>8 & 0x00FF); // tar A poids fort
			Ass.getSpeed(&valf1,&valf2);
			//printf("asser Vlongi = %d Vrot = %d\n",(int)valf1,(int)valf2);
			strWrite[10] = (char) ((int)valf1 & 0x00FF); // vit longi  poids faible;
			strWrite[11] = (char) (((int)valf1)>>8 & 0x00FF); // vit longi PF
			strWrite[12] = (char) ((int)valf2 & 0x00FF); // vit rot pf
			strWrite[13] = (char) (((int)valf2)>>8 & 0x00FF); // vit rot PF
			strWrite[14] = (char) (Ass.isConverge()); // convergence
			//printf("conv %d detect %d\n",(int)Ass.isConverge(),(int)Det.isObstacle());
			strWrite[15] = (char) (Ass.isBlocked()); // blocage
			strWrite[16] = checkSum();
			sizeWrite = strWrite[1]+4;
			break;
		case ID_MOTORS :
			strWrite[0] = ID_MOTORS;
			strWrite[1] = 6;// taille utile
			strWrite[2] = Rob.getVersion();
			Mot.getMotorsSpeed(&vali10,&vali11);
			strWrite[3] = (char) ((int)vali10 & 0x00FF); // tar X poids faible
			strWrite[4] = (char) (((int)vali10)>>8 & 0x00FF); // tar X poids fort
			strWrite[5] = (char) ((int)vali11 & 0x00FF); // tar Y poids faible
			strWrite[6] = (char) (((int)vali11)>>8 & 0x00FF); // tar X poids fort
			Mot.getMotorsPower(&valc1,&valc2);
			strWrite[7] = valc1;
			strWrite[8] = valc2;
			strWrite[9] = checkSum();
			sizeWrite = strWrite[1]+4;
			break;
		case ID_SENSORS :
			strWrite[0] = ID_SENSORS;
			strWrite[1] = 8;// taille utile
			strWrite[2] = Rob.getVersion();
			Sen.getEncoder(&vali20,&vali21);
			printf("codeurs D : %d  G : %d touch %d\n",vali20,vali21, (int)Sen.getTouch(BUTTON_TIRETTE));
			strWrite[3] = (char) ((int)vali20 & 0x00FF); // codeur droit poids faible
			strWrite[4] = (char) (((int)vali20)>>8 & 0x00FF); // codeur droit poids fort
			strWrite[5] = (char) ((int)vali21 & 0x00FF); // codeur gauche poids faible
			strWrite[6] = (char) (((int)vali21)>>8 & 0x00FF); // codeur gauche poids fort
			strWrite[7] = (unsigned char)(Sen.getSonar(Rob.sonFrLeft));
			strWrite[8] = (unsigned char)(Sen.getSonar(Rob.sonFrRight));
			strWrite[9] = (unsigned char)(Sen.getSonar(Rob.sonReLeft));
			strWrite[10] = (unsigned char)(Sen.getSonar(Rob.sonReRight));
			printf("sonars %d %d %d %d\n",Sen.getSonar(Rob.sonFrLeft),Rob.sonFrRight,Rob.sonReLeft,Sen.getSonar(Rob.sonReLeft));
			sizeWrite = strWrite[1]+4;
			break;
			
		case ID_ROBOT :
			//printf("Robot.\n");
			strWrite[0] = ID_ROBOT;
			strWrite[1] = 5;// taille utile 
			strWrite[2] = Rob.getVersion();
			strWrite[3] = (char) (Rob.getStateMatch()); // etat 
			strWrite[4] = (char) (Rob.getColor()); // couleur
			strWrite[5] = (char) (Rob.getCounter()& 0x00FF); // durée dans le match
			strWrite[6] = (char) (Rob.getCounter()>>8 & 0x00FF); // durée dans le match
			strWrite[7] = (char) (Rob.getTypeMatch()); // type de match
			strWrite[8] = (char) (Rob.getScore()); // score
			strWrite[9] = checkSum();
			sizeWrite = strWrite[1]+4;
			//printf("count : %.2f s score = %d\n",(float)(Rob.getCounter())/50,(char)(Rob.getScore()));
			break;
		case ID_IA :
			strWrite[0] = ID_IA;
			strWrite[1] = 2;// taille utile 
			strWrite[2] = Rob.getVersion();
			strWrite[3] = (char) (MyIA.getMacroStep()); // macro step 
			strWrite[4] = (char) (MyIA.getMicroStep()); // micro step
			strWrite[5] = checkSum();
			sizeWrite = strWrite[1]+4;
			break;
		case ID_CALGET : // récupération de la calibration demandée
			printf("calib %f\n",calibf);
			sprintf(calstr,"%f",calibf);
			printf("soit %s\n",calstr);
			strWrite[0] = ID_CALGET;
			//strWrite[1] = 5;// taille utile 
			strWrite[2] = Rob.getVersion();
			for (i=0;i<strlen(calstr);i++)
			{
				strWrite[i+3] = calstr[i];
			}
			strWrite[i+4] = checkSum();
			strWrite[1] = i+1;
			sizeWrite = strWrite[1]+4;
			break;
	}
	return 0;
	
}
// calcule le checksum, à savoir la somme de tous les octets, casté en char
char Remote::checkSum()
{
	int cs=0;
	for (int i=0;i<strWrite[1]+3;i++)
		cs+=(int)strWrite[i];
	return (char)cs;
}
