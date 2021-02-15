#import <Arduino.h>
namespace Led {
static void setup()
{
  pinMode(D0, OUTPUT);
}

static void loop()
{
  digitalWrite(D0, HIGH);
  delay(500);
  digitalWrite(D0, LOW);
  delay(500);
}
}
