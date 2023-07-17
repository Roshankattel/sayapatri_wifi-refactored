#ifndef SCAN_H
#define SCAN_H

#include <Arduino.h>
#include <Adafruit_PN532.h>

#include "welcome.h"
#include "httpReq.h"

extern int irqCurr;
extern int irqPrev;

extern Adafruit_PN532 nfc;

extern int httpCode;
extern std::string bleValue;

int scanProcess(void);
void startListeningToNFC(void);

#endif