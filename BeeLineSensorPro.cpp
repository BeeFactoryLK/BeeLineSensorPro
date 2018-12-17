/*--------------------------------------------------------------------------------------

 BeeLineSensorPro.cpp - Function and support library for the Beefactory 8X IR Sensor Panel

 Copyright (C) 2018 Bee Factory (beefactorylk <at> gmail <dot> com)

 ---

 This program is free software: you can redistribute it and/or modify it under the terms
 of the version 3 GNU General Public License as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 */

#include "Arduino.h"
#include "BeeLineSensorPro.h"

BeeLineSensorPro::BeeLineSensorPro(int SRCLK,int RCLK,int SER,int SEN,boolean c){
	sensor_type=1;
	color=c;

	SRCLK_Pin = SRCLK;
	RCLK_Pin = RCLK ;
	SER_Pin = SER ;
	SEN_PIN = SEN;

	pinMode(SER_Pin, OUTPUT);
	pinMode(RCLK_Pin, OUTPUT);
	pinMode(SRCLK_Pin, OUTPUT);
	clearRegisters();
	writeRegisters();
}
BeeLineSensorPro::BeeLineSensorPro(unsigned char *pins,boolean c){
	sensor_type=0;
	color=c;
	pinmap=(unsigned char*)malloc(sizeof(unsigned char)*8);
	for(int i=0;i<8;i++){
		pinmap[i]=pins[i];
		pinMode(pinmap[i], INPUT_PULLUP);
	}

}



void BeeLineSensorPro::calibrate(){
	while(!calibrate_init){//init calibration first time array filling
		Serial.println("Calibrate Staring..");
		readSensor();
	}
	for(int c=0;c<8;c++){
		readSensor();
		if(values[c]<values_min[c] || values_min[c]==0){
			values_min[c]=values[c];
		}
		if(values[c]>values_max[c]){
			values_max[c]=values[c];
		}
	}
}

int BeeLineSensorPro::readSensor(){//int s_val_avg[8][AVG_VALUES];
	location=0;
	for(int i=0;i<8;i++){
		if(sensor_type==1){
			setRegisterPin(i,HIGH);
			writeRegisters();
			delayMicroseconds(50);  
			s_val_avg[i][s_val_avg_index]=analogRead(SEN_PIN);
		}else{
			s_val_avg[i][s_val_avg_index]=analogRead(pinmap[i]);
		}
		//Serial.print(s_val_avg[i][s_val_avg_index]);
		//Serial.print(',');
		int total=0;
		for(int c=0;c<AVG_VALUES;c++){
			total+=s_val_avg[i][c];
		}
		values[i]=total/AVG_VALUES;

		// CALIBRATING ON RUN
		if(calibrate_init){
			if(values[i]<values_min[i] || values_min[i]==0){
				values_min[i]=values[i];
			}
			if(values[i]>values_max[i]){
				values_max[i]=values[i];
			}
		}
		if(color){
			values_map[i]=map(values[i],values_min[i],values_max[i],rangemap[i],0);
		}else{
			values_map[i]=map(values[i],values_min[i],values_max[i],0,rangemap[i]);
		//	setRegisterPin((i*2)+1,values_map[i]>75);			
		}
		if(i<4){
			location+=values_map[i];
		}else{
			location-=values_map[i];
		}

		if(sensor_type==1){
			setRegisterPin(i,LOW);
		}
	}

	//Serial.println(',');
	s_val_avg_index++;
	if(s_val_avg_index==AVG_VALUES){
		s_val_avg_index=0;
		calibrate_init=true;
	}
	return location;
}

void BeeLineSensorPro::clearRegisters() {
	for (int i = 8-1; i >=  0; i--) {
		registers[i] = LOW;
	}
}

void BeeLineSensorPro::writeRegisters() {
	digitalWrite(RCLK_Pin, LOW);
	for (int i = 8-1; i >=  0; i--) {
		digitalWrite(SRCLK_Pin, LOW);
		int val = registers[i];
		digitalWrite(SER_Pin, val);
		digitalWrite(SRCLK_Pin, HIGH);
	}
	digitalWrite(RCLK_Pin, HIGH);

}

void BeeLineSensorPro::setRegisterPin(int index, int value) {
	registers[index] = value;
}

