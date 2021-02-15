#include "BH1750.h"
#include "Led.h"
#include "Zoom.h"
#include "DHT11.h"

enum DeviceType {
  DT_Led = 1,   // 1 << 0,
  DT_BH1750 = 2,  // 1 << 1
  DT_Zoom = 3,
  DT_DHT11 = 4
};

const DeviceType g_deviceType = DT_DHT11;

void setup()
{
  switch (g_deviceType) {
    case DT_Led:
      Led::setup();
      break;
    case DT_BH1750:
      BH1750::setup();
      break;
    case DT_Zoom:
      Zoom::setup();
      break;
    case DT_DHT11:
      Dht11::setup();
      break;
  }
}

void loop()
{
  switch (g_deviceType) {
    case DT_Led:
      Led::loop();
      break;
    case DT_BH1750:
      BH1750::loop();
      break;
    case DT_Zoom:
      Zoom::loop();
      break;
          case DT_DHT11:
      Dht11::loop();
      break;
  }
}
