#pragma once

#include "Arduino.h"
#include "SSD1306Wire.h"
#include "logger.h"
#include "cb_mgr.h"
#include <list>
#include <stdlib.h>
#include "display_stuff/textfield.h"
#include "display_stuff/rectangle.h"
#include "display_stuff/progressBar.h"


//rotate only for GEOMETRY_128_64
SSD1306Wire display(0x3c, SDA_OLED, SCL_OLED, RST_OLED);
bool display_hasChanges = false;

std::list<Visualisation*> visualisations;

// typedef struct ProgessBar {
//     uint16_t x, y, width, height;
//     uint8_t progress;
//     bool isVisible;
// } ProgessBar;
// ProgessBar progressBar = {0, 0, 0, 0, 0, false};



void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void)  //Vext default OFF
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

/**
 * @brief  start and init the display.
 *          Display parameter are set for the Heltec LoRa board V3, display-size 128x64.
 *          Display parameter are ste in the OLEDDisplay.cpp.
 */
void display_start() {

  VextON();
  delay(100);

  display.init();
  display.clear();
  display.display();
  display.setContrast(255);
}

/**
 * @brief  Add a class object that inherits the Visualisation class, 
 *          it will be displayed at the next call the display_maintain function.
 * 
 * @param visualisation 
 */
void display_addVisualisation(Visualisation* visualisation) {
    visualisations.push_back(visualisation);
    display_hasChanges = true;
}

/**
 * @brief  remove the Visualisation from the display.
 *          Do not forget to delete the Visualiation object after call this function.        
 * 
 * @param visualisation 
 */
void display_removeVisualisation(Visualisation* visualisation) {
    if (visualisation == nullptr)
        return;

    visualisations.remove(visualisation);
    // delete(*visualisation);
    // *visualisation = nullptr;

    // display_hasChanges = true;
    // log_info("wurde geloscht");
}

/**
 * @brief display_maintain is necessary to update the display changing some Visualisation.
 * 
 */
void display_maintain() {
    for (Visualisation* visualisation : visualisations) {
        if (visualisation->animate() || visualisation->hasChanges())
            display_hasChanges = true;
    }
      
    if (!display_hasChanges)
        return;

    display.clear();

    for (Visualisation* visualisation : visualisations) {
        visualisation->show(&display);
    }

    display.display();
    display_hasChanges = false;
}
