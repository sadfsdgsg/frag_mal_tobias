
#pragma once

#include "visualisation.h"
#include <list>

class Textfield : public Visualisation {

private:
    String text;
    OLEDDISPLAY_TEXT_ALIGNMENT alignment;
    const uint8_t *fontData;
    std::list<String> textList;

public:
    Textfield(String text, uint16_t x, uint16_t y, uint16_t maxWidthInPixels, OLEDDISPLAY_TEXT_ALIGNMENT alignment = TEXT_ALIGN_LEFT, const uint8_t *fontData = ArialMT_Plain_10);
    ~Textfield();
    void additionalText(String text);
    void executeAnimation();
    void show(SSD1306Wire* display);

};