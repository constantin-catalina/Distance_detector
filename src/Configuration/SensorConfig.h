/**
 * @file SensorConfig.h
 * @brief Configuration settings for the sensor system
 * @author catalina
 */

#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

// Pin configuration
namespace PinConfig {
    // Color sensor pins
    namespace ColorSensor {
        constexpr uint8_t S0 = 4;
        constexpr uint8_t S1 = 5;
        constexpr uint8_t S2 = 6;
        constexpr uint8_t S3 = 7;
        constexpr uint8_t OUT = 9;
    }
    
    // Ultrasonic sensor pins
    namespace DistanceSensor {
        constexpr uint8_t TRIG = 3;
        constexpr uint8_t ECHO = 2;
    }
    
    // LED pins
    namespace LEDs {
        constexpr uint8_t RED = 11;
        constexpr uint8_t GREEN = 12;
        constexpr uint8_t YELLOW = 13;
    }
    
    // Buzzer pin
    constexpr uint8_t BUZZER = 8;
}

// Calibration settings
namespace CalibrationSettings {
    namespace ColorSensor {
        // Default calibration values - adjust these based on your specific sensor
        constexpr int RED_MIN = 25;
        constexpr int RED_MAX = 140;
        constexpr int GREEN_MIN = 32;
        constexpr int GREEN_MAX = 214;
        constexpr int BLUE_MIN = 25;
        constexpr int BLUE_MAX = 170;
    }
}

// System settings
namespace SystemSettings {
    // Distance thresholds
    constexpr float PROXIMITY_THRESHOLD = 10.0; // cm
    
    // Sensor reading delays
    constexpr unsigned int SENSOR_STABILIZATION_DELAY = 200; // ms
    constexpr unsigned int DISPLAY_REFRESH_DELAY = 1000; // ms
    
    // Color detection thresholds
    constexpr int COLOR_DETECTION_THRESHOLD = 20; // Minimum RGB difference for color detection
}

// Color definitions
enum class ColorIdentifier {
    NONE = 0,
    RED = 1,
    GREEN = 2,
    BLUE = 3
};

#endif // SENSOR_CONFIG_H
