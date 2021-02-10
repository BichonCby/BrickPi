#ifndef __MainBS_h__
#define __MainBS_h__

#define EMULATOR 


#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include "Config.h"
#include "Utils.h"
#ifdef EMULATOR
#include "Emul.h"
#else
#include "BrickPi3.h"
#endif
#include "MotorsBS.h"
#include "Sensors.h"
#include "Position.h"
#include "Asserv.h"
#include "Robot.h"
#include "IA.h"
#include "Action.h"
#include "Remote.h"
#include "Detection.h"

#ifdef EMULATOR
extern Emul BP;
#else
extern BrickPi3 BP;
#endif
extern Sensors Sen;
extern MotorsBS Mot;
extern Position Pos;
extern Asserv Ass;
extern Robot Rob;
extern IA MyIA;
extern Config Conf;
extern Action Act;
extern Remote Rem;
extern Detection Det;
#endif
