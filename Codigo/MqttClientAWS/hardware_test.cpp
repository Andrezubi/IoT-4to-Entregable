/*
#include <SPI.h>
#include "SmoothServo.h"
#include "RFID.h"
#include "LED.h"

// ─────────────────────────────────────────────────────────────────────────────
// Pin definitions
// ─────────────────────────────────────────────────────────────────────────────
#define RFID_SS_PIN   5
#define RFID_RST_PIN  4
#define SERVO_PIN     13
#define LED_PIN       15

// ─────────────────────────────────────────────────────────────────────────────
// Hardware objects
// ─────────────────────────────────────────────────────────────────────────────
SmoothServo servo(SERVO_PIN, 0, 2,15);
RFID        rfid(RFID_SS_PIN, RFID_RST_PIN);
LED         led(LED_PIN);

// ─────────────────────────────────────────────────────────────────────────────
// Timer state
// ─────────────────────────────────────────────────────────────────────────────
unsigned long moveStartMillis  = 0;
bool          timing           = false;
int           lastCommandAngle = 0;

// ─────────────────────────────────────────────────────────────────────────────
// Sweep state
// ─────────────────────────────────────────────────────────────────────────────
bool sweeping    = false;
int  sweepTarget = 180;

// ─────────────────────────────────────────────────────────────────────────────
// Serial input buffer
// ─────────────────────────────────────────────────────────────────────────────
String inputBuffer = "";

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

void printHelp() {
  Serial.println("─────────────────────────────────────");
  Serial.println(" Pet Door — Hardware Test Console");
  Serial.println("─────────────────────────────────────");
  Serial.println(" Commands:");
  Serial.println("   <angle>   move servo to angle (0-180)");
  Serial.println("   start     sweep servo back and forth");
  Serial.println("   stop      stop sweeping");
  Serial.println("   h         show this help");
  Serial.println("─────────────────────────────────────");
  Serial.println(" RFID: LED turns ON when tag present.");
  Serial.println("─────────────────────────────────────");
}

void startMove(int angle) {
  sweeping = false; // cancel sweep if active
  angle    = constrain(angle, 0, 180);

  Serial.print("[TEST] Command → move to ");
  Serial.print(angle);
  Serial.print("° from ");
  Serial.print(servo.getCurrentAngle());
  Serial.println("°");

  lastCommandAngle = angle;
  moveStartMillis  = millis();
  timing           = true;

  servo.moveTo(angle);
}

void updateSweep() {
  if (!sweeping)       return;
  if (!servo.isIdle()) return; // wait until servo reaches target before inverting

  sweepTarget = (sweepTarget == 180) ? 0 : 180;
  Serial.print("[SWEEP] → ");
  Serial.print(sweepTarget);
  Serial.println("°");
  servo.moveTo(sweepTarget);
}

// ─────────────────────────────────────────────────────────────────────────────
// setup()
// ─────────────────────────────────────────────────────────────────────────────

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  SPI.begin();
  rfid.init();
  servo.init();
  led.setState(LED::OFF);

  printHelp();
}

// ─────────────────────────────────────────────────────────────────────────────
// loop()
// ─────────────────────────────────────────────────────────────────────────────

void loop() {

  // ── Serial input ────────────────────────────────────────────────────────────
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      inputBuffer.trim();

      if (inputBuffer.length() > 0) {

        if (inputBuffer == "h" || inputBuffer == "H") {
          printHelp();

        } else if (inputBuffer == "start") {
          Serial.println("[SWEEP] Starting continuous sweep...");
          sweeping    = true;
          sweepTarget = 180;
          timing      = false; // sweep and timer don't mix
          servo.moveTo(sweepTarget);

        } else if (inputBuffer == "stop") {
          Serial.println("[SWEEP] Stopped.");
          sweeping = false;

        } else if (inputBuffer.toInt() != 0 || inputBuffer == "0") {
          startMove(inputBuffer.toInt());

        } else {
          Serial.print("[TEST] Unknown command: '");
          Serial.print(inputBuffer);
          Serial.println("' — type h for help.");
        }
      }

      inputBuffer = "";

    } else {
      inputBuffer += c;
    }
  }

  // ── Servo update + arrival timer ─────────────────────────────────────────
  bool arrived = servo.update();

  if (arrived && timing) {
    unsigned long elapsed = millis() - moveStartMillis;
    Serial.println("─────────────────────────────────────");
    Serial.print("[TIMER] Reached ");
    Serial.print(servo.getCurrentAngle());
    Serial.print("° in ");
    Serial.print(elapsed);
    Serial.println(" ms");
    Serial.println("─────────────────────────────────────");
    timing = false;
  }

  // ── Sweep ────────────────────────────────────────────────────────────────
  updateSweep();

  // ── RFID polling ─────────────────────────────────────────────────────────
  bool rfidChanged = rfid.update();

  if (rfidChanged) {
    if (rfid.getState() == RFID::TAG_PRESENT) {
      Serial.println("─────────────────────────────────────");
      Serial.print("[RFID] Tag detected → ");
      Serial.println(rfid.getCurrentUID());
      Serial.println("─────────────────────────────────────");
      led.setState(LED::ON);

    } else if (rfid.getState() == RFID::TAG_REMOVED) {
      Serial.print("[RFID] Tag removed  → was: ");
      Serial.println(rfid.getPreviousUID());
      led.setState(LED::OFF);
    }
  }

  // ── LED update ───────────────────────────────────────────────────────────
  led.update();
}
*/