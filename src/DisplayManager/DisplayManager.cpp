/**
 * @file DisplayManager.cpp
 * @brief Display manager implementation
 * @author catalina
 */

#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t i2cAddress, uint8_t columns, uint8_t rows)
    : _lcd(i2cAddress, columns, rows),
      _columns(columns),
      _rows(rows) {
}

void DisplayManager::begin(bool turnBacklightOn) {
    _lcd.init();
    if (turnBacklightOn) {
        _lcd.backlight();
    } else {
        _lcd.noBacklight();
    }
}

void DisplayManager::clear() {
    _lcd.clear();
}

void DisplayManager::setBacklight(bool on) {
    if (on) {
        _lcd.backlight();
    } else {
        _lcd.noBacklight();
    }
}

void DisplayManager::displayMessage(const String& message, uint8_t row, bool alignCenter) {
    if (row >= _rows) {
        return; // Row out of range
    }
    
    _lcd.setCursor(0, row);
    
    // Clear the row first
    for (int i = 0; i < _columns; i++) {
        _lcd.print(" ");
    }
    
    // Calculate position for center alignment
    int startPos = 0;
    if (alignCenter && message.length() < _columns) {
        startPos = (_columns - message.length()) / 2;
    }
    
    // Display the message
    _lcd.setCursor(startPos, row);
    _lcd.print(message);
}

void DisplayManager::displayDistance(float distance, const String& unit) {
    // Format: "Distance: XX.X cm"
    String distanceStr = String(distance, 1); // One decimal place
    String message = "Distance: " + distanceStr + " " + unit;
    
    displayMessage("Distance:", 0);
    displayMessage(distanceStr + " " + unit, 1, true);
}

void DisplayManager::displayColor(const String& colorName, const String& rgbValues) {
    displayMessage("Color detected:", 0);
    displayMessage(colorName, 1, true);
    
    // If RGB values are provided and we have more than 2 rows
    if (_rows > 2 && rgbValues.length() > 0) {
        displayMessage(rgbValues, 2, true);
    }
}

void DisplayManager::displayLabelValue(const String& label, const String& value, uint8_t row) {
    String message = label + ": " + value;
    
    // Truncate if necessary
    if (message.length() > _columns) {
        message = message.substring(0, _columns);
    }
    
    displayMessage(message, row);
}
