/**
 * @file DisplayManager.h
 * @brief Display manager for LCD via I2C
 * @author catalina
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/**
 * @class DisplayManager
 * @brief Interface for LCD display control
 * 
 * This class provides methods to initialize and control
 * an LCD display via I2C interface with convenient message formatting.
 */
class DisplayManager {
public:
    /**
     * @brief Constructor
     * 
     * @param i2cAddress I2C address of the LCD display
     * @param columns Number of columns in the display
     * @param rows Number of rows in the display
     */
    DisplayManager(uint8_t i2cAddress = 0x27, uint8_t columns = 16, uint8_t rows = 2);
    
    /**
     * @brief Initialize the display
     * @param turnBacklightOn Whether to turn on the backlight
     */
    void begin(bool turnBacklightOn = true);
    
    /**
     * @brief Clear the display
     */
    void clear();
    
    /**
     * @brief Turn backlight on/off
     * @param on true to turn on, false to turn off
     */
    void setBacklight(bool on);
    
    /**
     * @brief Display a message on the specified row
     * 
     * @param message Message to display
     * @param row Row number (0-based)
     * @param alignCenter Whether to center-align the message
     */
    void displayMessage(const String& message, uint8_t row = 0, bool alignCenter = false);
    
    /**
     * @brief Display distance measurement
     * 
     * @param distance Distance value
     * @param unit Unit string (cm, mm, etc.)
     */
    void displayDistance(float distance, const String& unit = "cm");
    
    /**
     * @brief Display color detection result
     * 
     * @param colorName Color name to display
     * @param rgbValues Optional RGB values string
     */
    void displayColor(const String& colorName, const String& rgbValues = "");
    
    /**
     * @brief Print formatted value with label
     * 
     * @param label Label to display
     * @param value Value to display
     * @param row Row to display on
     */
    void displayLabelValue(const String& label, const String& value, uint8_t row = 0);
    
private:
    LiquidCrystal_I2C _lcd;
    uint8_t _columns;
    uint8_t _rows;
};

#endif // DISPLAY_MANAGER_H
