#import <Arduino.h>
const int SoundPin = D8; // подключение на +
int DelaySound = 1000; // Пауза 1 секунда
namespace Zoom{
  void setup() 
  { 
  }
unsigned long previousTime = millis();

const unsigned long interval = 1000;
   
  void loop() 
  { 
    // Пример использования tone() 
    //tone(pin, частота) 
    tone(SoundPin, 1915);  // Воспроизводим сигнал с частотой 1915 Гц 
    delay(DelaySound);  // Пауза 1 секунда (1000 миллисекунд – значение переменной DelaySound ) – длительность воспроизведения сигнала
    tone(SoundPin, 1700); 
    delay(DelaySound);
    tone(SoundPin, 1519); 
    delay(DelaySound);
    tone(SoundPin, 1432); 
    delay(DelaySound);
    tone(SoundPin, 1275); 
    delay(DelaySound);
    tone(SoundPin, 1136); 
    delay(DelaySound);
    tone(SoundPin, 1014); 
    delay(DelaySound);
    noTone(7); // Выключаем звук 
      ArduinoOTA.handle();
  unsigned long diff = millis() - previousTime;
  if(diff > interval) {
    digitalWrite(led, !digitalRead(led));  // Change the state of the LED
    previousTime += diff;
  }
  }
}
