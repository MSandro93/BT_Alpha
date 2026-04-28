/*
  Streaming Music from Bluetooth
  
  Copyright (C) 2020 Phil Schatzmann
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

 //==> Example which shows how to use the built in ESP32 I2S < 3.0.0

#include <Arduino.h>
//#include "I2S.h"
#include "BluetoothA2DPSink.h"
#include "AudioTools.h"
#include <freertos/task.h>
#include <stdlib.h>

#define FWD 27
#define REV 32
#define LED 33
#define BT_EN 26
#define DAC_MCLK_EN 34
#define DAC_MUTEn 23
#define DAC_DMP 22
#define DAC_FMT 21
#define DAC_FLT 18
#define MCLK 16

I2SStream i2s;
BluetoothA2DPSink a2dp_sink(i2s);

int state = 0;
int timer_cnt = 0;
int switched_to_BT = 0;

void GPIO_Init()
{
  pinMode(FWD, INPUT);
  pinMode(REV, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BT_EN, OUTPUT);
  pinMode(DAC_DMP, OUTPUT);
  pinMode(DAC_FMT, OUTPUT);
  pinMode(DAC_MUTEn, OUTPUT);
  pinMode(DAC_FLT, OUTPUT);
}


// Then somewhere in your sketch:
void data_received_callback()
{
  Serial.println("Data packet received");
}


void setup()
{
  disableCore0WDT();

  digitalWrite(DAC_MUTEn, 0);

  Serial.begin(115200);
  GPIO_Init();
  
  digitalWrite(BT_EN, 1);

  auto cfg = i2s.defaultConfig();
  cfg.pin_bck = 19;
  cfg.pin_ws = 17;
  cfg.pin_data = 5;
  cfg.pin_mck = 0;
  cfg.bits_per_sample = 16;
  cfg.sample_rate = 44100;
  cfg.i2s_format = I2S_STD_FORMAT;

  if (!i2s.begin(cfg))
  {
      Serial.print("Failed to initialize I2S!");
      Serial.print("\r\n");
      while (1); // do nothing
  }

    a2dp_sink.set_auto_reconnect(true, 3);

    a2dp_sink.start("MyMusic");

    state = 0;
    timer_cnt = 0;

    digitalWrite(DAC_MUTEn, 1);
}

int cnt = 0;




void loop()
{ /*
  switch(state)
  {
    case 0: //idle
    {
      if(digitalRead(REV) == 1)
      {
        state = 1;
        break;
      }
      else if(digitalRead(FWD) == 1)
      {
        state = 3;
        break;
      }

      break;
    }

    case 1: //wating for enable/disable BT timeout
    {
      if(digitalRead(REV) == 0) // revert was ment not switching on/off
      {
        state = 2;
        timer_cnt = 0;
        break;
      }

      timer_cnt++;
      if(timer_cnt >= 60)
      {
        timer_cnt = 0;
        if(switched_to_BT = 0) //currently switched to normal radio
        {
          switched_to_BT = 1;
          digitalWrite(BT_EN, 1);
          a2dp_sink.start("MyMusic");
          digitalWrite(LED, 1);
          digitalWrite(DAC_MUTEn, 1); //unmute
          state = 0;
        }
        else
        {
          digitalWrite(DAC_MUTEn, 0);  //mute
          switched_to_BT = 0;
          digitalWrite(BT_EN, 0);
          a2dp_sink.stop();
          digitalWrite(LED, 0);
          state = 0;
        }
      }
      break;
    }

    case 2:
    {
      a2dp_sink.previous();
      state = 0;
      break;
    }

    case 3:
    {
      if(digitalRead(FWD) == 0)
      {
        a2dp_sink.next();
        state  = 0;
        break;
      }
    }
  }
*/

 vTaskDelay(10);
}