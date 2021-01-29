#ifndef __MainBS_h__
#define __MainBS_h__

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include "Config.h"
#include "Utils.h"
#include "BrickPi3.h"
#include "MotorsBS.h"
#include "Sensors.h"
#include "Position.h"
#include "Asserv.h"
#include "Robot.h"
#include "IA.h"
#include "Action.h"
#include "Remote.h"

extern BrickPi3 BP;
extern Sensors Sen;
extern MotorsBS Mot;
extern Position Pos;
extern Asserv Ass;
extern Robot Rob;
extern IA MyIA;
extern Config Conf;
extern Action Act;
extern Remote Rem;
#endif
