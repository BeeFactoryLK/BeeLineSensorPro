/*--------------------------------------------------------------------------------------

 BeeLineSensorPro.h - Function and support library for the Beefactory 8X IR Sensor Panel

 Copyright (C) 2018 Bee Factory (beefactorylk <at> gmail <dot> com)

 ---

 This program is free software: you can redistribute it and/or modify it under the terms
 of the version 3 GNU General Public License as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 */


#ifndef BeeLineSensorPro_h
#define BeeLineSensorPro_h 
#include "Arduino.h" 

#define AVG_VALUES 4
#define LINE_BLACK 0
#define LINE_WHITE 1

class BeeLineSensorPro{
	private:
	boolean sensor_type;//0 BeeLineSensor | 1 BeeLineSensorPro
	int SER_Pin ;   //pin 14 on the 75HC595
	int RCLK_Pin ;  //pin 12 on the 75HC595
	int SRCLK_Pin ; //pin 11 on the 75HC595
	boolean registers[8];

	int SEN_PIN;
	int LED_PIN;
	int s_val_avg[8][AVG_VALUES];
	int s_val_avg_index;
	boolean calibrate_init;
	int rangemap[8]={255*8,255*4,255*2,255*1,255*1,255*2,255*4,255*8};
	
	unsigned char *pinmap;
	
	public: 
	int values[8];
	int values_map[8];
	int values_min[8];
	int values_max[8];
	int location;
	boolean color;
	BeeLineSensorPro(int SRCLK,int RCLK,int SER,int SEN,boolean color);//int SER,int RCLK,int SRCLK,int SEN,
	BeeLineSensorPro(unsigned char *pins,boolean c);
	int readSensor();
	void calibrate();
	void clearRegisters();
	void writeRegisters();
	void setRegisterPin(int index, int value);

};
#endif