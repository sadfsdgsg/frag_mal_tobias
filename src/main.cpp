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
#include "WiFi.h"
#include "../lib/heltec/examples/Factory_Test/WIFI_Kit_32_FactoryTest/images.h"
#include "wifi_config.h"
#include "debug_config.h"

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

void WIFISetUp(void)
{
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.disconnect(true);
	delay(1000);
	WiFi.mode(WIFI_MODE_STA);
	WiFi.setAutoConnect(true);
	WiFi.begin(WIFI_SSID, WIFI_PASSPHRASE);
	delay(100);

	byte count = 0;
	while(WiFi.status() != WL_CONNECTED && count < 20)
	{
		count ++;
		delay(500);
		Heltec.display -> drawString(0, 0, "Connecting...");
		Heltec.display -> display();
	}

	Heltec.display -> clear();
	if(WiFi.status() == WL_CONNECTED)
	{
		Heltec.display -> drawString(0, 0, "Connected.");
		Heltec.display -> display();
		//delay(500);
	}
	else
	{
		Heltec.display -> clear();
		Heltec.display -> drawString(0, 0, "Failed to connect.");
		Heltec.display -> display();
		//while(1);
	}
	Heltec.display -> drawString(0, 10, "WiFi Setup done.");
	Heltec.display -> display();
	delay(500);
}

void WIFIScan(void)
{
	Heltec.display -> drawString(0, 20, "Starting scan.");
	Heltec.display -> display();

	int n = WiFi.scanNetworks();
	Heltec.display -> drawString(0, 30, "Scan done.");
	Heltec.display -> display();
	delay(500);
	Heltec.display -> clear();

	if (n == 0)
	{
		Heltec.display -> clear();
		Heltec.display -> drawString(0, 0, "No network found");
		Heltec.display -> display();
		while(1);
	}
	else
	{
    Serial.print(n);
		Heltec.display -> drawString(0, 0, (String)n);
		Heltec.display -> drawString(14, 0, "Networks found:");
		Heltec.display -> display();
		delay(500);

		for (int i = 0; i < n; ++i) {
		// Print SSID and RSSI for each network found
			Heltec.display -> drawString(0, (i+1)*9,(String)(i + 1));
			Heltec.display -> drawString(6, (i+1)*9, ":");
			Heltec.display -> drawString(12,(i+1)*9, (String)(WiFi.SSID(i)));
			Heltec.display -> drawString(90,(i+1)*9, " (");
			Heltec.display -> drawString(98,(i+1)*9, (String)(WiFi.RSSI(i)));
			Heltec.display -> drawString(114,(i+1)*9, ")");
			//display.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
			delay(10);
		}
	}

	Heltec.display -> display();
	delay(800);
	Heltec.display -> clear();
}


void clear_rectangle(int16_t width, int16_t height, int16_t start_x = 0 , int16_t start_y = 0){
	for (int _x = start_x; _x < start_x+width; _x++)
		for (int _y = start_y; _y < start_y+height; _y++)
			Heltec.display -> clearPixel(_x, _y);
}

void lauflicht(uint8_t speed_prct, uint8_t width = 6, uint8_t height = 3){
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

	Heltec.display -> display();

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

void setup(){
	Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
	//WIFISetUp();
  
	//WiFi.disconnect(true);// Reinitialize WiFi
	//WiFi.mode(WIFI_STA);
	
	//lauflicht();
	Heltec.display -> clear();
}

void loop()
{
	//delay(1000);
	//digitalWrite(LED, !digitalRead(LED));

	lauflicht(50, 20, 5);
	//WIFIScan();
}
