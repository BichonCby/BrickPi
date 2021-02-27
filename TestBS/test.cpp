/* permier test avec Brickpi */

#include "mainBS.h"
Config Conf;
#ifdef EMULATOR
Emul BP;
#else
BrickPi3 BP;
#endif
Robot Rob;
Sensors Sen;
MotorsBS Mot;
Position Pos;
Asserv Ass;
IA MyIA;
Action Act;
Remote Rem;
Detection Det;

  pthread_t seq;
  pthread_t stt; // stop and stop du robot
  pthread_t stra; // strategie, en parallèle de la sequence20ms
  pthread_t remt;

int puiss = 30;
int rot = 360;
int led=0;
bool seqRun = false;
using namespace std;

/***** threads  ******/
void *stopstart( void*)
{
    // on vérifie que les contacteurs sont OK
    // si BAU enclenché ou tirette non mise, on gueule
    Rob.setCounter(0);
    //~ // on attend la tirette
    printf("tirette\n");
    while (TIRETTE)
    {
        sleepms(100); // boucle 100ms
    }
    // on lance la tempo 
    printf("BAU\n");
    while (Rob.getCounter() < DURATION_MATCH && !BAU)
    {
      Rob.incCounter();
      sleepms(20); // boucle 100ms
    }
    printf("Fin du match\n");
    Rob.setStateMatch(MATCH_DISPLAY); // en fait ça sera funny ou display
    Ass.stopRobot();
    return NULL;
}
sensor_ultrasonic_t sonar;

void * strategy (void *)
{
    sleeps(1);
    Pos.setPosition((float)Rob.initPosX,(float)Rob.initPosY,(float)Rob.initPosA);
    //  d'abord le positionnement si besoin, l'attente de la couleur,
    // les tests actionneurs
    // on lance l'IA
    MyIA.launchIA();
    return NULL;
}
void * seq20ms(void *)
{
    //char *ret;
    if (seqRun)
    {
        // watchdog, c'est la merde
        std::cout << "CATASTROPHE";
        exit(-1);
    }
    seqRun = true;
#ifdef EMULATOR
    BP.runEmul();
#endif
    //test
    Sen.readEncoder();
    // la séquence en elle même
 //   printf("1");
    Det.detect();
    Pos.calcPosition();
    Ass.calcAsserv();
    Act.calcAction();
    
    seqRun = false; 
    return NULL;
}

void *remotecontrol(void *)
{
    Rem.threadRemote();
    return NULL;
}
int main(int argc, char **argv)
{
  std::cout << "Debut du programme\n";  
//  return 0;
    sleepms(20); //20ms c'est le temps de la séquence
  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
    wiringPiSetup();

  pthread_create(&stt,NULL,&stopstart,NULL);
  pthread_create(&stra,NULL,&strategy,NULL);
  pthread_create(&remt,NULL,&remotecontrol,NULL);
  //BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_ULTRASONIC);

  while (Rob.getStateMatch() != MATCH_END)
  {
    
    //std::thread t1(seq20ms);
    if (int ret = pthread_create(&seq,NULL,&seq20ms,NULL))
        {printf("%d ",ret);fflush(stdout);}
    pthread_detach(seq); // pour libérer la pile
    sleepms(20); //20ms c'est le temps de la séquence
   
  }
  BP.reset_all(); 
  std::cout << "\nFin du programme";
return 0;
} 
