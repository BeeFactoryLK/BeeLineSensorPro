/*--------------------------------------------------------------------------------------

 Prints Line error on serial console
 
 Copyright (C) 2018 Bee Factory (beefactorylk <at> gmail <dot> com)

 ---

 This program is free software: you can redistribute it and/or modify it under the terms
 of the version 3 GNU General Public License as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 */

#include <BeeLineSensorPro.h>

BeeLineSensorPro sensor = BeeLineSensorPro((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, A6, A7
}, LINE_BLACK);

void setup() {
  Serial.begin(115200);
}

void loop() {
  int err = sensor.readSensor();
  Serial.println(err);
  delay(100);
}
