/**
 * @file ColorSensor.cpp
 * @brief TCS230 Color Sensor library implementation
 * @author catalina
*/

#include "ColorSensor.h"

ColorSensor::ColorSensor(uint8_t s0Pin, uint8_t s1Pin, uint8_t s2Pin, uint8_t s3Pin, uint8_t outPin)
    : _s0Pin(s0Pin),
      _s1Pin(s1Pin),
      _s2Pin(s2Pin),
      _s3Pin(s3Pin),
      _outPin(outPin),
      _redMin(CalibrationSettings::ColorSensor::RED_MIN),
      _redMax(CalibrationSettings::ColorSensor::RED_MAX),
      _greenMin(CalibrationSettings::ColorSensor::GREEN_MIN),
      _greenMax(CalibrationSettings::ColorSensor::GREEN_MAX),
      _blueMin(CalibrationSettings::ColorSensor::BLUE_MIN),
      _blueMax(CalibrationSettings::ColorSensor::BLUE_MAX) {
}

void ColorSensor::begin(uint8_t frequencyScaling) {
    // Initialize pins
    pinMode(_s0Pin, OUTPUT);
    pinMode(_s1Pin, OUTPUT);
    pinMode(_s2Pin, OUTPUT);
    pinMode(_s3Pin, OUTPUT);
    pinMode(_outPin, INPUT);
    
    // Set frequency scaling
    setFrequencyScaling(frequencyScaling);
}

void ColorSensor::setCalibration(int redMin, int redMax, int greenMin, int greenMax, int blueMin, int blueMax) {
    _redMin = redMin;
    _redMax = redMax;
    _greenMin = greenMin;
    _greenMax = greenMax;
    _blueMin = blueMin;
    _blueMax = blueMax;
}

bool ColorSensor::runCalibration(unsigned long calibrationTime) {
    // Initialize min/max values
    _redMin = 1000;
    _redMax = 0;
    _greenMin = 1000;
    _greenMax = 0;
    _blueMin = 1000;
    _blueMax = 0;
    
    unsigned long startTime = millis();
    int red, green, blue;
    
    // Calibration loop
    while (millis() - startTime < calibrationTime) {
        // Read raw sensor values
        red = getRedPW();
        green = getGreenPW();
        blue = getBluePW();
        
        // Update min/max values
        _redMin = min(_redMin, red);
        _redMax = max(_redMax, red);
        _greenMin = min(_greenMin, green);
        _greenMax = max(_greenMax, green);
        _blueMin = min(_blueMin, blue);
        _blueMax = max(_blueMax, blue);
        
        delay(100); // Small delay between readings
    }
    
    // Check if calibration produced valid ranges
    if (_redMax - _redMin < 10 || _greenMax - _greenMin < 10 || _blueMax - _blueMin < 10) {
        return false; // Calibration failed
    }
    
    return true; // Calibration succeeded
}

void ColorSensor::readRawValues(int &red, int &green, int &blue) {
    red = getRedPW();
    delay(SystemSettings::SENSOR_STABILIZATION_DELAY);
    
    green = getGreenPW();
    delay(SystemSettings::SENSOR_STABILIZATION_DELAY);
    
    blue = getBluePW();
}

void ColorSensor::readRGB(int &red, int &green, int &blue) {
    int rawRed, rawGreen, rawBlue;
    
    // Get raw values
    readRawValues(rawRed, rawGreen, rawBlue);
    
    // Map to 0-255 range
    red = map(rawRed, _redMin, _redMax, 255, 0);
    green = map(rawGreen, _greenMin, _greenMax, 255, 0);
    blue = map(rawBlue, _blueMin, _blueMax, 255, 0);
    
    // Constrain to valid range
    red = constrain(red, 0, 255);
    green = constrain(green, 0, 255);
    blue = constrain(blue, 0, 255);
}

ColorIdentifier ColorSensor::detectColor() {
    int red, green, blue;
    readRGB(red, green, blue);
    
    // Check if values are valid for detection
    if (red < SystemSettings::COLOR_DETECTION_THRESHOLD && 
        green < SystemSettings::COLOR_DETECTION_THRESHOLD && 
        blue < SystemSettings::COLOR_DETECTION_THRESHOLD) {
        return ColorIdentifier::NONE;
    }
    
    // Determine dominant color
    if (red > green + SystemSettings::COLOR_DETECTION_THRESHOLD && 
        red > blue + SystemSettings::COLOR_DETECTION_THRESHOLD) {
        return ColorIdentifier::RED;
    } else if (green > red + SystemSettings::COLOR_DETECTION_THRESHOLD && 
               green > blue + SystemSettings::COLOR_DETECTION_THRESHOLD) {
        return ColorIdentifier::GREEN;
    } else if (blue > red + SystemSettings::COLOR_DETECTION_THRESHOLD && 
               blue > green + SystemSettings::COLOR_DETECTION_THRESHOLD) {
        return ColorIdentifier::BLUE;
    }
    
    return ColorIdentifier::NONE;
}

String ColorSensor::getColorName(ColorIdentifier colorId) {
    switch (colorId) {
        case ColorIdentifier::NONE:
            return "No color";
        case ColorIdentifier::RED:
            return "Red";
        case ColorIdentifier::GREEN:
            return "Green";
        case ColorIdentifier::BLUE:
            return "Blue";
        default:
            return "Unknown";
    }
}

int ColorSensor::getRedPW() {
    // Set sensor to read Red only
    digitalWrite(_s2Pin, LOW);
    digitalWrite(_s3Pin, LOW);
    
    // Read the output Pulse Width
    return pulseIn(_outPin, LOW);
}

int ColorSensor::getGreenPW() {
    // Set sensor to read Green only
    digitalWrite(_s2Pin, HIGH);
    digitalWrite(_s3Pin, HIGH);
    
    // Read the output Pulse Width
    return pulseIn(_outPin, LOW);
}

int ColorSensor::getBluePW() {
    // Set sensor to read Blue only
    digitalWrite(_s2Pin, LOW);
    digitalWrite(_s3Pin, HIGH);
    
    // Read the output Pulse Width
    return pulseIn(_outPin, LOW);
}

void ColorSensor::setFrequencyScaling(uint8_t scaling) {
    switch (scaling) {
        case FREQUENCY_SCALING_OFF:
            digitalWrite(_s0Pin, LOW);
            digitalWrite(_s1Pin, LOW);
            break;
        case FREQUENCY_SCALING_2:
            digitalWrite(_s0Pin, LOW);
            digitalWrite(_s1Pin, HIGH);
            break;
        case FREQUENCY_SCALING_20:
            digitalWrite(_s0Pin, HIGH);
            digitalWrite(_s1Pin, LOW);
            break;
        case FREQUENCY_SCALING_100:
            digitalWrite(_s0Pin, HIGH);
            digitalWrite(_s1Pin, HIGH);
            break;
        default:
            // Default to 20%
            digitalWrite(_s0Pin, HIGH);
            digitalWrite(_s1Pin, LOW);
    }
}
