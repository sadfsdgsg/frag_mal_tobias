
#include "textfield.h"

Textfield::Textfield(String text, uint16_t x, uint16_t y, uint16_t maxWidthInPixels, OLEDDISPLAY_TEXT_ALIGNMENT alignment, const uint8_t *fontData)
: Visualisation{x, y, maxWidthInPixels, 0}
{
    this->text = text;
    this->alignment = alignment;
    this->fontData = fontData;
}

Textfield::~Textfield() {

}

void Textfield::additionalText(String text) {
    this->textList.push_back(text);
}

void Textfield::executeAnimation() {
    if (this->textList.size() < 1 && this->text == "")
        return;

    switch (this->getFieldMode())
    {
    case FIELD_MODE_REPEATING:
        if (this->text != "")
            this->textList.push_back(this->text);
    case FIELD_MODE_LIMITED:
        if (this->textList.size() < 1) {
            this->text = "";
        } else {
            this->text = this->textList.front();
            this->textList.pop_front();
        }
        break;
    default:
        break;
    }
}

void Textfield::show(SSD1306Wire* display) {
    while(display->getStringWidth(this->text) > this->getWidth() && this->text.length() > 0) {
        this->text = this->text.substring(0, this->text.length() - 1);
    }
    display->setFont(this->fontData);
    display->setTextAlignment(this->alignment);
    display->drawString(this->getX(), this->getY(), this->text);
}

