#ifndef MAIN_H
#define MAIN_H

#include <SPI.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <PNGdec.h>

// #include "cert.h"

#include "fileHandle.h"
#include "buzz.h"
#include "scan.h"
#include "notify.h"
#include "ble.h"

unsigned long previousMillis = 0;
unsigned long turnOnTime = 0;

int httpCode;

uint8_t processState = 0;
uint32_t amount = 0;
bool rechargeRequest; // for recharge and payment

bool buzzStatus = false;
uint8_t buzzMode;

// uint16_t cardNotPresentCount = 0;

String userName;

std::string bleValue; // to store BLE data
bool shouldSaveConfig = false;

/* PN532  */
const int DELAY_BETWEEN_CARDS = 500;
boolean readerDisabled = false;
int irqCurr;
int irqPrev;

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

WiFiManager wm;

/*Merchant Details*/
String MERCHANT_EMAIL;
String MERCHANT_PASSWORD;

int status;

#endif