#include "scan.h"
#include "utils.h"
#include "config.h"

void startListeningToNFC()
{
    // Reset our IRQ indicators
    irqPrev = irqCurr = HIGH;

    Serial.println("Waiting for an ISO14443A Card ...");
    nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

String handleCardDetected()
{
    uint8_t success = false;
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0}; // Buffer to store the returned UID
    uint8_t uidLength;                     // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // read the NFC tag's info
    success = nfc.readDetectedPassiveTargetID(uid, &uidLength);
    debugln(success ? "Read successful" : "Read failed (not a card?)");
    String unhashTag;
    if (success)
    {
        // Display some basic information about the card
        debugln("Found an ISO14443A card");
        debug("  UID Length: ");
        Serial.print(uidLength, DEC);
        debugln(" bytes");
        debug("  UID Value: ");
        nfc.PrintHex(uid, uidLength);

        for (int i = 6; i > -1; i--)
        {
            unhashTag += int(uid[i]);
        }
        debugln(unhashTag);
        return (unhashTag);
    }
    return ("");
}

int scanProcess()
{
    if (((bleValue.length() == 1) && (bleValue[0] == 'B')) || Serial.available())
    {
        bleValue = ""; // clear BLE value
        welcomePage();
        return -1;
    }

#if TEST == 1
    debugln("Running Test Case");
    lcdDisplay("Running Test Case!", "");
    const String accessToken = merchantLogin();
    if (accessToken != "")
    {
        const String tag = "ff3e3a07fee14c0562c6dba74ae7a483a8ea5a7d80ffadba5b5116971d0af47d";
        userName = transactionRequest(accessToken, tag);
    }

    return 1;
#else
    irqCurr = digitalRead(PN532_IRQ);

    if (irqCurr == HIGH || irqPrev == LOW)
    {
        irqPrev = irqCurr;
        return false;
    }

    clearLCD();
    writeString("Card Scan Sucessful !", 160, 180);

    debugln("\nTHE FREE HEAP IS = " + String(ESP.getFreeHeap()));

    String tag = getHash(handleCardDetected());

    String accessToken = merchantLogin();

    if (httpCode == 200)
    {
        httpCode = 0;
        userName = transactionRequest(accessToken, tag);

        if (userName != "")
        {
            return 1;
        }
    }
    else if (httpCode == 404)
    {
        showError("Merchant Not Found !", "");
    }
    else if (httpCode == 400)
    {
        showError("Invalid Credentials!", "");
    }
    else if (httpCode == -1)
    {
        clearLCD();
        writeString("No Internet", 160, 180);
        delay(1000);
        welcomePage();
    }
    else
    {
        showError("Merchant Login Failed", "");
    }
    tag = "";

    return 0;
#endif
}