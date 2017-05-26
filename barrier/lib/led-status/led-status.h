#ifndef _LED_STATUS_H
#define _LED_STATUS_H

#include <Arduino.h>
#include <light-barrier.h>

class LedStatus {
public:
  LedStatus(LightBarrier* lightBarrier, int pinLed);
  void init();
  void loop(unsigned long now);

private:
  LightBarrier* lightBarrier;
  int pinLed;

  unsigned long nextLedChange;
  bool ledOn;

};

#endif
