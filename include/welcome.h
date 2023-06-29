#ifndef WELCOME_H
#define WELCOME_H

#include <Arduino.h>
#include "displayFunction.h"

extern uint32_t amount;
extern bool rechargeRequest;
extern std::string bleValue;

uint8_t welcomeProcess();
#endif