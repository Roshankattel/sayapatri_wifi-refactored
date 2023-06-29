#ifndef BLE_H
#define BLE_H

#include "NimBLEDevice.h"

static NimBLEServer *pServer;
extern std::string bleValue;

void setupBLE();

#endif