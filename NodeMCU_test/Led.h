#import <Arduino.h>
namespace Led{
	static void setup()
	{
	  pinMode(D0, OUTPUT);
	}
unsigned long previousTime = millis();

const unsigned long interval = 1000;
   
	static void loop()
	{
	  digitalWrite(D0, HIGH);
	  delay(500);
	  digitalWrite(D0, LOW);
	  delay(500);
     ArduinoOTA.handle();
  unsigned long diff = millis() - previousTime;
  if(diff > interval) {
    digitalWrite(led, !digitalRead(led));  // Change the state of the LED
    previousTime += diff;
  }
	}
}
