// include the library
#include "pinout.h"
#include "logger.h"
#include "Arduino.h"
#include <RadioLib.h>
#include "lora.hpp"

void handle_incoming_lora_msg(String str){
  //prints the incoming LORA messages if there are any...
  log_info(str.c_str());
}

void setup() {
  log_begin();
  lora_start();
  lora_shall_receive(true);
  register_lora_msg_handler(handle_incoming_lora_msg);
  logf_info("Logging mit printf ist toll %d, %s, %.3f", 1337, "!!!1einself", 3.1415926);
}

void loop() {
  lora_maintain();
  delay(10); //to not spam too much...
}
