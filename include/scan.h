#ifndef SCAN_H
#define SCAN_H

#include <Arduino.h>
#include <MFRC522.h>

#include "welcome.h"
#include "httpReq.h"

extern SPIClass hspi;
extern MFRC522 mfrc522;
extern int httpCode;
extern std::string bleValue;

int scanProcess();

#endif