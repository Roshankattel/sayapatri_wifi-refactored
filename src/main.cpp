#include "main.h"

int status;

void saveConfigCallback()
{

  debugln("Should save config");
  shouldSaveConfig = true;
}

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    value = pCharacteristic->getValue();
  }
};

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    debugln("Client Connected");
    showPair();
  };
  void onDisconnect(BLEServer *pServer)
  {
    debugln("Client Disconnected");
    clearPair();
    pServer->startAdvertising(); // restart advertising
  }
};

/*Private Function Prototypes*/
void notifyTimer(uint8_t state);
void checkWifiStatus();
void setupBLE();

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

  debugln(MERCHANT_EMAIL);
  debugln(MERCHANT_PASSWORD);

  // add custom parameters
  wm.addParameter(&merchat_email);
  wm.addParameter(&merchat_password);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  // pinMode(BATTERY_PIN, INPUT);

  digitalWrite(BUZZER_PIN, LOW);

  psramInit();

  debugln((String) "\n\nMemory available in PSRAM : " + ESP.getFreePsram());

  hspi.begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_SS);
  mfrc522.PCD_Init();

  // http.setReuse(true);

  tftInit();

  char *APssid = (char *)ps_malloc(30 * sizeof(char));
  // snprintf(APssid, 30, "SPWiFi-%08X", (uint32_t)ESP.getEfuseMac());
  snprintf(APssid, 30, "SP_Merchant");

  debugln(APssid);

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
    saveConfigFile();

  debugln("\nThe device started, now you can pair it with bluetooth!\n");
  debugln("THE FREE HEAP IS = " + String(ESP.getFreeHeap()));

  void setupBLE();

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
      processState++;
    break;

  case SCAN:

    if (buzzMode == ERROR)
      notifyTimer(WELCOME);

    status = scanProcess();

    if (status == 1)
      processState++;
    else if (status == -1)
      processState--;

    break;

  case NOTIFY:

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

void setupBLE()
{
  char *BTssid = (char *)ps_malloc(30 * sizeof(char));
  snprintf(BTssid, 30, "SPBT-%08X", (uint32_t)ESP.getEfuseMac());

  BLEDevice::init(BTssid);
  BLEServer *pServer = BLEDevice::createServer();
  // pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Start");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  // /*New added code*/
  // BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  // pAdvertising->addServiceUUID(SERVICE_UUID);
  // pAdvertising->setScanResponse(true);
  // pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  // pAdvertising->setMinPreferred(0x12);
  // BLEDevice::startAdvertising();

  free(BTssid);
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
        playBuzz(SINGLE);
    }
  }
}