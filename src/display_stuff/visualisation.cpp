
#include "visualisation.h"

Visualisation::Visualisation(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->heigth = heigth;
    this->fieldMode = FIELD_MODE_NO_ANIMATION;
    this->interval = 0;
    this->last_executed = 0;
    this->changes = true;
}

Visualisation::~Visualisation() {

}

uint16_t Visualisation::getX() {
    return this->x;
}

uint16_t Visualisation::getY() {
    return this->y;
}

uint16_t Visualisation::getWidth() {
    return this->width;
}

uint16_t Visualisation::getHeigth() {
    return this->heigth;
}

uint8_t Visualisation::getFieldMode() {
    return this->fieldMode;
}

void Visualisation::isChanged() {
    this->changes = true;
}

void Visualisation::setFieldMode(uint8_t fieldMode, unsigned long interval) {
    this->fieldMode = fieldMode;
    this->interval = interval;
}

bool Visualisation::hasChanges() {
    return this->changes;
}

void Visualisation::displayedChanges() {
    this->changes = false;
}

bool Visualisation::animate() {
    if (this->fieldMode == FIELD_MODE_NO_ANIMATION)
        return false;

    if (millis() - this->last_executed > this->interval)
    {
        this->last_executed = millis();
        this->changes = true;
        executeAnimation();
        return true;
    }
    return false;
}