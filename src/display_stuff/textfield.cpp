
#include "textfield.h"

Textfield::Textfield(std::string text, uint16_t x, uint16_t y, uint16_t maxWidthInPixels, OLEDDISPLAY_TEXT_ALIGNMENT alignment, const uint8_t *fontData)
: Visualisation{x, y, maxWidthInPixels, 0}
{
    this->text = text;
    this->alignment = alignment;
    this->fontData = fontData;
}

Textfield::~Textfield() {

}

void Textfield::setText(std::string text) {
    if (this->getFieldMode() != FIELD_MODE_NO_ANIMATION)
        additionalText(text);
    else
        this->text = text;
}

std::string Textfield::getText() {
    return this->text;
}

void Textfield::additionalText(std::string text) {
    this->textList.push_back(text);
}

void Textfield::clearTextList() {
    this->textList.clear();
    this->text = "";
}

void Textfield::executeAnimation() {
    if (this == nullptr || this->textList.size() < 1 && this->text == "")
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
    const char* c_text = this->text.c_str();   //SSD1306Wire only work
    String text = c_text;                      // with String, not std::string

    while(display->getStringWidth(text) > this->getWidth() && text.length() > 0) {
        text = text.substring(0, text.length() - 1);
    }
    display->setFont(this->fontData);
    display->setTextAlignment(this->alignment);
    display->drawString(this->getX(), this->getY(), text);
}

