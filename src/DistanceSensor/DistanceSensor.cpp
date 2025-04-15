/**
 * @file DistanceSensor.cpp
 * @brief Ultrasonic distance sensor library implementation
 * @author catalina
*/

#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(uint8_t trigPin, uint8_t echoPin)
    : _trigPin(trigPin),
      _echoPin(echoPin),
      _speedOfSound(0.0343) { // Default speed of sound in cm/µs at 20°C
}

void DistanceSensor::begin() {
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    
    // Ensure trigger pin is LOW to start
    digitalWrite(_trigPin, LOW);
}

float DistanceSensor::getDistance(DistanceUnit unit, uint8_t samples) {
    float totalDistance = 0.0;
    
    // Take multiple samples if requested
    for (int i = 0; i < samples; i++) {
        float duration = measurePulseDuration();
        
        // Calculate distance based on speed of sound
        // Divide by 2 because sound travels to the object and back
        float distanceCm = duration * _speedOfSound / 2.0;
        
        totalDistance += distanceCm;
        
        // Small delay between samples
        if (samples > 1 && i < samples - 1) {
            delay(20);
        }
    }
    
    // Calculate average
    float averageDistanceCm = totalDistance / samples;
    
    // Convert to requested unit
    switch (unit) {
        case INCHES:
            return averageDistanceCm / 2.54;
        case MILLIMETERS:
            return averageDistanceCm * 10.0;
        case CENTIMETERS:
        default:
            return averageDistanceCm;
    }
}

bool DistanceSensor::isObjectDetected(float threshold) {
    float distance = getDistance();
    return distance <= threshold && distance > 0;
}

void DistanceSensor::setSpeedOfSound(float speed) {
    _speedOfSound = speed;
}

void DistanceSensor::calibrateForTemperature(float temperatureC) {
    // Formula: speed of sound (m/s) = 331.3 + 0.606 * T
    // Convert to cm/µs: divide by 10000
    _speedOfSound = (331.3 + 0.606 * temperatureC) / 10000.0;
}

float DistanceSensor::measurePulseDuration() {
    // Clear the trigger pin
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    
    // Set the trigger pin HIGH for 10 microseconds
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
    
    // Read the echo pin - pulse duration in microseconds
    float duration = pulseIn(_echoPin, HIGH);
    
    return duration;
}
