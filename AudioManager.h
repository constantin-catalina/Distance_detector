/**
 * @file AudioManager.h
 * @brief Audio output manager for Arduino
 * @author catalina
 */

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Arduino.h>
#include "../Configuration/PitchesDefinitions.h"

/**
 * @class AudioManager
 * @brief Interface for audio output control
 * 
 * This class provides methods to play tones, melodies and sound effects
 * on a speaker or buzzer connected to an Arduino pin.
 */
class AudioManager {
public:
    /**
     * @brief Constructor
     * 
     * @param pin Output pin connected to the speaker/buzzer
     */
    AudioManager(uint8_t pin = PinConfig::BUZZER);
    
    /**
     * @brief Initialize the audio system
     */
    void begin();
    
    /**
     * @brief Play a single tone
     * 
     * @param frequency Frequency in Hz
     * @param duration Duration in milliseconds
     */
    void playTone(uint16_t frequency, uint32_t duration);
    
    /**
     * @brief Stop any currently playing tone
     */
    void stopTone();
    
    /**
     * @brief Play a melody defined by arrays of notes and durations
     * 
     * @param melody Array of note frequencies
     * @param durations Array of note durations (4 = quarter note, 8 = eighth note, etc.)
     * @param noteCount Number of notes in the melody
     * @param tempo Tempo multiplier (1.0 = normal speed)
     */
    void playMelody(const uint16_t* melody, const uint8_t* durations, uint8_t noteCount, float tempo = 1.0);
    
    /**
     * @brief Play a success sound effect
     */
    void playSuccessSound();
    
    /**
     * @brief Play an error sound effect
     */
    void playErrorSound();
    
    /**
     * @brief Play an alert sound effect
     */
    void playAlertSound();
    
    /**
     * @brief Set the speaker volume (if supported by hardware)
     * 
     * @param volume Volume level (0-255)
     * @return true if volume control is supported
     */
    bool setVolume(uint8_t volume);
    
    /**
     * @brief Check if audio is currently playing
     * @return true if audio is playing
     */
    bool isPlaying();
    
private:
    uint8_t _pin;
    bool _isPlaying;
    
    // Internal helper methods
    void playNote(uint16_t frequency, uint32_t duration);
};

#endif // AUDIO_MANAGER_H
