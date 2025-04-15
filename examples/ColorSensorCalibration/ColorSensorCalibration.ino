/**
 * @file ColorSensorCalibration.ino
 * @brief Example to calibrate and test the TCS230 color sensor
 * @author catalina
 */

#include <ColorSensor.h>
#include <DisplayManager.h>

// Create sensor and display instances
ColorSensor colorSensor;
DisplayManager display;

// Variables to store RGB values
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("TCS230 Color Sensor Calibration");
  
  // Initialize display
  display.begin();
  display.displayMessage("Color Sensor", 0, true);
  display.displayMessage("Calibration", 1, true);
  delay(2000);
  
  // Initialize color sensor
  colorSensor.begin(ColorSensor::FREQUENCY_SCALING_20);
  
  // Allow sensor to stabilize
  delay(1000);
  
  // Instruct the user
  display.clear();
  display.displayMessage("Move WHITE", 0, true);
  display.displayMessage("surface to sensor", 1, true);
  Serial.println("Place WHITE surface in front of the sensor for calibration");
  delay(5000);
  
  // Run automatic calibration
  display.clear();
  display.displayMessage("Calibrating...", 0, true);
  
  if (colorSensor.runCalibration(5000)) {
    display.displayMessage("Calibration OK", 1, true);
    Serial.println("Calibration successful!");
  } else {
    display.displayMessage("Calibration FAILED", 1, true);
    Serial.println("Calibration failed. Using default values.");
  }
  
  delay(2000);
  display.clear();
  display.displayMessage("Ready to test", 0, true);
  display.displayMessage("colors", 1, true);
  delay(2000);
}

void loop() {
  // Read RGB values from the sensor
  colorSensor.readRGB(redValue, greenValue, blueValue);
  
  // Detect color
  ColorIdentifier detectedColor = colorSensor.detectColor();
  String colorName = colorSensor.getColorName(detectedColor);
  
  // Display results on LCD
  display.clear();
  display.displayMessage("Color: " + colorName, 0);
  
  // Format RGB values as string (R:xxx G:xxx)
  String rgbString = "R:" + String(redValue) + " G:" + String(greenValue);
  display.displayMessage(rgbString, 1);
  
  // Print details to serial monitor
  Serial.println("Detected color: " + colorName);
  Serial.print("Red: ");
  Serial.print(redValue);
  Serial.print(" Green: ");
  Serial.print(greenValue);
  Serial.print(" Blue: ");
  Serial.println(blueValue);
  
  // Add a delay before the next reading
  delay(500);
}
