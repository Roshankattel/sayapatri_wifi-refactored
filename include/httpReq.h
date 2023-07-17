
#ifndef HTTPSREQ_H
#define HTTPSREQ_H

#include "psram.h"

#include <WiFiClientSecure.h>
#include <HTTPClient.h>

extern uint8_t processState;
extern uint32_t amount;
extern int httpCode;
extern bool rechargeRequest;
extern String userName;

extern String MERCHANT_EMAIL;
extern String MERCHANT_PASSWORD;

String merchantLogin(void);
String transactionRequest(const String accessToken, const String tagData);
#endif