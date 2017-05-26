#include "led-status.h"

LedStatus::LedStatus(LightBarrier* lightBarrier, int pinLed) : lightBarrier(lightBarrier), pinLed(pinLed) {

}

void LedStatus::init(){
  pinMode(pinLed, OUTPUT);
}

void LedStatus::loop(unsigned long now){
  if(this->lightBarrier->isCalibrated()){
    digitalWrite(this->pinLed, this->lightBarrier->isObstacle() ? HIGH : LOW);
  }else{
    if(now>=this->nextLedChange){
      this->ledOn = !this->ledOn;
      digitalWrite(this->pinLed, this->ledOn ? HIGH : LOW);
    }
  }
}
