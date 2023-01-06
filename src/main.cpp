// include the library
#include "pinout.h"
#include "logger.h"
#include "cb_mgr.h"
#include "Arduino.h"
#include <RadioLib.h>
#include "lora.hpp"
#include "display.hpp"
#include "bluetooth.hpp"

Textfield* btScanText = nullptr;
ProgressBar* progressBar = nullptr;
Textfield* btInfo = nullptr;
Textfield* btAdresses = nullptr;
Rectangle* line = nullptr;

cb_mgr* callbackManager = nullptr;

void handle_incoming_lora_msg(uint8_t* data, size_t dataLength);
void createBTResultView();
void showBTResults();
void showBTScanProzessBar();
void removeBTScanProzessBar();
void startBluetoothScan();


void handle_incoming_lora_msg(uint8_t* data, size_t dataLength){
  //prints the incoming LORA messages if there are any...

  log_info("Message Begin ->");
  for (int i = 0; i < dataLength; i++)
  {
     logf_info("Byte %i = '%d'", i, data[i]);
  }
  log_info("<- Message End\n");
}

void createBTResultView() {
    btInfo = new Textfield("BLE 0 devices found", 2, 35, 124, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    btAdresses = new Textfield("", 2, 50, 124, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    btAdresses->setFieldMode(FIELD_MODE_REPEATING, 2000);
    display_addVisualisation(btInfo);
    display_addVisualisation(btAdresses);
}

void showBTResults() {
  std::string infoText = "BLE " +  std::to_string(bluetooth_getScanResults()->getCount()) + " devices found";
  btInfo->setText(infoText);

  btAdresses->clearTextList();
  for (int i = 0; i < bluetooth_getScanResults()->getCount(); i++) {
    btAdresses->additionalText(bluetooth_getScanResults()->getDevice(i).getAddress().toString());
  }
}

void showBTScanProzessBar() {
  btScanText = new Textfield("Bluetooth Scan läuft...", 2, 5, 124, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
  progressBar = new ProgressBar(10, 20, 100, 8);
  progressBar->setProgressTime(100);
  display_addVisualisation(btScanText);
  display_addVisualisation(progressBar);
}

void removeBTScanProzessBar() {
  display_removeVisualisation(btScanText);
  display_removeVisualisation(progressBar);
  if (btScanText != nullptr) {
    delete(btScanText);
    btScanText = nullptr;
  }
  if (progressBar != nullptr) {
    delete(progressBar);
    progressBar = nullptr;
  }
}

void startBluetoothScan() {
  showBTScanProzessBar();
  bluetooth_startScan(10);
}


void setup() {
  log_begin();
  display_start();
  lora_start();
  lora_shall_receive(true);
  register_lora_msg_handler(handle_incoming_lora_msg);

  bluetooth_start();
  createBTResultView();

  callbackManager = new cb_mgr();
  callbackManager->register_callback(&startBluetoothScan, 30000);

}

void loop() {
  lora_maintain();
  display_maintain();
  callbackManager->maintain();

  if (bluetooth_newScanAvailable()) {
    removeBTScanProzessBar();
    showBTResults();
  }

  delay(10); //to not spam too much...
}
