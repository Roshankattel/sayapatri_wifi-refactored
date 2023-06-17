#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "buzz.h"
#include "displayFunction.h"

const String getHash(const String data);
void showError(String errMsg1, String errMsg2 = "");

#endif