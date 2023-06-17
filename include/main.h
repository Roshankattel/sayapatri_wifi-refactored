#ifndef MAIN_H
#define MAIN_H

#include <SPI.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <TFT_eSPI.h>
#include <PNGdec.h>
#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
// #include "cert.h"

#include "fileHandle.h"
#include "buzz.h"
#include "scan.h"
#include "notify.h"

unsigned long previousMillis = 0;
unsigned long turnOnTime = 0;

int httpCode;

uint8_t processState = 0;
uint32_t amount = 0;
uint32_t sum = 0;
bool rechargeRequest; // for recharge and payment

bool buzzStatus = false;
uint8_t buzzMode;

// uint16_t cardNotPresentCount = 0;

String userName;

std::string value;

bool shouldSaveConfig = false;

MFRC522::MIFARE_Key key;
SPIClass hspi(HSPI);
MFRC522 mfrc522(HSPI_SS, HSPI_RST, hspi);

WiFiManager wm;

/*Merchant Details*/
char *MERCHANT_EMAIL;
char *MERCHANT_PASSWORD;

bool deviceConnected;
BLEServer *pServer = NULL;
// BLE UUID
const char *SERVICE_UUID = "4539568a-3950-47b8-b363-e474529f0ede";
const char *CHARACTERISTIC_UUID = "3fa39d68-c3d3-4978-bcca-2a525f96cfd3";

#endif