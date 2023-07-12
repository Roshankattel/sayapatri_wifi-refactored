#include "main.h"

/*Private Function Prototypes*/
void notifyTimer(uint8_t state);
void checkWifiStatus();
void saveConfigCallback();

void setup()
{
#if DEBUG == 1
  Serial.begin(115200);
#endif

  bool spiffsSetup = loadConfigFile();

  if (!spiffsSetup)
  {
    debugln("Forcing config mode as there is no saved config");
    wm.resetSettings();
  }

  wm.setDebugOutput(false);
  wm.setDarkMode(true);
  // Set config save notify callback
  wm.setSaveConfigCallback(saveConfigCallback);

  // setup custom fields in wifiManager settings
  WiFiManagerParameter merchat_email("email", "Merchant Email", MERCHANT_EMAIL.c_str(), 50);
  WiFiManagerParameter merchat_password("password", "Merchant password", MERCHANT_PASSWORD.c_str(), 50);

  debugln("Merchant Email: " + MERCHANT_EMAIL);
  debugln("Merchant Password: " + MERCHANT_PASSWORD);

  // add custom parameters
  wm.addParameter(&merchat_email);
  wm.addParameter(&merchat_password);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  // pinMode(BATTERY_PIN, INPUT);

  digitalWrite(BUZZER_PIN, LOW);

  psramInit();

  debugln((String) "\n\nMemory available in PSRAM : " + ESP.getFreePsram());

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();

#if DEBUG == 1
  // Got ok data, print it out!
  Serial.print("\nFound chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);
#endif

  // configure board to read RFID tags
  nfc.SAMConfig();

  if (!versiondata)
  {
    debugln("\nDidn't find PN53x board");
    writeString("NFC not working !", 160, 180);
    while (1)
      ; // halt
  }

  // http.setReuse(true);

  tftInit();

  char *APssid = (char *)ps_malloc(30 * sizeof(char));
  // snprintf(APssid, 30, "SPWiFi-%08X", (uint32_t)ESP.getEfuseMac());
  snprintf(APssid, 30, "SP_Merchant");

  debugln("DEVICE NAME: " + String(APssid));

  if (digitalRead(TRIGGER_PIN) == LOW)
  {
    showHotspot((uint32_t)ESP.getEfuseMac());
    wm.resetSettings();
    wm.setConfigPortalTimeout(TIMEOUT);

    if (!wm.startConfigPortal(APssid, "123456789"))
    {
      delay(3000);
      ESP.restart();
      delay(5000);
    }
    free(APssid);
  }
  else
  {
    // connect to existing wifi credentials
    wm.setConnectTimeout(30);
    writeString("Connecting to WiFi", 160, 180);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    // WiFi.setHostname(hostname.c_str()); // define hostname
    WiFi.setHostname(APssid); // define hostname
    bool result = wm.autoConnect(APssid, "123456789");
    debugln("\nThe wifi result is = " + String(result));
  }

  homePage();
  showConnection(WiFi.SSID());
  playBuzz(SINGLE);

  delay(1000);

  MERCHANT_EMAIL = merchat_email.getValue();
  MERCHANT_PASSWORD = merchat_password.getValue();

  if (shouldSaveConfig)
  {
    saveConfigFile();
  }

  setupBLE();

  debugln("\nThe device started, now you can pair it with bluetooth!\n");

  showImage(BTH_X, BTH_Y, (uint8_t *)bluetooth, sizeof(bluetooth));
  welcomePage();
}

void loop()
{
  checkWifiStatus();
  checkBuzzerStatus();

  switch (processState)
  {
  case WELCOME:

    if (welcomeProcess() == 1)
    {
      processState++;
      startListeningToNFC();
    }
    break;

  case SCAN:
    if (buzzMode == ERROR)
    {
      notifyTimer(WELCOME);
    }

    status = scanProcess();

    if (status == 1)
    {
      processState++;
    }
    else if (status == -1)
    {
      processState--;
    }
    break;

  case INFORM:
    notifyProcess();
    notifyTimer(WELCOME);
    break;
  }
}

void notifyTimer(uint8_t state)
{
  if (millis() - turnOnTime > NOTIFY_TIME)
  {
    welcomePage();
    debugln("\nCalled Notify Timer");
    processState = state;
    buzzMode = 0;
    amount = 0;
  }
}

void checkWifiStatus()
{
  static int lastWifiStatus = WL_IDLE_STATUS;

  if (WiFi.status() != lastWifiStatus)
  {
    lastWifiStatus = WiFi.status();
    wifiDisplay(lastWifiStatus == WL_CONNECTED);
    if (lastWifiStatus != WL_CONNECTED)
    {

      debugln("Reconnecting to WiFi..." + String(millis()));
      wm.setConnectTimeout(120); // Wait 2 mins for reconnect else restart
      if (wm.autoConnect())
      {
        playBuzz(SINGLE);
      }
    }
  }
}

void saveConfigCallback()
{
  debugln("Should save config");
  shouldSaveConfig = true;
}