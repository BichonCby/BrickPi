/* permier test avec Brickpi */

#include "mainBS.h"
Config Conf;
BrickPi3 BP;
Sensors Sen;
MotorsBS Mot;
Position Pos;
Asserv Ass;
Robot Rob;
IA MyIA;
Action Act;
Remote Rem;

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
    while (TIRETTE)
    {
        sleepms(100); // boucle 100ms
    }
    // on lance la tempo 
    while (Rob.getCounter() < DURATION_MATCH && !BAU)
    {
      Rob.incCounter();
      sleepms(100); // boucle 100ms
    }
    Rob.setStateMatch(MATCH_END); // en fait ça sera funny ou display
   // return;
}
sensor_ultrasonic_t sonar;

void * strategy (void *)
{
    //  d'abord le positionnement si besoin, l'attente de la couleur,
    // les tests actionneurs
    // on lance l'IA
    MyIA.launchIA();
    //return;
}
void * seq20ms(void *)
{
    if (seqRun)
    {
        // watchdog, c'est la merde
        std::cout << "CATASTROPHE";
        exit(-1);
    }
    seqRun = true;
    Sen.readEncoder();
    //Sen.getSonar(PORT_1);
    //   if(BP.get_sensor(PORT_1, &sonar)){
    //  printf("erreur");
    //}else{
    //  printf("Ultrasonic sensor (S1): CM %5.1f Inches %5.1f  \n", sonar.cm, sonar.inch);
    //}
    
    printf(" %5.1f cm \n", Sen.getSonar(PORT_1));
    //std::cout << ;
    //   Sen.getEncoder(1,2);
    std::cout << ".";//Rob.getCounter();
    fflush(stdout);

    Mot.setMotorPower(1,30);
    //BP.set_motor_dps(PORT_A,rot);
    BP.set_led(led);
    // la séquence en elle même
    Pos.calcPosition();
    Ass.calcAsserv();
    Act.calcAction();
    seqRun = false; 
}

void *remotecontrol(void *)
{
    Rem.threadRemote();
}
int main(int argc, char **argv)
{
  pthread_t seq;
  pthread_t stt; // stop and stop du robot
  pthread_t stra; // strategie, en parallèle de la sequence20ms
  pthread_t remt;
  std::cout << "Debut du programme\n";  
//  return 0;
    sleepms(1000); //20ms c'est le temps de la séquence
  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
  pthread_create(&stt,NULL,&stopstart,NULL);
  pthread_create(&stra,NULL,&strategy,NULL);
  pthread_create(&remt,NULL,&remotecontrol,NULL);
  
  //BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_ULTRASONIC);

  while (Rob.getStateMatch() != MATCH_END)
  {
    //std::thread t1(seq20ms);
    pthread_create(&seq,NULL,&seq20ms,NULL);
    sleepms(1000); //20ms c'est le temps de la séquence
    //usleep(1000000);
  }
  BP.reset_all(); 
  std::cout << "\nFin du programme";
return 0;
} 