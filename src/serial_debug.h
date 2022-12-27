#pragma once

#include "debug_config.h"
#include "Arduino.h"


#ifdef SERIAL_DEBUG
    #define IS_DEBUGGING true
    char msg_buf[100];
    #define START_DEBUG(x) Serial.begin(x);
    #define DEBUG(x) Serial.print(__FILE__);Serial.print(":");Serial.print(__LINE__);Serial.println(x);
#else
    #define IS_DEBUGGING false
    #define DEBUG(x)
    #define START_DEBUG(x)

#endif