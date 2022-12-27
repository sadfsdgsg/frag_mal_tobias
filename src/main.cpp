// include the library
#include "pinout.h"
#include "serial_debug.h"
#include "Arduino.h"
#include <RadioLib.h>
#include "lora.hpp"

void handle_incoming_lora_msg(String str){
  //prints the incoming LORA messages if there are any...
  DEBUG(str);
}

void setup() {
  lora_start();
  lora_shall_receive(true);
  register_lora_msg_handler(handle_incoming_lora_msg);
}

void loop() {
  lora_maintain();
  delay(10); //to not spam too much...
}
