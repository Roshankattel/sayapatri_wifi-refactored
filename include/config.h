#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#define DISABLE_ALL_LIBRARY_WARNINGS

const int WIFI_RECONNECT = 3000;  // Wifi Reconnect time(milliseconds)
const int TIMEOUT = 120;          // seconds to run for wifi connection
const int AMOUNT_LIMIT = 1000000; // Amount limit is 1 lakh
const int NOTIFY_TIME = 10000;

const int DIGIT_ZERO_ASCII_VALUE = 48;

/*Server IP*/
const String SERVER_IP = "rfid.goswivt.com/api/v1";

// JSON configuration file
const String JSON_CONFIG_FILE = "/config.json";

// #define TFT_MOSI 19
// #define TFT_SCLK 18
// #define TFT_CS 15 // Chip select control pin
// #define TFT_DC 2  // Data Command control pin

const uint8_t BATTERY_PIN = 14;
const uint8_t BUZZER_PIN = 12;
const uint8_t TRIGGER_PIN = 13;
const uint8_t HSPI_MISO = 27;
const uint8_t HSPI_MOSI = 26;
const uint8_t HSPI_SCLK = 25;
const uint8_t HSPI_SS = 33;
const uint8_t HSPI_RST = 32;

enum PROCESS
{
    WELCOME,
    SCAN,
    NOTIFY
};

enum BUZZ_MODE
{
    SINGLE,
    SUCCESS,
    ERROR
};

#endif
