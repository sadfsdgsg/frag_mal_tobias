
#include "rectangle.h"

Rectangle::Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t heigth, bool isFilled)
: Visualisation{x, y, width, heigth}
{
    this->isFilled = isFilled;
}

Rectangle::~Rectangle() {

}

void Rectangle::executeAnimation() {

}

void Rectangle::show(SSD1306Wire* display) {
    if (this->isFilled) {
        display->fillRect(this->getX(), this->getY(), this->getWidth(), this->getHeigth());
    } else {
        display->drawRect(this->getX(), this->getY(), this->getWidth(), this->getHeigth());
    }


}

