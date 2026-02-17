// Platform libraries.
#include <Arduino.h>  // To add IntelliSense for platform constants.

// Third-party libraries.

// My classes.
#include "speed-servo.h"
#include "status-led.h"
#include "proximity-sensor.h"

#include "config.h"  // To store configuration and secrets.

SpeedServo lidServo;
SpeedServo switchServo;
StatusLed led;
ProximitySensor sensor;

int lastSwitchState = 0;
long playCount = 0;
bool isLidOpen = false;
bool monitorSensor = false;

void setup() {
  initSerial();
  initServos();
  initLed();
  initSensor();
  monitorSensor = true;  // Always keep proximity monitoring active.
  pinMode(PIN_SWITCH, INPUT_PULLUP);

  Serial.printf("Application version: %s\n", APP_VERSION);
  Serial.println("Setup completed.");
}

void initSerial() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Initializing serial connection DONE.");
}

void initServos() {
  lidServo.attach(PIN_LID_SERVO);
  lidServo.moveNowTo(LID_START_POSITION);

  switchServo.attach(PIN_SWITCH_SERVO);
  switchServo.moveNowTo(SWITCH_START_POSITION);
}

void initLed() {
  led.setPin(LED_BUILTIN);
  led.turnOff();
}

void initSensor() {
  sensor.attach(PIN_SENSOR_SDA, PIN_SENSOR_SCL, SENSOR_TRIGGER_THRESHOLD);
}

void loop() {
  int switchState = digitalRead(PIN_SWITCH);
  boolean isSwitchTurnedOn = (switchState != lastSwitchState) && (switchState == LOW);

  if (isSwitchTurnedOn) {
    Serial.printf("[Loop] Switch toggled -> ON (state=%d)\n", switchState);
    led.turnOn();
    run();
    isLidOpen = false;
    led.turnOff();
  } else {
    // Check the proximity sensor.
    if (sensor.isInRange()) {
      if (!isLidOpen && monitorSensor) {
        Serial.println("[Loop] Proximity triggered, opening lid fast");
        openLidFast();
        isLidOpen = true;
      }
    } else {
      if (isLidOpen) {
        Serial.println("[Loop] Proximity cleared, closing lid fast");
        closeLidFast();
        isLidOpen = false;
      }
    }
  }

  lastSwitchState = switchState;

  // Wait 250 ms before next reading (required for the sensor).
  delay(250);
}

void run() {
  Serial.printf("[Run] playCount=%ld -> case %ld\n", playCount, playCount % 10);
  switch (playCount % 10) {
    case 0:
    case 1:
      runSlow();
      break;
    case 2:
      runWaitThenFast();
      break;
    case 3:
      runFast();
      break;
    case 4:
      runFastThenClap();
      monitorSensor = true;
      break;
    case 5:
      runOpenCloseThenFast();
      monitorSensor = false;
      break;
    case 6:
      runPeekThenFast();
      break;
    case 7:
      runFastWithDelay();
      monitorSensor = true;
      break;
    case 8:
      runClap();
      monitorSensor = false;
      break;
    case 9:
      runHalf();
      break;
    default:
      break;
  }

  playCount++;
}

void runSlow() {
  openLidSlow();
  flipSwitchSlow();
  closeLidSlow();
}

void runWaitThenFast() {
  Serial.println("[Action] Wait 5s then fast flip");
  delay(5000);
  flipSwitchFast();
}

void runFast() {
  Serial.println("[Action] Fast flip");
  flipSwitchFast();
}

void runFastThenClap() {
  Serial.println("[Action] Fast flip then clap");
  flipSwitchFast();
  clapLid();
}

void runOpenCloseThenFast() {
  Serial.println("[Action] Open/close then fast");
  openLidSlow();
  delay(2000);
  closeLidSlow();
  delay(2000);
  flipSwitchFast();
}

void runPeekThenFast() {
  Serial.println("[Action] Peek then fast");
  switchServo.moveSlowTo(SWITCH_HALF_POSITION);
  delay(3000);
  switchServo.moveFastTo(SWITCH_START_POSITION);
  delay(3000);
  flipSwitchFast();
}

void runFastWithDelay() {
  Serial.println("[Action] Open slow, wait, fast flip, close fast");
  openLidSlow();
  delay(4000);
  flipSwitchFast();
  closeLidFast();
}

void runClap() {
  Serial.println("[Action] Clap x4 then flip");
  clapLid();
  clapLid();
  clapLid();
  clapLid();
  openLidFast();
  flipSwitchFast();
  closeLidFast();
}

void runHalf() {
  Serial.println("[Action] Half flip then full");
  switchServo.moveSlowTo(SWITCH_HALF_POSITION);
  delay(3000);
  switchServo.moveFastTo(SWITCH_END_POSITION);
  switchServo.moveFastTo(SWITCH_START_POSITION);
}

void openLidSlow() {
  Serial.println("[Servo] Lid slow -> end");
  lidServo.moveSlowTo(LID_END_POSITION);
}

void openLidFast() {
  Serial.println("[Servo] Lid fast -> end");
  lidServo.moveFastTo(LID_END_POSITION);
}

void closeLidSlow() {
  Serial.println("[Servo] Lid slow -> start");
  lidServo.moveSlowTo(LID_START_POSITION);
}

void closeLidFast() {
  Serial.println("[Servo] Lid fast -> start");
  lidServo.moveFastTo(LID_START_POSITION);
}

void clapLid() {
  Serial.println("[Servo] Clap lid");
  openLidFast();
  closeLidFast();
}

void flipSwitchSlow() {
  Serial.println("[Servo] Switch slow flip");
  switchServo.moveSlowTo(SWITCH_END_POSITION);
  switchServo.moveSlowTo(SWITCH_START_POSITION);
}

void flipSwitchFast() {
  Serial.println("[Servo] Switch fast flip");
  switchServo.moveFastTo(SWITCH_END_POSITION);
  switchServo.moveFastTo(SWITCH_START_POSITION);
}
