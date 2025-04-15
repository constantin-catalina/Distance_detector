/**
 * @file LedManager.cpp
 * @brief LED manager implementation
 * @author catalina
 */

#include "LedManager.h"

LedManager::LedManager(uint8_t redPin, uint8_t greenPin, uint8_t yellowPin)
    : _redPin(redPin),
      _greenPin(greenPin),
      _yellowPin(yellowPin) {
}

void LedManager::begin() {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_yellowPin, OUTPUT);
    
    // Ensure all LEDs are off at start
    allOff();
}

void LedManager::setLed(ColorIdentifier colorId, uint8_t brightness) {
    // Turn off all LEDs first
    allOff();
    
    // Set the specified LED
    uint8_t pin = getColorPin(colorId);
    if (pin != 0) {
        analogWrite(pin, brightness);
    }
}

void LedManager::allOff() {
    analogWrite(_redPin, 0);
    analogWrite(_greenPin, 0);
    analogWrite(_yellowPin, 0);
}

void LedManager::blinkLed(ColorIdentifier colorId, uint8_t count, uint16_t onTime, uint16_t offTime) {
    uint8_t pin = getColorPin(colorId);
    if (pin == 0) return;
    
    for (int i = 0; i < count; i++) {
        digitalWrite(pin, HIGH);
        delay(onTime);
        digitalWrite(pin, LOW);
        
        // Don't delay after the last blink
        if (i < count - 1) {
            delay(offTime);
        }
    }
}

void LedManager::pulseLed(ColorIdentifier colorId, uint8_t count, uint16_t duration) {
    uint8_t pin = getColorPin(colorId);
    if (pin == 0) return;
    
    uint16_t stepDuration = duration / 510; // 255 steps up, 255 steps down
    
    for (int i = 0; i < count; i++) {
        // Fade in
        for (int brightness = 0; brightness <= 255; brightness++) {
            analogWrite(pin, brightness);
            delay(stepDuration);
        }
        
        // Fade out
        for (int brightness = 255; brightness >= 0; brightness--) {
            analogWrite(pin, brightness);
            delay(stepDuration);
        }
    }
}

void LedManager::showColorResult(ColorIdentifier colorId, uint16_t duration) {
    setLed(colorId);
    delay(duration);
    allOff();
}

uint8_t LedManager::getColorPin(ColorIdentifier colorId) {
    switch (colorId) {
        case ColorIdentifier::RED:
            return _redPin;
        case ColorIdentifier::GREEN:
            return _greenPin;
        case ColorIdentifier::NONE:
            return _yellowPin;
        default:
            return 0; // Invalid color
    }
}
