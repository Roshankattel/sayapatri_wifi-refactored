#ifndef BUZZ_H
#define BUZZ_H

#include <Arduino.h>
#include "config.h"

extern bool buzzStatus;
extern uint8_t buzzMode;
extern unsigned long turnOnTime;

void playBuzz(uint8_t mode);
void checkBuzzerStatus();

#endif