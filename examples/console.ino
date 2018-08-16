#include <SPI.h>

#include <WiFi101.h>
#include <RemoteConsole.h>
#include "auth.h"

RemoteConsole remote_console(1000);

#define BUILTIN_LED_IO 13

int count = 0;

// Error handler 

void error(const __FlashStringHelper *err) {
	remote_console.println(err);
	while(1)
	{
		digitalWrite(BUILTIN_LED_IO, !digitalRead(BUILTIN_LED_IO));
		delay(100);
	}
}

void setup_wifi(void)
{

	WiFi.setPins(8,7,4,2);
	if (WiFi.status() == WL_NO_SHIELD) {
		error(F("WiFi module not present"));
	}

	//IPAddress ip = IPAddress(192,168,1,22);
	//WiFi.config(ip);


	remote_console.println(F("Waiting 5s for descovery of networks"));
	delay(5000);
	
	//wifi_list_networks();
}

void wifi_connect()
{

	if(WiFi.status() == WL_CONNECTED)
	{
		return;
	}

	int tries = 5;
	while(WiFi.status() != WL_CONNECTED)
	{
		remote_console.print(F("Attempting to connect to WPA SSID: "));
		remote_console.println(wifi_ssid);
		WiFi.begin(wifi_ssid, wifi_password);
		{
			if(tries < 0)
			{
				error(F("Unable to connect to WiFi"));
			}
		}
		delay(5000);
	}

	Serial.print("IP Address: ");
	IPAddress ip = WiFi.localIP();
	Serial.println(ip); 
}

// Setup Routine

void setup() {

	pinMode(BUILTIN_LED_IO, OUTPUT);
	digitalWrite(BUILTIN_LED_IO, 0);

	remote_console.begin(115200, 30);

	digitalWrite(BUILTIN_LED_IO, 1);

	setup_wifi();
	wifi_connect();
	 
	remote_console.connect();

	digitalWrite(BUILTIN_LED_IO, 0);
}


void loop() 
{	
	remote_console.print("Message tick tock ");
	remote_console.println(count);
	remote_console.loop();
	delay(1000);
	count++;
}

