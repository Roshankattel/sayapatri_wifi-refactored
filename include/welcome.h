#ifndef WELCOME_H
#define WELCOME_H

#include <Arduino.h>
#include "displayFunction.h"

extern uint32_t amount;
extern uint32_t sum;
extern bool rechargeRequest;
extern std::string value;

uint8_t welcomeProcess();
#endif