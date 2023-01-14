
#include "progressBar.h"

ProgressBar::ProgressBar(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth)
: Visualisation{x, y, width, heigth}
{
    this->isFilled = isFilled;
    this->progress = 0;
}

ProgressBar::~ProgressBar() {

}

void ProgressBar::setProgressTime(unsigned long progressTimeInMs) {
    this->setFieldMode(FIELD_MODE_LIMITED, progressTimeInMs / 100);
    this->progress = 0;
    this->isChanged();
}

void ProgressBar::setProgress(uint8_t progress) {
    this->progress = progress;
    this->isChanged();
}

void ProgressBar::executeAnimation() {
    if (this->progress < 100)
        this->progress++;

    this->isChanged();
}

void ProgressBar::show(SSD1306Wire* display) {
    display->drawProgressBar(this->getX(), this->getY(), this->getWidth(), this->getHeigth(), this->progress);
}

