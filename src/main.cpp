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
#include "config.h"

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

void setup()
{
	Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
	Heltec.display->clear();
	WIFISetUp();
  
	WiFi.disconnect(true);// Reinitialize WiFi
	delay(1000);
	WiFi.mode(WIFI_STA);

	display_test();
}

void loop()
{
	WIFIScan();
	delay(2000);
}
