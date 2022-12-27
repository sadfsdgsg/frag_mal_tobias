#pragma once
#include "WiFi.h"
#include "wifi_config.h"
#include "heltec.h"


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