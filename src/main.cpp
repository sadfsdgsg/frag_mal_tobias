// include the library
#include "pinout.h"
#include "logger.h"
#include "Arduino.h"
#include <RadioLib.h>
#include "lora.hpp"
#include "display.hpp"


void handle_incoming_lora_msg(uint8_t* data, size_t dataLength){
  //prints the incoming LORA messages if there are any...

  log_info("Message Begin ->");
  for (int i = 0; i < dataLength; i++)
  {
     logf_info("Byte %i = '%d'", i, data[i]);
  }
  log_info("<- Message End\n");
}

double prog = 0;

Textfield* report1;
Textfield* report2;
Textfield* report3;
Rectangle* line;
ProgressBar* progressBar;
ProgressBar* automatetProgressBar;

void createView() {
  report1 = new Textfield("FIELD_MODE_LIMITED", 5, 2, 110, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
  report1->setFieldMode(FIELD_MODE_LIMITED, 3000);
  report1->additionalText("yippi ya yeah");
  report1->additionalText("moin ihr Luschen");
  report1->additionalText("Das war ich nicht!");
  report1->additionalText("Das war schon kaputt");
  display_addVisualisation(report1);

  report2 = new Textfield("FIELD_MODE_REPEATING", 5, 15, 110, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
  report2->setFieldMode(FIELD_MODE_REPEATING, 3000);
  report2->additionalText("yippi ya yeah");
  report2->additionalText("moin ihr Luschen");
  report2->additionalText("Das war ich nicht!");
  report2->additionalText("Das war schon kaputt");
  display_addVisualisation(report2);

  report3 = new Textfield("FIELD_MODE_NO_ANIMATION", 5, 28, 110, TEXT_ALIGN_LEFT, ArialMT_Plain_10);
  display_addVisualisation(report3);

  line = new Rectangle(0, 14, 127, 1, false);
  display_addVisualisation(line);

  progressBar = new ProgressBar(10, 45, 100, 8);
  display_addVisualisation(progressBar);

  automatetProgressBar = new ProgressBar(10, 55, 100, 8);
  automatetProgressBar->setProgressTime(300);
  display_addVisualisation(automatetProgressBar);
}


void setup() {
  log_begin();
  display_start();
  createView();

  lora_start();
  lora_shall_receive(true);
  register_lora_msg_handler(handle_incoming_lora_msg);
  logf_info("Logging mit printf ist toll %d, %s, %.3f", 1337, "!!!1einself", 3.1415926);
}

void loop() {
  lora_maintain();
  display_maintain();

  if (prog < 100)
  {
    prog += 0.15;
    progressBar->setProgress(prog);

  } else if (prog < 200) {

  }


  delay(10); //to not spam too much...
}
