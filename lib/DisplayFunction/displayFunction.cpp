#include "displayFunction.h"

TFT_eSPI tft = TFT_eSPI();
PNG png;

int FRAMES = 27;
int ANIMATION_WIDTH = 100;
int ANIMATION_HEIGHT = 100;

int16_t xpos;
int16_t ypos;

void tftInit(void)
{
    tft.init();
    tft.setRotation(1);
    startScreen();
}

/*Display Functions*/
void pngDraw(PNGDRAW *pDraw)
{
    uint16_t lineBuffer[MAX_IMAGE_WDITH];
    png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void showPair(void)
{
    showImage(PAIR_X, PAIR_Y, (uint8_t *)diamond, sizeof(diamond));
    showImage(PAIR_X + 16, PAIR_Y, (uint8_t *)diamond, sizeof(diamond));
}

void clearPair(void)
{
    tft.fillRect(PAIR_X, PAIR_Y, 4, 4, TFT_WHITE);
    tft.fillRect(PAIR_X + 16, PAIR_Y, 4, 4, TFT_WHITE);
}

void clearLCD(void)
{
    tft.fillRect(0, 130, 320, 80, TFT_WHITE);
}

void showImage(int x, int y, uint8_t *image, int size)
{
    xpos = x;
    ypos = y;
    int16_t rc = png.openFLASH(image, size, pngDraw);
    if (rc == PNG_SUCCESS)
    {
        tft.startWrite();
        rc = png.decode(NULL, 0);
        tft.endWrite();
    }
}

void chargingDisplay(void)
{
    showImage(CHARGE_X, CHARGE_Y, (uint8_t *)charge, sizeof(charge));
}

void wifiDisplay(bool status)
{
    if (status)
        showImage(WIFI_X, WIFI_Y, (uint8_t *)wifi, sizeof(wifi));
    else
        tft.fillRect(WIFI_X, WIFI_Y, 30, 30, TFT_WHITE);
}

void homePage(void)
{
    tft.fillScreen(TFT_WHITE);
    tft.setTextSize(1);
    // tft.drawString("ID: " MERCHANT_ID, 3, 7, GFXFF);
    showImage(SAYAPATRI_X, SAYAPATRI_Y, (uint8_t *)logo, sizeof(logo));
    showImage(BATTERY_X, BATTERY_Y, (uint8_t *)battery, sizeof(battery));
    tft.setTextSize(1);
    chargingDisplay();
}

void startScreen(void)
{
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    // tft.setFreeFont(CF_OL24);
    tft.drawCentreString("S T A R T I N G", 160, 180, GFXFF);
    for (size_t i = 0; i < 3; i++)
    {
        for (int i = 0; i < FRAMES; i++)
        {
            delay(40);
            tft.pushImage(110, 70, ANIMATION_WIDTH, ANIMATION_HEIGHT, start[i]);
        }
    }
    tft.setSwapBytes(false);
    showImage(26, 70, (uint8_t *)logo, sizeof(logo));
    delay(3000);
}

void showConnection(String ssid)
{
    tft.fillRect(0, 180, 320, 26, TFT_WHITE);
    tft.drawCentreString("Connected to:", 160, 140, GFXFF);
    tft.drawCentreString(ssid, 160, 180, GFXFF);
}

void showHotspot(uint32_t chipId)
{
    char *APssid = (char *)ps_malloc(30 * sizeof(char));
    // snprintf(APssid, 30, "SPWiFi-%08X", chipId);
    snprintf(APssid, 30, "SP_Merchant1");
    tft.fillRect(0, 180, 320, 26, TFT_WHITE);
    tft.drawCentreString("Connect to Wifi:", 160, 170, GFXFF);
    tft.drawCentreString(APssid, 160, 200, GFXFF);
    free(APssid);
}

void welcomePage(void)
{
    clearLCD();
    // checkChargeStatus();
    tft.drawCentreString("Welcome !", 160, 140, GFXFF);
    tft.drawCentreString("Please Enter Amount", 160, 180, GFXFF);
}

void scanPage(bool rechargeReq, int amount)
{
    clearLCD();
    String prompt = rechargeReq ? "Recharge" : "Pay";
    tft.drawCentreString(prompt + " Rs." + String(amount), 160, 140, GFXFF);
    tft.drawCentreString("Please Scan Your Card !", 160, 180, GFXFF);
}

void writeString(String msg, int X, int Y)
{
    tft.drawCentreString(msg, X, Y, GFXFF);
}