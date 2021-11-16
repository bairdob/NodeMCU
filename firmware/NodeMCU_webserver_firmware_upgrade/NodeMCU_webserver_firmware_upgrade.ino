
//Файл для загрузки должен иметь расширение *.bin
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
const char* ssid =     "Keenetic-3556";     //  SSID wi-fi роутера
const char* password = "bair457589"; // Пароль от wi-fi
#define OTAUSER         "admin"    // Логин для входа в OTA
#define OTAPASSWORD     "admin"    // Пароль для входа в ОТА
#define OTAPATH         "/firmware"// Путь, который будем дописывать после ip адреса в браузере.
#define SERVERPORT      80         // Порт для входа, он стандартный 80 это порт http
ESP8266WebServer HttpServer(SERVERPORT);
ESP8266HTTPUpdateServer httpUpdater;
//-----------------------------------------------------------------
void setup(void) {
  WiFi.begin(ssid, password);
  /* Ждем подключения к Wifi */
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  httpUpdater.setup(&HttpServer, OTAPATH, OTAUSER, OTAPASSWORD);
  HttpServer.onNotFound(handleNotFound);
  HttpServer.begin();

}
void loop(void) {
  HttpServer.handleClient();       // Прослушивание HTTP-запросов от клиентов

}
/* Выводить надпись, если такой страницы ненайдено */
void handleNotFound() {
  HttpServer.send(404, "text/plain", "404: Not found");
}
