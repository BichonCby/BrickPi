#ifndef __Remote_h__
#define __Remote_h__

#define STREMOTE_WAITINI 1
#define STREMOTE_WAIT_RECEIVE 2
#define STREMOTE_WAIT_SEND 3

// les identifiants de trame
#define ID_INFO 0x01
#define ID_ORDER 0x02
#define ID_ACK 0x03
#define ID_POSITION 0x10
#define ID_ASSERV 0x11

// les réponses de la trame Ack
#define ACK_OK 0
#define ACK_BAD_VERSION 1
#define ACK_IMPOSSIBLE 2

class Remote{
public:
Remote(void); // constructeur
void threadRemote();
private:
int decodeFrame();
int encodeFrame(char id);// on va mettre dans stWrite le contenue de la trame id
int stRemote; // etat de la machine de lecture/écriture
char strRead[500]; // ça fait beaucoup
char strWrite[20];
int sizeWrite;
int sizeRead;
};

#endif
