
#pragma once

#include "visualisation.h"

class Rectangle : public Visualisation {

private:
    bool isFilled;

public:
    Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, bool isFilled);
    ~Rectangle();
    void executeAnimation();
    void show(SSD1306Wire* display);

};