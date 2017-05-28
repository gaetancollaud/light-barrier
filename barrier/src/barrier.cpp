#define MY_NODE_ID 20
#define MY_DEBUG
#define MY_RADIO_NRF24
#define BARRIER_SENSOR_ID 10
#define BARRIER_ACCURACY_ID 11

#define PIN_LIGHT_SENSOR A0
#define PIN_LASER 7

#define ACCURACY_SEND_DELAY_MS 1000

#include <Arduino.h>
#include <light-barrier.h>
#include <led-status.h>
#include <MySensors.h>

LightBarrier lightBarrier(PIN_LASER, PIN_LIGHT_SENSOR);
LedStatus ledStatus(&lightBarrier, LED_BUILTIN);

MyMessage msgBarrier(BARRIER_SENSOR_ID, V_STATUS);
MyMessage msgAccuracy(BARRIER_ACCURACY_ID, V_LEVEL);

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
void checkSendMessageObstacle(){
  bool obstacle = lightBarrier.isObstacle();
  if(lastObstacle!=obstacle){
    lastObstacle = obstacle;
    send(msgBarrier.set(obstacle));
  }
}

unsigned long nextAccuracy = 0;
void checkSendMessageAccuracy(unsigned long now){
	if(now>nextAccuracy){
		nextAccuracy = now + ACCURACY_SEND_DELAY_MS;
		send(msgAccuracy.set(lightBarrier.getDiffPercent(), 1));
		lightBarrier.skipNextMs(20);
	}
}


void loop(){
  unsigned long now = millis();

  lightBarrier.loop(now);
  ledStatus.loop(now);

  checkSendMessageObstacle();
  checkSendMessageAccuracy(now);
}
