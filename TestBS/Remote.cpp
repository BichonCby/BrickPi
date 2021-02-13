#include "mainBS.h"
//#include <fcntl.h>
#include <sys/stat.h>
//#include <sys/types.h>

Remote::Remote()
{}

void Remote::threadRemote()
{
	
	int fd,i;
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
	float ftmp1,ftmp2;
	int itmp1, itmp2;
	//for (int j=0;j<sizeRead;j++)
	//	printf("%x ",strRead[j]);
	//printf("\n");
	if (sizeRead <3 || strRead[2] != Rob.getVersion())
	{
		encodeFrame(ID_NACK);
		return -1;
	}
	// vérification du CS et de la longueur
	if ((strRead[1]+4) !=sizeRead)
	{
		encodeFrame(ID_NACK);
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
			//printf("ordre reçu\n");
			switch (strRead[3]) // le type de déplacement
			{
				case 'P':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					itmp2 = (int16_t)((int)strRead[6] + ((int)(strRead[7]))*256);
					Ass.goForward(itmp1,itmp2,30); // voir comment on récupère la vitesse
					break;
				case 'I':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					itmp2 = (int16_t)((int)strRead[6] + ((int)(strRead[7]))*256);
					Ass.goBackward(itmp1,itmp2,30); // voir comment on récupère la vitesse
					break;
				case 'R':
					itmp1 = (int16_t)((int)strRead[4] + ((int)(strRead[5]))*256);
					Ass.turn(itmp1,30); // voir comment on récupère la vitesse
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
				default :
					break;
			}
			encodeFrame(ID_ACK);
			break;
		default : // on ne connait pas la trame
			printf("Trame reçue inconnue\n");
	}
	return 0;
}

int Remote::encodeFrame(char id)
{
	// l'identifiant est la question
	// si c'est un ordre, on va répondre OK avec le numéro de l'ordre
	// si c'est un requete de trame, on renvoie la trame
	float valf1, valf2, valf3;
	int vali1, vali2;
	int16_t vali10,vali11;
	int32_t vali20,vali21;
	int8_t valc1,valc2;
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
			
			strWrite[4] = ACK_OK; // pour l'instant tout est OK
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
			//printf("pos (X,Y,A) = %f %f %f\n",val1, val2, val3);
			//printf("posX pf %d\n",strWrite[3]);
			//printf("posX pF %d\n",strWrite[4]);
			break;
		case ID_ASSERV :
			strWrite[0] = ID_ASSERV;
			strWrite[1] = 13;// taille utile
			strWrite[2] = Rob.getVersion();
			Ass.getTarget(&valf1,&valf2,&valf3,&vali1);
			strWrite[3] = (char) (vali1); // type de déplacement
			strWrite[4] = (char) ((int)valf1 & 0x00FF); // tar X poids faible
			strWrite[5] = (char) (((int)valf1)>>8 & 0x00FF); // tar X poids fort
			strWrite[6] = (char) ((int)valf2 & 0x00FF); // tar Y poids faible
			strWrite[7] = (char) (((int)valf2)>>8 & 0x00FF); // tar Y poids fort
			strWrite[8] = (char) ((int)valf3 & 0x00FF); // tar A poids faible
			strWrite[9] = (char) (((int)valf3)>>8 & 0x00FF); // tar A poids fort
			Ass.getSpeed(&valf1,&valf2);
			strWrite[10] = (char) ((int)valf1 & 0x00FF); // vit longi  poids faible;
			strWrite[11] = (char) (((int)valf1)>>8 & 0x00FF); // vit longi PF
			strWrite[12] = (char) ((int)valf2 & 0x00FF); // vit rot pf
			strWrite[13] = (char) (((int)valf2)>>8 & 0x00FF); // vit rot PF
			strWrite[14] = (char) (Ass.isConverge()); // convergence
			printf("conv %d %d\n",Ass.isConverge(),strWrite[14]);
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
			strWrite[1] = 4;// taille utile
			strWrite[2] = Rob.getVersion();
			Sen.getEncoder(&vali20,&vali21);
			strWrite[3] = (char) ((int)vali20 & 0x00FF); // codeur droit poids faible
			strWrite[4] = (char) (((int)vali20)>>8 & 0x00FF); // codeur droit poids fort
			strWrite[5] = (char) ((int)vali21 & 0x00FF); // codeur gauche poids faible
			strWrite[6] = (char) (((int)vali21)>>8 & 0x00FF); // codeur gauche poids fort
			sizeWrite = strWrite[1]+4;
			break;
			
		case ID_ROBOT :
			printf("Robot.\n");
			strWrite[0] = ID_ROBOT;
			strWrite[1] = 4;// taille utile
			strWrite[2] = Rob.getVersion();
			strWrite[3] = (char) (Rob.getStateMatch()); // etat 
			strWrite[4] = (char) (Rob.getColor()); // couleur
			strWrite[5] = (char) (Rob.getCounter()); // durée dans le match
			strWrite[6] = (char) (Rob.getTypeMatch()); // type de match
			strWrite[7] = (char) (Rob.getScore()); // score
			strWrite[8] = checkSum();
			sizeWrite = strWrite[1]+4;
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
