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
		printf("imposible de créer le pipe, communication impossible \n");
		//return;
	}
	printf("pipe OK\n");
	stRemote = STREMOTE_WAIT_RECEIVE; // init
	// on va lire et attendre de recevoir quelque chose
	// début de la machine à état pour la com remote
	// pour l'instant on boucle en lecture écriture
	while (1)
	{
		switch (stRemote)
		{
			case STREMOTE_WAIT_RECEIVE :
				fd=open(fiforead,O_RDONLY);
				if (fd != -1) // la lecture s'est bien passée
				{
					printf("Reçu par le lecteur :\n");
					// traitement
					i=0;
					while(read(fd,&input,1)>0)
					{
						strRead[i++] = input;
						//decodeByte(input);
					}
					decodeFrame();
					printf("La lecture se termine!\n");
				}
				else
					printf("pipe HS\n"); // oups!
				close (fd);
				stRemote = STREMOTE_WAIT_SEND;
				break;
			case STREMOTE_WAIT_SEND :
				fd=open(fifowrite,O_WRONLY);
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

// fonction de décodage octet par octet de la trame reçue. Pour l'instant on écrit
int Remote::decodeByte(char b)
{
	printf("%c",b);
	return 0;
}
// fonction de décodage de la trame reçue. Pour l'instant on écrit
int Remote::decodeFrame()
{
	printf("%s",strRead);
	if (strRead[0] == 'c')
		strcpy(strWrite,"Salut!");
	else 
		strcpy(strWrite,"Hello!");
	sizeWrite=6;
	return 0;
}

int Remote::encodeFrame(char id)
{
	// l'identifiant est la question
	// si c'est un ordre, on va répondre OK avec le numéro de l'ordre
	// si c'est un requete de trame, on renvoie la trame
	float val1, val2, val3;
	switch (id)
	{
		case ID_ACK :
			strWrite[0] = ID_ACK; // identifiant
			strWrite[1] = 2;// taille utile hors version
			strWrite[2] = Rob.getVersion();
			strWrite[3] = strRead[3]; // l'odre donné
			
			strWrite[4] = ACK_OK; // pour l'instant tout est OK
			sizeWrite = 5;
			break;
		case ID_POSITION :
			strWrite[0] = ID_POSITION;
			strWrite[1] = 10;// taille utile
			strWrite[2] = Rob.getVersion();
			Pos.getPosition(&val1,&val2,&val3);
			strWrite[3] = (char) ((int)val1 & 0x00FF); // pos X poids faible
			strWrite[4] = (char) (((int)val1)>>8 & 0x00FF); // pos X poids fort
			strWrite[5] = (char) ((int)val2 & 0x00FF); // pos Y poids faible
			strWrite[6] = (char) (((int)val2)>>8 & 0x00FF); // pos Y poids fort
			strWrite[7] = (char) ((int)val3 & 0x00FF); // pos A poids faible
			strWrite[8] = (char) (((int)val3)>>8 & 0x00FF); // pos A poids fort
			// ...
			sizeWrite = 13;
			break;
		default :
			break;
	}
	return 0;
	
}
