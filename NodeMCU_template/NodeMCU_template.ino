#include "Led.h"
#include "BH1750.h"
#include "Zoom.h"
#include "DHT11.h"

const size_t DeviceTypeCount = 4;
enum DeviceType {
  DT_Led = 1,
  DT_BH1750 = 2,
  DT_Zoom = 4,
  DT_DHT11 = 8
};
//const auto g_connectedDevicesTypes = DT_Led | DT_Zoom; example
const auto g_connectedDevicesTypes = DT_Led;

void setup()
{
  for (int i = 0; i < DeviceTypeCount; i++) {
    if (g_connectedDevicesTypes & (1 << i)) {
      switch (1 << i) {
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
  }
}

void loop()
{
  for (int i = 0; i < DeviceTypeCount; i++) {
    if (g_connectedDevicesTypes & (1 << i)) {
      switch (1 << i) {
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
  }
}
