#include "light-barrier.h"


LightBarrier::LightBarrier(int pinLaser, int pinLightSensor) : pinLaser(pinLaser), pinLightSensor(pinLightSensor) {

}

void LightBarrier::init(){
  pinMode(pinLightSensor, INPUT_PULLUP);
  pinMode(pinLaser, OUTPUT);
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
        int readOn = analogRead(pinLightSensor);
        digitalWrite(pinLaser, LOW);
        laserOn = false;

        if(!calibrated || abs(normalOn-readOn)<abs(normalOff-readOn)){
          //clear or init
          sumOn += readOn;
          sumOff += lastReadOff;
          count++;

          obstacle = false;
        }else{
          //something in the way
          obstacle = true;
        }
      }else{
        lastReadOff = analogRead(pinLightSensor);
        digitalWrite(pinLaser, HIGH);
        laserOn = true;
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

void LightBarrier::checkCalibration(unsigned long now){
  if(nextTimeCalibration<=now){
    nextTimeCalibration = now + 500;
    if(count>0){
      computeNormal();
      float percent = getDiffPercent();
      calibrated = percent>DIFF_TOLERANCE_PERCENT;

#ifdef DEBUG_LIGHT_BARRIER
      Serial.print(normalOff);
      Serial.print("\t");
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
bool LightBarrier::isObstacle(){
  return this->obstacle;
}
