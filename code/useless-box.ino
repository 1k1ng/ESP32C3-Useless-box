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

const int NUM_REACTIONS = 18;  // 0-9 original, 10-17 new.
int lastReaction = -1;

// Picks a random reaction, never the same one twice in a row. The first press is always the gentle slow one.
int pickReaction() {
  if (playCount == 0) {
    return 0;
  }
  int reaction;
  do {
    reaction = random(NUM_REACTIONS);
  } while (reaction == lastReaction);
  return reaction;
}

void run() {
  int reaction = pickReaction();
  lastReaction = reaction;
  monitorSensor = true;  // Reactions that need a quiet lid switch it off below.
  Serial.printf("[Run] playCount=%ld -> reaction %d\n", playCount, reaction);
  switch (reaction) {
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
    case 10:
      runFakeOut();
      break;
    case 11:
      runTease();
      break;
    case 12:
      runSleepy();
      break;
    case 13:
      runLidDisco();
      monitorSensor = false;
      break;
    case 14:
      runAngry();
      break;
    case 15:
      runWiggle();
      break;
    case 16:
      runDoubleTake();
      break;
    case 17:
      runRandomSpeed();
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

// Pause for a random time between minMs and maxMs.
void randomPause(long minMs, long maxMs) {
  delay(random(minMs, maxMs + 1));
}

// Arm creeps out, gets scared and retreats, lid closes, then a surprise flip.
void runFakeOut() {
  Serial.println("[Action] Fake-out");
  openLidSlow();
  switchServo.moveSlowTo(SWITCH_HALF_POSITION);
  randomPause(500, 1500);
  switchServo.moveFastTo(SWITCH_START_POSITION);
  closeLidSlow();
  randomPause(2000, 6000);
  openLidFast();
  flipSwitchFast();
  closeLidFast();
}

// Arm pokes toward the switch a random number of times before the real flip.
void runTease() {
  Serial.println("[Action] Tease");
  openLidFast();
  int pokes = random(2, 5);
  for (int i = 0; i < pokes; i++) {
    switchServo.moveAtSpeedTo(random(40, 85), random(3, 10));
    randomPause(200, 700);
    switchServo.moveFastTo(SWITCH_START_POSITION);
    randomPause(300, 900);
  }
  flipSwitchFast();
  closeLidFast();
}

// Takes its time: long wait, then a very slow flip.
void runSleepy() {
  Serial.println("[Action] Sleepy");
  randomPause(6000, 12000);
  openLidSlow();
  randomPause(1000, 3000);
  flipSwitchSlow();
  closeLidSlow();
}

// Lid flaps a random number of times with random rhythm, then a quick flip.
void runLidDisco() {
  Serial.println("[Action] Lid disco");
  int claps = random(4, 9);
  for (int i = 0; i < claps; i++) {
    clapLid();
    randomPause(0, 400);
  }
  openLidFast();
  flipSwitchFast();
  closeLidFast();
}

// Flips immediately, then slams the lid and jabs the arm out again.
void runAngry() {
  Serial.println("[Action] Angry");
  flipSwitchFast();
  clapLid();
  clapLid();
  switchServo.moveFastTo(SWITCH_END_POSITION);
  randomPause(100, 400);
  switchServo.moveFastTo(SWITCH_START_POSITION);
  clapLid();
}

// Arm wiggles back and forth between peek and half position before flipping.
void runWiggle() {
  Serial.println("[Action] Wiggle");
  openLidFast();
  switchServo.moveSlowTo(60);
  int wiggles = random(3, 7);
  for (int i = 0; i < wiggles; i++) {
    switchServo.moveFastTo(SWITCH_HALF_POSITION);
    switchServo.moveFastTo(60);
  }
  randomPause(300, 1200);
  flipSwitchFast();
  closeLidFast();
}

// Looks around: lid opens and closes slowly, opens again, then flips.
void runDoubleTake() {
  Serial.println("[Action] Double take");
  openLidSlow();
  randomPause(800, 2000);
  closeLidSlow();
  randomPause(800, 2500);
  openLidFast();
  randomPause(300, 1000);
  flipSwitchFast();
  closeLidFast();
}

// Flip with a random speed for every movement.
void runRandomSpeed() {
  Serial.println("[Action] Random speed");
  lidServo.moveAtSpeedTo(LID_END_POSITION, random(1, 20));
  randomPause(200, 2000);
  switchServo.moveAtSpeedTo(SWITCH_END_POSITION, random(1, 20));
  switchServo.moveAtSpeedTo(SWITCH_START_POSITION, random(1, 20));
  lidServo.moveAtSpeedTo(LID_START_POSITION, random(1, 20));
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
