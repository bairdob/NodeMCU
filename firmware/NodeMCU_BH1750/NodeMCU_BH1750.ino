#include <BH1750FVI.h>

BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes); // D2 - SDA, D1 - SCL

void setup() 
{
  Serial.begin(115200);
  LightSensor.begin();   // Инициализируем датчик
}

void loop()
{
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Освещенность: ");
  Serial.print(lux);
  Serial.println(" lX");
  delay(250);
}
