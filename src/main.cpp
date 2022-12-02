#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include "OTA.h"

#define ON 0
#define OFF 1
#define DIRECT 1
#define REVERSE 0

ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer server(80);

byte Rel_1 = D1;
byte Rel_2 = D2;
byte Rel_3 = D3;
byte Rel_4 = D4;

byte pin_arr[] = {Rel_1, Rel_2, Rel_3, Rel_4};

unsigned long system_run_time = 0;
unsigned long timer_on = 0;
unsigned long timer_off = 0;
uint16_t cycle_pause = 2000;
uint16_t turn_on_delay = 2000;
uint16_t turn_off_delay = 500;
uint16_t work_time = 1000;

void all_set_state(bool state)
{
  delay(cycle_pause);
  Serial.println("FULL STATE");
  for (byte i = 0; i < 4; i++)
  {
    digitalWrite(pin_arr[i], state);
  }
}

void light_bar(bool dir)
{
  if (dir == 1)
  {
    Serial.println("Прямое свечение");
    for (byte i = 0; i < 4; i++)
    {
      delay(turn_on_delay);
      digitalWrite(pin_arr[i], ON);
      Serial.println("ON " + String(pin_arr[i]));
    }
    all_set_state(OFF);
  }

  if (dir == 0)
  {
    Serial.println("Реверсивное свечение");
    for (byte i = 3; i < 0; i--)
    {
      delay(turn_on_delay);
      digitalWrite(pin_arr[i], ON);
      Serial.println("OFF " + String(pin_arr[i]));
    }
    all_set_state(OFF);
  }
}

void running_lights()
{
  for (byte i = 0; i < 4; i++)
  {
    Serial.println(i);
    while (true)
    {
      system_run_time = millis();
      if (system_run_time - timer_on > turn_on_delay) // Включить лампу
      {
        Serial.println("ON " + String(pin_arr[i]));
        digitalWrite(pin_arr[i], ON);
        timer_off = timer_on = system_run_time;

        if (i == 0)
        {
          break;
        }
      }

      if ((system_run_time - timer_off > turn_off_delay))
      {
        Serial.println("OFF " + String(pin_arr[i - 1]));
        digitalWrite(pin_arr[i - 1], OFF);
        break;
      }

      delay(1);
    }
    Serial.println("\n");
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("LampStedUpdater");
  WiFi.begin();

  OTA_Update();

  for (byte i = 0; i < 4; i++)
  {
    pinMode(pin_arr[i], OUTPUT);
    delay(5);
    digitalWrite(pin_arr[i], OFF);
    delay(5);
  }
  httpUpdater.setup(&server); // обновления через web-интерфейс  http://{local_IP}/update

  server.begin();
}

void loop()
{
  server.handleClient();
  ArduinoOTA.handle();

  // light_bar(1);

  all_set_state(ON);

  // light_bar(0);

  all_set_state(OFF);
  delay(7000);

  yield();
}