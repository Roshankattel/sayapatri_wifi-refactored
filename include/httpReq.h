
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

extern char *MERCHANT_EMAIL;
extern char *MERCHANT_PASSWORD;

String merchantLogin();
String transactionRequest(const String accessToken, const String tagData);
#endif