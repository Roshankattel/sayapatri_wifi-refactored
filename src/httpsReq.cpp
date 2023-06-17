#include "httpReq.h"
#include "config.h"

HTTPClient http;
WiFiClientSecure client;

String sendHttpPostRequest(const String endpoint, const String httpRequestData, const String token = "")
{
    if (WiFi.status() != WL_CONNECTED)
        return "";

    String payload;

    debugln("\nPOST REQUEST DATA:\n" + httpRequestData);

    http.setTimeout(5000); // Set timeout to 5 seconds
    client.setInsecure();

    if (http.begin(client, "https://" + SERVER_IP + "/" + endpoint))
    {
        if (!token.isEmpty())
            http.addHeader("Authorization", "Bearer " + token);

        http.addHeader("Content-Type", "application/json");
        httpCode = http.POST(httpRequestData);
        payload.reserve(http.getSize());
        payload = http.getString();
        http.end();

        debugln("\n\nPOST REQUEST RESPONSE:");
        debugln(payload);

        if (httpCode > 0)
            debugln("\n[HTTP] POST... code:" + String(httpCode));

        else
            debugln("\n[HTTP] POST... failed, error:" + http.errorToString(httpCode));
    }
    else
        debugln("\n[HTTP] Unable to connect");

    return payload;
}

String merchantLogin()
{
    String httpRequestData;
    httpRequestData.reserve(1024);

    SpiRamJsonDocument doc(2048);
    doc["email"] = MERCHANT_EMAIL;
    doc["password"] = MERCHANT_PASSWORD;
    doc["role"] = "merchant";

    serializeJson(doc, httpRequestData);

    String response = sendHttpPostRequest("auth/login", httpRequestData, "");
    if (!response.isEmpty())
    {
        DeserializationError error = deserializeJson(doc, response);
        if (!error)
            return doc["accessToken"].as<String>();
    }
    return "";
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
    doc["channel"] = "POS";

    serializeJson(doc, httpRequestData);

    String response = sendHttpPostRequest("transaction/add", httpRequestData, accessToken);
    if (!response.isEmpty())
    {
        DeserializationError error = deserializeJson(doc, response);
        if (!error)
        {
            userName = doc["client"]["name"].as<String>();
            return userName;
        }
    }
    return "";
}
