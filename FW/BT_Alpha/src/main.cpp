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



 // Cedits to: Phil Schatzmann (https://github.com/pschatzmann/ESP32-A2DP/tree/main)



#include <Arduino.h>
#include "BluetoothA2DPSink.h"
#include "AudioTools.h"

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


void setup()
{
  disableCore0WDT();

  digitalWrite(DAC_MUTEn, 0);

  Serial.begin(115200);
  GPIO_Init();

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

    state = 0;
    timer_cnt = 0;
}


void setState(int s_)
{
  state = s_;
  Serial.print(state);
  Serial.print("\r\n");
}


void loop()
{
  switch(state)
  {
    case 0: //idle
    {
      if((digitalRead(REV) == 1)  && (digitalRead(FWD) == 0))
      {
        setState(1);
      }
      else if((digitalRead(FWD) == 1) && (digitalRead(REV) == 0))
      {
        setState(2);
      }
      break;
    }

    case 1: //is holded left
    {
      timer_cnt++;

      if(timer_cnt >= 40) // was pressed long
        {
          if(switched_to_BT == 0) //currently switched to normal radio
          {
            switched_to_BT = 1;
            digitalWrite(BT_EN, 1);      
            digitalWrite(LED, 1);    
            a2dp_sink.start("BT Alpha");
            digitalWrite(DAC_MUTEn, 1); //unmute
            setState(3);
          }
          else
          {
            digitalWrite(DAC_MUTEn, 0);  //mute
            switched_to_BT = 0;
            digitalWrite(BT_EN, 0);
            a2dp_sink.disconnect();
            digitalWrite(LED, 0);
            setState(3);
          }
          timer_cnt = 0;
          break;
        }

      else if((digitalRead(REV)) == 1 && (digitalRead(FWD) == 1)) 
      {
        if (a2dp_sink.is_connected() == true) // was pressed short
        {
          a2dp_sink.previous();
          setState(0);
          Serial.print("a");
        }    
        break;
      }
    }


    case 2:  //next
    {
      if((digitalRead(REV)) == 1 && (digitalRead(FWD) == 1))
      {
        if (a2dp_sink.is_connected() == true)
        {
          a2dp_sink.next();
          setState(0);
        }
      }
      break;
    }

    case 3:
    {
      if((digitalRead(REV)) == 1 && (digitalRead(FWD) == 1))
      {
        setState(0);
      }
      break;
    }
  }

  delay(50);
}