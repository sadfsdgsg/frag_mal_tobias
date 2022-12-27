// include the library
#include "pinout.h"
#include "serial_debug.h"
#include "Arduino.h"
#include <RadioLib.h>

#define RADIOLIB_DEBUG

SX1262 radio = new Module(LORA_SS, DIO1, LORA_RST, LORA_BUSY);


void setup() {
  Serial.begin(115200);

  SPI.begin(LORA_SCK,LORA_MISO,LORA_MOSI,LORA_SS);
  radio.setDio1Action(set_LoRa_flag);
  state = radio.startReceive();
  //zuhören wird durch diese methoden unterbrochen:
  //radio. standby(), sleep(), transmit(), receive(), readData(), scanChannel()
  
  if (IS_DEBUGGING){
    if (state == RADIOLIB_ERR_NONE) {
      DEBUG(F("success!"));
    } else {
      sprintf(msg_buf, "failed, code %d", state);
      DEBUG(msg_buf);
      while (true);
    }
  }

  DEBUG(F("[SX1262] Waiting for incoming transmission ... "));
}

void handle_incoming_lora_msg(){
  //since we are handling the current message, we don't want to receive the next... for now...
  lora_interrupt_enabled = false;

  String str;
  int state = radio.readData(str);

  if (state == RADIOLIB_ERR_NONE) {
    if (IS_DEBUGGING){
      DEBUG(F("success!"));
      //print received data, signal strength indicator and signal to noise ratio
      sprintf(msg_buf, "[LoRa] Data:\t\t%s\n, RSSI:\t\t %fdBm, SNR:\t\t%fdB", str, radio.getRSSI(), radio.getSNR());
      DEBUG(msg_buf);
    }

  } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
    // timeout occurred while waiting for a packet
    DEBUG(F("[LoRa] timeout!"));

  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    DEBUG(F("[LoRa] CRC error!"));

  } else {
    // some other error occurred
    sprintf(msg_buf,"[LoRa] failed, code %d", state);
    DEBUG(msg_buf);
  }
  // put module back to listen mode
  radio.startReceive();

  // we're ready to receive more packets,
  // enable interrupt service routine
  lora_interrupt_enabled = true;
}


void loop() {
  if (lora_rcv_flag){
    handle_incoming_lora_msg();
  }
}
