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
	Pos.setPosition(-2000,5,5);
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
	
	for (int j=0;j<sizeRead;j++)
		printf("%d ",strRead[j]);
	printf("\n");
	switch(strRead[0])
	{
		case ID_INFO : // on demande a lire une trame
			// on peut tout vérifier
			printf("demande de la trame ");
			encodeFrame(strRead[3]);
			break;
		case ID_ORDER : // on donne un ordre
			// on va traiter l'ordre, une fois qu'on saura comment faire
			printf("ordre reçu\n");
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
			printf("Position.\n");
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
			strWrite[9] = 0;
			strWrite[10] = 0;
			strWrite[11] = 0;
			strWrite[12] = 0;
			// ...
			sizeWrite = 13;
			Pos.getPosition(&val1,&val2,&val3);
			printf("pos (X,Y,A) = %f %f %f\n",val1, val2, val3);
			//printf("posX pf %d\n",strWrite[3]);
			//printf("posX pF %d\n",strWrite[4]);
			break;
		default :
			break;
	}
	return 0;
	
}
