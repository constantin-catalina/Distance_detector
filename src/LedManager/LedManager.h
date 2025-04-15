/**
 * @file LedManager.h
 * @brief LED manager for status indicators
 * @author catalina
 */

#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
#include "../Configuration/SensorConfig.h"

/**
 * @class LedManager
 * @brief Interface for controlling status LEDs
 * 
 * This class provides methods to control RGB LEDs for status indication
 * with various patterns and effects.
 */
class LedManager {
public:
    /**
     * @brief Constructor
     * 
     * @param redPin Pin for red LED
     * @param greenPin Pin for green LED
     * @param yellowPin Pin for yellow LED
     */
    LedManager(
        uint8_t redPin = PinConfig::LEDs::RED,
        uint8_t greenPin = PinConfig::LEDs::GREEN,
        uint8_t yellowPin = PinConfig::LEDs::YELLOW
    );
    
    /**
     * @brief Initialize LED pins
     */
    void begin();
    
    /**
     * @brief Turn on an LED by color
     * 
     * @param colorId Color identifier
     * @param brightness Brightness level (0-255)
     */
    void setLed(ColorIdentifier colorId, uint8_t brightness = 255);
    
    /**
     * @brief Turn off all LEDs
     */
    void allOff();
    
    /**
     * @brief Blink an LED
     * 
     * @param colorId Color identifier
     * @param count Number of blinks
     * @param onTime On time in milliseconds
     * @param offTime Off time in milliseconds
     */
    void blinkLed(ColorIdentifier colorId, uint8_t count = 1, uint16_t onTime = 200, uint16_t offTime = 200);
    
    /**
     * @brief Pulse an LED (fade in/out)
     * 
     * @param colorId Color identifier
     * @param count Number of pulses
     * @param duration Duration of each pulse in milliseconds
     */
    void pulseLed(ColorIdentifier colorId, uint8_t count = 1, uint16_t duration = 1000);
    
    /**
     * @brief Show color detection result with LED
     * 
     * @param colorId Detected color identifier
     * @param duration Duration to show color in milliseconds
     */
    void showColorResult(ColorIdentifier colorId, uint16_t duration = 2000);
    
private:
    // Pin configuration
    uint8_t _redPin;
    uint8_t _greenPin;
    uint8_t _yellowPin;
    
    // Helper methods
    uint8_t getColorPin(ColorIdentifier colorId);
};

#endif // LED_MANAGER_H
