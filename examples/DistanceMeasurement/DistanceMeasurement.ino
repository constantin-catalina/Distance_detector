/**
 * @file DistanceMeasurement.ino
 * @brief Example for ultrasonic distance sensor
 * @author catalina
 */

#include <DistanceSensor.h>
#include <DisplayManager.h>
#include <AudioManager.h>
#include <LedManager.h>

// Create sensor and peripheral instances
DistanceSensor distanceSensor;
DisplayManager display;
AudioManager audio;
LedManager leds;

// Proximity thresholds for different alerts
const float PROXIMITY_CLOSE = 10.0;  // cm
const float PROXIMITY_MEDIUM = 25.0; // cm
const float PROXIMITY_FAR = 50.0;    // cm

void setup() {
  Serial.begin(9600);
  Serial.println("Distance Measurement System");
  
  // Initialize components
  distanceSensor.begin();
  display.begin();
  audio.begin();
  leds.begin();
  
  // Run a quick test
  leds.blinkLed(ColorIdentifier::RED, 1);
  leds.blinkLed(ColorIdentifier::GREEN, 1);
  audio.playSuccessSound();
  
  // Welcome message
  display.displayMessage("Distance Sensor", 0, true);
  display.displayMessage("Ready", 1, true);
  delay(2000);
  
  // Calibrate for room temperature (adjust as needed)
  distanceSensor.calibrateForTemperature(22.0); // 22°C
}

void loop() {
  // Measure distance (average 3 readings for stability)
  float distance = distanceSensor.getDistance(DistanceSensor::CENTIMETERS, 3);
  
  // Display the distance
  display.displayDistance(distance);
  
  // Print to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check proximity and provide feedback
  if (distance <= PROXIMITY_CLOSE) {
    // Object is very close
    leds.setLed(ColorIdentifier::RED);
    if (!audio.isPlaying()) {
      audio.playAlertSound();
    }
  } else if (distance <= PROXIMITY_MEDIUM) {
    // Object at medium distance
    leds.setLed(ColorIdentifier::NONE); // Yellow LED
  } else if (distance <= PROXIMITY_FAR) {
    // Object at far distance
    leds.setLed(ColorIdentifier::GREEN);
  } else {
    // No object in range
    leds.allOff();
  }
  
  // Delay before next reading
  delay(200);
}
