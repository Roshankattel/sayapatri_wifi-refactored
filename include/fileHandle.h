#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include <FS.h> //This should be at first
#include <SPIFFS.h>

bool saveConfigFile();
bool loadConfigFile();

extern char *MERCHANT_EMAIL;
extern char *MERCHANT_PASSWORD;

#endif