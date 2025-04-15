/**
 * @file ColorDistanceSystem.ino
 * @brief Complete system combining distance and color detection
 * @author catalina
*/

#include <ColorSensor.h>
#include <DistanceSensor.h>
#include <DisplayManager.h>
#include <AudioManager.h>
#include <LedManager.h>
#include <PitchesDefinitions.h>

// Create component instances
ColorSensor colorSensor;
DistanceSensor distanceSensor;
DisplayManager display;
AudioManager audio;
LedManager leds;

// System mode enum
enum SystemMode {
  DISTANCE_MODE,
  COLOR_MODE
};

// Current system state
SystemMode currentMode = DISTANCE_MODE;
unsigned long lastModeChangeTime = 0;
const unsigned long MODE_SWITCH_INTERVAL = 10000; // 10 seconds

// Melody for color detection notification
const uint16_t detectionMelody[] = {
  Notes::NOTE_C4, Notes::NOTE_G3, Notes::NOTE_G3, Notes::NOTE_A3, 
  Notes::NOTE_G3, 0, Notes::NOTE_B3, Notes::NOTE_C4
};

const uint8_t detectionDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  Serial.begin(9600);
  Serial.println("Color and Distance Detection System");
  
  // Initialize all components
  colorSensor.begin();
  distanceSensor.begin();
  display.begin();
  audio.begin();
  leds.begin();
  
  // Calibrate distance sensor for room temperature
  distanceSensor.calibrateForTemperature(22.0); // 22°C
  
  // Show startup message
  display.displayMessage("System Starting", 0, true);
  display.displayMessage("Please Wait...", 1, true);
  
  // Load sensor calibration values
  colorSensor.setCalibration(
    CalibrationSettings::ColorSensor::RED_MIN,
    CalibrationSettings::ColorSensor::RED_MAX,
    CalibrationSettings::ColorSensor::GREEN_MIN,
    CalibrationSettings::ColorSensor::GREEN_MAX,
    CalibrationSettings::ColorSensor::BLUE_MIN,
    CalibrationSettings::ColorSensor::BLUE_MAX
  );
  
  // Startup animation
  leds.blinkLed(ColorIdentifier::RED, 1);
  leds.blinkLed(ColorIdentifier::GREEN, 1);
  leds.blinkLed(ColorIdentifier::NONE, 1); // Yellow
  audio.playSuccessSound();
  
  delay(1000);
  display.clear();
  display.displayMessage("Ready!", 0, true);
  delay(1000);
}

void loop() {
  // Check if it's time to switch modes
  unsigned long currentTime = millis();
  if (currentTime - lastModeChangeTime >= MODE_SWITCH_INTERVAL) {
    switchMode();
    lastModeChangeTime = currentTime;
  }
  
  // Process current mode
  if (currentMode == DISTANCE_MODE) {
    processDistanceMode();
  } else {
    processColorMode();
  }
  
  // Small delay for stability
  delay(100);
}

void switchMode() {
  // Toggle between modes
  if (currentMode == DISTANCE_MODE) {
    currentMode = COLOR_MODE;
    display.clear();
    display.displayMessage("Color Mode", 0, true);
    display.displayMessage("Active", 1, true);
    leds.pulseLed(ColorIdentifier::GREEN, 2);
  } else {
    currentMode = DISTANCE_MODE;
    display.clear();
    display.displayMessage("Distance Mode", 0, true);
    display.displayMessage("Active", 1, true);
    leds.pulseLed(ColorIdentifier::RED, 2);
  }
  
  delay(1000);
}

void processDistanceMode() {
  // Measure distance
  float distance = distanceSensor.getDistance();
  
  // Display distance
  display.displayDistance(distance);
  
  // Print to serial
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check if object is in color detection range
  if (distance <= SystemSettings::PROXIMITY_THRESHOLD) {
    // Briefly notify user that we're switching to color mode
    display.clear();
    display.displayMessage("Object Detected!", 0, true);
    display.displayMessage("Checking Color...", 1, true);
    
    delay(1000);
    
    // Switch to color mode immediately
    currentMode = COLOR_MODE;
    lastModeChangeTime = millis();
    
    // Process color right away
    processColorMode();
  }
}

void processColorMode() {
  // Check if object is in range first
  float distance = distanceSensor.getDistance();
  
  if (distance <= SystemSettings::PROXIMITY_THRESHOLD) {
    // Read color
    int red, green, blue;
    colorSensor.readRGB(red, green, blue);
    
    // Detect dominant color
    ColorIdentifier detectedColor = colorSensor.detectColor();
    String colorName = colorSensor.getColorName(detectedColor);
    
    // Format RGB values
    String rgbValues = "R:" + String(red) + " G:" + String(green) + " B:" + String(blue);
    
    // Display results
    display.displayColor(colorName, rgbValues);
    
    // Light corresponding LED
    leds.setLed(detectedColor);
    
    // Play notification melody if color is detected
    if (detectedColor != ColorIdentifier::NONE && !audio.isPlaying()) {
      audio.playMelody(detectionMelody, detectionDurations, 
                      sizeof(detectionMelody) / sizeof(detectionMelody[0]));
    }
    
    // Print to serial
    Serial.print("Color: ");
    Serial.println(colorName);
    Serial.println(rgbValues);
  } else {
    // No object in range
    display.clear();
    display.displayMessage("Color Mode", 0, true);
    display.displayMessage("No object", 1, true);
    leds.allOff();
  }
}
