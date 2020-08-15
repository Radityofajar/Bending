#include "HX711.h" 
#include <EEPROM.h>
 
#define CalibrationM 0
#define CalibrationC 4
 
//pin analog A0 u/DT dan A1 u/SCK
HX711 scale(A0, A1); // (DT, SCK)

uint16_t Calibration1Tera;
uint16_t Calibration2Tera;
long CalibrationDeformation1;
long CalibrationDeformation2;
float i = 0.0;
 
long lastMillis;
 
void setup() {
  Serial.begin(9600);
  Serial.println("Calibration Load Cell");
  Serial.println();
   
  float m,c;
  EEPROM.get(CalibrationM, m);
  EEPROM.get(CalibrationC, c);
  scale.power_up();
  scale.set_scale(m);
  scale.set_offset(c);
  scale.power_down();
 
  lastMillis = millis();
}
 
void loop() {
  if(Serial.available())
  {
      scale.power_up();
      delay(1000);
      Calibration1Tera = Serial.parseInt();
      CalibrationDeformation1 = scale.read_average(10);
      Serial.println("Load = " + String(Calibration1Tera) + " , measured = " + String(CalibrationDeformation1) + " bit");
      Serial.println("Place Heavier Load  :"); 
      scale.power_down();
      delay(500);
      scale.power_up();
      delay(1000);
      Calibration2Tera = Serial.parseInt();
      CalibrationDeformation2 = scale.read_average(10);
      Serial.println("Load = " + String(Calibration2Tera) + " gram, measured = " + String(CalibrationDeformation2) + " bit");
       
      float m = 1.0 * (CalibrationDeformation2 - CalibrationDeformation1) / ( Calibration2Tera - Calibration1Tera);
      float c = CalibrationDeformation2 - (1.0 * m * Calibration2Tera); //offset
 
      scale.set_scale(m);
      scale.set_offset(c);
      EEPROM.put(CalibrationM, m);
      EEPROM.put(CalibrationC, c);
 
      Serial.print("Scale = "); 
      Serial.println(m);
      Serial.print("Offset = ");
      Serial.println(c);
      
      scale.power_down();
      Serial.println("Calibration Done.");
      
    while (i < 21.0){
      scale.power_up();
      delay(2000);
      float deformasi = scale.get_units(10);
      float shear = (deformasi * (20 - i)); //rumus momen
      scale.power_down();
      //Serial.println("Deformasi : ");
      Serial.print(deformasi);
      Serial.print(" ");
      Serial.print("\t");
      //Serial.println("Bending Moment : ");
      Serial.print(shear);
        i = i+1;
    }
  }
}

