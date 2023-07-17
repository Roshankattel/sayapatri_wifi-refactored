#include "fileHandle.h"
#include "psram.h"
#include "config.h"

bool saveConfigFile(void)
{
    debugln("Saving configuration...");

    SpiRamJsonDocument json(512);
    json["merchant_email"] = MERCHANT_EMAIL;
    json["merchant_password"] = MERCHANT_PASSWORD;

    serializeJsonPretty(json, Serial);

    File configFile = LittleFS.open(CONFIG_FILE, "w");

    if (!configFile)
    {
        debugln("\nfailed to open config file for writing");
        return 0;
    }

    if (serializeJson(json, configFile) == 0)
    {
        debugln("\nFailed to write to file");
        return 0;
    }

    debugln("\nWrote Sucessfully!");

    // Close file
    configFile.close();

    return 1;
}

bool loadConfigFile(void)
{

    debugln("\nMounting File System...");

    if (!(LittleFS.begin(false) || LittleFS.begin(true)))
    {
        debugln("\nFailed to mount FS");
        return 0;
    }

    if (!(LittleFS.exists(CONFIG_FILE)))
    {
        debugln("Failed to load json config");
        return 0;
    }

    debugln("\nThe file exists and reading config file\n");

    File configFile = LittleFS.open(CONFIG_FILE, "r");

    if (!configFile)
        return 0;

    SpiRamJsonDocument json(1024);
    DeserializationError error = deserializeJson(json, configFile);

    if (error)
    {
        debugln("Failed to parse JSON: " + String(error.c_str()));
        return 0;
    }

    debugln(CONFIG_FILE);
    serializeJsonPretty(json, Serial);
    debugln("\n");

    MERCHANT_EMAIL = json["merchant_email"].as<String>();
    MERCHANT_PASSWORD = json["merchant_password"].as<String>();

    return 1;
}
