#include <Arduino.h>           // To add IntelliSense for platform constants.
#if defined(ARDUINO_ARCH_ESP32)
#include <ESP32Servo.h>        // ESP32 LEDC-based servo control.
#else
#include <Servo.h>             // To control the SG90 servo motors.
#endif

#ifndef speed_servo_h
#define speed_servo_h

class SpeedServo {
  public:
    void attach(uint8_t pin);
    void moveNowTo(int position);
    void moveFastTo(int position);
    void moveSlowTo(int position);

  private:
    Servo _servo;
    int _lastPosition;
    void _moveTo(int newPosition, unsigned long stepDelay);
};

#endif /* speed_servo_h */
