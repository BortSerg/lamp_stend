#include <Arduino.h>
#include <Ticker.h>

#define ON 0
#define OFF 1
#define DIRECT 1
#define REVERSE 0

byte Rel_1 = D1;
byte Rel_2 = D2;
byte Rel_3 = D3;
byte Rel_4 = D4;

byte pin_arr[] = {Rel_1, Rel_2, Rel_3, Rel_4};

byte count = 1;
uint16_t start_delay = 1000;
uint16_t pause = 500;
uint64_t system_startup = 0;
uint64_t previus_time = 0;

void set_one_rel(byte mode, byte num_rel)
{
  digitalWrite(num_rel, mode);
}

void set_all_rel(bool state)
{
  delay(start_delay);
  for (byte i = 0; i < 4; i++)
  {
    digitalWrite(pin_arr[i], state);
  }
}

void light_bar(byte mode, uint16_t pause = 0)
{
  if (mode == DIRECT)
  {
    for (byte i = 0; i < 4; i++)
    {
      delay(pause);
      digitalWrite(pin_arr[i], ON);
    }
  }

  if (mode == REVERSE)
  {
    for (int i = 3; i >= 0; i--)
    {
      delay(pause);
      digitalWrite(pin_arr[i], ON);
    }
  }
  set_all_rel(OFF);
}

void running_point(byte mode, uint16_t pause = 0)
{
  if (mode == DIRECT)
  {
    for (byte i = 0; i < 4; i++)
    {
      delay(start_delay);
      // Serial.println("ON " + String(pin_arr[i]));
      digitalWrite(pin_arr[i], ON);

      if (i > 0)
      {
        delay(pause);
        // Serial.println("OFF " + String(pin_arr[i - 1]));
        digitalWrite(pin_arr[i - 1], OFF);
      }
    }
  }

  if (mode == REVERSE)
  {
    for (int i = 3; i >= 0; i--)
    {
      delay(start_delay);
      // Serial.println("ON " + String(pin_arr[i]));
      digitalWrite(pin_arr[i], ON);

      if (i < 3)
      {
        delay(pause);
        // Serial.println("OFF " + String(pin_arr[i + 1]));
          digitalWrite(pin_arr[i + 1], OFF);
      }
    }
  }
  set_all_rel(OFF);
}

void setup()
{
  Serial.begin(115200);
  for (byte i = 0; i < 4; i++)
  {
    pinMode(pin_arr[i], OUTPUT);
    delay(5);
    digitalWrite(pin_arr[i], OFF);
    delay(5);
  }
}

void loop()
{
  count++;
  
  switch (count)
  {
    case 1:
      Serial.println("running point direct paus");
      running_point(DIRECT, pause);
      break;

    case 2:
      Serial.println("running point reverse pause");
      running_point(REVERSE, pause);
      break;
    case 3:  
      Serial.println("running point direct");
      running_point(DIRECT);
      break;
    case 4:
      Serial.println("running point reverse");
      running_point(REVERSE);
      break;
    case 5:
      Serial.println("light bar direct");
      light_bar(DIRECT, start_delay);
      break;
    case 6:
      Serial.println("light bar reverse");
      light_bar(REVERSE, start_delay);
      break;
    case 7:
     Serial.println("ON all lamps");
     set_all_rel(ON);
      break;
    case 8:
      Serial.println("OFF all lamps");
      set_all_rel(OFF);
      count = 0;
      break;
  }
  delay(15000);
  
}