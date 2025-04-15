/**
 * @file AudioManager.cpp
 * @brief Audio output manager implementation
 * @author catalina
 */

#include "AudioManager.h"

// Predefined melodies
namespace {
    // Success melody
    const uint16_t successMelody[] = {
        Notes::NOTE_C4, Notes::NOTE_E4, Notes::NOTE_G4, Notes::NOTE_C5
    };
    const uint8_t successDurations[] = { 16, 16, 16, 4 };
    
    // Error melody
    const uint16_t errorMelody[] = {
        Notes::NOTE_E5, Notes::NOTE_B4, Notes::NOTE_G4
    };
    const uint8_t errorDurations[] = { 8, 8, 4 };
    
    // Alert melody
    const uint16_t alertMelody[] = {
        Notes::NOTE_C4, Notes::NOTE_C4, 0, Notes::NOTE_C4, Notes::NOTE_C4
    };
    const uint8_t alertDurations[] = { 8, 8, 16, 8, 4 };
}

AudioManager::AudioManager(uint8_t pin)
    : _pin(pin),
      _isPlaying(false) {
}

void AudioManager::begin() {
    pinMode(_pin, OUTPUT);
    stopTone();
}

void AudioManager::playTone(uint16_t frequency, uint32_t duration) {
    _isPlaying = true;
    
    if (frequency > 0) {
        tone(_pin, frequency, duration);
    } else {
        // Rest (silence)
        noTone(_pin);
        delay(duration);
    }
    
    // Add a small gap between notes
    delay(duration * 0.1);
    _isPlaying = false;
}

void AudioManager::stopTone() {
    noTone(_pin);
    _isPlaying = false;
}

void AudioManager::playMelody(const uint16_t* melody, const uint8_t* durations, uint8_t noteCount, float tempo) {
    _isPlaying = true;
    
    for (int i = 0; i < noteCount; i++) {
        // Calculate note duration
        int noteDuration = 1000 / durations[i] / tempo;
        
        // Play the note
        playNote(melody[i], noteDuration);
        
        // Brief pause between notes
        delay(noteDuration * 0.3);
    }
    
    _isPlaying = false;
}

void AudioManager::playSuccessSound() {
    playMelody(successMelody, successDurations, sizeof(successMelody) / sizeof(successMelody[0]));
}

void AudioManager::playErrorSound() {
    playMelody(errorMelody, errorDurations, sizeof(errorMelody) / sizeof(errorMelody[0]));
}

void AudioManager::playAlertSound() {
    playMelody(alertMelody, alertDurations, sizeof(alertMelody) / sizeof(alertMelody[0]));
}

bool AudioManager::setVolume(uint8_t volume) {
    // Basic Arduino doesn't support volume control
    // This would need external components or specific hardware
    return false;
}

bool AudioManager::isPlaying() {
    return _isPlaying;
}

void AudioManager::playNote(uint16_t frequency, uint32_t duration) {
    if (frequency == 0) {
        // Rest - no tone
        noTone(_pin);
        delay(duration);
    } else {
        // Play tone
        tone(_pin, frequency, duration);
        delay(duration);
    }
}
