#include "httpReq.h"
#include "config.h"

HTTPClient http;
WiFiClientSecure client;

String sendHttpPostRequest(const String endpoint, const String httpRequestData, const String token = "")
{
    if (WiFi.status() != WL_CONNECTED)
        return "";

    String payload;

    debugln("\nPOST REQUEST DATA:");
    debugln(httpRequestData);

    http.setTimeout(5000); // Set timeout to 5 seconds
    client.setInsecure();

    if (!http.begin(client, "https://" + SERVER_IP + "/" + endpoint))
    {
        debugln("\n[HTTP] Unable to connect");
        return "";
    }

    if (!token.isEmpty())
        http.addHeader("Authorization", "Bearer " + token);

    http.addHeader("Content-Type", "application/json");
    httpCode = http.POST(httpRequestData);
    payload.reserve(http.getSize());
    payload = http.getString();
    http.end();

    debugln("\n\nPOST REQUEST RESPONSE:");
    debugln(payload);

    if (httpCode != 200)
    {
        debugln("\n[HTTP] POST...Failed, code:" + String(httpCode));
        return "";
    }

    debugln("\n[HTTP] POST... Success, Code:" + String(httpCode));
    return payload;
}

String merchantLogin()
{
    String httpRequestData;
    httpRequestData.reserve(1024);

    SpiRamJsonDocument doc(4096);
    doc["email"] = MERCHANT_EMAIL;
    doc["password"] = MERCHANT_PASSWORD;
    doc["role"] = "merchant";

    serializeJson(doc, httpRequestData);

    String response = sendHttpPostRequest("auth/login", httpRequestData, "");
    if (response.isEmpty())
    {
        debugln("\nFailed! The response is empty");
        return "";
    }

    DeserializationError error = deserializeJson(doc, response);
    if (error)
        return "";

    return doc["accessToken"].as<String>();
}

String transactionRequest(const String accessToken, const String tagData)
{
    // DynamicJsonDocument doc(1024);
    String httpRequestData;
    httpRequestData.reserve(1024);

    SpiRamJsonDocument doc(2048);
    doc["tagData"] = tagData;
    doc["amount"] = amount;
    doc["rechargeRequest"] = rechargeRequest;
    doc["channel"] = "pos";

    serializeJson(doc, httpRequestData);

    String response = sendHttpPostRequest("transaction/add", httpRequestData, accessToken);

    if (response.isEmpty())
    {
        debugln("\nFailed! The response is empty");
        return "";
    }

    DeserializationError error = deserializeJson(doc, response);
    if (error)
        return "";

    userName = doc["client"]["name"].as<String>();
    return userName;
}
