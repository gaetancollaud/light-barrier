#define MY_NODE_ID 20
#define MY_DEBUG
#define MY_RADIO_NRF24
#define BARRIER_SENSOR_ID 10

#include <Arduino.h>
#include <light-barrier.h>
#include <led-status.h>
#include <MySensors.h>

LightBarrier lightBarrier;
LedStatus ledStatus(&lightBarrier, LED_BUILTIN);


MyMessage msgBarrier(BARRIER_SENSOR_ID, V_STATUS);
MyMessage msgCalibration(BARRIER_CALIBRATION_ID, V_LEVEL);

void presentation(){
	sendSketchInfo("LightBarrier", "1.0");
  present(BARRIER_SENSOR_ID, S_BINARY);
}

void setup(){
  Serial.begin(115200);

  lightBarrier.init();
	ledStatus.init();
}

bool lastObstacle = false;
void checkSendMessage(){
  bool obstacle = lightBarrier.isObstacle();
  if(lastObstacle!=obstacle){
    lastObstacle = obstacle;
    send(msgBarrier.set(obstacle));
  }
}


void loop(){
  unsigned long now = millis();

  lightBarrier.loop(now);
  ledStatus.loop(now);

  checkSendMessage();
}
