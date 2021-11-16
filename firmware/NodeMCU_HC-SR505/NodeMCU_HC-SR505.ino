int led = 16; // Подключим 13 пин, у которого есть свой светодиод
int pirPin = D1; // назначение пина для мини ИК датчика
int value; // переменная для хранения положения датчика
void setup () {
  Serial.begin(9600);
  pinMode (led, OUTPUT); // пин светодиода работает как выход
  pinMode (pirPin, INPUT); // пин датчика работает как вход
}
void loop () {
  value = digitalRead (pirPin); // чтение значения с датчика
  if (value == HIGH) { // когда с ИК сенсора появляется высокий уровень, светодиод загорается
    digitalWrite (led, HIGH);
    Serial.println("movement"); // Движение
  }
  else {
    digitalWrite (led, LOW);
    Serial.println("no movement"); // Движение закончено
  }
  delay(1000);
}
