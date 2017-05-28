#ifndef _LIGHT_BARRIER_H
#define _LIGHT_BARRIER_H

#include <Arduino.h>

#define PULSE_DELAY_MS 10
#define DIFF_TOLERANCE_PERCENT 2
// #define DEBUG_LIGHT_BARRIER

class LightBarrier {
public:
  LightBarrier(int pinLaser, int pinLightSensor);
  void init();
  void loop(unsigned long now);

  bool isCalibrated();
  bool isObstacle();
  float getDiffPercent();
  void skipNextMs(int ms);

private:
  int pinLaser;
  int pinLightSensor;

  unsigned long nextTimeCalibration = 0;
  unsigned long nextLaserChange=0;

  int skipNextMsCount = 0;
  int normalOff = 1024;
  int normalOn = 0;
  long sumOff = 0;
  long sumOn = 0;
  int count = 0;
  bool calibrated = false;
  int lastReadOff;
  bool laserOn=false;
  bool obstacle = false;


  void computeNormal();
  // bool inSight(bool init);
  void checkCalibration(unsigned long);
  void checkLaser(unsigned long);

};

#endif
