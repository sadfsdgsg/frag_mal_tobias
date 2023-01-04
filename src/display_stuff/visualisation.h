
#pragma once

#include "SSD1306Wire.h"

#define FIELD_MODE_NO_ANIMATION (0)
#define FIELD_MODE_LIMITED (1)
#define FIELD_MODE_REPEATING (2)

class Visualisation {

private:
    uint16_t x, y;
    uint16_t width, heigth;
    uint8_t fieldMode;
    unsigned long interval;
    unsigned long last_executed;
    bool changes;

protected:
    virtual void executeAnimation() = 0;
    uint16_t getX();
    uint16_t getY();
    uint16_t getWidth();
    uint16_t getHeigth();
    uint8_t getFieldMode();
    void isChanged();

public:
    Visualisation(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth);
    ~Visualisation();
    void setFieldMode(uint8_t fieldMode, unsigned long interval);
    bool hasChanges();
    void displayedChanges();
    bool animate();
    virtual void show(SSD1306Wire* display) = 0;
};