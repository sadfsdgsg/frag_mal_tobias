// include the library
#include "pinout.h"
#include "logger.h"
#include "cb_mgr.h"
#include "Arduino.h"
#include <RadioLib.h>
#include "lora.hpp"
#include "display.hpp"
#include "bluetooth.hpp"

Textfield* btLabel = nullptr;
Textfield* btDeviceCount = nullptr;
ProgressBar* progressBar = nullptr;
Textfield* btAdresses = nullptr;
Rectangle* rectangleBLE = nullptr;
Textfield* loraLabel = nullptr;
Textfield* loraConterLabel = nullptr;
Textfield* loraInMsgTextfield_1 = nullptr;
Textfield* loraInMsgTextfield_2 = nullptr;


cb_mgr* callbackManager = nullptr;

void handle_incoming_lora_msg(uint8_t* data, size_t dataLength);
void createView();
void showBTResults();
void showBTScanProzessBar();
void removeBTScanProzessBar();
void startBluetoothScan();
void removeBTDevicesCout();

void handle_incoming_lora_msg(uint8_t* data, size_t dataLength){
  //prints the incoming LORA messages if there are any...
  
  static int messageCounter = 0;
  std::string displayedCounterText = std::to_string(++messageCounter);
  displayedCounterText += " Messages";
  loraConterLabel->setText(displayedCounterText);

  std::string message = "";
  for (int i = 0; i < dataLength; i++)
    message += data[i];

  loraInMsgTextfield_1->setText(loraInMsgTextfield_2->getText());

  std::string displayedMessageText = std::to_string(dataLength);
  displayedMessageText += " ";
  displayedMessageText += message;
  loraInMsgTextfield_2->setText(displayedMessageText);


  std::string loraMessageLogText = "Lora message [";
  loraMessageLogText += std::to_string(dataLength);
  loraMessageLogText += "] ->";
  for (int i = 0; i < dataLength; i++)
  {
    loraMessageLogText += "'";
    loraMessageLogText += std::to_string(i);
    loraMessageLogText += " | ";
    loraMessageLogText += std::to_string(data[i]);
    loraMessageLogText += "'";
  }
  loraMessageLogText += "<-";
  log_info(loraMessageLogText.c_str());
}

void createView() {
    rectangleBLE = new Rectangle(1,26,126,34,false);
    display_addVisualisation(rectangleBLE);

    rectangleBLE = new Rectangle(1,1,126,24,false);
    display_addVisualisation(rectangleBLE);

    loraLabel = new Textfield("Lora", 2, 26, 50, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    display_addVisualisation(loraLabel);

    loraConterLabel = new Textfield("0 Messages", 50, 26, 60, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    display_addVisualisation(loraConterLabel);

    loraInMsgTextfield_1 = new Textfield("no Message", 2, 36, 124, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    display_addVisualisation(loraInMsgTextfield_1);

    loraInMsgTextfield_2 = new Textfield("", 2, 46, 124, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    display_addVisualisation(loraInMsgTextfield_2);

    btLabel = new Textfield("Blutooth", 2, 1, 64, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    display_addVisualisation(btLabel);

    btAdresses = new Textfield("", 2, 12, 124, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
    btAdresses->setFieldMode(FIELD_MODE_REPEATING, 10000);
    display_addVisualisation(btAdresses);
}

void showBTResults() {
  btLabel->setText("Blutooth");

  btDeviceCount = new Textfield("0", 50, 1, 60, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
  display_addVisualisation(btDeviceCount);
  std::string devisesCount = std::to_string(bluetooth_getScanResults()->getCount());
  devisesCount += " Devices";
  btDeviceCount->setText(devisesCount);

  btAdresses->setText("--  Beginn Liste  --");

  btAdresses->clearTextList();
  for (int i = 0; i < bluetooth_getScanResults()->getCount(); i++) {
    btAdresses->additionalText(bluetooth_getScanResults()->getDevice(i).getAddress().toString());
  }
}

void showBTScanProzessBar() {
  btLabel->setText("Scanning...");
  progressBar = new ProgressBar(64, 4, 50, 6);
  progressBar->setProgressTime(4500);
  display_addVisualisation(progressBar);
}

void removeBTScanProzessBar() {
  display_removeVisualisation(progressBar);
  if (progressBar != nullptr) {
    delete(progressBar);
    progressBar = nullptr;
  }
}

void removeBTDevicesCout() {
  display_removeVisualisation(btDeviceCount);
  if (btDeviceCount != nullptr) {
    delete(btDeviceCount);
    btDeviceCount = nullptr;
  }
}

void startBluetoothScan() {
  removeBTDevicesCout();
  showBTScanProzessBar();
  bluetooth_startScan(5);
}


void setup() {
  log_begin();

// setup Display
  display_start();
  createView();

// setup LoRa
  lora_start();
  lora_shall_receive(true);
  register_lora_msg_handler(handle_incoming_lora_msg);

// setup bluetooth
  bluetooth_start();
  callbackManager = new cb_mgr();
  callbackManager->register_callback(&startBluetoothScan, 300000);
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
