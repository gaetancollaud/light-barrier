#include "light-barrier.h"
#include <Arduino.h>

void LightBarrier::init(){
  pinMode(PIN_LIGHT_SENSOR, INPUT_PULLUP);
  pinMode(PIN_LASER, OUTPUT);
  this->nextLaserChange = millis();
}

void LightBarrier::loop(unsigned long now){
  this->checkCalibration(now);
  this->checkLaser(now);
}

void LightBarrier::checkLaser(unsigned long now){
    if(nextLaserChange<=now){
      nextLaserChange += PULSE_DELAY_MS;

      if(laserOn){
        int readOn = analogRead(PIN_LIGHT_SENSOR);
        digitalWrite(PIN_LASER, LOW);

        if(!calibrated || abs(normalOn-readOn)<abs(normalOff-readOn)){
          //clear or init
          sumOn += readOn;
          sumOff += lastReadOff;
          count++;
        }else{
          //something in the way
          Serial.println("Something in the way");
        }
      }else{
        lastReadOff = analogRead(PIN_LIGHT_SENSOR);
        digitalWrite(PIN_LASER, HIGH);

      }

    }
}

void LightBarrier::computeNormal(){
    normalOff = sumOff/count;
    normalOn = sumOn/count;

    count = 0;
    sumOff = 0;
    sumOn = 0;
}

float LightBarrier::getDiffPercent(){
  int diff = normalOff-normalOn;
  return 100.0*diff/1024;
}

// bool LightBarrier::inSight(bool init){
//   digitalWrite(PIN_LASER, HIGH);
//   delay(PULSE_DELAY_MS);
//   int readOn = analogRead(PIN_LIGHT_SENSOR);
//
//   digitalWrite(PIN_LASER, LOW);
//   delay(PULSE_DELAY_MS);
//   int readOff = analogRead(PIN_LIGHT_SENSOR);
//
//   if(init || abs(normalOn-readOn)<abs(normalOff-readOn)){
//     //clear or init
//     sumOn += readOn;
//     sumOff += readOff;
//     count++;
//   }else{
//     //something in the way
//     Serial.println("Something in the way");
//   }
// }

void LightBarrier::checkCalibration(unsigned long now){
  if(nextTimeCalibration<=now){
    nextTimeCalibration = now + 500;
    if(count>0){
      computeNormal();

      float percent = getDiffPercent();
      calibrated = percent>DIFF_TOLERANCE_PERCENT;

#ifdef DEBUG_LIGHT_BARRIER
      Serial.print(count);
      Serial.print("\t");
      Serial.print(sumOn);
      Serial.print("\t");
      Serial.print(sumOff);

      Serial.print("\t\t");
      Serial.print(normalOn);
      Serial.print("\t");
      Serial.print(percent);
      Serial.print("%\t");
      Serial.println(calibrated);
#endif



    }
  }
}

bool LightBarrier::isCalibrated(){
  return this->calibrated;
}
