#pragma once
#include <cmath>

class Peak {
public:
    Peak(float holdTimeSamples = 4410, float decayRate = 0.9995f) 
        : holdTimeSamples(holdTimeSamples), decayRate(decayRate) {}

    float trackSignal(float input) {
        float absSample = std::fabs(input);

        // If new sample is higher than the current peak, reset hold
        if (absSample > peakValue) {
            peakValue = absSample;
            holdCounter = holdTimeSamples;
        }
        else {
            // If holding, just decrement counter
            if (holdCounter > 0) {
                holdCounter--;
            } 
            else {
                // Apply decay when not holding
                peakValue *= decayRate;
            }
        }

        return peakValue;
    }

    void reset() {
        peakValue = 0.0f;
        holdCounter = 0;
    }

private:
    float peakValue = 0.0f;
    int holdCounter = 0;
    int holdTimeSamples;   // how long to hold before decaying
    float decayRate;       // how quickly the peak decays
};

