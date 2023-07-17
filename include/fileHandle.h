#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include <FS.h> //This should be at first
#include <LittleFS.h>

bool saveConfigFile(void);
bool loadConfigFile(void);

extern String MERCHANT_EMAIL;
extern String MERCHANT_PASSWORD;

#endif