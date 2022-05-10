#include "BLEHIDDevice.h"
#include "BLEHIDKeyboard.h"
#include "BLEDevice.h"

BLEHIDKeyboard keyboardDev;
BLEAdvertData advdata;

#define ENABLE_PIN 8

void setup() {
  Serial.begin(115200);
  advdata.addFlags();
  advdata.addCompleteName("AMEBA_BLE_HID");
  advdata.addAppearance(GAP_GATT_APPEARANCE_HUMAN_INTERFACE_DEVICE);
  advdata.addCompleteServices(BLEUUID(HID_SERVICE_UUID));

  BLEHIDDev.init();

  BLE.init();
  BLE.configAdvert()->setAdvData(advdata);
  BLE.setDeviceName("AMEBA_BLE_HID");
  BLE.setDeviceAppearance(GAP_GATT_APPEARANCE_HUMAN_INTERFACE_DEVICE);
  BLE.configSecurity()->setPairable(true);
  BLE.configSecurity()->setAuthFlags(GAP_AUTHEN_BIT_BONDING_FLAG);
  BLE.configServer(3);
  BLE.addService(BLEHIDDev.hidService());
  BLE.addService(BLEHIDDev.battService());
  BLE.addService(BLEHIDDev.devInfoService());

  pinMode(ENABLE_PIN, INPUT);

  BLE.beginPeripheral();
}

void loop() {
  if (BLE.connected() && digitalRead(ENABLE_PIN)) {
    Serial.println("Sending keystrokes");
    keyboardDev.keyReleaseAll();
    delay(100);
    keyboardDev.keySequence("fuck");
    delay(100);
    keyboardDev.keyPress(HID_KEY_ENTER);
    delay(100);
    keyboardDev.keyReleaseAll();
    delay(100);
  } else {
    delay(20);
  }
}
