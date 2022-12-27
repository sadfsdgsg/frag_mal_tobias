#pragma once

#include "heltec.h"
#include "stdlib.h"


void lauflicht(uint8_t speed_prct, uint8_t width = 6, uint8_t height = 3){
	//TODO: umbauen für cb_mgr
	static unsigned long last = 0;
	static int16_t x = 0;
	static const int16_t y = 20;
	static int dir = 1;
	speed_prct = speed_prct > 100 ? 100 : speed_prct;
	uint8_t wait = 11 - speed_prct/10;

	#ifdef DISPLAY_DEBUG
		static bool debug_wait = false;
		if (debug_wait && millis() - last < 200){
			return;
		} else {
			debug_wait = false;
		}
	#endif

	if (millis() - last < wait)
		return;
	last = millis();


	for (uint8_t i = 0; i < height; i++)
		Heltec.display -> setPixelColor(x+(-dir*width), y+i, OLEDDISPLAY_COLOR::BLACK);	

	x += dir;
	for (uint8_t i = 0; i < height; i++)
		Heltec.display -> setPixelColor(x, y+i, OLEDDISPLAY_COLOR::WHITE);

	#ifdef DISPLAY_DEBUG
		int16_t debug_y;
		int16_t font_height = 13;
		if (y-font_height > 0)
			debug_y = y-font_height;
		else
			debug_y = y+font_height;
		clear_rectangle(39, font_height, 60, y-font_height);
		Heltec.display -> drawString(60, debug_y, String(x));
		if (0 >= x || x >= 127)
			debug_wait = true;
	#endif

	if (0 >= x || x >= 127)
		dir = -dir;
}

void warte_animation(){
	static unsigned long last = 0;
	static int16_t pos_x = 0;
	static const int16_t pos_y = 20;
	static int dir = 1;

	if (millis() - last < 10){
		return;
	}
	last = millis();

	pos_x += dir;

	if (pos_x > 127 || pos_x < 0)
		dir = -dir;

	Heltec.display -> setPixelColor(pos_x, pos_y, OLEDDISPLAY_COLOR::INVERSE);
	Heltec.display -> setPixelColor(pos_x, pos_y+dir, OLEDDISPLAY_COLOR::INVERSE);
	Heltec.display -> display();
}