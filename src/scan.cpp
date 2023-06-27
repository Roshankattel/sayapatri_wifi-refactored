#include "scan.h"
#include "utils.h"
#include "config.h"

int scanProcess()
{
    if (((value.length() == 1) && (value[0] == 'B')) || Serial.available())
    {
        value = ""; // clear BLE value
        welcomePage();
        return -1;
    }

#ifdef TEST == 1
    const String accessToken = merchantLogin();
    if (accessToken != "")
    {
        const String tag = "5ac5b7f9861b6c48fe0da5d3d1dd281a00ba4873ff0e2ab9e9c26857d98b9a35";
        userName = transactionRequest(accessToken, tag);
    }

    return 1;
#else
    if (!mfrc522.PICC_IsNewCardPresent())
        return 0;

    if (mfrc522.PICC_ReadCardSerial())
    {
        String unhashTag = "";
        for (int i = 6; i > -1; i--) // taking in reverse order
        {
            // debugln(mfrc522.uid.uidByte[i]);
            unhashTag += mfrc522.uid.uidByte[i];
            mfrc522.uid.uidByte[i] = 0;
        }
        // Dump debug info about the card; PICC_HaltA() is automatically called
        // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();

        debugln("\nTag Data:" + unhashTag);

        String tag = getHash(unhashTag);

        clearLCD();
        writeString("Card Scan Sucessful !", 160, 180);

        debugln("\nTHE FREE HEAP IS = " + String(ESP.getFreeHeap()));

        String accessToken = merchantLogin();

        if (httpCode == 200)
        {
            httpCode = 0;
            userName = transactionRequest(accessToken, tag);

            if (userName != "")
                return 1;
        }
        else if (httpCode == 404)
            showError("Merchant Not Found !");

        else if (httpCode == 400)
            showError("Invalid Credentials!");

        else if (httpCode == -1)
        {
            clearLCD();
            writeString("No Internet", 160, 180);
            delay(1000);
            welcomePage();
        }
        else
            showError("Merchant Login Failed");

        tag = "";
    }
    return 0;
#endif
}