//BH1750.h
#import <Arduino.h>
#include <BH1750FVI.h>
static BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes); // D2 - SDA, D1 - SCL
namespace BH1750 {
static void setup()
{
  Serial.begin(115200);
  LightSensor.begin();   // Инициализируем датчик
}

static void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Освещенность: ");
  Serial.print(lux);
  Serial.println(" lX");
  delay(250);
}
}
