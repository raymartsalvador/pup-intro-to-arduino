/*
 * Reverse Parking Assistant
 * Intro to Arduino - PUP Binan Workshop
 *
 * An HC-SR04 ultrasonic sensor measures the distance to a wall and
 * warns the driver with three LEDs and an active buzzer:
 *
 *   > 30 cm       SAFE      green LED only
 *   15 - 30 cm    CAUTION   yellow LED + slow beep
 *   < 15 cm       STOP      red LED + fast beep
 *
 * Wiring:
 *   HC-SR04  VCC -> 5V    GND -> GND   TRIG -> D9   ECHO -> D10
 *   Green  LED (+ long leg) -> D5  via 220 ohm,  (- short leg) -> GND
 *   Yellow LED (+ long leg) -> D6  via 220 ohm,  (- short leg) -> GND
 *   Red    LED (+ long leg) -> D7  via 220 ohm,  (- short leg) -> GND
 *   Buzzer (+ long leg) -> D8,                   (-) -> GND
 *
 * This sketch only uses the two blocks you already know: setup() and loop().
 * No custom functions, no classes - just variables, if/else, and delay().
 */

// ---- Pin numbers (which Arduino pin each part is plugged into) ----
int trigPin   = 9;    // HC-SR04 trigger
int echoPin   = 10;   // HC-SR04 echo
int greenPin  = 5;    // green LED  (via 220 ohm)
int yellowPin = 6;    // yellow LED (via 220 ohm)
int redPin    = 7;    // red LED    (via 220 ohm)
int buzzerPin = 8;    // active buzzer (long leg = +)

// ---- Distance limits, in centimetres ----
int safeCm    = 30;   // farther than this -> SAFE
int cautionCm = 15;   // 15..30 -> CAUTION,  below 15 -> STOP

// setup() runs ONCE when the Arduino turns on.
void setup() {
  // Tell each pin whether it sends power OUT or reads a signal IN.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);   // open the Serial Monitor at 9600 baud to see the distance
}

// loop() runs OVER and OVER, forever, after setup() finishes.
void loop() {
  // ---- Step 1: send one ultrasonic "ping" ----
  // A short HIGH pulse on the trigger pin tells the sensor to chirp.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // ---- Step 2: measure how long the echo takes to come back ----
  // pulseIn waits for the echo pin to go HIGH and times the trip.
  long duration = pulseIn(echoPin, HIGH, 30000);  // give up after ~30 ms

  // ---- Step 3: turn that time into a distance in centimetres ----
  // Sound travels about 0.034 cm per microsecond. We divide by 2
  // because the sound goes TO the wall and bounces BACK (two trips).
  int distanceCm = duration * 0.034 / 2;

  // If we never heard an echo, pretend the wall is very far away.
  if (duration == 0) {
    distanceCm = 999;
  }

  // ---- Step 4: show the distance on the Serial Monitor ----
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  // ---- Step 5: turn everything OFF, then light up what we need ----
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(buzzerPin, LOW);

  if (distanceCm > safeCm) {
    // SAFE - green light only, no beeping
    digitalWrite(greenPin, HIGH);
    delay(100);

  } else if (distanceCm >= cautionCm) {
    // CAUTION - yellow light + slow beep
    digitalWrite(yellowPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(150);
    digitalWrite(buzzerPin, LOW);
    delay(300);

  } else {
    // STOP - red light + fast beep
    digitalWrite(redPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(80);
    digitalWrite(buzzerPin, LOW);
    delay(80);
  }
}
