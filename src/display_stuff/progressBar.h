
#pragma once

#include "visualisation.h"

class ProgressBar : public Visualisation {

private:
    bool isFilled;
    uint8_t progress;
    

public:
    ProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth);
    ~ProgressBar();
    void setProgressTime(unsigned long progressTime);
    void setProgress(uint8_t progress);
    void executeAnimation();
    void show(SSD1306Wire* display);

};