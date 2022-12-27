/*
 * HelTec Automation(TM) WIFI_Kit_32 factory test code, witch includ
 * follow functions:
 * - Basic OLED function test;
 * - Basic serial port test(in baud rate 115200);
 * - LED blink test;
 * - WIFI join and scan test;
 * - Timer test and some other Arduino basic functions.
 * by Aaron.Lee from HelTec AutoMation, ChengDu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.cn
 * this project also releases on GitHub:
 * https://github.com/HelTecAutomation/Heltec_ESP32
*/

/*
to get the heltec board to run, do the following defines in heltec.h:
#define WIFI_LoRa_32
#define RST_LoRa 15
#define DIO0 0
*/

#include "heltec.h"
#include "../lib/heltec/examples/Factory_Test/WIFI_Kit_32_FactoryTest/images.h"
#include "wifi_stuff.hpp"
#include "cb_mgr.h"
#include "debug_config.h"
#include "animations.hpp"

void display_test(){
	unsigned long b = 1;
	String str = "a";
	uint16_t remaining_width = Heltec.display->getWidth();
	uint8_t width_letter = 10;

	while (remaining_width >= width_letter){
		Heltec.display -> clear();
		Heltec.display -> setColor(OLEDDISPLAY_COLOR::WHITE);
		str += "a";
		Heltec.display -> drawString(0, 0, str);
		Heltec.display -> drawString(0, 15, String(b));
		Heltec.display -> display();
		b++;
		remaining_width -= width_letter;
		delay(500);
	}
	Heltec.display -> setColor(OLEDDISPLAY_COLOR::WHITE);
}

/**
 * Funktion um Bereich des Displays aufzuräumen.
 * 
 * */
void clear_rectangle(int16_t width, int16_t height, int16_t start_x = 0 , int16_t start_y = 0){
	for (int _x = start_x; _x < start_x+width; _x++)
		for (int _y = start_y; _y < start_y+height; _y++)
			Heltec.display -> clearPixel(_x, _y);
}

void count_up(){
	static unsigned long i = 0;
	clear_rectangle(127, 13);
	Heltec.display -> drawString(0, 0, String(i++));
}

cb_mgr callback_manager; 
void setup(){
	Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/);
	//WIFISetUp();
  
	//WiFi.disconnect(true);// Reinitialize WiFi
	//WiFi.mode(WIFI_STA);
	
	//lauflicht();
	Heltec.display -> clear();
	callback_manager.register_callback(count_up, 1000);
}

void loop(){
	//delay(1000);
	//digitalWrite(LED, !digitalRead(LED));

	lauflicht(50, 20, 5);
	callback_manager.maintain();
	Heltec.display -> display();
	//WIFIScan();
}
