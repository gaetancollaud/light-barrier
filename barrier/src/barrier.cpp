#define MY_NODE_ID 20
#define MY_DEBUG
#define MY_RADIO_NRF24
#define PIN_LED LED_BUILTIN
#define BARRIER_SENSOR_ID 10

#include <Arduino.h>
#include <light-barrier.h>
#include <MySensors.h>

LightBarrier lightBarrier;


MyMessage msgBarrier(BARRIER_SENSOR_ID, V_STATUS);

void presentation(){
	sendSketchInfo("LightBarrier", "1.0");
  present(BARRIER_SENSOR_ID, S_BINARY);
}

void setup(){
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  lightBarrier.init();
}

bool lastObstacle = false;
void sendMessage(bool obstacle){
  if(lastObstacle!=obstacle){
    lastObstacle = obstacle;
    send(msgBarrier.set(obstacle));
  }
}


void loop(){
  unsigned long now = millis();

  lightBarrier.loop(now);


  bool obstacle = lightBarrier.isObstacle();
  // digitalWrite(PIN_LED, obstacle ? HIGH : LOW);
  sendMessage(obstacle);
}
