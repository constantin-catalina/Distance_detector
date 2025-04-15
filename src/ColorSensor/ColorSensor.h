/**
 * @file ColorSensor.h
 * @brief TCS230 Color Sensor library header
 * @author catalina
*/

#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <Arduino.h>
#include "../Configuration/SensorConfig.h"

/**
 * @class ColorSensor
 * @brief Interface for the TCS230 color sensor
 * 
 * This class provides methods to initialize and read RGB values from a TCS230 color sensor,
 * with calibration support for accurate color detection.
 */
class ColorSensor {
public:
    /**
     * @brief Constructor with optional pin configuration
     * 
     * @param s0Pin S0 frequency scaling selection pin
     * @param s1Pin S1 frequency scaling selection pin
     * @param s2Pin S2 photodiode selection pin
     * @param s3Pin S3 photodiode selection pin
     * @param outPin Output frequency pin
     */
    ColorSensor(
        uint8_t s0Pin = PinConfig::ColorSensor::S0,
        uint8_t s1Pin = PinConfig::ColorSensor::S1,
        uint8_t s2Pin = PinConfig::ColorSensor::S2,
        uint8_t s3Pin = PinConfig::ColorSensor::S3,
        uint8_t outPin = PinConfig::ColorSensor::OUT
    );

    /**
     * @brief Initialize sensor pins and configuration
     * @param frequencyScaling Frequency scaling to use (20%, 100%, etc.)
     */
    void begin(uint8_t frequencyScaling = FREQUENCY_SCALING_20);

    /**
     * @brief Set calibration values for color detection
     * 
     * @param redMin Minimum red pulse width value
     * @param redMax Maximum red pulse width value
     * @param greenMin Minimum green pulse width value
     * @param greenMax Maximum green pulse width value
     * @param blueMin Minimum blue pulse width value
     * @param blueMax Maximum blue pulse width value
     */
    void setCalibration(
        int redMin, int redMax,
        int greenMin, int greenMax,
        int blueMin = CalibrationSettings::ColorSensor::BLUE_MIN,
        int blueMax = CalibrationSettings::ColorSensor::BLUE_MAX
    );
    
    /**
     * @brief Run calibration procedure to automatically determine min/max values
     * @param calibrationTime Time in ms to spend on calibration
     * @return true if calibration succeeded, false otherwise
     */
    bool runCalibration(unsigned long calibrationTime = 5000);

    /**
     * @brief Read raw pulse width values from the sensor
     * 
     * @param red Reference to store red pulse width
     * @param green Reference to store green pulse width
     * @param blue Reference to store blue pulse width
     */
    void readRawValues(int &red, int &green, int &blue);

    /**
     * @brief Read calibrated RGB values (0-255)
     * 
     * @param red Reference to store red value
     * @param green Reference to store green value
     * @param blue Reference to store blue value
     */
    void readRGB(int &red, int &green, int &blue);

    /**
     * @brief Detect dominant color based on RGB values
     * @return ColorIdentifier enum representing the detected color
     */
    ColorIdentifier detectColor();

    /**
     * @brief Get color name as string for display
     * @param colorId ColorIdentifier to convert to string
     * @return String representation of the color
     */
    String getColorName(ColorIdentifier colorId);

    // Frequency scaling options
    static const uint8_t FREQUENCY_SCALING_OFF = 0;
    static const uint8_t FREQUENCY_SCALING_2 = 1;
    static const uint8_t FREQUENCY_SCALING_20 = 2;
    static const uint8_t FREQUENCY_SCALING_100 = 3;

private:
    // Pin configuration
    uint8_t _s0Pin;
    uint8_t _s1Pin;
    uint8_t _s2Pin;
    uint8_t _s3Pin;  
    uint8_t _outPin;

    // Calibration values
    int _redMin;
    int _redMax;
    int _greenMin;
    int _greenMax;
    int _blueMin;
    int _blueMax;

    // Helper methods
    int getRedPW();
    int getGreenPW();
    int getBluePW();
    void setFrequencyScaling(uint8_t scaling);
};

#endif // COLOR_SENSOR_H
