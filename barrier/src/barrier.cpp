#include <Arduino.h>
#include <light-barrier.h>

#define PIN_LED LED_BUILTIN

LightBarrier lightBarrier;


void setup(){
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  lightBarrier.init();
}

void loop(){
  unsigned long now = millis();

  lightBarrier.loop(now);
  digitalWrite(PIN_LED, lightBarrier.isObstacle() ? HIGH : LOW);
}
