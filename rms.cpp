#include "rms.h"
#include <cmath>
#include <iostream>

Rms::Rms(int bufferSize)
{
				this->bufferSize = bufferSize;
				std::cout << "initialising RMS" << std::endl;
				rmsBuffer = new float[bufferSize];
				for(int i = 0; i<bufferSize; i++){
				rmsBuffer[i] = 0;
				}
}

Rms::~Rms()
{
				delete[] rmsBuffer;
				rmsBuffer = nullptr;
}

float Rms::trackSignal(float incomingSignal)
{
    // Square the new signal
    float squaredSignal = incomingSignal * incomingSignal;

    // Subtract the old value from the sum
    bufferSum -= rmsBuffer[bufferPosition];

    // Store the new squared value in the buffer
    rmsBuffer[bufferPosition] = squaredSignal;

    // Add the new squared value to the sum
    bufferSum += squaredSignal;

    // Move buffer position circularly
    bufferPosition = (bufferPosition + 1) % bufferSize;

    // Calculate and return the RMS
    RMSSignal = std::sqrt(bufferSum / bufferSize);
    return RMSSignal;
}

float Rms::resetRmsSize()
{
				bufferSum = 0;
				RMSSignal = 0;
				bufferPosition = 0;
				return RMSSignal;
}
