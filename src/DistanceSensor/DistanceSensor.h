/**
 * @file DistanceSensor.h
 * @brief Ultrasonic distance sensor library header
 * @author catalina
 */

#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <Arduino.h>
#include "../Configuration/SensorConfig.h"

/**
 * @class DistanceSensor
 * @brief Interface for ultrasonic distance sensors (HC-SR04)
 * 
 * This class provides methods to initialize and read distance values 
 * from an ultrasonic sensor with configurable settings.
 */
class DistanceSensor {
public:
    /**
     * @brief Constructor with optional pin configuration
     * 
     * @param trigPin Trigger pin to send pulse
     * @param echoPin Echo pin to receive pulse
     */
    DistanceSensor(
        uint8_t trigPin = PinConfig::DistanceSensor::TRIG,
        uint8_t echoPin = PinConfig::DistanceSensor::ECHO
    );
    
    /**
     * @brief Initialize sensor pins
     */
    void begin();
    
    /**
     * @brief Measure the distance to an object
     * 
     * @param unit Distance unit (default: centimeters)
     * @param samples Number of samples to average (default: 1)
     * @return float Distance in specified unit
     */
    float getDistance(DistanceUnit unit = CENTIMETERS, uint8_t samples = 1);
    
    /**
     * @brief Check if an object is within the specified proximity
     * 
     * @param threshold Distance threshold in cm
     * @return true if object is within threshold distance
     */
    bool isObjectDetected(float threshold = SystemSettings::PROXIMITY_THRESHOLD);
    
    /**
     * @brief Set the speed of sound for accurate measurements
     * 
     * @param speed Speed of sound in m/s
     */
    void setSpeedOfSound(float speed);
    
    /**
     * @brief Calculate speed of sound based on temperature
     * 
     * @param temperatureC Temperature in Celsius
     */
    void calibrateForTemperature(float temperatureC);
    
    // Distance units
    enum DistanceUnit {
        CENTIMETERS,
        INCHES,
        MILLIMETERS
    };
    
private:
    // Pin configuration
    uint8_t _trigPin;
    uint8_t _echoPin;
    
    // Settings
    float _speedOfSound; // in cm/µs
    
    // Helper methods
    float measurePulseDuration();
};

#endif // DISTANCE_SENSOR_H
